
#include <Servo.h>
#include <Arduino.h>
#define USE_TIMER_2 true

extern Servo rightGate;
extern Servo leftGate;

#define IN1 10          //L298N IN1 pin connecting to Arduino pin 10
#define IN2 11          //L298N IN2 pin connecting to Arduino pin 11
#define IN3 5           //L298N IN3 pin connecting to Arduino pin 5
#define IN4 6           //L298N IN4 pin connecting to Arduino pin 6

#define SideIR_A A0
#define SideIR_B A1
#define FrontIR_1 A2
#define FrontIR_2 A3
#define FrontIR_3 A4

void Forward(int speed);     //slow speed will be 0.5*speed      //車子前進子程式
void Back(int speed);              //車子後退子程式
void Stop(void);              //車子停止子程式
void Left(void);             //車子左轉子程式
void Right(void);             //車子右轉子程式
void Left_slow(void);             //I can get rid of this
void Right_back(void);             //車子右轉子程式
void CWSpin(void);
void CCWSpin(void);
void lineTracking_F(void);
void lineTracking_B(void);



