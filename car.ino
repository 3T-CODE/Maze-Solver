// kiem soat duoc cam bien ---- quan trong !
// co hai van de chinh: dieu khien xe va giai thuat giai me cung
// dieu khien xe thi sao? thi gom cac tac vu co ban nhu quay trai, quay phai, di thang quay dau nguoc lai va dung
// giai thuat thi sao? phan giai thuat va implemented bang code 
// 
// cach xoay 90 do bang encoder?

// khi dat xe o giua cach hai tuong 3cm thi cam bien tra ve no bi lech ... . --- do mcu
// cam bien < 2cm thi phai co cach xu ly! 2cm - 4cm --- cho lui xe

//wall distance???

//====================x===================
// lieu co the chay qua het me cung mot luot roi de ghi lai du lieu roi sau do moi cho no chay lai de toi uu ko?

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
//purpose of this? read speed òf two motors?
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

// Your functions are in the area below
//--------------------------------------------------//

void speedLeftPID(float speedSetPoint ) {
  float Kp = 1, Ki = 0.2;
  float e = speedSetPoint - car.getSpeedLeft();
  float P = Kp * e;
  static float I = 0;
  I += Ki * e;
  setSpeedLeft = constrain(P + I, 0, 255);
}

void speedRightPID(float speedSetPoint) {
  float Kp = 1, Ki = 0.2;
  float e = speedSetPoint - car.getSpeedRight();
  float P = Kp * e;
  static float I = 0;
  I += Ki * e;
  setSpeedRight = constrain(P + I, 0, 255);
}

//void distancePID() {
//  float Kp = 2, Ki = 0.25;
//  float e = distanceSetPoint - car.getDistanceRight();
//  float P = Kp * e;
//  static float I = 0;
//  if (e > -0.5 && e < 0.5) {
//    I = 0;
//  } else {
//    I += Ki * e;
//  }
//  setDistance = constrain(P + I, -50, 50);
//}

//void rateOfChangeDistancePID() {
//  float Kp = 0.1, Ki = 0.1;
//  float e = rateSetPoint - car.getRateOfChangeDistanceLeft();
//  float P = Kp * e;
//  static float I = 0;
//  if (e > -0.5 && e < 0.5) {
//    I = 0;
//  } else {
//    I += Ki * e;
//  }
//  setRate = constrain(P + I, -50, 50);
//}

//--------------------------------------------------//


void loop() {
  digitalWrite(trig, LOW);
  car.setSpeedLeft(speedValueLeft);
  car.setSpeedRight(speedValueRight);
  car.configureSpeed(speedValueLeft, speedValueRight);  //??
  
  // Your code is in the area below
  //--------------------------------------------------//
  //-----Example code-----//
  if (turnFinish) {
    if (car.getDistanceRight() > 25) {
      turn.status = true;
      turnRight = true;
    } else if (car.getDistanceHead() < 10){
      turn.status = true;
      turnRight = false;
    }
    if (turn.status) {
      turnFinish = false;
    }
  }
  if (turn.status) {
    if (turnRight) {
      speedLeftPID(40);
      speedRightPID(40);
      car.setMotorLeft(setSpeedLeft, 1);
      car.setMotorRight(setSpeedRight, 0); 
    } else {
      speedLeftPID(40);
      speedRightPID(40);
      car.setMotorLeft(setSpeedLeft, 0);
      car.setMotorRight(setSpeedRight, 1);
    }
  } else {
    speedLeftPID(40);
    speedRightPID(40);
    car.setMotorLeft(setSpeedLeft, 1);
    car.setMotorRight(setSpeedRight, 1);
  }
  if (turn.value > 45) {
    turn.status = false;
    turn.value = 0;
    turnFinish = true;
    if (car.getDistanceHead() > 10) {
    speedLeftPID(40);
    speedRightPID(40);
    car.setMotorLeft(setSpeedLeft, 1);
    car.setMotorRight(setSpeedRight, 1);
    delay(550); //550
    }
  }

  if (car.getDistanceHead()> 500) {
    car.setMotorLeft(0, 0);
    car.setMotorRight(0, 0);
    delay(100);
  }


//  speedLeftPID();
//  speedRightPID();
//  car.setMotorLeft(setSpeedLeft, 1);
//  car.setMotorRight(setSpeedRight, 1);
  

  
     
//  // Use Serial Monitor or Plotter in Arduino IDE to display the values on the screen
//  Serial.print(car.getDistanceHead());
//  Serial.print(" ");
//  Serial.print(car.getSpeedLeft());
//////  Serial.print(car.getDistanceLeft());
//  Serial.print(" ");
//  Serial.println(car.getSpeedRight());
//  Serial.print(" ");
//  Serial.println(car.getDistanceRight());
  //-----Example code-----//

  //Righ-hand rule
  //always move forward, when detects obstacle --> always turn right
  //then move forward. when detects obstacle ahead and right, turn left
  // if detects obstacle ahead, right, left, turn around 180
  
  
  
  //--------------------------------------------------//
}
