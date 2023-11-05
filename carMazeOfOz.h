#ifndef carMazeOfOz_h
#define carMazeOfOz_h
#include <Arduino.h>

/** @defgroup TIMER_CONFIGURATION
  * @{
  */
#define STEP_TIME_64      4e-6
#define TIMER1_STEP_CYCLE 65536
#define COUNTER_TIMER     TCNT1           //timer counter channel 1
/**
  * @}
  */
/** @defgroup PWM_LIMIT
  * @{
  */
#define MAX_PWM           255
#define MIN_PWM           0
/**
  * @}
  */
//----------------------------------------------------------------
#define SOUND_SPEED      34320
#define PERIMETER_       255254.4
#define FORWARD          1
#define BACKWARD         0 
#define HIGH             1
#define LOW              0

class carMazeOfOz {
  public:
    carMazeOfOz();
    void setPin();
    void setInterrupt();
    void setMotorLeft(byte speed, bool direction);
    void setMotorRight(byte speed, bool direction);
    void setSpeedLeft(volatile float speed);
    void setSpeedRight(volatile float speed);
    float getSpeedLeft();
    float getSpeedRight();
    float getDistanceHead();
    float getDistanceLeft();
    float getDistanceRight();
    void configureSpeed(volatile float &speedValueLeft, volatile float &speedValueRight);
  private:
    volatile float speedValueLeft, speedValueRight;
};

#endif
