//Motor control video: https://www.youtube.com/watch?v=7spK_BkMJys
#define DEBUG
#include <Wire.h>
#include "/home/krisztian/arduino/ClawMachine_Nano/communication.h"
#include "/home/krisztian/arduino/ClawMachine_Nano/timer.h"
//#include "/home/krisztian/arduino/ClawMachine_Nano/millisTimer.h"


#define STEP_PIN_X 2
#define STEP_PIN_Y 3
#define STEP_PIN_Z 4

#define STEP_DIR_X 5
#define STEP_DIR_Y 6
#define STEP_DIR_Z 7

#define ENABLER 8

#define LIMIT_X 9
#define LIMIT_Y 10
#define LIMIT_Z 11

#define X_DIRECTION_STEP_COUNT 30
#define Y_DIRECTION_STEP_COUNT 40
#define Z_DIRECTION_STEP_COUNT 80
int zDirectionMaxStepCountRange = 3000; //can be calibrated


//suggestion in the video: 700-3000, this sets the amount of microseconds between triggering the stepper's input signal

#define X_MICRO_TIME 3000
#define Y_MICRO_TIME 3000
#define Z_MICRO_TIME 800

Timer xTimer(X_MICRO_TIME);
Timer yTimer(Y_MICRO_TIME);
Timer zTimer(Z_MICRO_TIME);


Move* Move::instance = nullptr; //have to be 'assigned'
Move msgMove(false);
bool limiterStates[3];

void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ENABLER, OUTPUT);

  pinMode(STEP_PIN_X, OUTPUT);
  pinMode(STEP_PIN_Y, OUTPUT);
  pinMode(STEP_PIN_Z, OUTPUT);

  pinMode(STEP_DIR_X, OUTPUT);
  pinMode(STEP_DIR_Y, OUTPUT);
  pinMode(STEP_DIR_Z, OUTPUT);

  pinMode(LIMIT_X, INPUT);
  pinMode(LIMIT_Y, INPUT);
  pinMode(LIMIT_Z, INPUT);

  digitalWrite(ENABLER, LOW);

  limiterStates[0] = 0;
  limiterStates[1] = 0;
  limiterStates[2] = 0; //there is no physical limiter switch for this

  msgMove.instance = &msgMove;
  Wire.onReceive(Move::onReceiveCallBack);
  Wire.onRequest(requestEvent);

  //Debug:
  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("SETUP RAN.");
  #endif // DEBUG
}

void moveLeft(uint16_t stepCount)
{
  digitalWrite(STEP_DIR_X, HIGH);
  for(int i = 0; i<stepCount; i++)
  {
    if (limiterStates[0] != 0 ) break;
    digitalWrite(STEP_PIN_X, HIGH);
    xTimer.doDelay();
    digitalWrite(STEP_PIN_X, LOW);
    xTimer.doDelay();
  }
}

void moveRight(uint16_t stepCount)
{
  digitalWrite(STEP_DIR_X, LOW);
  for(int i = 0; i<stepCount; i++)
  {
    if (limiterStates[0] != 0 ) break;
    digitalWrite(STEP_PIN_X, HIGH);
    xTimer.doDelay();
    digitalWrite(STEP_PIN_X, LOW);
    xTimer.doDelay();
  }
}

void moveUp(uint16_t stepCount)
{
  digitalWrite(STEP_DIR_Y, HIGH);
  for(int i = 0; i<stepCount; i++)
  {
    if (limiterStates[1] != 0 ) break;
    digitalWrite(STEP_PIN_Y, HIGH);
    yTimer.doDelay();
    digitalWrite(STEP_PIN_Y, LOW);
    yTimer.doDelay();
  }
}

void moveDown(uint16_t stepCount)
{
  digitalWrite(STEP_DIR_Y, LOW);
  for(int i = 0; i<stepCount; i++)
  {
    if (limiterStates[1] != 0 ) break;
    digitalWrite(STEP_PIN_Y, HIGH);
    yTimer.doDelay();
    digitalWrite(STEP_PIN_Y, LOW);
    yTimer.doDelay();
  }
}

void moveClawUp(uint16_t stepCount)
{
  digitalWrite(STEP_DIR_Z, HIGH);
  for(int i = 0; i<stepCount; i++)
  {
    digitalWrite(STEP_PIN_Z, HIGH);
    zTimer.doDelay();
    digitalWrite(STEP_PIN_Z, LOW);
    zTimer.doDelay();
  }
}

void moveClawDown(uint16_t stepCount)
{
  digitalWrite(STEP_DIR_Z, LOW);
  for(int i = 0; i<stepCount; i++)
  {
    digitalWrite(STEP_PIN_Z, HIGH);
    zTimer.doDelay();
    digitalWrite(STEP_PIN_Z, LOW);
    zTimer.doDelay();
  }
}

void moveHome()
{
  while(limiterStates[0])
  {
    moveLeft(X_DIRECTION_STEP_COUNT);
  }

  while(limiterStates[1])
  {
    moveDown(Y_DIRECTION_STEP_COUNT);
  }
}

bool containsGivenBits(uint8_t inThis, uint8_t contained)
{
  uint8_t temp = inThis & contained;
  if(temp == contained)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void calibration()
{
  while(msgMove.getClawCalibState() < (Claw_Calibration::CLAW_CALIB_TOP_DONE | Claw_Calibration::CLAW_CALIB_DOWN_DONE )) //if calib done top and down are set they are the biggest number possible
  {
    #ifdef DEBUG
    Serial.println("In the < top | down done while loop");
    #endif // DEBUG

    //setting the top position for the claw since it does not have a limiter
    while(containsGivenBits(msgMove.getClawCalibState(), Claw_Calibration::CLAW_CALIB_TOP_STATE_IN_PROGRESS))
    {
      #ifdef DEBUG
      Serial.println("Top state in progress.");
      #endif // DEBUG

      if (msgMove.getY() == Y_Direction::UP)
      {
        moveClawUp(Z_DIRECTION_STEP_COUNT);
      }
      if (msgMove.getY() == Y_Direction::DOWN)
      {
        moveClawDown(Z_DIRECTION_STEP_COUNT);
      }
      if (msgMove.getButtonState() == Main_Button::PUSHED)
      {
        zDirectionMaxStepCountRange = 0;
        msgMove.topCalibDone();
        //msgMove.sendMsgToNano(); //not yet implemented
        //we are done with the Top position calibration:
      }

      msgMove.setDefaultValues(); //making sure that if we loose connection, everything stops
    }

    //calib state is constantly read from NANO side
    //delay(1);

    while(containsGivenBits(msgMove.getClawCalibState(), Claw_Calibration::CLAW_CALIB_DOWN_STATE_IN_PROGRESS))
    {
      #ifdef DEBUG
      Serial.println("DOWN state in progress.");
      #endif // DEBUG

      if (msgMove.getY() == Y_Direction::UP)
      {
        moveClawUp(Z_DIRECTION_STEP_COUNT);
        zDirectionMaxStepCountRange -= Z_DIRECTION_STEP_COUNT;
      }
      if (msgMove.getY() == Y_Direction::DOWN)
      {
        moveClawDown(Z_DIRECTION_STEP_COUNT);
        zDirectionMaxStepCountRange += Z_DIRECTION_STEP_COUNT;
      }
      if (msgMove.getButtonState() == Main_Button::PUSHED)
      {
        if(zDirectionMaxStepCountRange > 0)
        {
          msgMove.downCalibDone();
          //msgMove.sendMsgToNano();
        }
        else
        {
          msgMove.setBadCalibState(); //unset should be done from NANO side
          //msgMove.sendMsgToNano();
        }
      }
      msgMove.setDefaultValues(); //making sure that if we loose connection, everything stops
    }
    //finish calib should be done from NANO side
  }
}

void requestEvent()
{
  #ifdef DEBUG
  Serial.println("REQUEST EVENT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  #endif // DEBUG

  msgMove.refreshMovementState(zDirectionMaxStepCountRange);
  MovementDataPack toBeSent = msgMove.getMovementState();
  Wire.write((byte*)&toBeSent, sizeof(MovementDataPack));
}

void loop() 
{
  if (msgMove.getClawCalibState() == Claw_Calibration::CLAW_CALIB_INIT)
  {
    #ifdef DEBUG
    Serial.println("Starting calibration");
    #endif // DEBUG
    calibration();
  }
  else
  {
    #ifdef DEBUG
    Serial.println("Checking movement");
    #endif // DEBUG
    if (msgMove.getX() == X_Direction::LEFT)            moveLeft(X_DIRECTION_STEP_COUNT);
    if (msgMove.getX() == X_Direction::RIGHT)           moveRight(X_DIRECTION_STEP_COUNT);
    if (msgMove.getY() == Y_Direction::UP)              moveUp(Y_DIRECTION_STEP_COUNT);
    if (msgMove.getY() == Y_Direction::DOWN)            moveDown(Y_DIRECTION_STEP_COUNT);
    if (msgMove.getClaw() == Claw_Direction::CLAW_UP)   moveClawUp(Z_DIRECTION_STEP_COUNT);
    if (msgMove.getClaw() == Claw_Direction::CLAW_DOWN) moveClawDown(Z_DIRECTION_STEP_COUNT);
    //put full claw action here with zDirection max range
  }
  msgMove.setDefaultValues(); //making sure that if we loose connection, everything stops
}


