#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LiquidCrystal_I2C.h>
ESP8266WebServer server(80);

const char* ssid = "";
const char* password = "";

const int led = 2;
int S = 0, M = 1, H = 0;
int buttonPin = 14;
int buttonPin2 = 12;
int piezoPin = 2;
int buttonValue, buttonValue2;
int yeet = 2;
bool liga = false;

LiquidCrystal_I2C lcd(0x27,5,4);

void paginahtmlon() {
  digitalWrite(led, 1);

  String page = "<html><style>h1{font-size: 50px;color: rgb(243, 12, 12);margin: 30px;text-align: center;}"
  "body{background-image: url();background-repeat: no repeat;background-size: center; background-position: rigth center;}"
  "</style>"
  "<body>"
    "<h1>TIME DE POMODORO</h1>"
    "<button>Ligar</button>"
  "</body>"
"</html>";

  server.send(200, "text/html", page);
  digitalWrite(led, 0);
}


void paginahtmloff(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}




void setup() {
  
 lcd.init();
 lcd.backlight();
 pinMode(buttonPin, INPUT);
 pinMode(buttonPin2, INPUT);
 pinMode(piezoPin, OUTPUT);
 digitalWrite(buttonPin, HIGH);
 digitalWrite(buttonPin2, HIGH);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(buttonPin), liga_desliga, RISING);
  
  //conexão wifi
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //IMPRESSÃO MONITOR SERIAL SERVER
  Serial.println("");
  Serial.print("Conectado a rede: ");
  Serial.println(ssid);
  Serial.print("Endereço de IP: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

 //CHAMADA DA PAGINA HTML
  server.on("/", paginahtmlon);
  server.onNotFound(paginahtmloff);
  server.begin();
  Serial.println("HTTP server started");

}

void loop() {

  server.handleClient();
  buttonValue = digitalRead(buttonPin);
 buttonValue2 = digitalRead(buttonPin2);


   
    if (liga)
{
 
        lcd.setCursor(1, 0);
        lcd.print ("Pomodoro Timer");
        lcd.setCursor(6, 1);
        lcd.print(":");
        lcd.setCursor(9, 1);
        lcd.print(":");
        S--;
        delay(1000);
        if (S < 0) {
          M--;
          S = 59;
        }
       if (M < 0) {
          H--;
                M = 59;
        }
       if (H < 0) {
          if (yeet % 2 == 0) {
            lcd.setCursor(1, 0);
            lcd.print("Work  finished");
          } else {
            lcd.setCursor(1,0);
            lcd.print("Break finished");
                      }

          // Piezo
          tone(piezoPin, 261);
          delay(10000);
          noTone(piezoPin);
          if (yeet % 2 == 0) {
            H = 0; M = 5; S = 0;
            yeet++;
             } else {
            H = 0; M = 25; S = 0;
            yeet++;
          }
       }
         if (M > 9) {
          lcd.setCursor(7, 1);
          lcd.print(M);
        } else {
          lcd.setCursor(7, 1);
          lcd.print("0");
          lcd.setCursor(8, 1);
          lcd.print(M);
          lcd.setCursor(9, 1);
          lcd.print(":");
        }
        if (S > 9) {
          lcd.setCursor(10, 1);
          lcd.print(S);
        } else {
          lcd.setCursor(10, 1);
          lcd.print("0");
          lcd.setCursor(11, 1);
          lcd.print(S);
          lcd.setCursor(12, 1);
          lcd.print(" ");
        }
        if (H > 9) {
          lcd.setCursor(4, 1);
          lcd.print(H);
        } else {
          lcd.setCursor(4, 1);
          lcd.print("0");
          lcd.setCursor(5, 1);
          lcd.print(H);
          lcd.setCursor(6, 1);
          lcd.print(":");
         
        }
 }
}

ICACHE_RAM_ATTR void liga_desliga(){
    lcd.setCursor(1, 0);
    lcd.print("Press to start");
    liga = !liga;
    delay (100);
    Serial.print (liga);
   
}
