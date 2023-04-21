#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define SSID_AP            "ESP8266_WiFi_Car"       
#define PASSWORD_AP        "12345678"               

// motor settings
#define RIGHT_MOTOR_PIN1  5                         
#define RIGHT_MOTOR_PIN2  4                         
#define LEFT_MOTOR_PIN1   14                        
#define LEFT_MOTOR_PIN2   12                        
ESP8266WebServer server(80);
int car_mode = 0;                             
void setup() {
  
  Serial.begin(115200);
  Serial.println("ESP8266 Wifi Car");
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN2, OUTPUT);
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN2, OUTPUT);
  car_control(); // stop the car
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID_AP, PASSWORD_AP);

  server.on("/", HTTP_GET, handle_OnConnect);
  server.on("/forward", HTTP_GET, handle_forward);
  server.on("/backward", HTTP_GET, handle_backward);
  server.on("/left", HTTP_GET, handle_left);
  server.on("/right", HTTP_GET, handle_right);
  server.on("/stop", HTTP_GET, handle_stop);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("ESP8266 car server started.");
}

void loop() { 
  server.handleClient();
  car_control();
}
void handle_OnConnect() {
  car_mode = 0;
  Serial.println("Client connected");
  server.send(200, "text/html", SendHTML());
}
void handle_stop() {
  car_mode = 0;
  Serial.println("Stopped");
  server.send(200, "text/html", SendHTML());
}
void handle_forward() {
  car_mode = 1;
  Serial.println("Go forward...");
  server.send(200, "text/html", SendHTML());
}

void handle_backward() {
  car_mode = 2;
  Serial.println("Go backward...");
  server.send(200, "text/html", SendHTML());
}

void handle_left() {
  car_mode = 3;
  Serial.println("Turn left...");
  server.send(200, "text/html", SendHTML());
}


void handle_right() {
  car_mode = 4;
  Serial.println("Turn right...");
  server.send(200, "text/html", SendHTML());
}

void handle_NotFound() {
  car_mode = 0;
  Serial.println("Page error");
  server.send(404, "text/plain", "Not found");
}

void car_control() {
  switch (car_mode) {
    case 0: // stop car
      digitalWrite(RIGHT_MOTOR_PIN1, LOW);
      digitalWrite(RIGHT_MOTOR_PIN2, LOW);
      digitalWrite(LEFT_MOTOR_PIN1, LOW);
      digitalWrite(LEFT_MOTOR_PIN2, LOW);
      break;
    case 1: // go forward
      digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
      digitalWrite(RIGHT_MOTOR_PIN2, LOW);
      digitalWrite(LEFT_MOTOR_PIN1, HIGH);
      digitalWrite(LEFT_MOTOR_PIN2, LOW);
      break;
    case 2: // go backward
      digitalWrite(RIGHT_MOTOR_PIN1, LOW);
      digitalWrite(RIGHT_MOTOR_PIN2,HIGH);
      digitalWrite(LEFT_MOTOR_PIN1, LOW);
      digitalWrite(LEFT_MOTOR_PIN2, HIGH);
      break;
    case 4: // turn right
      digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
      digitalWrite(RIGHT_MOTOR_PIN2, LOW);
      digitalWrite(LEFT_MOTOR_PIN1, LOW);
      digitalWrite(LEFT_MOTOR_PIN2, LOW);
      break;
    case 3: // turn left
      digitalWrite(RIGHT_MOTOR_PIN1, LOW);
      digitalWrite(RIGHT_MOTOR_PIN2, LOW);
      digitalWrite(LEFT_MOTOR_PIN1, HIGH);
      digitalWrite(LEFT_MOTOR_PIN2, LOW);

  }
}

// output HTML web page for user
String SendHTML() {
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<title>ESP8266 WiFi Car</title>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>body {background-color: Moccasin;} h1 {color: SaddleBrown;} h2 {color: Olive;}</style>";
  html += "</head>";
  html += "<body>";
  html += "<div align=\"center\">";
  html += "<h1>ESP8266 WiFi Car</h1>";
  html += "<h2>Press \"stop\" after the server has been reset</h2>";
  html += "<br>\n";
  html += "<form method=\"GET\">";
  html += "<input type=\"button\" value=\"Go forward\" onclick=\"window.location.href='/forward'\">";
  html += "<br><br>\n";
  html += "<input type=\"button\" value=\"Go backward\" onclick=\"window.location.href='/backward'\">";
  html += "<br><br>\n";
  html += "<input type=\"button\" value=\"Turn left\" onclick=\"window.location.href='/left'\">";
  html += "<br><br>\n";
  html += "<input type=\"button\" value=\"Turn right\" onclick=\"window.location.href='/right'\">";
  html += "<br><br>\n";
  html += "<input type=\"button\" value=\"Car stop\" onclick=\"window.location.href='/stop'\">";
  html += "</form>\n";
  html += "</div>\n";
  html += "</body>\n";
  html += "</html>\n";
  return html;
}
