#include <WiFi.h>
#include <WiFiManager.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PubSubClient.h> 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Target MQTT broker IP and topic
const char* mqtt_server = "~"; // Replace with your MQTT broker IP
const char* mqtt_topic = "~"; // Replace with your set MQTT topic

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
const long interval = 5000; 

// Callback executed when WiFiManager enters AP mode
void configModeCallback(WiFiManager *myWiFiManager) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("WiFi Timeout.");
  display.println("Connect to AP:");
  display.println(myWiFiManager->getConfigPortalSSID());
  display.print("IP: ");
  display.println(WiFi.softAPIP());
  display.display();
}

void reconnectMQTT() {
  if (!client.connected()) {
    display.fillRect(0, 45, 128, 15, SSD1306_BLACK); 
    display.setCursor(0, 45);
    display.setTextSize(1);
    display.print("MQTT: Connecting...");
    display.display();

    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      display.fillRect(0, 45, 128, 15, SSD1306_BLACK); 
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED init failed"));
    while(true) { delay(10); } 
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Connecting WiFi...");
  display.display();

  WiFi.mode(WIFI_STA); 
  WiFiManager wm;
  
  // Set connection parameters
  wm.setConnectTimeout(20); // Timeout for connecting to saved WiFi (seconds)
  wm.setConfigPortalTimeout(180); // Timeout for the AP portal (seconds)
  wm.setAPCallback(configModeCallback); // Update display when AP starts

  bool res = wm.autoConnect("TempMonitor_Setup"); 

  if(!res) {
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Setup Failed.");
    display.println("Rebooting...");
    display.display();
    delay(3000);
    ESP.restart();
  } 

  sensors.begin();
  client.setServer(mqtt_server, 1883);

  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("WiFi Connected!");
  display.display();
  delay(1000);
}

void loop() {
  if (!client.connected()) { 
    reconnectMQTT(); 
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > interval) {
    lastMsg = now;
    
    sensors.requestTemperatures(); 
    float tempF = sensors.getTempFByIndex(0);

    display.clearDisplay();
    
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("WiFi: Connected");
    
    display.setCursor(0, 10);
    if(client.connected()) { 
      display.println("MQTT: Online"); 
    } else { 
      display.println("MQTT: Offline"); 
    }

    display.setCursor(0, 25);
    display.setTextSize(2);

    if(tempF <= -100) { 
      display.print("SENSOR ERR");
    } else {
      display.print(tempF, 1); 
      display.print(" F");

      if(client.connected()) {
        char tempString[8];
        dtostrf(tempF, 1, 2, tempString);
        client.publish(mqtt_topic, tempString);
      }
    }
    
    display.display();
  }
}