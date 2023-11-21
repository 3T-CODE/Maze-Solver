

#include "carMazeOfOz.h"


const byte trig = 8;
carMazeOfOz car;

volatile float speedValueLeft, speedValueRight;
volatile float speedValueLeft_SAMPLE[11], speedValueRight_SAMPLE[11];
volatile unsigned long timerPoint = 0, currentEncoderLeft = 0, currentEncoderRight = 0;

// Your variables are in the area below
//--------------------------------------------------//
//int adjustedSpeed = 255;
bool turnFinish = true;
bool turnRight = false;
byte setSpeedLeft = 80;
byte setSpeedRight = 80; 


//const float speedSetPoint = 70;
const float distanceSetPoint = 9;
const float rateSetPoint = 0;

struct countPulse {
  byte value = 0;
  bool status = false;
};

volatile countPulse turn;

//--------------------------------------------------//

void setup() {
  Serial.begin(9600);
  car.setPin();
  car.setInterrupt();
  attachInterrupt(LOW, ENC_LEFT_ISR, RISING);
  attachInterrupt(HIGH, ENC_RIGHT_ISR, RISING);


  
}


void loop() {
   digitalWrite(trig, LOW);
  // car.setSpeedLeft(speedValueLeft);
  // car.setSpeedRight(speedValueRight);
  // car.configureSpeed(speedValueLeft, speedValueRight);  //??
  
  // Your code is in the area below
  //--------------------------------------------------//
  //-----Example code-----//
  // if (turnFinish) {
  //   if (car.getDistanceRight() > 25) {
  //     turn.status = true;
  //     turnRight = true;
  //   } else if (car.getDistanceHead() < 10){
  //     turn.status = true;
  //     turnRight = false;
  //   }
  //   if (turn.status) {
  //     turnFinish = false;
  //   }
  // }
  // if (turn.status) {
  //   if (turnRight) {
  //     speedLeftPID(40);
  //     speedRightPID(40);
  //     car.setMotorLeft(setSpeedLeft, 1);
  //     car.setMotorRight(setSpeedRight, 0); 
  //   } else {
  //     speedLeftPID(40);
  //     speedRightPID(40);
  //     car.setMotorLeft(setSpeedLeft, 0);
  //     car.setMotorRight(setSpeedRight, 1);
  //   }
  // } else {
  //   speedLeftPID(40);
  //   speedRightPID(40);
  //   car.setMotorLeft(setSpeedLeft, 1);
  //   car.setMotorRight(setSpeedRight, 1);
  // }
  // if (turn.value > 45) {
  //   turn.status = false;
  //   turn.value = 0;
  //   turnFinish = true;
  //   if (car.getDistanceHead() > 10) {
  //   speedLeftPID(40);
  //   speedRightPID(40);
  //   car.setMotorLeft(setSpeedLeft, 1);
  //   car.setMotorRight(setSpeedRight, 1);
  //   delay(100);
  //   }
  // }

  // if (car.getDistanceHead()> 200) {
  //   car.setMotorLeft(0, 0);
  //   car.setMotorRight(0, 0);
  //   delay(10);
  // }


  //-----PID_Trial------//
  
  //Begin_Express
  


   // Use Serial Monitor or Plotter in Arduino IDE to display the values on the screen

  car.setMotorRight( byte(Move(10)),1); 
  car.setMotorLeft( byte(Move(10) + Stick_R_distancePID(10, Move(10))),1) ;
  Serial.print(Move(10));

}

//------ADD_Function-------//

//--------Read_Enc---------//

void ENC_LEFT_ISR() {
  static byte i = 0;
  speedValueLeft_SAMPLE[10] -= speedValueLeft_SAMPLE[i];
  speedValueLeft_SAMPLE[i] = (COUNTER_TIMER - currentEncoderLeft + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE;
  speedValueLeft_SAMPLE[10] += speedValueLeft_SAMPLE[i];
  speedValueLeft = PERIMETER_ / (speedValueLeft_SAMPLE[10] / 10);
  car.setSpeedLeft(speedValueLeft);
  i = (i + 1) % 10;
  currentEncoderLeft = COUNTER_TIMER;
  if (turn.status) {
   turn.value++;
  }

}

void ENC_RIGHT_ISR() {
  static byte i = 0;
  speedValueRight_SAMPLE[10] -= speedValueRight_SAMPLE[i];
  speedValueRight_SAMPLE[i] = (COUNTER_TIMER - currentEncoderRight + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE;
  speedValueRight_SAMPLE[10] += speedValueRight_SAMPLE[i];
  speedValueRight = PERIMETER_ / (speedValueRight_SAMPLE[10] / 10);
  car.setSpeedRight(speedValueRight);
  i = (i + 1) % 10;
  currentEncoderRight = COUNTER_TIMER;
  if (turn.status) {
    turn.value++;
  }
}

//---------PID_FUNC---------//

// void speedLeftPID(float speedSetPoint ) {
//   float Kp = 1, Ki = 0.2;
//   float e = speedSetPoint - car.getSpeedLeft();
//   float P = Kp * e;
//   static float I = 0;
//   I += Ki * e;
//   setSpeedLeft = constrain(P + I, 0, 255);
// }

// void speedRightPID(float speedSetPoint) {
//   float Kp = 1, Ki = 0.2;
//   float e = speedSetPoint - car.getSpeedRight();
//   float P = Kp * e;
//   static float I = 0;
//   I += Ki * e;
//   setSpeedRight = constrain(P + I, 0, 255);
// }

//Stick to Right Wall Function//
float Stick_R_distancePID(float dis_Point, float speed) {
 float Kp = 2, Ki = 0.25;
 float e =  car.getDistanceRight() - dis_Point ;
 float P = Kp * e;
 static float I = 0;
 if (e > -0.5 && e < 0.5) {
   I = 0;
 } else {
   I += Ki * e;
 }
 
 return P +I;
 
}

//Move to Head Wall
float Move(float dis_Point)
{

  float Kp = 1 , Ki = 0.2;
  float e = car.getDistanceHead() - dis_Point;
  float P = Kp * e ;
  static float I = 0 ;
  if (e > -0.5 && e < 0.5)
  {
    I = 0 ;
  }
  else 
  {
    I+=Ki*e;
  }

  return  P + I;

  
}

//Give Signal to Turn
char Move_and_Turn(float Left_Dis , float Right_Dis , float Head_Dis)
{
if (Head_Dis <= 7)
  {
     delay(30);
    if (Right_Dis >= Left_Dis)
    {
       return 'r';
    }
    else 
    {
       return 'l';
    }

  }
  else
    {
    return 'm';
    }
}


