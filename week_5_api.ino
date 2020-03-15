#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include <Servo.h>

#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "smart-parking-system-7b042.firebaseio.com"
#define FIREBASE_AUTH "I4uHzmw41XxDAw8COZPNY5BjR2HgIKpldKp3r2nL"
#define WIFI_SSID "Yoo"
#define WIFI_PASSWORD "freeloader"
FirebaseData firebaseData;
#define IR D0
#define IR2 D8

#define IRcar1 D5
#define IRcar2 D6
#define IRcar3 D7


#define servoVer D3
#define servoVer2 D4
Servo servo;
Servo servo2;


bool ir;
bool ir2;
bool ircar1;
bool ircar2;
bool ircar3;
int count = 3;
int slot1 = 0;
int slot2 = 0;
int slot3 = 0;
int ava;

void setup() {
  // put your setup code here, to run once:

//Serial.begin(9600);
Serial.begin(115200);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();


 

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

 
  Firebase.reconnectWiFi(true);

  Firebase.setInt(firebaseData, "/Available Parking Slots :", 3);



lcd.begin();
lcd.setCursor(0,0);
lcd.print("000");    
pinMode(IR,INPUT);
pinMode(IR2,INPUT);
pinMode(IRcar1,INPUT);
pinMode(IRcar2,INPUT);
pinMode(IRcar3,INPUT);
//pinMode(servoVer,OUTPUT);
//pinMode(servoVer2,OUTPUT);
servo.attach(D3);//D3
servo2.attach(D4);//D4
servo.write(0);


servo2.write(0);

lcd.begin();
lcd.print("000");

} 

void loop() {
  // put your main code here, to run repeatedly:

  ir = digitalRead(IR);
  ir2 = digitalRead(IR2);

  ircar1 = digitalRead(IRcar1);
  ircar2 = digitalRead(IRcar2);
  ircar3 = digitalRead(IRcar3);
  //lcd.begin();
  //lcd.print("000");
  
  lcd.setCursor(0,0);
  lcd.print(slot1);
  lcd.setCursor(1,0);
  lcd.print(slot2);
  lcd.setCursor(2,0);
  lcd.print(slot3);

if(ircar1 == 0)
  {
    Serial.println("Slot 1 is full");
    lcd.setCursor(0,0);
    slot1 = 1;
    lcd.print(slot1);
  }
  else {slot1 = 0;}

 if(ircar2 == 0)
  {
    Serial.println("Slot 2 is full");
    lcd.setCursor(1,0);
    slot2 = 1 ;
    
    lcd.print(slot2);
  }
 else {slot2 =0;}
 
  if(ircar3 == 0)
  {
    Serial.println("Slot 3 is full");
    lcd.setCursor(2,0);
    slot3 = 1 ;
    
    lcd.print(slot3);
  }
  else { slot3 = 0;}

  if(ir == 0)
  {
    Serial.println("object1 found");
    servo.write(90);
    delay(2000);
  }
  
  else if (ir == 1)
  {
    Serial.println("nothing");
    servo.write(0);
   // delay(1000);
  }
  
  
  if(ir2 == 0)
  {
    Serial.println("object2 found");
    servo2.write(90);
    delay(2000);
    
  }
  else if (ir2 == 1)
  {
    Serial.println("nothing2");
    servo2.write(0);
   // delay(2000);
  }

  lcd.setCursor(0,1);
  lcd.print("Available:");
  lcd.setCursor(11,1);
  ava = (3-(slot1+slot2+slot3));
  lcd.print(ava);

 
  if(Firebase.setInt(firebaseData, "/Available Parking Slots :", ava))
  {
    
     Serial.println("Set int data success");

  }else{
    

    Serial.print("Error in setInt, ");
    Serial.println(firebaseData.errorReason());
  }
   
}
