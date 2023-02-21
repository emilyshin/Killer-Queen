#include <Arduino.h>

int spd = 255;               // 設定速度值0-255
int timer = 1000;           // 定義整數 timer
int count;
char input;

const int IN1 = 10;          //L298N IN1 pin 連接 Arduino pin 10
const int IN2 = 11;          //L298N IN2 pin 連接 Arduino pin 11
const int IN3 = 5;           //L298N IN3 pin 連接 Arduino pin 5
const int IN4 = 6;            //L298N IN4 pin 連接 Arduino pin 6

void setup() 
{
  Serial.begin(9600);     //確認藍牙傳輸速率 9600 bits/second
  pinMode(6,OUTPUT);      // Arduino 輸出電壓控制車子
  pinMode(5,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  delay(500);  
}

void Forward(void)          //車子前進子程式
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

void loop() {            //主程式：車子前進1秒 (1000 ms)，後退1秒，左轉1秒，右轉1秒，停止1秒，如此再連續循環
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
    Forward ();
    delay(timer);
    Back();
    delay(timer);
    Left();
    delay(timer);
    Right();
    delay(timer);
    Stop();
    delay(timer);
}