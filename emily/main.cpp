/* -------------------- Inclusion of Libraries -------------------- */
#include <Arduino.h>
#define USE_TIMER_2 true
#include <TimerInterrupt.h>
#include <Servo.h>
// #include <PID_v1.h>
// #include <Metro.h>

/* -------------------- Declaration -------------------- */
Servo rightGate;
Servo leftGate;
Servo waving;

/* -------------------- PIN location -------------------- */
/* Servo Motors */
const int wavingPin            = 10; //8
const int rightGatePin         = 9; //9
const int leftGatePin          = 8; //3

/* DC Motors */
const int IN1_rightWheel_F     = 11;          //L298N IN1 pin connecting to Arduino pin 10 //11
const int IN2_rightWheel_B     = 12;          //L298N IN2 pin connecting to Arduino pin 11 //12
const int IN3_leftWheel_F      = 5;           //L298N IN3 pin connecting to Arduino pin 5
const int IN4_leftWheel_B      = 6;           //L298N IN4 pin connecting to Arduino pin 6

/* IR Sensors */
const byte side_front_A        = A0;
const byte side_back_B         = A8;
const byte front_left_1        = A2; //A1
const byte front_mid_2         = A1; //A2
const byte front_right_3       = A3;

/* Bumpers */
const int bumper_1             = 30;
const int bumper_2             = 32;

/* Switches */
const int Switch_start         = 40;
const int Switch_sabotage      = 42;
const int Switch_changeRotation= 44;

/* IR Beacon */
const byte phototransistor     = A10;

/* Ultrasonic Sensors */
const int frontUltra_IN        = 14;
const int frontUltra_OUT       = 15;

/* -------------------- Sensors Threshold -------------------- */
int blackTH_A = 300;
int whiteTH_A = 80;

int blackTH_B = 300;
int whiteTH_B = 100;

int redTH_1   = 600;
int whiteTH_1 = 550;

int redTH_2   = 580;
int whiteTH_2 = 530;

int redTH_3   = 650;
int whiteTH_3 = 580;

/* -------------------- Setup for IR Beacon Testing -------------------- */
int arrayForIRTesting[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int countForIRTesting;
int lastValueForPT;
int valueForPT;

/* -------------------- Other Constants -------------------- */
int spd              = 200;               // Set the value of speed. Range: 0~255
int flag;
int duration_us;
int startingDistance = 8;
int instantDistance;

long total_time;

long start_ultra_time;
long start_corner;

/* -------------------- Helper Functions -------------------- */
void Forward(int speed)          
{
  analogWrite(IN1_rightWheel_F, speed);
  analogWrite(IN2_rightWheel_B, 0);
  analogWrite(IN3_leftWheel_F,  speed);
  analogWrite(IN4_leftWheel_B,  0);
}

void Back(int speed) 
{
  analogWrite(IN1_rightWheel_F, 0);
  analogWrite(IN2_rightWheel_B, speed);
  analogWrite(IN3_leftWheel_F,  0);
  analogWrite(IN4_leftWheel_B,  speed);
} 

void Stop(void)             
{
  analogWrite(IN1_rightWheel_F, 0);
  analogWrite(IN2_rightWheel_B, 0);
  analogWrite(IN3_leftWheel_F,  0);
  analogWrite(IN4_leftWheel_B,  0);
}

void TurnLeft_Forward(int fasterWheel, int slowerWheel)             
{
  analogWrite(IN1_rightWheel_F, fasterWheel);
  analogWrite(IN2_rightWheel_B, 0);
  analogWrite(IN3_leftWheel_F,  slowerWheel);
  analogWrite(IN4_leftWheel_B,  0);
} 

void TurnRight_Forward(int fasterWheel, int slowerWheel)            
{
  analogWrite(IN1_rightWheel_F, slowerWheel);
  analogWrite(IN2_rightWheel_B, 0);
  analogWrite(IN3_leftWheel_F,  fasterWheel);
  analogWrite(IN4_leftWheel_B,  0);
}

void TurnLeft_Backward(int fasterWheel, int slowerWheel)             
{
  analogWrite(IN1_rightWheel_F, 0);
  analogWrite(IN2_rightWheel_B, fasterWheel);
  analogWrite(IN3_leftWheel_F,  0);
  analogWrite(IN4_leftWheel_B,  slowerWheel);
} 

void TurnRight_Backward(int fasterWheel, int slowerWheel)    
{
  analogWrite(IN1_rightWheel_F, 0);
  analogWrite(IN2_rightWheel_B, slowerWheel);
  analogWrite(IN3_leftWheel_F,  0);
  analogWrite(IN4_leftWheel_B,  fasterWheel);
} 

void CCWSpin(int speed) 
{
  analogWrite(IN1_rightWheel_F, speed);
  analogWrite(IN2_rightWheel_B, 0);
  analogWrite(IN3_leftWheel_F,  0);
  analogWrite(IN4_leftWheel_B,  speed);
}

void CWSpin(int speed)
{
  analogWrite(IN1_rightWheel_F, 0);
  analogWrite(IN2_rightWheel_B, speed);
  analogWrite(IN3_leftWheel_F,  speed);
  analogWrite(IN4_leftWheel_B,  0);
}

void lineTracking_Forward(void) 
{
  // if(millis()-start_corner <20000 )
  // {
  //   TurnLeft_Forward((0.75 - (millis()-start_corner)/(20000*2))*spd, (0.25 + (millis()-start_corner)/(3000*2))*spd);
  // }
  if(analogRead(side_front_A) > blackTH_A) {
    TurnRight_Forward(1.1*spd, 0.2*spd); //0.2*spd
  }
  if(analogRead(side_front_A) < whiteTH_A) {
    TurnLeft_Forward(0.8*spd, 0.6*spd); //0.8*spd 0.5*spd
  }
  //Forward(spd);
}

void lineTracking_Center(void) 
{
  if(analogRead(front_mid_2) > redTH_2 || analogRead(front_left_1) > redTH_1) {
    TurnRight_Forward(1.1*spd, 0.2*spd);
  }
  if(analogRead(front_mid_2) < whiteTH_2) {
    TurnLeft_Forward(0.8*spd, 0.6*spd);
  }
}

void lineTracking_Center_ComingHome(void)
{
  if(analogRead(front_mid_2) > redTH_2 || analogRead(front_right_3) > redTH_3) {
    TurnLeft_Forward(1.1*spd, 0.2*spd);
  }
  if(analogRead(front_mid_2) < whiteTH_2) {
    TurnRight_Forward(0.8*spd, 0.6*spd);
  }
}

void lineTracking_Backward(void) 
{
  // while((analogRead(side_back_B) > whiteTH_B))
  // {
  //   TurnRight_Backward(0.6*spd, 0.4*spd);
  //   //TurnRight_Backward(0.8*spd, 0.7*spd);
  // }
  // while((analogRead(side_front_A) > blackTH_A))
  // {
  //   TurnRight_Backward(0.6*spd, 0.5*spd);
  // }
  
  //   Back(spd);
  


  
  if(analogRead(side_back_B) > blackTH_B) {
    TurnRight_Backward(1.1*spd, 0.2*spd);
  }
  if(analogRead(side_back_B) < whiteTH_B) {
    TurnLeft_Backward(0.8*spd, 0.5*spd);
  }
}

void IRTesting(void) 
{
  if(countForIRTesting == 14)
  {
    countForIRTesting = 0;
    valueForPT = arrayForIRTesting[0];
  }

  arrayForIRTesting[countForIRTesting] = analogRead(A10);

  for(int i = 0; i < 15; i++) {
    valueForPT = max(arrayForIRTesting[i], valueForPT);
  }

  countForIRTesting++;
  delay(10);
}

void ultraFollowing_Pcontrol(void)
{
  digitalWrite(frontUltra_OUT, HIGH);
  delay(10);
  digitalWrite(frontUltra_OUT, LOW);

  duration_us = pulseIn(frontUltra_IN, HIGH);
  instantDistance = 0.017 * duration_us;

  if(instantDistance > startingDistance){
    TurnLeft_Forward(((instantDistance - startingDistance)/40 + 0.9)*spd, 0.6*spd);
    delay(50);
  }
  if(instantDistance < startingDistance){
    TurnRight_Forward(((startingDistance - instantDistance)/40 + 0.9)*spd, 0.6*spd);
    delay(50);
  }
}

void wavingMovement(void) 
{
  delay(100);
  waving.write(60);
  delay(100);
  waving.write(120);
  delay(100);
  waving.write(60);
  delay(100);
  waving.write(120);
  // delay(100);
  // waving.write(60);
  // delay(100);
  // waving.write(120);
  // delay(100);
  // waving.write(60);
  // delay(100);
  // waving.write(120);
  // delay(500);
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
  pinMode(rightGatePin, OUTPUT);
  pinMode(leftGatePin,  OUTPUT);
  pinMode(wavingPin,    OUTPUT);
  rightGate.attach(rightGatePin);
  leftGate.attach(leftGatePin);
  waving.attach(wavingPin);
  rightGate.write(0);
  leftGate.write(0);
  waving.write(90);

  /* IR Sensors */
  pinMode(side_front_A, INPUT);
  pinMode(front_left_1, INPUT);
  pinMode(front_mid_2,  INPUT);
  pinMode(front_right_3,INPUT);
  pinMode(side_back_B,  INPUT);

  /* Bumpers */
  pinMode(bumper_1,INPUT);
  pinMode(bumper_2,INPUT);

  /* Switches */
  pinMode(Switch_start        ,INPUT); // middle one
  pinMode(Switch_changeRotation   ,INPUT);
  pinMode(Switch_sabotage ,INPUT);

  /* Phototransistor */
  pinMode(phototransistor, INPUT);
  countForIRTesting = 0;
  lastValueForPT = 0;
  valueForPT = 0;

  /* Ultrasonic Sensors */
  pinMode(frontUltra_IN,INPUT);
  pinMode(frontUltra_OUT,OUTPUT);

  /* the Rest */
  delay(500);
  flag = 0;

}

void loop() {
  // Serial.print(analogRead(A0));
  // Serial.print(", ");
  // Serial.print(analogRead(A1));
  // Serial.print(", ");
  // Serial.print(analogRead(A2));
  // Serial.print(", ");
  // Serial.print(analogRead(A3));
  // Serial.print(", ");
  // Serial.println(analogRead(A8));
  // IRTesting();
  // Serial.println(valueForPT);

  total_time = millis();
  if (total_time >= 130000 && total_time <= 131000){
    Stop();
    leftGate.write(90);
    rightGate.write(90);
    wavingMovement();
    flag = 100;
  }

  if (digitalRead(Switch_start) == 0 && flag == 50){
    //Stop();
    //rightGate.write(0);
    //leftGate.write(0);
      if(digitalRead(Switch_changeRotation) == 0)
      {
        flag = 1;
      }
      else
      {
        flag = 51;
      }
  }
  if(digitalRead(Switch_start) == 1) {
    /* In the Studio. Flag: X */
    if(flag == 0){
      wavingMovement();
      if(digitalRead(Switch_changeRotation) == 0)
      {
        flag = 1;
      }
      else
      {
        flag = 51;
      }
      
    }
    // add the third switch here, to determine CW or CCW movement (important for robustness)
    // this current code is when the robot is facing left, implement code for when it's facing right
    if(flag == 1){ 
      CWSpin(0.7*spd); //0.7 //find out the threshold for when this stops reliably detecting the beacon
      IRTesting();
      if(valueForPT < 200){
        Stop();
        delay(1000);
        flag = 2;
      }
    }
    if(flag == 51){ 
      CCWSpin(0.7*spd);
      IRTesting();
      if(valueForPT < 200){
        Stop();
        delay(1000);
        flag = 52;
      }
    }
    if(flag == 2){
      CWSpin(0.7*spd);
      IRTesting();
      if(/*(valueForPT < lastValueForPT) && valueForPT > 900*/ valueForPT > 950) {
        Stop();
        delay(1000);
        // CCWSpin(0.6*spd);
        // delay(700);
        flag = 4;
      } else {
        lastValueForPT = valueForPT;
      }    
    }
    if(flag == 52){
      CCWSpin(0.7*spd);
      IRTesting();
      if(/*(valueForPT < lastValueForPT) && valueForPT > 900*/ valueForPT > 950) {
        Stop();
        delay(1000);
        // CCWSpin(0.6*spd);
        // delay(700);
        flag = 4;
      } else {
        lastValueForPT = valueForPT;
      }    
    }
    if(flag == 3){
      Stop();
      delay(750);
      flag = 4;
      Back(255);
    }

    /* Leave the Studio and Moving toward the Edge. Flag: 1X */
    //if((digitalRead(bumper_1) == 1 || digitalRead(bumper_2) == 1) && flag == 4){
    if( flag == 4){

       //Back(spd); //what's the point of this
       //delay(700);
      //  Stop();
      //  delay(500);
      //  Forward(spd*0.8);
      //  delay(500);
      //  Back(spd);
      //  delay(3000);
       Forward(spd*0.8);
       start_ultra_time = total_time;
       delay(100);
       flag = 11; 
    }
    if(flag == 11){
      ultraFollowing_Pcontrol();
      // if(analogRead(side_back_B) > blackTH_B) {
      
        if (total_time - start_ultra_time >= 4000){
        Stop();
        delay(500);
        lineTracking_Center();
        flag = 12;
      }
      
     
    }
    if(flag == 12){
      lineTracking_Center();
      if(analogRead(side_front_A) > blackTH_A || analogRead(front_right_3) > redTH_3){
        Stop();
        delay(500);
        flag = 21;
      }
    }  

    /* At the Edge, Start Side Line Tracking, Dumping the Presses. Flag: 2X */
    if(flag == 21){
      TurnLeft_Backward(1.2*spd, 0.2*spd);
      delay(1000);
      Back(255);
      delay(1500);
      Forward(spd);
      delay(500);
      flag = 22;

    } 
    if(analogRead(front_mid_2) < whiteTH_2 && flag == 22){ //whiteth3
      lineTracking_Forward();
    }
    if(analogRead(front_mid_2) > redTH_2 && flag == 22) { //redth3
      Stop();
      delay(1000);
      //leftGate.write(90);
      delay(1000);
      flag = 23;
      if (digitalRead(Switch_sabotage) == 1){
        flag = 31;
      }
      Forward(spd);
      delay(500);
      lineTracking_Forward();
    }
    if(flag == 23){
      lineTracking_Forward();
    }
    if(analogRead(front_mid_2) > redTH_2 && flag == 23){ //redth2
      Stop();
      delay(1000);
      leftGate.write(90);
      rightGate.write(90);
      flag = 31;
      delay(1000);
    }

    /* Start Coming Home. Flag: 3X */
    if(flag == 31){
      leftGate.write(90);
      rightGate.write(90); //dump both
      lineTracking_Backward();
    }
    if((digitalRead(bumper_1) == 1 || digitalRead(bumper_2) == 1) && flag == 31){
      TurnRight_Forward(1*spd, 0);
      flag = 32;
      delay(500);
    }
    if(flag == 32){
      lineTracking_Center_ComingHome();
    }
    if((analogRead(side_front_A) > blackTH_A || analogRead(front_right_3) > redTH_3) && flag == 32){
      Forward(spd);
      delay(750);
      Stop();
      delay(1000);
      wavingMovement();
      flag = 50;
      rightGate.write(0);
      leftGate.write(0);
    }
  }
}
