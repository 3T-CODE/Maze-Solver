#include "carMazeOfOz.h"


const byte trig = 8;
carMazeOfOz car;

volatile float speedValueLeft, speedValueRight;
volatile float speedValueLeft_SAMPLE[11], speedValueRight_SAMPLE[11];
volatile unsigned long timerPoint = 0, currentEncoderLeft = 0, currentEncoderRight = 0;

// Your variables is in the area below
//--------------------------------------------------//





//--------------------------------------------------//

void setup() {
  Serial.begin(9600);
  car.setPin();
  car.setInterrupt();
  attachInterrupt(LOW, ENC_LEFT_ISR, RISING);
  attachInterrupt(HIGH, ENC_RIGHT_ISR, RISING);
}

void ENC_LEFT_ISR() {
  static byte i = 0;
  speedValueLeft_SAMPLE[10] -= speedValueLeft_SAMPLE[i];
  speedValueLeft_SAMPLE[i] = (COUNTER_TIMER - currentEncoderLeft + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE;
  speedValueLeft_SAMPLE[10] += speedValueLeft_SAMPLE[i];
  speedValueLeft = PERIMETER_ / (speedValueLeft_SAMPLE[10] / 10);
  car.setSpeedLeft(speedValueLeft);
  i = (i + 1) % 10;
  currentEncoderLeft = COUNTER_TIMER;
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
}

// Your functions is in the area below
//--------------------------------------------------//





//--------------------------------------------------//

void loop() {
  digitalWrite(trig, LOW);
  car.setSpeedLeft(speedValueLeft);
  car.setSpeedRight(speedValueRight);
  car.configureSpeed(speedValueLeft, speedValueRight);
  
  // Your code is in the area below
  //--------------------------------------------------//
  //-----Example code-----//
  if (car.getDistanceHead() < 50) {
    if (car.getDistanceHead() < 20) {
      car.setMotorLeft(MAX_PWM, BACKWARD);
      car.setMotorRight(MAX_PWM, FORWARD);
    } else {
      car.setMotorLeft(150, FORWARD);
      car.setMotorRight(150, FORWARD);
    }
  } else {
    car.setMotorLeft(200, FORWARD);
    car.setMotorRight(200, FORWARD);
  }
  
  // Use Serial Monitor or Plotter in Arduino IDE to display the values on the screen
  Serial.print(car.getDistanceHead());
  Serial.print(" ");
  Serial.print(car.getSpeedLeft());
  Serial.print(" ");
  Serial.println(car.getSpeedRight());
  //-----Example code-----//
  
  
  
  
  
  //--------------------------------------------------//
}
