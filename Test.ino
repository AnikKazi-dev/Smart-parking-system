#include <Servo.h>
#define IR D1
#define IR2 D2
#define servoVer D3
#define servoVer2 D4
Servo servo;
Servo servo2;


bool ir;
bool ir2;
void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);
pinMode(IR,INPUT);
pinMode(IR2,INPUT);
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

//  for(int i=0;i<180;i=i+30)
//  {
 //   servo2.write(i);
//    delay(100);
 // }

  if(ir == 0)
  {
    Serial.println("object1 found");
    servo.write(90);
    delay(5000);
    
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
    delay(5000);
    
  }
  else if (ir2 == 1)
  {
    Serial.println("nothing2");
    servo2.write(0);
   // delay(1000);
  }
   
}
