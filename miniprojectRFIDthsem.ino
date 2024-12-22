#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <MFRC522.h>
#include <SPI.h>
#include <time.h>
#include <Ticker.h>
// Wi-Fi credentials
const char* ssid = "Anil_Desktop";  // Replace with your Wi-Fi SSID
const char* password = "Kumaranil@400";  // Replace with your Wi-Fi password

// Pins for RFID module
#define RST_PIN 17   // Reset pin for MFRC522 (ESP32 GPIO5)
#define SS_PIN 5     // Slave select pin for MFRC522 (ESP32 GPIO4)
#define RLED 12
#define YLED 14
#define GLED 26
// Pin for controlling the LED (e.g., GPIO 2)
#define LED_PIN 2
#define PERIOD 500
 Ticker ticker,ticker1;
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// Variable to store the latest RFID data
String latestRFID = "";  // Initialize with a default message
bool ledState = false;  // LED state (false = OFF, true = ON)

// Create an instance of the server
AsyncWebServer server(80);  // HTTP server on port 80

// Function to get the current timestamp
String getTimestamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "Error: Time not available";
  }
  
  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timestamp);
}
void NoConnectBlink()
{
  byte r_st=digitalRead(RLED);
  digitalWrite(RLED,!r_st);
}
void setup() {
  // Initialize Serial Monitor
    pinMode(RLED,OUTPUT);
    pinMode(YLED,OUTPUT);
    pinMode(GLED,OUTPUT);
    digitalWrite(RLED,0);
    digitalWrite(YLED,0);
    digitalWrite(GLED,0);
    ticker.attach(1.0,NoConnectBlink);
  Serial.begin(115200);
  // explicitly set mode, esp defaults to STA+AP

  // Initialize RFID reader
  SPI.begin();
  mfrc522.PCD_Init();

  // Initialize LED pin as output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Ensure LED is OFF initially

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    digitalWrite(RLED,HIGH);
    delay(500);
    digitalWrite(RLED,LOW);
  }
  digitalWrite(RLED,LOW);
  ticker.detach();
  digitalWrite(YLED,HIGH);
  delay(2000);
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // Print ESP32 IP address
  digitalWrite(YLED,LOW);
  // Synchronize time from NTP server
  configTime(0, 0, "pool.ntp.org");  // Use pool.ntp.org for time synchronization

  // Define route for GET requests
  server.on("/getRfidData", HTTP_GET, [](AsyncWebServerRequest *request){
    String rfidData = readRFID() ; // Get the latest RFID data
    String deviceID = "DEVICE123";  // Example static Device ID
    String timestamp = getTimestamp();  // Get the current timestamp

    String response = "{\"rfid\":\"" +latestRFID + "\", \"device_id\":\"" + deviceID + "\", \"timestamp\":\"" + timestamp + "\"}";
    
    request->send(200, "application/json", response);  // Send RFID, Device ID, and Timestamp as JSON
  });

  // Define route for POST requests to control the LED
  server.on("/controlLED", HTTP_POST, [](AsyncWebServerRequest *request){
    String ledCommand = "";  // Variable to store the LED control command

    // Read "led" parameter from POST request body (if available)
    if (request->hasParam("led", true)) {
      ledCommand = request->getParam("led", true)->value();
    }

    // Control LED based on received command
    if (ledCommand == "ON") {
      ledState = true;
      digitalWrite(LED_PIN, HIGH);  // Turn LED ON
    } else if (ledCommand == "OFF") {
      ledState = false;
      digitalWrite(LED_PIN, LOW);  // Turn LED OFF
    } else {
      // If command is invalid, return an error message
      request->send(400, "application/json", "{\"message\":\"Invalid LED command. Use 'ON' or 'OFF'.\"}");
      return;
    }

    // Get the current timestamp
    String timestamp = getTimestamp();
    // Prepare response with the current LED state and timestamp
    String message = ledState ? "LED turned ON" : "LED turned OFF";
    String response = "{\"led_state\":\"" + message + "\", \"timestamp\":\"" + timestamp + "\"}";

    request->send(200, "application/json", response);  // Send response with LED state and timestamp
  });

  // Start the server
  server.begin();
}

void loop() {
  // Look for new RFID cards and store the latest RFID data
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      String rfidData = "";
      // Read the UID of the RFID tag
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        rfidData += String(mfrc522.uid.uidByte[i], HEX);
      }

      // Store the latest RFID data
      latestRFID = rfidData;
       digitalWrite(GLED,HIGH);
       delay(1000);
      // Print the latest RFID data to Serial Monitor
      Serial.println("RFID Data: " + latestRFID);
      digitalWrite(GLED,LOW);
    }
  }
}

// Function to read RFID
String readRFID() {
  String rfidData = "";
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        rfidData += String(mfrc522.uid.uidByte[i], HEX);
      }
    }
  }
  return rfidData;
}
