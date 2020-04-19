#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include <Servo.h>


#include <ESP8266WiFi.h>
#define WIFI_SSID "Yoo"
#define WIFI_PASSWORD "freeloader"
WiFiServer server(80); 

#include "FirebaseESP8266.h"
#define FIREBASE_HOST "test-98fc6.firebaseio.com"
#define FIREBASE_AUTH "jgklbUwA9Ht0fMmWOrkeLD7AfQmdTtInXDmzHGZ5"
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
int bcar1 = 1;
int bcar2 = 1;
int bcar3 = 1;
int fcar1 = 1;
int fcar2 = 1;
int fcar3 = 1;


void setup() {
  // put your setup code here, to run once:

//Serial.begin(9600);
    Serial.begin(115200);
    delay(10);
    
  // Connect to WiFi network 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println("");
  Serial.println("WiFi connected");

    // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.print("URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");


 

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

if(ircar1 == 0 || bcar1 == 0)
  {
    Serial.println("Slot 1 is full");
    lcd.setCursor(0,0);
    slot1 = 1;
    lcd.print(slot1);
  }
  else {slot1 = 0;}

 if(ircar2 == 0 || bcar2 == 0)
  {
    Serial.println("Slot 2 is full");
    lcd.setCursor(1,0);
    slot2 = 1 ;
    
    lcd.print(slot2);
  }
 else {slot2 =0;}
 
  if(ircar3 == 0 || bcar3 == 0)
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

  WiFiClient client = server.available();
  if (!client) {
    return;
  }

   
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
  
  if (request.indexOf("/car1=Reserve") != -1)  {
    if((ircar1 == 0) || (bcar1 == 0)){
    //bcar1 = 1;
    fcar1 = 0;
    }
    else {
    bcar1 = 0;
    fcar1 = 1;
    }
  }

 if (request.indexOf("/car1=Release") != -1)  {
    bcar1 = 1;
  }

 if (request.indexOf("/car2=Reserve") != -1)  {
    if((ircar2 == 0) || (bcar2 == 0)){
    //bcar2 = 1;
    fcar2 = 0;
    }
    else {
    bcar2 = 0;
    fcar2 = 1;
  }
  }
 if (request.indexOf("/car2=Release") != -1)  {
    bcar2 = 1;
  }
 if (request.indexOf("/car3=Reserve") != -1)  {
    if((ircar3 == 0) || (bcar3 == 0)){
    //bcar3 = 1;
    fcar3 = 0;
    }
    else {
    bcar3 = 0;
    fcar3 = 1;
  }
  }
 if (request.indexOf("/car3=Release") != -1)  {
    bcar3 = 1;
  }


  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<style> body {background-image: url('https://i.ibb.co/Y7KnxYg/background.jpg'); background-repeat: no-repeat; background-attachment: fixed; background-size: 100% 100%;} </style>");
  //client.println("body {background-image: url('https://i.ibb.co/Y7KnxYg/background.jpg');");

   client.print("<head><font color=\"white\">AVAILABLE SLOTS:</font></head>");
   client.print("<body>");
   client.print("<table border = 5 bordercolor = red align = center>");
   client.print("<tr>");
   client.print("<th colspan = 5><font color=\"white\">Slot 1</font></th>");
   client.print("<th colspan = 5><font color=\"white\">Slot 2</font></th>");
   client.print("<th colspan = 5><font color=\"white\">Slot 3</font></th>");
   client.print("</tr>");
   client.print("<tr>");
   
   if((ircar1 == 0 || bcar1 == 0) && (ircar2 == 0 || bcar2 == 0) && (ircar3 == 0 || bcar3 == 0))
{
  client.print("<td colspan = 5><img src=\"https://i.ibb.co/2txzQLn/car1.png\" alt=\"\" border=3 height=500 width=260></img></td>");
  client.print("<td colspan = 5><img src=\"https://i.ibb.co/K9fMWtr/car2.png\" alt=\"\" border=3 height=500 width=260></img></td>");
  client.print("<td colspan = 5><img src=\"https://i.ibb.co/2cw44qp/car3.png\" alt=\"\" border=3 height=500 width=260></img></td>");
}


  else if((ircar1 == 0 || bcar1 == 0) && (ircar2 == 0 || bcar2 == 0)) {
    client.print("<td colspan = 5><img src=\"https://i.ibb.co/2txzQLn/car1.png\" alt=\"\" border=3 height=500 width=260></img></td>");
    client.print("<td colspan = 5><img src=\"https://i.ibb.co/K9fMWtr/car2.png\" alt=\"\" border=3 height=500 width=260></img></td>");
    client.print("<td colspan = 5 height=500 width=260></td>");
  }

  else if((ircar2 == 0 || bcar2 == 0) && (ircar3 == 0 || bcar3 == 0)) {
    client.print("<td colspan = 5 height=600 width=260></td>");
    client.print("<td colspan = 5><img src=\"https://i.ibb.co/K9fMWtr/car2.png\" alt=\"\" border=3 height=500 width=260></img></td>");
    client.print("<td colspan = 5><img src=\"https://i.ibb.co/2cw44qp/car3.png\" alt=\"\" border=3 height=500 width=260></img></td>");
  }

   else if((ircar1 == 0 || bcar1 == 0) && (ircar3 == 0 || bcar3 == 0)) {
    client.print("<td colspan = 5><img src=\"https://i.ibb.co/2txzQLn/car1.png\" alt=\"\" border=3 height=500 width=260></img></td>");
    client.print("<td colspan = 5 height=600 width=260></td>");
    client.print("<td colspan = 5><img src=\"https://i.ibb.co/2cw44qp/car3.png\" alt=\"\" border=3 height=500 width=260></img></td>");

   }
 
  else if(ircar1 == 0 || bcar1 == 0) {
    client.print("<td colspan = 5><img src=\"https://i.ibb.co/2txzQLn/car1.png\" alt=\"\" border=3 height=500 width=260></img></td>");
    client.print("<td colspan = 5 height=600 width=260></td>");
    client.print("<td colspan = 5 height=600 width=260></td>");
  } 

  else if(ircar2 == 0 || bcar2 == 0) {
    client.print("<td colspan = 5 height=600 width=260></td>");
    client.print("<td colspan = 5><img src=\"https://i.ibb.co/K9fMWtr/car2.png\" alt=\"\" border=3 height=500 width=260></img></td>");
    client.print("<td colspan = 5 height=600 width=260></td>");
  } 


  else if(ircar3 == 0 || bcar3 == 0 ) {
    client.print("<td colspan = 5 height=600 width=260></td>");
    client.print("<td colspan = 5 height=600 width=260></td>");
    client.print("<td colspan = 5><img src=\"https://i.ibb.co/2cw44qp/car3.png\" alt=\"\" border=3 height=500 width=260></img></td>");
  }

  else if(ircar1 == 1 && ircar2 == 1 && ircar3 == 1) {
    client.print("<td colspan = 5 height=600 width=260></td>");
    client.print("<td colspan = 5 height=600 width=260></td>");
    client.print("<td colspan = 5 height=600 width=260></td>");
  }


   client.print("</tr>");
   client.print("</table>");
   client.print("<br>");
   client.print("<br>");
   client.print("<br>");
   client.print("<font color=\"white\">BOOK A SLOT</font> <br>");
   client.print("<font color=\"white\">SLOT 1:</font>");
   client.print("<a href=\"car1=Reserve\"><button>RESERVE </button></a>");
   client.print("<a href=\"car1=Release\"><button>RELEASE </button></a><br>");
   client.print("<br>");
   client.print("<font color=\"white\">SLOT 2:</font>");
   client.print("<a href=\"car2=Reserve\"><button>RESERVE </button></a>");
   client.print("<a href=\"car2=Release\"><button>RELEASE </button></a><br>");
   client.print("<br>");
   client.print("<font color=\"white\">SLOT 3:</font>");
   client.print("<a href=\"car3=Reserve\"><button>RESERVE </button></a>");
   client.print("<a href=\"car3=Release\"><button>RELEASE </button></a><br>");
   client.print("<br>");
   if(bcar1 == 0){
    client.print("<font color=\"white\">SLOT 1 IS RESERVED FOR YOU</font><br>");
   }
   if(fcar1 == 0){
    client.print("<font color=\"white\">SLOT 1 IS OCCUPIED TRY ANOTHER SLOT</font><br>");
   }

   if(bcar2 == 0){
    client.print("<font color=\"white\">SLOT 2 IS RESERVED FOR YOU</font><br>");
   }
   if(fcar2 == 0){
    client.print("<font color=\"white\">SLOT 2 IS OCCUPIED TRY ANOTHER SLOT</font><br>");
   }

   if(bcar3 == 0){
    client.print("<font color=\"white\">SLOT 3 IS RESERVED FOR YOU</font><br>");
   }
   if(fcar3 == 0){
    client.print("<font color=\"white\">SLOT 3 IS OCCUPIED TRY ANOTHER SLOT</font><br>");
   }
  
   //client.print("<meta http-equiv=\"refresh\" content=\"5\"/>");
   client.print("<meta http-equiv=\"refresh\" content=\"5;url= http://192.168.43.153/\" />");
   client.print("</body>");
   client.print("</html>");
   
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

fcar1 = 1;
fcar2 = 1;
fcar3 = 1;
   
}
