/* -------------------- Inclution of Libraries -------------------- */
#include <Arduino.h>
#define USE_TIMER_2 true
#include <TimerInterrupt.h>
#include <Servo.h>
// #include <PID_v1.h>

/* -------------------- Declaration -------------------- */
Servo rightGate;
Servo leftGate;

/* -------------------- PIN location -------------------- */
/* Servo Motors */
const int rightGatePin      = 3;
const int leftGatePin       = 9;

/* DC Motors */
const int IN1_rightWheel_F  = 10;          //L298N IN1 pin connecting to Arduino pin 10
const int IN2_rightWheel_B  = 11;          //L298N IN2 pin connecting to Arduino pin 11
const int IN3_leftWheel_F   = 5;           //L298N IN3 pin connecting to Arduino pin 5
const int IN4_leftWheel_B   = 6;           //L298N IN4 pin connecting to Arduino pin 6

/* IR Sensors*/
// int side_front_A       = A0;
// int side_back_B        = A1;
// int front_left_1       = A2;
// int front_mid_2        = A3;
// int front_right_3      = A4;

/* -------------------- Sensors Threshold -------------------- */
int blackTH_A = 450;
int whiteTH_A = 300;

int blackTH_B;
int whiteTH_B;

int blackTH_1;
int whiteTH_1;

int blackTH_2 = 800;
int whiteTH_2 = 700;

int blackTH_3;
int whiteTH_3;

/* -------------------- Other Constants -------------------- */
int spd    = 175;               // Set the value of speed. Range: 0~255
int timer  = 1000;              // Define a timer
double rotationAngle;
int count;
char input;
int flag;

/* -------------------- Helper Functions -------------------- */
void Forward(void)          
{
  analogWrite(IN1_rightWheel_F, spd);
  analogWrite(IN2_rightWheel_B, 0);
  analogWrite(IN3_leftWheel_F,  spd);
  analogWrite(IN4_leftWheel_B,  0);
} 

void Forward_slow(void)          
{
  analogWrite(IN1_rightWheel_F, spd*0.5);
  analogWrite(IN2_rightWheel_B, 0);
  analogWrite(IN3_leftWheel_F,  spd*0.5);
  analogWrite(IN4_leftWheel_B,  0);
}

void Back(void)            
{
  analogWrite(IN1_rightWheel_F, 0);
  analogWrite(IN2_rightWheel_B, spd);
  analogWrite(IN3_leftWheel_F,  0);
  analogWrite(IN4_leftWheel_B,  spd);
} 

void Back_fast(void)             
{
  analogWrite(IN1_rightWheel_F, 0);
  analogWrite(IN2_rightWheel_B, 255);
  analogWrite(IN3_leftWheel_F,  0);
  analogWrite(IN4_leftWheel_B,  255);
} 

void Stop(void)             
{
  analogWrite(IN1_rightWheel_F, 0);
  analogWrite(IN2_rightWheel_B, 0);
  analogWrite(IN3_leftWheel_F,  0);
  analogWrite(IN4_leftWheel_B,  0);
}

void Left(void)             
{
  analogWrite(IN1_rightWheel_F, spd);
  analogWrite(IN2_rightWheel_B, 0);
  analogWrite(IN3_leftWheel_F,  0);
  analogWrite(IN4_leftWheel_B,  0);
} 

void Right(void)           
{
  analogWrite(IN1_rightWheel_F, 0);
  analogWrite(IN2_rightWheel_B, 0);
  analogWrite(IN3_leftWheel_F,  spd);
  analogWrite(IN4_leftWheel_B,  0);
} 

void Left_slow(void)              
{
  analogWrite(IN1_rightWheel_F, 0.5*spd);
  analogWrite(IN2_rightWheel_B, 0);
  analogWrite(IN3_leftWheel_F,  0.3*spd);
  analogWrite(IN4_leftWheel_B,  0);
} 

void Right_back(void)             
{
  analogWrite(IN1_rightWheel_F, 0);
  analogWrite(IN2_rightWheel_B, 255);
  analogWrite(IN3_leftWheel_F,  0);
  analogWrite(IN4_leftWheel_B,  0);
} 

void CWSpin(void) 
{
  analogWrite(IN1_rightWheel_F, spd);
  analogWrite(IN2_rightWheel_B, 0);
  analogWrite(IN3_leftWheel_F,  0);
  analogWrite(IN4_leftWheel_B,  spd);
}

void CCWSpin(void)
{
  analogWrite(IN1_rightWheel_F, 0);
  analogWrite(IN2_rightWheel_B, spd);
  analogWrite(IN3_leftWheel_F,  spd);
  analogWrite(IN4_leftWheel_B,  0);
}

void lineTracking_F(void) {
  if(analogRead(A0) > blackTH_A) {
    analogWrite(IN1_rightWheel_F, 0.2*spd);
    analogWrite(IN2_rightWheel_B, 0);
    analogWrite(IN3_leftWheel_F,  1.1*spd);
    analogWrite(IN4_leftWheel_B,  0);  
  }
  if(analogRead(A0) < whiteTH_A) {
    analogWrite(IN1_rightWheel_F, 0.8*spd);
    analogWrite(IN2_rightWheel_B, 0);
    analogWrite(IN3_leftWheel_F,  0.5*spd);
    analogWrite(IN4_leftWheel_B,  0);
  }
}

void lineTracking_B(void) {
  if(analogRead(A1) > blackTH_B) {
    analogWrite(IN1_rightWheel_F, 0);
    analogWrite(IN2_rightWheel_B, spd);
    analogWrite(IN3_leftWheel_F,  0);
    analogWrite(IN4_leftWheel_B,  0.3*spd);
  }
  if(analogRead(A1) < whiteTH_B) {
    analogWrite(IN1_rightWheel_F, 0);
    analogWrite(IN2_rightWheel_B, 0.7*spd);
    analogWrite(IN3_leftWheel_F,  0);
    analogWrite(IN4_leftWheel_B,  spd);
  }
}

void setup() 
{
  Serial.begin(9600);     
  /* DC Motors */
  pinMode(IN1_rightWheel_F, OUTPUT);
  pinMode(IN2_rightWheel_B, OUTPUT);
  pinMode(IN3_leftWheel_F,  OUTPUT);
  pinMode(IN4_leftWheel_B,  OUTPUT);      

  /* Servo Motors */
  pinMode(rightGatePin,OUTPUT);
  pinMode(leftGatePin,OUTPUT);
  rightGate.attach(rightGatePin);
  rightGate.write(0);
  leftGate.attach(leftGatePin);
  leftGate.write(0);

  /* IR Sensors */
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  pinMode(A6,INPUT);
  pinMode(A7,INPUT);

  delay(500);
  flag = 0;
  Left();
}

/* --------------------------------- Servo Motor Testing ---------------------------- */

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
  // Serial.println(analogRead(A3));
  // Serial.print(", ");
  // Serial.println(analogRead(A0));

  if(analogRead(A3) < whiteTH_2 && flag == 0){
    lineTracking_F();
  }
  if(analogRead(A3) > blackTH_2 && flag == 0){
    Stop();
    delay(1000);
    Right_back();
    delay(700);
    flag = 1;
    Back_fast();
    delay(3000);
    Forward();
    delay(500);
    // Left_slow();
  }
  Serial.println(analogRead(A0)); 
  if(analogRead(A3) < whiteTH_2 && flag == 1){
    lineTracking_F();
  }
  if(analogRead(A3) > blackTH_2 && flag == 1) {
    Stop();
    delay(1000);
    rightGate.write(90);
    flag = 2;
    Forward();
    delay(300);
    lineTracking_F();
  }
  if(analogRead(A3) < whiteTH_2 && flag == 2){
    lineTracking_F();
  }
  if(analogRead(A3) > blackTH_2 && flag == 2) {
    Stop();
    delay(1000);
    leftGate.write(90);
    flag = 3;
    delay(1000);
    Stop();
  }
}