//=====================================================================
// Project: Lab Power Monitor Display
// File: power_display_mqtt_tft.ino
//
// Author: Hylan
// Date: 2026-08-03
//
// Description:
// This sketch runs on an ESP32-based TFT display for a lab power monitor.
// It connects to Wi-Fi, subscribes to MQTT topics from the power monitor,
// and displays real-time electrical values including voltage, frequency,
// current, power, total watts, energy, and microcontroller temperature.
//
// The display also includes status indicators for Wi-Fi, MQTT activity,
// and a live clock synchronized with NTP time. Incoming MQTT messages are
// parsed in a callback and stored as strings for screen updates.
//
// Features:
// - Wi-Fi connection
// - MQTT subscribe and display updates
// - TFT_eSPI graphics output
// - Live time display using NTP
// - Microcontroller temperature readout
// - Connection and activity status indicators
//
// Hardware:
// - ESP32
// - TFT display using TFT_eSPI
// - MQTT broker on local network
// - Power monitor publishing sensor data
//=====================================================================

#include <WiFi.h>
#include <PubSubClient.h>
#include <TFT_eSPI.h>
#include <time.h>


// -------Time Clock setup-------
const int  TIMEZONE_OFFSET = -6;

// -------- WiFi --------
const char* ssid = "Hawkins WIFI";
const char* password = "1167Wedwards";

// -------- MQTT --------
const char* mqtt_server = "192.168.1.218";
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// -------- Display Pins --------
// Change if your board uses different pins
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   -1
#define TFT_BL    21

TFT_eSPI tft = TFT_eSPI();
//bool mqttActivity = false;
unsigned long lastMQTT = 0;
unsigned long mqttReconnects = 0;
unsigned long wifiReconnects = 0;


// -------- Values received from MQTT --------

String voltage1  = "---";
String voltage2  = "---";
String frequency = "---";
String current1   = "---";
String current2   = "---";
String power1     = "---";
String power2     = "---";
String energy     = "---";
String total      = "---";
String micro_temp = "---";

float microTemp = 0.00;

// =========================================
// MQTT Callback
// Runs whenever a message arrives
// =========================================

void callback(char* topic, byte* payload, unsigned int length)
{      // mqttActivity = true;
     lastMQTT = millis();
  //Serial.print("MQTT: ");   
 // Serial.println(topic);
  String msg;

  for (int i = 0; i < length; i++)
  {
    msg += (char)payload[i];
  }

  if (String(topic) == "powermonitor/voltage_1")
    voltage1 = msg;

   if (String(topic) == "powermonitor/voltage_2")
    voltage2 = msg;

  if (String(topic) == "powermonitor/frequency")
    frequency = msg;

  if (String(topic) == "powermonitor/current_1")
    current1 = msg;

  if (String(topic) == "powermonitor/current_2")
    current2 = msg;

  if (String(topic) == "powermonitor/power_1")
    power1 = msg;

  if (String(topic) == "powermonitor/power_2")
    power2 = msg;
  if (String(topic) == "powermonitor/energy")
    energy = msg;
  if (String(topic) == "powermonitor/power_total")
    total = msg;
  if (String(topic) == "powermonitor/micro_temp")
    microTemp = msg .toFloat();

//Serial.print("Topic: ");
//Serial.print(topic);
//Serial.print("  Value:  ");
//Serial.println(msg);
   
}

// ============Reconnect MQTT=============================================

void reconnectMQTT()
{
  while (!mqttClient.connected())
  {
    Serial.println("Connecting MQTT...");

    if (mqttClient.connect("PowerDisplay_01","myuser","2N3904"))
    {
      Serial.println("MQTT Connected");

      mqttClient.subscribe("powermonitor/voltage_1");
      mqttClient.subscribe("powermonitor/voltage_2");
      mqttClient.subscribe("powermonitor/frequency");
      mqttClient.subscribe("powermonitor/current_1");
      mqttClient.subscribe("powermonitor/current_2");
      mqttClient.subscribe("powermonitor/power_1");
      mqttClient.subscribe("powermonitor/power_2");
      mqttClient.subscribe("powermonitor/energy");
      mqttClient.subscribe("powermonitor/power_total");
      mqttClient.subscribe("powermonitor/micro_temp");
    }
    else
    {
      Serial.print("Connect failed, state=");
      Serial.println(mqttClient.state());
      delay(1000);
    }
  }
}

// ==============DrawScreen=========================================================

void drawScreen()
{  //====== System Indicator flashes red every screen write==============
  tft.fillRect(0,5,10,10, TFT_RED);
  delay(50);
  tft.fillRect(0,5,10,10, TFT_BLACK);
  //==========WiFi status indictor is green when wifi is connected  red when lost========
  if (WiFi.status() == WL_CONNECTED)
  {
    tft.fillRect(0,40,10,10, TFT_GREEN);

  }
else
{
  tft.fillRect(0,40,10,10, TFT_RED);
}
  

 tft.setTextColor(TFT_BLUE);
  tft.setTextSize(2);

  String lastVoltage1="";
  if(voltage1 !=lastVoltage1)
  {tft.fillRect(100,73,80,20,TFT_BLACK); 
  tft.setCursor(100,75);
  tft.print(voltage1);
  lastVoltage1=voltage1;
  }
 String lastVoltage2="";
  if(voltage2 !=lastVoltage2)
  {tft.fillRect(200,73,80,20,TFT_BLACK); 
  tft.setCursor(200,75);
  tft.print(voltage2);
  lastVoltage2=voltage2;
  }
   tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  String lastFrequency="";
  if(frequency !=lastFrequency)
  {tft.fillRect(100,97,80,20,TFT_BLACK); 
  tft.setCursor(100,100);
  tft.print(frequency);
  lastFrequency=frequency;
  }

   tft.setTextColor(TFT_RED);
  tft.setTextSize(2);
 String lastCurrent1="";
  if(current1 !=lastCurrent1)
  {tft.fillRect(100,123,80,20,TFT_BLACK); 
  tft.setCursor(100,125);
  tft.print(current1);
  lastCurrent1=current1;
  }

String lastCurrent2="";
  if(current2 !=lastCurrent2)
  {tft.fillRect(200,123,80,20,TFT_BLACK); 
  tft.setCursor(200,125);
  tft.print(current2);
  lastCurrent2=current2;
  }

 tft.setTextColor(TFT_ORANGE);
  tft.setTextSize(2);

 String lastPower1="";
  if(power1 !=lastPower1)
  {tft.fillRect(100,143,80,20,TFT_BLACK); 
  tft.setCursor(100,150);
  tft.print(power1);
  lastPower1=power1;
  }
String lastPower2="";
  if(power2 !=lastPower2)
  {tft.fillRect(200,143,80,20,TFT_BLACK); 
  tft.setCursor(200,150);
  tft.print(power2);
  lastPower2=power2;
  }

 tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);

String lasttotal="";
  if(total !=lasttotal)
  {tft.fillRect(200,173,80,20,TFT_BLACK); 
  tft.setCursor(200,175);
  tft.print(total);
  lasttotal=total;
  }

String lastenergy="";
  if(energy !=lastenergy)
  {tft.fillRect(200,208,80,20,TFT_BLACK); 
  tft.setCursor(200,210);
  tft.print(energy);
  lastenergy=energy;
  }
// =====================MQTT Status display blue when recieving data yellow when no updates============
if (millis() - lastMQTT < 70)
{
  tft.fillRect(0,20,10,10, TFT_BLUE);
}
else 
{
  tft.fillRect(0,20,10,10, TFT_YELLOW);
}
}

// ========================================= setup ===============================================================


void setup()
{
  Serial.begin(115200);
   pinMode(TFT_BL, OUTPUT);
   digitalWrite(TFT_BL, HIGH);
  SPI.begin(14, 12, 13, 15);
  tft.begin();
   tft.setRotation(3);
  Serial.print("Display ID = 0x");
  Serial.println(tft.readcommand8(0x04), HEX);
  Serial.println("TFT Started");

   tft.fillScreen(TFT_BLACK);


  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(3);
  tft.setCursor(90,5);
  tft.println("LAB POWER");

  tft.setTextSize(2);
  tft.setCursor(20,75);
  tft.println("VOLTS: ");

  tft.setCursor(20,100);
  tft.println("HERTZ:");

  tft.setCursor(20,125);
  tft.println("AMPS:");

  tft.setCursor(20,150);
  tft.println("WATTS:");

  tft.setCursor(20,175);
  tft.println("Total Watts:");

  tft.setCursor(20,210);
  tft.println("Energy KWh:");

 Serial.print("Width = ");
  Serial.println(tft.width());
  Serial.print("Height = ");
  Serial.println(tft.height());
  

 
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
// Central Time with automatic Daylight Saving Time
    configTime(-6 * 3600, 3600, "pool.ntp.org", "time.nist.gov");

    // Optional: wait until time has been obtained
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
      delay(500);
    }
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);


  
 
}

// ========================================= Loop ===============================================
void loop()
{
  mqttClient.loop();
  if (!mqttClient.connected())
  {
    Serial.print("MQTT State = ");
    Serial.println(mqttClient.state());
    Serial.print("WiFi Status = ");
    Serial.println(WiFi.status());
    Serial.print("Time since last MQTT =");
    Serial.println(millis() - lastMQTT);
   mqttReconnects++;
    Serial.print("MQTT reconnect #");
    Serial.println(mqttReconnects);

    reconnectMQTT();
  }
if (WiFi.status() != WL_CONNECTED)
{ 
  wifiReconnects++;
  Serial.print("WiFi  reconnects #");
  Serial.println(wifiReconnects);
  
}
  drawScreen();
    
//wifi clock=======================================================================================
//ESP32 Wi-Fi Clock (HH:MM) for Lab Power Monitor

    struct tm timeinfo;

    if (getLocalTime(&timeinfo)) {

        char timeString[6];      // "HH:MM"
        strftime(timeString, sizeof(timeString), "%H:%M", &timeinfo);

        // Draw the time in the upper-right corner
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        tft.setCursor(260, 5);      // Adjust for your display
        tft.print(timeString);

        float tempF = microTemp * 9.0 / 5.0 + 32.0;
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.setCursor(260, 30);      // Adjust for your display
        tft.printf("%.1fF",tempF);

        // Draw your temperature just below the time
        // Example:
        // tft.setCursor(235, );
        // tft.printf("%.1fF", temperatureF);
    }




  //delay(500);
  delay(50);
}