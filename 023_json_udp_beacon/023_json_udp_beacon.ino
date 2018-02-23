// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License
//
// This example shows how to send a JSON document to a UDP socket.
// At regular interval, it sends a UDP packet that contains the status of
// analog and digital pins.
// The JSON document looks like the following:
// {
//   "analog": [ 0, 1, 2, 3, 4, 5 ],
//   "digital": [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 ]
// }
//
// If you want to test this program, you need to be able to receive the UDP
// packets.
// For example, you can run netcat on your computer
// $ ncat -ulp 8888
// See https://nmap.org/ncat/

#include <ArduinoJson.h>
#include <ESP8266WiFi.h> //这是支持Wifi基本功能的头文件
#include <WiFiUdp.h>

const char* ssid = "JYWHD-26";  //所接入的无线网络名称
const char* password = "jc18806863286";  

IPAddress remoteIp(192, 168, 26, 103);  // <- EDIT!!!!
unsigned short remotePort = 8888;
unsigned short localPort = 8888;
WiFiUDP udp;

void setup() {
  // Initialize serial port
  Serial.begin(74880);
  while (!Serial) continue;

  WiFi.disconnect();//断开之前的连接
  WiFi.mode(WIFI_STA);  //将此Wifi芯片设为STA模式
  WiFi.begin(ssid, password); //接入到无线环境热点
  Serial.println("\nConnecting to WiFi");
 
 //如果Wifi状态不是WL_CONNECTED，则表示连接失败
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); //在串口窗口输出.，表示连接进度信息
    delay(1000); //延时1秒
  }

  WiFi.printDiag(Serial);

  // Enable UDP
  udp.begin(localPort);
}

void loop() {
  // Allocate JsonBuffer
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<500> jsonBuffer;

  // Create the root object
  JsonObject& root = jsonBuffer.createObject();

  // Create the "analog" array
  JsonArray& analogValues = root.createNestedArray("analog");
  for (int pin = 0; pin < 6; pin++) {
    // Read the analog input
    int value = analogRead(pin);

    // Add the value at the end of the array
    analogValues.add(value);
  }

  // Create the "digital" array
  JsonArray& digitalValues = root.createNestedArray("digital");
  for (int pin = 0; pin < 14; pin++) {
    // Read the digital input
    int value = digitalRead(pin);

    // Add the value at the end of the array
    digitalValues.add(value);
  }

  // Log
  Serial.println();
  Serial.print(F("Sending to "));
  Serial.print(remoteIp);
  Serial.print(F(" on port "));
  Serial.println(remotePort);
  root.printTo(Serial);

  // Send UDP packet
  udp.beginPacket(remoteIp, remotePort);
  root.printTo(udp);
  udp.println();
  udp.endPacket();

  // Wait
  delay(10000);
}

// See also
// --------
//
// The website arduinojson.org contains the documentation for all the functions
// used above. It also includes an FAQ that will help you solve any
// serialization problem.
// Please check it out at: https://arduinojson.org/
//
// The book "Mastering ArduinoJson" contains a tutorial on serialization.
// It begins with a simple example, then adds more features like serializing
// directly to a file or any stream.
// Please check it out at: https://arduinojson.org/book/
