/*
  文件名称：softap.ino
  功能：ESP8266 建立热点
  作者：www.doit.am
  日期：2017-2
  版本：1.0
*/

#include <ESP8266WiFi.h>
/* 建立Wifi热点并设置一个Web服务 */
#include <ESP8266WebServer.h>

/*自定义AP的SSID名称和接入密码 */
//const char *ssid = "TestingAP";
const char *ssid = "ZhangJiaChen";
const char *password = "12345678";
//声明web服务对象server和端口80
ESP8266WebServer server(80);

/*在web浏览器中输入 http://192.168.4.1来连接到AP的web模式，
这个地址是预先在头文件中定义好的，可以在其中修改 */
//设定web服务的成功响应信息
void handleRoot() {
  server.send(200, "text/html", "<h1 style='text-align:center;font-size:48px;'>张嘉辰<br/>你是个小天才</h1>");
}
void setup() {
  delay(1000);
  Serial.begin(74880);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.print("Configuring access point...");
  /*如果使用开放式AP模式，可以把下面语句的password参数省略 */
  WiFi.softAP(ssid, password);
//获取AP的IP地址，存入变量myIP
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("\nAP IP address: ");
  Serial.println(myIP);
  //调用on方法，给客户端发送响应信息，启动web服务，并在串口回显
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}
//循环等待处理客户端的连接
void loop() {
  server.handleClient();
}

