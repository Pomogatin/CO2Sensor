#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // 2 - ? TX ???????, 3 - ? RX

byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
unsigned char response[9];
unsigned int ppm = 0;
int G = 4;
int R = 5;
int B = 6;

long long lastTime = 0;

void setup() {
  pinMode (R, OUTPUT);
  pinMode (G, OUTPUT);
  pinMode (B, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
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
    
    int m = ppm;
    if (m < 300)
      Serial.println("Ошибка");
    else if (m < 450) {
      
      digitalWrite(B, HIGH);
      delay(500);
      digitalWrite(B, LOW);
      delay(500);
      Serial.println("Нормальный уровень на открытом воздухе"); 
    }
    else if (m < 600) {
      
      digitalWrite(B, HIGH);
      delay(1000);
      digitalWrite(B, LOW);
      delay(1000);
      Serial.println("Приемлимые уровни");
    }
    else if (m < 1000) {
      
      digitalWrite(B, HIGH);
      Serial.println("Жалобы на не свежий воздух");
    }
    else if (m < 1001) {
      
      digitalWrite(G, HIGH);
      delay(500);
      digitalWrite(G, LOW);
      delay(500);
      Serial.println("Максимальный уровень стандартов ASHRAE и OSHA");
    }
    else if (m < 2500) {
      
      digitalWrite(G, HIGH);
      delay(1000);
      digitalWrite(G, LOW);
      delay(1000);
      Serial.println("Общая вялость");
    }
    else if (m < 3000) {
      
      digitalWrite(G, HIGH);
      Serial.println("Возможный нежелательные эффекты на здоровье");
    }
    else if (m >= 3000) {
     
      digitalWrite(R, HIGH);
      delay(500);
      digitalWrite(R, LOW);
      delay(500);
      Serial.println("Легкое отравление, учищется пульс и частота дыхания, тошнота и рвота");
    }
    else if (m < 5000) {
     
      digitalWrite(R, HIGH);
      delay(1000);
      digitalWrite(R, LOW);
      delay(1000);
      Serial.println("Есть угроза здровоья и жизни");
    }
    else if (m >= 5000) {
      
      digitalWrite(R, HIGH);
      Serial.println("Все очень плохо");
    }
  }
}

