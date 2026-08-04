
 Project: Lab Power Monitor Display
 File: power_display_mqtt_tft.ino

 Author: Hylan
 Date: 2026-08-03

 Description:
 This sketch runs on an ESP32-based TFT display for a lab power monitor.
 It connects to Wi-Fi, subscribes to MQTT topics from the power monitor,
 and displays real-time electrical values including voltage, frequency,
 current, power, total watts, energy, and microcontroller temperature.

 The display also includes status indicators for Wi-Fi, MQTT activity,
 and a live clock synchronized with NTP time. Incoming MQTT messages are
 parsed in a callback and stored as strings for screen updates.

 Features:
 - Wi-Fi connection
 - MQTT subscribe and display updates
 - TFT_eSPI graphics output
 - Live time display using NTP
 - Microcontroller temperature readout
 - Connection and activity status indicators

 Hardware:
 - ESP32
 - TFT display using TFT_eSPI
 - MQTT broker on local network
 - Power monitor publishing sensor data

