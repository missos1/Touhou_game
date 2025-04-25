# Touhou Touhou Koumakyou ~ The Embodiment of Scarlet Devil (HMD edition)

Download link: [Tải tại đây](https://drive.google.com/drive/folders/1WGBy4llzN4hkpyrL9SvCHxuKzkXIuGa9?usp=sharing)

---

# Giới thiệu game

Đây là bản fanmade được lấy cảm hứng từ Touhou 6: The Embodiment of Scarlet Devil.  
Game thuộc thể loại **danmaku (bullet hell)** – nơi bạn phải né tránh hàng loạt đạn bay chi chít và tiêu diệt kẻ địch với nhân vật chính.  
Mục tiêu của bạn là sống sót, tiêu diệt boss, thu thập item và đạt điểm số cao nhất có thể.  
Game có các cơ chế như **graze**, **spellcard**, **auto-collection**, **item tăng sức mạnh**... giúp người chơi cảm nhận đầy đủ không khí Touhou chính hiệu.

---

# Di chuyển

- **WASD**: Lên / Trái / Phải / Xuống  
- **Space**: Bắn  
- **Shift**: Focus (Tập trung, di chuyển chậm, hiện hitbox)

### So sánh Player thường và Focus:
| Normal | Focus |
|--------|-------|
| ![](SDLGame1/Preview/player_normal.png) | ![](SDLGame1/Preview/player_focus.png) |

---

# Sidebar

Hiển thị hình ảnh nhân vật (HP, power v.v):  
![](SDLGame1/Preview/hp_left.png)
![](SDLGame1/Preview/power_n_graze_meter.png)

---

# Một số cơ chế game

### **Graze**
Khi đạn bay sát người chơi (nhưng không trúng), bạn sẽ nhận điểm hoặc tăng thanh năng lượng.  
---

### **Các item**

| Item | Mô tả |
|------|-------|
| ![](SDLGame1/Preview/fullpower.png) | **Full Power** – Tăng sức mạnh lên tối đa |
| ![](SDLGame1/Preview/heal.png) | **Heal** – Hồi máu |
| ![](SDLGame1/Preview/item.png) | **Point** – Tăng điểm |
| ![](SDLGame1/Preview/power.png) | **Power** – Tăng sức mạnh |

---

### **Auto Collection**

Phía trên màn hình chơi có một vùng. Khi nhân vật bay vào đó, các item sẽ tự động bị hút về.  
![](SDLGame1/Preview/item_collect.png)

---

### **Hitbox nhỏ nên đừng sợ đạn**

Tất cả viên đạn đều có hitbox nhỏ hơn nhiều so với sprite, nên bạn có thể né cực sát nếu tinh mắt và bình tĩnh.  
![](SDLGame1/Preview/hitboxdes.png)

---

### **Boss**

Khi bạn phá xong một thanh máu của boss:
- Mọi đạn trên màn hình sẽ bị xóa.
- Bạn sẽ nhận được một **Star Item** để tăng điểm hoặc hiệu ứng.  
![](SDLGame1/Preview/star.png)

---

### **Spellcard**

- Boss sẽ sử dụng **Spellcard** – những đòn tấn công đặc biệt có giới hạn thời gian.  
- Nếu bạn tiêu diệt boss **trong thời gian quy định**, bạn nhận thêm **điểm thưởng lớn**.  
- Nếu hết giờ mà chưa tiêu diệt xong thì chỉ chuyển pha, không có bonus.

![](SDLGame1/Preview/spellcard_showcase.png)
