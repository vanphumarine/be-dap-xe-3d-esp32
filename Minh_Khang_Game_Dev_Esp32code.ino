// ESP32 - Xe đạp cho bé: Hall (vòng đạp) + biến trở (góc lái) + nút nhảy
// Gửi mỗi 30ms một dòng: vòng_phút,ADC_thô,nút_nhảy,1   (115200 baud, USB + Bluetooth SPP)
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

const int HALL_PIN = 27;   // cảm biến Hall KY-003 (chân S, qua phân áp 20kΩ xuống GND)
const int POT_PIN  = 34;   // biến trở góc lái (cấp 3V3)
const int BTN_PIN  = 26;   // nút nhảy (một chân nối GND)

// ---- Cài đặt cho bé đạp chậm ----
const unsigned long MIN_PULSE_MS = 120;   // chống nhiễu xung (tối đa ~500 vòng/phút)
const unsigned long STOP_MS      = 4000;  // quá 4 giây không có xung thì coi là dừng (~15 vòng/phút)
const float CAD_GAIN             = 1.0;   // nhân tốc độ gửi về game (tăng 1.2-1.5 nếu muốn xe chạy nhanh hơn)
const float SMOOTH               = 0.35;  // 0-1: càng nhỏ càng mượt, càng lớn càng nhạy

volatile unsigned long lastT = 0, period = 0;
int jumpState = 0;
bool lastRead = false;
unsigned long lastChange = 0;
float cadSmooth = 0;

void IRAM_ATTR tick() {
  unsigned long n = millis();
  unsigned long dt = n - lastT;
  if (dt > MIN_PULSE_MS) {
    if (lastT != 0 && dt <= STOP_MS) period = dt;  // xung đầu tiên hoặc sau khi dừng lâu: chưa tính vòng
    else period = 0;
    lastT = n;
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("XeDap-Be");
  analogReadResolution(12);
  analogSetPinAttenuation(POT_PIN, ADC_11db);
  pinMode(HALL_PIN, INPUT);          // KY-003 đã có điện trở kéo lên trên module
  pinMode(BTN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(HALL_PIN), tick, FALLING);
}

void loop() {
  // Góc lái: đọc 8 lần lấy trung bình
  long sum = 0;
  for (int i = 0; i < 8; i++) sum += analogRead(POT_PIN);
  int raw = sum / 8;

  // Nút nhảy có chống dội
  bool r = (digitalRead(BTN_PIN) == LOW);
  if (r != lastRead) { lastRead = r; lastChange = millis(); }
  if (millis() - lastChange > 15) jumpState = r ? 1 : 0;

  // Vòng đạp
  noInterrupts(); unsigned long p = period, l = lastT; interrupts();
  unsigned long since = millis() - l;
  float target = 0;
  if (l != 0 && p != 0 && since <= STOP_MS) {
    unsigned long eff = (since > p) ? since : p;  // đạp chậm dần thì giảm dần, không rớt về 0 đột ngột
    target = 60000.0f / eff * CAD_GAIN;
  }
  cadSmooth += (target - cadSmooth) * SMOOTH;
  if (cadSmooth < 0.5f) cadSmooth = 0;

  String line = String(cadSmooth, 1) + "," + String(raw) + "," + String(jumpState) + ",1";
  Serial.println(line);                          // cổng USB
  if (SerialBT.hasClient()) SerialBT.println(line);  // Bluetooth (chỉ gửi khi đã có máy kết nối)
  delay(30);
}
