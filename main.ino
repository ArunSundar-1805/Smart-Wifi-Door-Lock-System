#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Wi-Fi credentials
const char* ssid = ".";                // Replace with your WiFi SSID
const char* password = "Arun2005";     // Replace with your WiFi password

// Telegram Bot credentials
String chatId = "5519682539";
String BOTtoken = "8195137245:AAGy_zLPvBK2-sOOg-nEhjd9SLh78fUU8w0";

WiFiClientSecure clientTCP;
UniversalTelegramBot bot(BOTtoken, clientTCP);

// GPIO Pins
#define GREEN_LED  12  // Door Unlocked
#define RED_LED    14  // Door Locked
#define FLASH_LED   4  // Flash LED (built-in on ESP32-CAM board)

// CAMERA_MODEL_AI_THINKER GPIOs
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

int lockState = 0; // 0 = locked, 1 = unlocked

const unsigned long BOT_MTBS = 1000; // Interval for checking Telegram messages
unsigned long bot_lasttime = 0;

// Send photo to Telegram
String sendPhotoTelegram() {
  const char* myDomain = "api.telegram.org";
  String response = "";

  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return "Camera capture failed";
  }

  if (clientTCP.connect(myDomain, 443)) {
    Serial.println("Connected to Telegram");

    String head = "--IotCircuitHub\r\nContent-Disposition: form-data; name=\"chat_id\"\r\n\r\n" + chatId +
                  "\r\n--IotCircuitHub\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"photo.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--IotCircuitHub--\r\n";

    uint16_t imageLen = fb->len;
    uint16_t totalLen = imageLen + head.length() + tail.length();

    clientTCP.println("POST /bot" + BOTtoken + "/sendPhoto HTTP/1.1");
    clientTCP.println("Host: " + String(myDomain));
    clientTCP.println("Content-Length: " + String(totalLen));
    clientTCP.println("Content-Type: multipart/form-data; boundary=IotCircuitHub");
    clientTCP.println();
    clientTCP.print(head);

    uint8_t *fbBuf = fb->buf;
    for (size_t n = 0; n < fb->len; n += 1024) {
      size_t chunkSize = (n + 1024 < fb->len) ? 1024 : (fb->len - n);
      clientTCP.write(fbBuf, chunkSize);
      fbBuf += chunkSize;
    }
    clientTCP.print(tail);
    esp_camera_fb_return(fb);

    while (clientTCP.connected()) {
      if (clientTCP.available()) {
        response = clientTCP.readString();
        break;
      }
    }
    clientTCP.stop();
    Serial.println(response);
  } else {
    Serial.println("Connection to Telegram failed");
    response = "Connection failed";
  }

  return response;
}

// Unlock the door
String unlockDoor() {
  if (lockState == 0) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    lockState = 1;
    return "🔓 Door Unlocked.\nSend /lock to lock.";
  } else {
    return "Door is already unlocked.";
  }
}

// Lock the door
String lockDoor() {
  if (lockState == 1) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    lockState = 0;
    return "🔒 Door Locked.\nSend /unlock to unlock.";
  } else {
    return "Door is already locked.";
  }
}

// Handle Telegram messages
void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String text = bot.messages[i].text;
    String chat_id = bot.messages[i].chat_id;

    if (chat_id != chatId) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    if (text == "/start") {
      bot.sendMessage(chatId,
        "Welcome to Smart Door Lock 🚪\n"
        "/photo - Take Photo\n"
        "/unlock - Unlock Door\n"
        "/lock - Lock Door",
        "");
    } else if (text == "/photo") {
      bot.sendMessage(chatId, "📸 Capturing photo...", "");
      digitalWrite(FLASH_LED, HIGH);
      delay(200);
      sendPhotoTelegram();
      digitalWrite(FLASH_LED, LOW);
    } else if (text == "/unlock") {
      bot.sendMessage(chatId, unlockDoor(), "");
    } else if (text == "/lock") {
      bot.sendMessage(chatId, lockDoor(), "");
    }
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout detector
  Serial.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(FLASH_LED, OUTPUT);

  // Default to locked state
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  clientTCP.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected: " + WiFi.localIP().toString());

  // Camera configuration
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x", err);
    delay(1000);
    ESP.restart();
  }
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    if (numNewMessages > 0) {
      handleNewMessages(numNewMessages);
    }
    bot_lasttime = millis();
  }
}
