#include <ESP8266WiFi.h>

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // 2 - к TX сенсора, 3 - к RX

SoftwareSerial espSerial(0, 1); // 0 - к TX модуля, 1 - к RX

byte mac[] = {0x62, 0x01, 0x94, 0x29, 0xB7, 0xDF }; // Мак адрес

WiFiClient client;


byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};//байтовый пакет сенсора
unsigned char response[9];
unsigned int ppm = 0;

int G = 4;//диод зеленый
int R = 5;//диод красный
int B = 6; //диод голубой
int m = 0;//пришлось сделать эту переменную глобальной, из-за лупа


long long lastTime = 0;
char server[] = "site.ru"; // адресс сервера, пример

void setup() {
  pinMode (R, OUTPUT); 
  pinMode (G, OUTPUT);
  pinMode (B, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  espSerial.begin(115200);
  WiFi.begin(mac); // и этот порт я тоже стыбрил
}

void loop() {
  if (2000 < millis() - lastTime) {
    lastTime = millis();

    mySerial.write(cmd, 9);
    mySerial.readBytes(response, 9);

    int i;
    byte crc = 0;
    for (i = 1; i < 8; i++) crc += response[i];
    crc = 255 - crc;
    crc++;
    if ( !(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc) ) {
      Serial.println("CRC error: " + String(crc) + " / " + String(response[8]));
      return;
    }

    unsigned int responseHigh = (unsigned int) response[2];
    unsigned int responseLow = (unsigned int) response[3];
    ppm = (256 * responseHigh) + responseLow;

    Serial.println(ppm);

    digitalWrite(G, LOW);
    digitalWrite(R, LOW);
    digitalWrite(B, LOW);
 { 
    int m = ppm;
    char str = "Heloo World";
    if (m < 300){
      str = "Ошибка";
      Serial.println("Ошибка");
    }  
    else if (m < 450) {
      
      str = "Нормальный уровень на открытом воздухе";
      digitalWrite(B, HIGH);
      delay(500);
      digitalWrite(B, LOW);
      delay(500);
      Serial.println("Нормальный уровень на открытом воздухе"); 
    }
    else if (m < 600) {
      
      str = "Приемлимые уровни";
      digitalWrite(B, HIGH);
      delay(1000);
      digitalWrite(B, LOW);
      delay(1000);
      Serial.println("Приемлимые уровни");
    }
    else if (m < 1000) {
      
      str = "Жалобы на не свежий воздух";
      digitalWrite(B, HIGH);
      Serial.println("Жалобы на не свежий воздух");
    }
    else if (m < 1001) {
      
      str = "Максимальный уровень стандартов ASHRAE и OSHA";
      digitalWrite(G, HIGH);
      delay(500);
      digitalWrite(G, LOW);
      delay(500);
      Serial.println("Максимальный уровень стандартов ASHRAE и OSHA");
    }
    else if (m < 2500) {
      
      str = "Общая вялость";
      digitalWrite(G, HIGH);
      delay(1000);
      digitalWrite(G, LOW);
      delay(1000);
      Serial.println("Общая вялость");
    }
    else if (m < 3000) {
      
      str = "Возможный нежелательные эффекты на здоровье";
      digitalWrite(G, HIGH);
      Serial.println("Возможный нежелательные эффекты на здоровье");
    }
    else if (m >= 3000) {
      
      str = "Легкое отравление, учищется пульс и частота дыхания, тошнота и рвота";
      digitalWrite(R, HIGH);
      delay(500);
      digitalWrite(R, LOW);
      delay(500);
      Serial.println("Легкое отравление, учищется пульс и частота дыхания, тошнота и рвота");
    }
    else if (m < 5000) {
      
      str = "Есть угроза здровоья и жизни";
      digitalWrite(R, HIGH);
      delay(1000);
      digitalWrite(R, LOW);
      delay(1000);
      Serial.println("Есть угроза здровоья и жизни");
    }
    else if (m >= 5000) {
      
      str = "Все очень плохо";
      digitalWrite(R, HIGH);
      Serial.println("Все очень плохо");
    }
 }
  }
}
/*--------------------------------------------------------------
 Функция отправляет данные на WEB сервер. 
 Её я тоже взял из сети...
 ...лишь подставил свои переменные
 --------------------------------------------------------------*/
void sendData(int m, char str) {
  client.connect(server, 80);
  client.print( "GET /add.php?");
  client.print("k=");  // Специальный код, например asREb25C
  client.print("&");
  client.print("m=");
  client.print(m);
  client.print("&");
  client.print("str=");
  client.print(str);
  client.println(" HTTP/1.1");
  client.print( "Host: " );
  client.println(server);
  client.println( "Connection: close" );
  client.println();
  client.println();
  client.stop();
  client.flush();
   }


