#include <Servo.h>
#define IR D1
#define IR2 D2

#define IRcar1 D5
#define IRcar2 D6
#define IRcar3 D7
#define IRcar4 D8

#define servoVer D3
#define servoVer2 D4
Servo servo;
Servo servo2;


bool ir;
bool ir2;
bool ircar1;
bool ircar2;
bool ircar3;
bool ircar4;
void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);
pinMode(IR,INPUT);
pinMode(IR2,INPUT);
pinMode(IRcar1,INPUT);
pinMode(IRcar2,INPUT);
pinMode(IRcar3,INPUT);
pinMode(IRcar4,INPUT);
//pinMode(servoVer,OUTPUT);
//pinMode(servoVer2,OUTPUT);
servo.attach(D3);//D3
servo2.attach(D4);//D4
servo.write(0);


servo2.write(0);

} 

void loop() {
  // put your main code here, to run repeatedly:

  ir = digitalRead(IR);
  ir2 = digitalRead(IR2);

  ircar1 = digitalRead(IRcar1);
  ircar2 = digitalRead(IRcar2);
  ircar3 = digitalRead(IRcar3);
  ircar4 = digitalRead(IRcar4);

if(ircar1 == 0)
  {
    Serial.println("Slot 1 is full");
    
  }

 if(ircar2 == 0)
  {
    Serial.println("Slot 2 is full");
    
  }

  if(ircar3 == 0)
  {
    Serial.println("Slot 3 is full");
    
  }

  if(ircar4 == 0)
  {
    Serial.println("Slot 4 is full");
    
  }

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
   
}
