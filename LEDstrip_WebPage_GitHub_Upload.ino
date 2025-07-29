// Made by Kaigo from KaigoBuilds

#include <WiFi.h>
#include <WebServer.h>
#include <FastLED.h>

// Wi-Fi credentials
const char* ssid = ""; // Add Wifi name/Id
const char* password = ""; // Add Wifi password

// LED setup
// Change based on setup
#define LED_PIN     16 
#define NUM_LEDS    10
#define BRIGHTNESS  128
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

WebServer server(80);

// HTML page stored in flash memory
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 RGB Control</title>
  <style>
    body {
      background-color: black;
      font-family: Arial;
      text-align: center;
      margin-top: 30px;
      color: white;
    }
    .grid {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 15px;
      max-width: 500px;
      margin: auto;
    }
    button {
      padding: 20px;
      font-size: 18px;
      border: 2px solid black;
      border-radius: 8px;
      color: white;
      cursor: pointer;
    }
    .red { background-color: red; }
    .green { background-color: green; }
    .blue { background-color: blue; }
    .yellow { background-color: yellow; color: black; }
    .cyan { background-color: cyan; color: black; }
    .magenta { background-color: magenta; }
    .white { background-color: white; color: black; }
    .orange { background-color: orange; }
    .purple { background-color: purple; }
    .off { background-color: gray; }
  </style>
</head>
<body>
  <h1>ESP32 RGB LED Control</h1>
  <div class="grid">
    <button class="red" onclick="fetch('/red')">Red</button>
    <button class="green" onclick="fetch('/green')">Green</button>
    <button class="blue" onclick="fetch('/blue')">Blue</button>
    <button class="yellow" onclick="fetch('/yellow')">Yellow</button>
    <button class="cyan" onclick="fetch('/cyan')">Cyan</button>
    <button class="magenta" onclick="fetch('/magenta')">Magenta</button>
    <button class="white" onclick="fetch('/white')">White</button>
    <button class="orange" onclick="fetch('/orange')">Orange</button>
    <button class="purple" onclick="fetch('/purple')">Purple</button>
  </div>
  <br>
  <button class="off" onclick="fetch('/off')">Turn Off</button>
</body>
</html>
)rawliteral";

// Set all LEDs to a color
void setColor(CRGB color) {
  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();
}

// Web handlers
void handleRoot()   { server.send(200, "text/html", htmlPage); }
void handleRed()    { setColor(CRGB::Red);     server.send(200, "text/plain", "Red ON"); }
void handleGreen()  { setColor(CRGB::Green);   server.send(200, "text/plain", "Green ON"); }
void handleBlue()   { setColor(CRGB::Blue);    server.send(200, "text/plain", "Blue ON"); }
void handleYellow() { setColor(CRGB::Yellow);  server.send(200, "text/plain", "Yellow ON"); }
void handleCyan()   { setColor(CRGB::Cyan);    server.send(200, "text/plain", "Cyan ON"); }
void handleMagenta(){ setColor(CRGB::Magenta); server.send(200, "text/plain", "Magenta ON"); }
void handleWhite()  { setColor(CRGB::White);   server.send(200, "text/plain", "White ON"); }
void handleOrange() { setColor(CRGB(255, 128, 0)); server.send(200, "text/plain", "Orange ON"); }
void handlePurple() { setColor(CRGB(128, 0, 255)); server.send(200, "text/plain", "Purple ON"); }
void handleOff()    { setColor(CRGB::Black);   server.send(200, "text/plain", "LEDs OFF"); }

void setup() {
  Serial.begin(115200);

  // Initialize FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  setColor(CRGB::Black); // Start off

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected!");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/red", handleRed);
    server.on("/green", handleGreen);
    server.on("/blue", handleBlue);
    server.on("/yellow", handleYellow);
    server.on("/cyan", handleCyan);
    server.on("/magenta", handleMagenta);
    server.on("/white", handleWhite);
    server.on("/orange", handleOrange);
    server.on("/purple", handlePurple);
    server.on("/off", handleOff);
    server.begin();
    Serial.println("HTTP server started");
  } else {
    Serial.println("\nFailed to connect to WiFi.");
  }
}

void loop() {
  server.handleClient();
}
