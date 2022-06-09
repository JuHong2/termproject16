#include <LiquidCrystal_I2C.h>/*LiquidCrystal_I2C*/

#define A_SOIL_HUMI A0
#define O_RGB_R 10
#define O_RGB_G 9
#define O_RGB_B 11

#define LCD_I2C_ADDR 0x27

int relay = 8;

LiquidCrystal_I2C lcd(LCD_I2C_ADDR, 16, 2);

int soilHumidity;
/*디지털핀 초기화하기*/
void initPin() {
  
  pinMode(O_RGB_R, OUTPUT);
  pinMode(O_RGB_G, OUTPUT);
  pinMode(O_RGB_B, OUTPUT);

  digitalWrite(O_RGB_R, LOW);
  digitalWrite(O_RGB_G, LOW);
  digitalWrite(O_RGB_B, LOW);

}
/*LCD INTRO출력하기*/
void introLcd() {
  lcd.print("Planting Kit");
  lcd.setCursor(0, 1);
  lcd.print("Rev3.0");
}
/*LCD 습도 프린트하기*/
void printLcd() {
  //lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Moisture : ");
  lcd.print(soilHumidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  if(soilHumidity < 20) lcd.print("Need Water");
  else if(soilHumidity < 50) lcd.print("Soil is Dry");
  else if(soilHumidity < 90) lcd.print("Soil is Wet");
  else lcd.print("Enough Water");
  
}

/*LCD 초기화하기*/
void initLcd() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  introLcd();
}

/*토양습도 계산하기*/
void calcSoilHumidity() {
  soilHumidity = map(analogRead(A_SOIL_HUMI), 1000, 400, 0, 100);
  if(soilHumidity > 100) soilHumidity = 100;
  else if(soilHumidity < 0) soilHumidity = 0;
}

void writeRGB(bool R, bool G, bool B) {
  digitalWrite(O_RGB_R, R);
  digitalWrite(O_RGB_G, G);
  digitalWrite(O_RGB_B, B);
}

void setup() {
  initPin();
  initLcd();
  //RGB LED를 보라색(빨강+파랑)으로 출력합니다.
  delay(2000);
  writeRGB(HIGH, LOW, HIGH);
  pinMode(relay, OUTPUT);
}

void loop() {
  
  calcSoilHumidity();
  
  printLcd();
  
  /* 습도가 낮아지면, 펌프가 동작되고
     습도가 높아지면, 펌프가 꺼집니다. */  
  if (soilHumidity < 20) 
    {
      delay(300);
      lcd.clear();
      lcd.noBacklight();
      digitalWrite(relay, LOW);
      delay(400);  
      digitalWrite(relay, HIGH);
      delay(2000);
      } else {
      digitalWrite(relay, HIGH);
      delay(2000);
     }
} 