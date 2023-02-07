#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>
#include <SoftwareSerial.h>

Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

//setting pins for the ultrasonic sensor inside the bin//
#define trigPinT 8
#define echoPinT 9

//setting pins for the ultrasonic sensor outside the bin//
#define trigPinL 12
#define echoPinL 13

//setting pin for the servo motor//
#define servoPin 10

#define ledPin 11

int distance_set = 20; //set distance for the lid to open
int distance_setT = 5; //set distance for the trash level
float distance_L,distance_T,duration_L,duration_T;


SoftwareSerial ESP1(6,7); 
long T;
void setup()
{
  lcd.begin();               
  lcd.backlight(); 
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Project By:");
  lcd.setCursor(6,1);
  lcd.print("TZM");
  delay(5000);
  servo.attach(servoPin);
  servo.write(0);
  pinMode(trigPinT, OUTPUT);
  pinMode(echoPinT, INPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  ESP1.begin(115200);
  
}
void loop()
{
  digitalWrite(trigPinT, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinT, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinT, LOW);

  duration_T = pulseIn(echoPinT, HIGH);
  distance_T = 0.017*duration_T;
  
  //printing results on the LCD display//
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Trash from: ");
  lcd.print(distance_T);
  lcd.setCursor(12,1);
  lcd.print("cm");

  if (distance_T<5){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sorry!Bin full!");
    
  }


  
  //printing results on the ThingSpeak cloud using NodeMCU//
  ESP1.print(distance_T);

  

  digitalWrite(trigPinL, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinL, LOW);

  duration_L = pulseIn(echoPinL, HIGH);
  distance_L = 0.017*duration_L;

  Serial.print("Distance in cm: ");
  Serial.println(distance_L);
  

 
  delay(1000);
 
  
  if (distance_L < distance_set && distance_T>distance_setT){
    servo.write(90);
    digitalWrite(ledPin, HIGH);
    delay(500);
    
    }
  
  else {
    servo.write(0);
    digitalWrite(ledPin, LOW);
  }
  

  delay(500);

  
}
