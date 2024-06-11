#include <LCD_I2C.h>
#include <Servo.h>

LCD_I2C lcd(0x27, 16, 2);
Servo servoMotor1;
Servo servoMotor2;

// I2C address
#define I2C_ADDR 0x27 

#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Pin definitions
#define trigPin 9
#define echoPin 10
#define greenLedPin 5 
#define redLedPin 4 
#define ldr_pin A0 
#define led_pin 6
#define irsensor1 8 
#define irsensor2 7 

int SuSayac = 0; 
int YemekSayac = 0; 

// Variables
long Sure;
int Mesafe;
bool irDurum1 = false;
bool irDurum2 = false;

void setup() {
  lcd.begin();
  lcd.backlight();

  servoMotor1.attach(2); 
  servoMotor2.attach(3); 

  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLedPin, OUTPUT); 
  pinMode(redLedPin, OUTPUT); 
  pinMode(ldr_pin, INPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(irsensor1, INPUT); 
  pinMode(irsensor2, INPUT); 

  servoMotor1.write(0);
  servoMotor2.write(0);
}

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  Sure = pulseIn(echoPin, HIGH);
  Mesafe = Sure * 0.034 / 2;

  Serial.print("Mesafe: ");
  Serial.println(Mesafe);


  if (Mesafe <= 6) {
    digitalWrite(greenLedPin, HIGH); 
    digitalWrite(redLedPin, LOW); 
    servoMotor1.write(0);
    servoMotor2.write(0);
  } else if (Mesafe >= 7) {
    digitalWrite(greenLedPin, LOW); 
    digitalWrite(redLedPin, HIGH);
    servoMotor1.write(60);
    servoMotor2.write(0);
  } else {
    digitalWrite(greenLedPin, LOW); 
    digitalWrite(redLedPin, HIGH); 
    servoMotor1.write(60);
    servoMotor2.write(0);
  }


  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Su Seviyesi: "); 
  lcd.print(Mesafe); 

  // LDR sensor code
  int ldrValue = analogRead(ldr_pin);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  if (ldrValue < 500) { 
    digitalWrite(led_pin, LOW);
  } else {
    digitalWrite(led_pin, HIGH);
  }

  if (digitalRead(irsensor1) == LOW) {
    if (!irDurum1) {
      SuSayac++; 
      irDurum1 = true; 
    }
  } else {
    irDurum1 = false; 
  }

  if (digitalRead(irsensor2) == LOW) {
    if (!irDurum2) {
      YemekSayac++; 
      irDurum2 = true; 
      Serial.print(YemekSayac);
    }
  } else {
    irDurum2 = false;
  }

  lcd.setCursor(0, 1); 
  lcd.print("Su: "); 
  lcd.print(SuSayac); 
  lcd.setCursor(7, 1); 
  lcd.print("Yemek: "); 
  lcd.print(YemekSayac); 

  delay(2000); 
}
