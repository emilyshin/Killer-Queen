#include <Arduino.h>
#define USE_TIMER_2 true
#include <TimerInterrupt.h>
#include <Servo.h>
// #include <PID_v1.h>

Servo rightGate;
Servo leftGate;
int rightGatePin = 3;
int leftGatePin = 9;
double rotationAngle;
int blackTH = 450;
int whiteTH = 300;

int spd = 150;               // Set the value of speed. Range: 0~255
int timer = 1000;            // Define a timer
int count;
char input;

const int IN1 = 10;          //L298N IN1 pin connecting to Arduino pin 10
const int IN2 = 11;          //L298N IN2 pin connecting to Arduino pin 11
const int IN3 = 5;           //L298N IN3 pin connecting to Arduino pin 5
const int IN4 = 6;           //L298N IN4 pin connecting to Arduino pin 6
int flag;

void Forward(void)           //車子前進子程式
{
  analogWrite(IN1, spd);
  analogWrite(IN2, 0);
  analogWrite(IN3, spd);
  analogWrite(IN4, 0);
} 

void Forward_slow(void)           //車子前進子程式
{
  analogWrite(IN1, spd*0.5);
  analogWrite(IN2, 0);
  analogWrite(IN3, spd*0.5);
  analogWrite(IN4, 0);
}

void Back(void)              //車子後退子程式
{
  analogWrite(IN1, 0);
  analogWrite(IN2, spd);
  analogWrite(IN3, 0);
  analogWrite(IN4, spd);
} 

void Back_fast(void)              //車子後退子程式
{
  analogWrite(IN1, 0);
  analogWrite(IN2, 255);
  analogWrite(IN3, 0);
  analogWrite(IN4, 255);
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

void Left_slow(void)              //車子左轉子程式
{
  analogWrite(IN1, 0.5*spd);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0.3*spd);
  analogWrite(IN4, 0);
} 

void Right_back(void)             //車子右轉子程式
{
  analogWrite(IN1, 0);
  analogWrite(IN2, 255);
  analogWrite(IN3, 0);
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

void lineTracking_F(void) {
  if(analogRead(A0) > blackTH) {
    analogWrite(IN1, 0.2*spd);
    analogWrite(IN2, 0);
    analogWrite(IN3, 1.1*spd);
    analogWrite(IN4, 0);  
  }
  if(analogRead(A0) < whiteTH) {
    analogWrite(IN1, 0.8*spd);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0.5*spd);
    analogWrite(IN4, 0);
  }
}

void lineTracking_B(void) {
  if(analogRead(A1) > blackTH) {
    analogWrite(IN1, 0);
    analogWrite(IN2, spd);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0.3*spd);
  }
  if(analogRead(A1) < whiteTH) {
    analogWrite(IN1, 0);
    analogWrite(IN2, 0.7*spd);
    analogWrite(IN3, 0);
    analogWrite(IN4, spd);
  }
}

void setup() 
{
  Serial.begin(9600);     
  pinMode(6,OUTPUT);      // Arduino for the L298N logic control
  pinMode(5,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);

  pinMode(3,OUTPUT);
  pinMode(9,OUTPUT);

 rightGate.attach(rightGatePin);
 rightGate.write(0);
 leftGate.attach(leftGatePin);
 leftGate.write(0);
  delay(500);
  flag = 0;
  // // // Left(); 
  // Left(); 
  Left();

}

/* --------------------------------- Servo Motor Testing ---------------------------- */

// void timeHandler(void) {
//   rotationAngle = map(analogRead(A0), (double)0, (double)1023, 0, 180);
//   frontGate.write(rotationAngle);
// }

// void setup() {
//   Serial.begin(9600);
//   rightGate.attach(rightGatePin);
//   rightGate.write(0);
//   leftGate.attach(leftGatePin);
//   leftGate.write(0);
//   // ITimer2.attachInterrupt(10, timeHandler);
// }

// void loop() {
//   delay(1000);
//   rightGate.write(90);
//   delay(1000);
//   leftGate.write(90);
//   delay(1000);
//   rightGate.write(0);
//   leftGate.write(0);
// }


/* -------------------------------IR Sensor Testing--------------------------------- */

// void setup() {
//   Serial.begin(9600);
// }

// void loop() {
//   // Serial.print(analogRead(A0));
//   // Serial.print(", ");
//   // Serial.println(analogRead(A1));
//   Serial.println(analogRead(A3));
// }

/* -------------------------------Locomotion, Tracking Testing--------------------------------- */

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

/* -------------------------------- Line Tracking (Forward) --------------------------- */
void loop() {
  // analogWrite(10,spd);
  Serial.println(analogRead(A3));
  // Serial.print(", ");
  // Serial.println(analogRead(A0));


  // if(analogRead(A3) < 700 && flag == 0){
  //   lineTracking_F();
  //   Serial.println("Smile");
  // }
  // if(analogRead(A3) > 800 && flag == 0){
  //   Stop();
  //   delay(1000);
  //   Right_back();
  //   delay(700);
  //   flag = 1;
  //   Back_fast();
  //   delay(3000);
  //   Forward();
  //   delay(500);
  //   // Left_slow();
  // }
  // Serial.println(analogRead(A0)); 
  // if(analogRead(A3) < 700 && flag == 1){
  //   lineTracking_F();
  // }
  // if(analogRead(A3) > 800 && flag == 1) {
  //   Stop();
  //   delay(1000);
  //   rightGate.write(90);
  //   flag = 2;
  //   Forward();
  //   delay(300);
  //   lineTracking_F();
  // }
  // if(analogRead(A3) > 800 && flag == 2) {
  //   Stop();
  //   delay(1000);
  //   leftGate.write(90);
  //   flag = 3;
  //   delay(1000);
  //   Stop();
  // }
}