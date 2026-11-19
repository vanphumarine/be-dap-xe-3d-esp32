# 🚵 Bé Đạp Xe Địa Hình 3D · ESP32 Bike Simulator

Dự án trò chơi đạp xe địa hình 3D tương tác kết hợp phần cứng **ESP32** dành cho trẻ em vận động tại nhà. Game hỗ trợ **đèn pha chiếu sáng ban đêm**, **nhấn nút 2 lần để bắn đạn**, và điều khiển linh hoạt qua **cảm biến xe đạp thật** (Web Serial / Bluetooth SPP / BLE) hoặc chơi ngay bằng **bàn phím / tay cầm**.

---

## 📂 Cấu trúc dự án

| Tên tệp | Mô tả |
| :--- | :--- |
| `index.html` | Trang chủ giới thiệu dự án, tính năng đèn ban đêm & bắn đạn, bảng đấu nối và liên kết mở nhanh |
| `Minh_Khang_Game_Dev.html` | Game **Bé đạp xe địa hình 3D** (Cinematic Edition v2) với 20 màn chơi, đèn ban đêm, bắn đạn & chế độ khám phá tự do |
| `So-do-ESP32-KY003-da-sua.html` | Sơ đồ đấu dây tương tác cho ESP32 (hỗ trợ cả module KY-003 và Hall A3144 rời) kèm mô phỏng tín hiệu |
| `Minh_Khang_Game_Dev_Esp32code.ino` | Mã nguồn Firmware nạp cho mạch ESP32 (đọc vòng đạp, góc lái, nút nhấn nhảy/bắn đạn và gửi dữ liệu qua USB + Bluetooth) |

---

## ✨ Tính năng nổi bật

### 1. Game Đạp Xe Địa Hình 3D (`Minh_Khang_Game_Dev.html`)
* **Đèn pha chiếu sáng ban đêm (`💡 Đèn`)**: Tự động bật đèn pha chiếu sáng mặt đường, quầng sáng và đèn hậu khi vào các màn ban đêm hoặc khu vực núi lửa tối; bé cũng có thể chuyển thủ công giữa *Tự động / BẬT / TẮT* bất kỳ lúc nào.
* **Nhấn nút 2 lần để bắn đạn sao**: Khi nhấn nút nhảy (`⤴` trên màn hình, phím `Space` trên bàn phím hoặc nút cứng trên xe ESP32) **2 lần liên tiếp nhanh**, xe sẽ bắn ra đạn ngôi sao cầu vồng để phá đá, khúc gỗ, thú cản đường (cộng thêm sao `⭐`) hoặc đẩy lùi quái thú đang đuổi phía sau.
* **20 màn chơi đa dạng chủ đề**: Thung lũng nắng mai, Rừng hoa anh đào, Sa mạc cát vàng, Rừng đom đóm đêm, Rừng pha lê tím, Trạm vũ trụ ngân hà, Rừng bí ngô ma quái, Đỉnh núi lửa rồng... cùng 3 mức độ thử thách.
* **Chế độ Khám phá tự do**: Bản đồ mở gồm 5 khu vực khí hậu khác nhau với hệ thống nhiệm vụ thu thập và mini-map.
* **Cá nhân hóa cho nhiều bé**: Tạo nhiều hồ sơ lưu sao riêng, tùy chọn 6 nhân vật, 6 mẫu xe đạp 3D, màu áo, màu mũ bảo hiểm và 4 góc nhìn camera.
* **Tích hợp công cụ Hiệu chỉnh (`🎛️ Hiệu chỉnh`)**: Dễ dàng ghi lại góc lái *Hết Trái / Thẳng / Hết Phải* và điều chỉnh hệ số *Gain* vòng đạp (`×0.1` đến `×4.0`) phù hợp với kích thước bánh xe và lực đạp của bé.

### 2. Sơ Đồ Đấu Dây Tương Tác (`So-do-ESP32-KY003-da-sua.html`)
* Chuyển đổi trực quan giữa hai loại cảm biến Hall phổ biến: **Module KY-003** và **Cảm biến A3144 rời**.
* Bấm vào từng đường dây nguồn (`5V`, `3V3`, `GND`) hoặc chân tín hiệu để làm nổi bật đường đi trên mạch.
* Tích hợp thanh trượt mô phỏng vòng đạp, góc lái và nút nhấn nhảy/bắn đạn để kiểm tra gói tin gửi về game.

### 3. Firmware ESP32 (`Minh_Khang_Game_Dev_Esp32code.ino`)
* Sử dụng ngắt ngoài (`IRAM_ATTR`) đọc xung từ cảm biến Hall (`GPIO27`) có lọc nhiễu và làm mượt tốc độ khi bé đạp chậm dần.
* Đọc trung bình 8 lần giá trị ADC góc lái từ biến trở (`GPIO34`) và nút nhấn chống dội (`GPIO26`) — hỗ trợ cả thao tác nhấn 1 lần nhảy và nhấn 2 lần nhanh để bắn đạn khi truyền về game.
* Gửi dữ liệu định kỳ mỗi `30ms` ở tốc độ `115200 baud` đồng thời qua cổng **USB Serial** và **Bluetooth Classic (`XeDap-Be`)** theo định dạng:
  ```text
  vong_phut,ADC_tho,nut_nhay,1
  ```

---

## 🔌 Sơ đồ đấu nối phần cứng ESP32

| Linh kiện | Chân trên ESP32 | Hướng dẫn đấu nối & Lưu ý an toàn |
| :--- | :--- | :--- |
| **Cảm biến Hall (Tín hiệu)** | `GPIO27` (`D27`) | • **Module KY-003 (cấp 5V)**: Nối thêm **1 điện trở `20kΩ`** từ chân `S` xuống `GND` để chia áp với trở kéo lên `10kΩ` có sẵn trên module (mức HIGH ≈ `3.3V`).<br>• **Hall A3144 rời (cấp 5V)**: Nối điện trở kéo lên `10kΩ` giữa chân `OUT` và `3V3`. |
| **Biến trở góc lái `10kΩ`** | `GPIO34` (`D34`) | Chân giữa nối `GPIO34`, hai chân bìa nối `3V3` và `GND`. **Lưu ý:** Chỉ cấp nguồn `3.3V`, không cấp `5V` vào chân ADC của ESP32. |
| **Nút nhấn (Nhảy / Nhấn 2 lần bắn đạn)** | `GPIO26` (`D26`) | Một chân nối `GPIO26`, chân còn lại nối `GND` (không cần điện trở ngoài vì code đã bật sẵn `INPUT_PULLUP`). Nhấn 1 lần để nhảy, nhấn 2 lần nhanh để bắn đạn. |
| **Nguồn & Mass chung** | `VIN` / `3V3` / `GND` | Cấp `5V` (từ chân `VIN`) cho cảm biến Hall; nối chung toàn bộ dây `GND` về chân `GND` của ESP32. |

---

## 🚀 Hướng dẫn cài đặt & sử dụng

1. **Nạp code cho ESP32**:
   * Mở tệp `Minh_Khang_Game_Dev_Esp32code.ino` bằng **Arduino IDE**.
   * Chọn đúng bo mạch ESP32 của bạn và nạp chương trình.
   * Khi hoạt động, ESP32 sẽ phát Bluetooth tên **`XeDap-Be`** và truyền dữ liệu qua cổng COM ở tốc độ `115200 baud`.
2. **Mở game trên trình duyệt**:
   * Truy cập link **GitHub Pages** của dự án (hoặc mở trực tiếp `index.html` / `Minh_Khang_Game_Dev.html`) bằng trình duyệt **Google Chrome** hoặc **Microsoft Edge** trên máy tính để sử dụng Web Serial / Web Bluetooth.
3. **Kết nối, điều khiển đèn, nhảy & bắn đạn**:
   * Trong giao diện game, bấm **`🔌 Serial/BT`** (hoặc **`🔵 Bluetooth`**) để kết nối với ESP32.
   * Dùng nút **`💡 Đèn`** để chọn chế độ đèn pha khi vào các màn đêm/hang tối; nhấn nút 1 lần để nhảy hoặc nhấn 2 lần nhanh để bắn đạn sao.
   * Bấm **`🎛️ Hiệu chỉnh`**, quay ghi đông sang *Hết Trái*, *Thẳng*, *Hết Phải* và bấm lưu từng mốc; chỉnh *Gain* vòng đạp phù hợp với bé.
   * Đạp bàn đạp (hoặc nhấn phím `W` / `▲`) để bắt đầu hành trình!

---

## 📜 Bản quyền & Chia sẻ
Dự án được chia sẻ miễn phí cho cộng đồng Maker, giáo viên STEAM và các bậc phụ huynh muốn tự chế tạo đồ chơi vận động tương tác cho bé.