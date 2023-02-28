

 #include "main.h"

Servo rightGate;
Servo leftGate;


int rightGatePin = 3;
int leftGatePin = 9;
double rotationAngle;
int blackTH = 450;
int whiteTH = 300;


int spd = 175;               // Set the value of speed. Range: 0~255
int timer = 1000;            // Define a timer
int count;
char input;

int flag;


void Forward(int speed)           //車子前進子程式
{
  analogWrite(IN1, speed);
  analogWrite(IN2, 0);
  analogWrite(IN3, speed);
  analogWrite(IN4, 0);
} 


void Back(int speed)              //車子後退子程式
{
  analogWrite(IN1, 0);
  analogWrite(IN2, speed);
  analogWrite(IN3, 0);
  analogWrite(IN4, speed);
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
  if(analogRead(SideIR_A) > blackTH) {
    analogWrite(IN1, 0.2*spd);
    analogWrite(IN2, 0);
    analogWrite(IN3, 1.1*spd);
    analogWrite(IN4, 0);  
  }
  if(analogRead(SideIR_A) < whiteTH) {
    analogWrite(IN1, 0.8*spd);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0.5*spd);
    analogWrite(IN4, 0);
  }
}

void lineTracking_B(void) {
  if(analogRead(SideIR_B) > blackTH) {
    analogWrite(IN1, 0);
    analogWrite(IN2, spd);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0.3*spd);
  }
  if(analogRead(SideIR_B) < whiteTH) {
    analogWrite(IN1, 0);
    analogWrite(IN2, 0.7*spd);
    analogWrite(IN3, 0);
    analogWrite(IN4, spd);
  }
}



