# Smart Door Lock with ESP32-CAM

A **Smart Door Lock** system using the **ESP32-CAM** module, controlled remotely via **Telegram Bot**. This project allows you to lock/unlock a door and capture images in real-time using your phone.

## ✅ Features

- ✅ Lock and unlock the door with Telegram commands.
- ✅ Send live photos from the camera to Telegram.
- ✅ Secure communication using WiFi and HTTPS.
- ✅ Visual status indication using LEDs.
- ✅ Built on ESP32-CAM hardware with minimal components.

## 📦 Components Used

- ESP32-CAM (AI-Thinker module)
- USB-to-Serial adapter (for programming)
- LEDs for door lock/unlock status
- Flash LED (built-in on ESP32-CAM)
- Telegram account and Bot

## 🚀 How it Works

1. ESP32-CAM connects to a WiFi network.
2. It interacts with a Telegram bot using HTTPS requests.
3. Users send commands through Telegram to:
   - `/start` – Show available commands.
   - `/photo` – Capture and send a photo from the camera.
   - `/unlock` – Unlock the door and turn on the green LED.
   - `/lock` – Lock the door and turn on the red LED.
4. Unauthorized users receive an "Unauthorized user" message.

## ⚙ Setup Instructions

### 1️⃣ Create a Telegram Bot
1. Search for [BotFather](https://telegram.me/BotFather) on Telegram.
2. Use `/newbot` and follow instructions to create a bot.
3. Copy the **Bot Token** and paste it in the code (`BOTtoken`).

### 2️⃣ Get your Chat ID
1. Start a chat with your bot.
2. Use a bot or service like [getIDs bot](https://telegram.me/getidsbot) to find your chat ID.
3. Replace `chatId` in the code.

### 3️⃣ Configure the Code
- Replace the WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
Replace the Telegram credentials:

cpp
Copy code
String chatId = "YOUR_CHAT_ID";
String BOTtoken = "YOUR_BOT_TOKEN";
4️⃣ Install Dependencies
Install the following libraries via Arduino Library Manager or PlatformIO:

WiFi

WiFiClientSecure

UniversalTelegramBot

ArduinoJson

5️⃣ Upload the Code
Use PlatformIO or Arduino IDE in VS Code.

Select the correct board: ESP32-CAM.

Connect the board via USB-to-Serial adapter.

Upload the code and open the Serial Monitor to see debug messages.

🔐 Security Considerations
The bot token and WiFi password are hardcoded for simplicity.

Do NOT expose this information in public repositories.

Use environment variables or .gitignore to exclude sensitive data in real deployments.

📂 Folder Structure
bash
Copy code
smart-door-lock/
├── smart_door_lock.ino    # Main Arduino sketch file
├── README.md             # Project documentation
├── .gitignore            # (Optional) Exclude sensitive files
📷 Usage
Send /start to your bot to see available commands.

Send /photo to receive a live image from the camera.

Send /unlock or /lock to control the door.

📦 Future Improvements
Add authentication layers for more security.

Log user activity.

Integrate with home automation systems.

Encrypt stored credentials.
