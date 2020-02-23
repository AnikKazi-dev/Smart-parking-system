#include <Servo.h>
#define IR D2
Servo servo;
bool ir;

void setup() {

pinMode(IR,INPUT);

servo.attach(D1);//D1
servo.write(0);


} 

void loop() {
  ir = digitalRead(IR);
  
  servo.write(0);
  delay(1000);
  servo.write(180);
   delay(1000);

  
   
}
