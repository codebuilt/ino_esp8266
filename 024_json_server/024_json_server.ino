// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License
//
// This example shows how to implement an HTTP server that sends JSON document
// in the responses.
// It uses the Ethernet library but can be easily adapted for Wifi.
//
// It sends the value of the analog and digital pins.
// The JSON document looks like the following:
// {
//   "analog": [ 0, 1, 2, 3, 4, 5 ],
//   "digital": [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 ]
// }

#include <ArduinoJson.h>
#include <ESP8266WiFi.h> //这是支持Wifi基本功能的头文件

const char* ssid = "JYWHD-26";  //所接入的无线网络名称
const char* password = "jc18806863286";  

WiFiServer server(80);

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

  // Start to listen
  server.begin();

  Serial.println(F("Server is ready."));
  Serial.print(F("Please connect to http://"));
  Serial.println(WiFi.localIP());
}

void loop() {
  // Wait for an incomming connection
  WiFiClient client = server.available();

  // Do we have a client?
  if (!client) return;

  Serial.println(F("New client"));

  // Read the request (we ignore the content in this example)
  //while (client.available()) client.read();

  // Wait until the client sends some data  
  while(!client.available()) delay(1);  // 一定要加上这句，否则客户端可能无法正常读取响应！
  while (client.available()) Serial.write(client.read());

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

  Serial.print(F("Sending: "));
  root.printTo(Serial);
  Serial.println();

  // Write response headers
  //client.println("HTTP/1.0 200 OK");
  //client.println("Content-Type: application/json");
  //client.println("Connection: close");
  //client.println();

  client.print("HTTP/1.0 200 OK\r\n");
  client.print("Content-Type: application/json\r\n");
  client.print("Connection: close\r\n");
  client.print("\r\n");

  // Write JSON document
  root.prettyPrintTo(client);

  // Disconnect
  client.stop();
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
// directly to a file or an HTTP client.
// Please check it out at: https://arduinojson.org/book/
