#include <Arduino.h>
#define USE_TIMER_2 true
#include <TimerInterrupt.h>

#include <Servo.h>

/*
Servo frontGate;
Servo backGate;
int frontGatePin = 5;
int backGatePin = 11;
double rotationAngle;


// void timeHandler(void) {
//   rotationAngle = map(analogRead(A0), (double)0, (double)1023, 0, 180);
//   frontGate.write(rotationAngle);
// }

void setup() {
  Serial.begin(9600);
  frontGate.attach(frontGatePin);
  frontGate.write(0);
  // ITimer2.attachInterrupt(10, timeHandler);
}

void loop() {
  rotationAngle = map(analogRead(A0), (double)0, (double)1023, 0, 180);
  frontGate.write(rotationAngle);
  Serial.println(rotationAngle);
}

*/

/* -------------------------------IR Sensor Testing--------------------------------- */

// void setup() {
//   Serial.begin(9600);
// }

// void loop() {
//   Serial.println(analogRead(A0));
// }

/* -------------------------------Locomotion, Tracking Testing--------------------------------- */

int spd = 100;               // Set the value of speed. Range: 0~255
int timer = 1000;            // Define a timer
int count;
char input;

const int IN1 = 10;          //L298N IN1 pin connecting to Arduino pin 10
const int IN2 = 11;          //L298N IN2 pin connecting to Arduino pin 11
const int IN3 = 5;           //L298N IN3 pin connecting to Arduino pin 5
const int IN4 = 6;           //L298N IN4 pin connecting to Arduino pin 6

void setup() 
{
  Serial.begin(9600);     
  pinMode(6,OUTPUT);      // Arduino for the L298N logic control
  pinMode(5,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  delay(500);  
}

void Forward(void)           //車子前進子程式
{
  analogWrite(IN1, spd);
  analogWrite(IN2, 0);
  analogWrite(IN3, spd);
  analogWrite(IN4, 0);
} 

void Back(void)              //車子後退子程式
{
  analogWrite(IN1, 0);
  analogWrite(IN2, spd);
  analogWrite(IN3,0 );
  analogWrite(IN4, spd);
} 

void Stop(void)              //車子停止子程式
{
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

void Left(void)              //車子左轉子程式
{
  analogWrite(IN1, spd);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
} 

void Right(void)             //車子右轉子程式
{
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, spd);
  analogWrite(IN4, 0);
} 

void CWSpin(void) 
{
  analogWrite(IN1, spd);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, spd);
}

void CCWSpin(void)
{
  analogWrite(IN1, 0);
  analogWrite(IN2, spd);
  analogWrite(IN3, spd);
  analogWrite(IN4, 0);
}

/* (test for simple locomotion function)
void loop() {            
//  count++;
//   if(count>500){
//     if(Serial.available()) 
//     {
//       input = Serial.read();
//       Serial.println(input);
//       count = 0;
//     }
    
//     if (input == 'f'){
//       Serial.println("forward");
//       Forward();
//     }
//     else if (input == 'b'){
//       Serial.println("backward");
//       Back();
//     }
//     else if (input == 'r'){
//       Serial.println("right");
//       Right();
//     }
//     else if (input == 'l'){
//       Serial.println("left");
//       Left();
//     }
//     else if (input == 's'){
//       Serial.println("stop");
//       Stop();
//     }
//   }
    Forward();
    Back();
    delay(timer);
    Stop();
    delay(timer);
    Left();
    delay(timer);
    Right();
    delay(timer);
    Stop();
    delay(timer);
}
*/

/* (black line detected -> turn)
void loop() {         
  Forward();
  if(analogRead(A0)<100 && analogRead(A1)<100) {
    Stop();
    delay(timer);
    Left();
    delay(timer*2);
    Stop();
    delay(timer*10);
  }
}
*/

/* (spinning)
void loop() {         
    CWSpin();
    delay(timer*2);
    Stop();
    delay(timer);
    CCWSpin();
    delay(timer*2);
    Stop();
    delay(timer);
}
*/

/* (line tracking when "BACKING OFF". For this part, comment out the setup() before)
void setup() {
  Serial.begin(9600);     
  pinMode(6,OUTPUT);      
  pinMode(5,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  analogWrite(IN1, 0);
  analogWrite(IN2, 0.3*spd);
  analogWrite(IN3, 0);
  analogWrite(IN4, spd);
  }

void loop() {
  if(analogRead(A0) > 900) {
    analogWrite(IN1, 0);
    analogWrite(IN2, 0.3*spd);
    analogWrite(IN3, 0);
    analogWrite(IN4, spd);
  }
  if(analogRead(A0) < 100) {
    analogWrite(IN1, 0);
    analogWrite(IN2, spd);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0.3*spd);
  }
}
*/