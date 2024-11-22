#include <SoftwareSerial.h> // Include the SoftwareSerial library
SoftwareSerial esp8266(0, 1);

#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h> 

#define TRIG_PIN A0 
#define ECHO_PIN A1 
#define MAX_DISTANCE 200 
#define MAX_SPEED 100 
#define MAX_SPEED_OFFSET 20
#define IR_PIN1 A2
#define CLEANER_PIN A3
#define IR_PIN2 A4


NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;   

boolean goesForward=false;
int distance = 100;
int speedSet = 0;

void setup() {

Serial.begin(9600);
  esp8266.begin(115200); // Initialize ESP8266 communication
  esp8266.println("AT+RST"); // Reset ESP8266
  delay(1000);
  esp8266.println("AT+CWMODE=1"); // Set ESP8266 mode to Station (client) mode
  delay(1000);
  esp8266.println("AT+CWJAP=\"SSID\",\"PASSWORD\""); // Connect to Wi-Fi network
  delay(5000);
  Serial.println("Connected to Wi-Fi network");
  

  myservo.attach(10);  
  myservo.write(115); 
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  pinMode(IR_PIN1, INPUT);
  pinMode(IR_PIN2, INPUT);
  pinMode(CLEANER_PIN, OUTPUT);
}

void loop() {

  esp8266.println("AT+CIPSTART=\"TCP\",\"api.openweathermap.org\",80"); // Connect to OpenWeatherMap API
  delay(5000);
  esp8266.print("GET /data/2.5/weather?q=London&appid=API_KEY HTTP/1.1\r\nHost: api.openweathermap.org\r\nConnection: close\r\n\r\n"); // Send HTTP request to OpenWeatherMap API
  delay(5000);
  while (esp8266.available()) { // Read OpenWeatherMap API response
    Serial.write(esp8266.read());
  }
  delay(5000);
  esp8266.println("AT+CIPCLOSE"); // Close TCP connection
  delay(5000);



 int distanceR = 0;
 int distanceL =  0;
 delay(40);
 
 if(distance<=15)
 {
  moveStop();
  delay(100);
  moveBackward();
  delay(300);
  moveStop();
  delay(200);
  distanceR = lookRight();
  delay(200);
  distanceL = lookLeft();
  delay(200);

  if(distanceR>=distanceL)
  {
    turnRight();
    moveStop();
  }else
  {
    turnLeft();
    moveStop();
  }
 }else
 {
  moveForward();
 }
 if(digitalRead(IR_PIN1) == HIGH){
   turnRight1();
   digitalWrite(CLEANER_PIN, HIGH); 
  if (digitalRead(IR_PIN2) == HIGH){
   moveStop();
   turnLeft1();
   digitalWrite(CLEANER_PIN, HIGH); 
 }
 
  else {
   digitalWrite(CLEANER_PIN, LOW);
 }
 }else {
   digitalWrite(CLEANER_PIN, LOW);
 distance = readPing();
 } 
}

int lookRight()
{
    myservo.write(50); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(115); 
    return distance;
}

int lookLeft()
{
    myservo.write(170); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(115); 
    return distance;
    delay(100);
}

int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

void moveStop() {
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  } 
  
void moveForward() {

 if(!goesForward)
  {
    goesForward=true;
    motor1.run(FORWARD);      
    motor2.run(FORWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD);     
   for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) 
   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
   }
  }
}


void moveBackward() {
    goesForward=false;
    motor1.run(BACKWARD);      
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);  
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) 
  {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
}  

void turnRight() {
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);     
  delay(500);
  motor1.run(FORWARD);      
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);      
} 
 
void turnLeft() {
  motor1.run(FORWARD);     
  motor2.run(BACKWARD);  
  motor3.run(BACKWARD);
  motor4.run(FORWARD);   
  delay(500);
  motor1.run(FORWARD);     
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}  

void turnRight1() {
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);     
  delay(500);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(7000);    
  motor1.run(FORWARD);      
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);      
} 

void turnLeft1() {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);     
  delay(500);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(7000);    
  motor1.run(FORWARD);      
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);      
} 
