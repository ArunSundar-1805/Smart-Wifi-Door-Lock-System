# Smart WiFi Door Lock System (ESP32-CAM)

A Smart Door Lock system using an ESP32-CAM module, remotely controlled via a Telegram Bot. This project enables you to lock and unlock your door and capture images in real-time using your smartphone.

---

## Features

- Lock and unlock the door with Telegram commands.
- Send live photos from the camera to Telegram.
- Secure communication using WiFi and HTTPS.
- Visual status indication using LEDs.
- Minimal hardware requirements.

---

## Components Used

- ESP32-CAM (AI-Thinker module)
- USB-to-Serial adapter (for programming)
- LEDs for door lock/unlock status
- Flash LED (built-in on ESP32-CAM)
- Telegram account and Bot

---

## How It Works

1. ESP32-CAM connects to a WiFi network.
2. The device interacts with a Telegram bot via HTTPS requests.
3. Users send commands through Telegram:
   - `/start` – Shows available commands.
   - `/photo` – Captures and sends a photo from the camera.
   - `/unlock` – Unlocks the door and turns on the green LED.
   - `/lock` – Locks the door and turns on the red LED.
4. Unauthorized users receive an "Unauthorized user" message.

---

## Setup Instructions

### 1. Create a Telegram Bot

- Search for [BotFather](https://telegram.me/BotFather) in Telegram.
- Use `/newbot` and follow the instructions to create a bot.
- Copy the Bot Token and paste it in the code.

### 2. Get Your Chat ID

- Start a chat with your bot.
- Use a service like [getIDs bot](https://telegram.me/getidsbot) to find your chat ID.
- Replace `chatId` in the code.

### 3. Configure the Code

- Replace WiFi credentials:
  ```cpp
  const char* ssid = "YOUR_WIFI_SSID";
  const char* password = "YOUR_WIFI_PASSWORD";
  ```
- Replace Telegram credentials:
  ```cpp
  String chatId = "YOUR_CHAT_ID";
  String BOTtoken = "YOUR_BOT_TOKEN";
  ```

### 4. Install Dependencies

Install these libraries via Arduino Library Manager or PlatformIO:
- WiFi
- WiFiClientSecure
- UniversalTelegramBot
- ArduinoJson

### 5. Upload the Code

- Use PlatformIO or Arduino IDE in VS Code.
- Select the correct board: ESP32-CAM.
- Connect via USB-to-Serial adapter.
- Upload code and use Serial Monitor for debugging.

---

## Security Considerations

- Do **not** expose credentials in public repositories.
- Use environment variables or `.gitignore` to exclude sensitive data for production deployments.

---

## Folder Structure

```
smart-door-lock/
├── smart_door_lock.ino   # Main Arduino sketch
├── README.md             # Documentation
├── .gitignore            # (Optional) Exclude sensitive files
```

---

## Usage

- Send `/start` to your bot for available commands.
- Send `/photo` to receive a live image from the camera.
- Send `/unlock` or `/lock` to control the door.

---

## Future Improvements

- Add authentication layers for enhanced security.
- Log user activity.
- Integrate with home automation systems.
- Encrypt stored credentials.

---
