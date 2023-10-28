//Motor control video: https://www.youtube.com/watch?v=7spK_BkMJys

#include <Wire.h>
#include "/home/krisztian/arduino/ClawMachine_Nano/communication.h"

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


#define X_DIRECTION_STEP_COUNT 3
#define Y_DIRECTION_STEP_COUNT 3
#define Z_DIRECTION_STEP_COUNT 3



//const int TIME = 1500;
const int MICROTIME = 1500; //suggestion in the video: 700-3000, this sets the amount of microseconds between triggering the stepper's input signal
//const int stepCount = 1000; //100 step is about 1/4 revolution, at least it seems that way

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
}

void moveLeft(uint16_t stepCount)
{
  digitalWrite(STEP_DIR_X, HIGH);
  for(int i = 0; i<stepCount; i++)
  {
    if (limiterStates[0] != 0 ) break;
    digitalWrite(STEP_PIN_X, HIGH);
    delayMicroseconds(MICROTIME);
    digitalWrite(STEP_PIN_X, LOW);
    delayMicroseconds(MICROTIME);
  }
}

void moveRight(uint16_t stepCount)
{
  digitalWrite(STEP_DIR_X, LOW);
  for(int i = 0; i<stepCount; i++)
  {
    if (limiterStates[0] != 0 ) break;
    digitalWrite(STEP_PIN_X, HIGH);
    delayMicroseconds(MICROTIME);
    digitalWrite(STEP_PIN_X, LOW);
    delayMicroseconds(MICROTIME);
  }
}

void moveUp(uint16_t stepCount)
{
  digitalWrite(STEP_DIR_Y, HIGH);
  for(int i = 0; i<stepCount; i++)
  {
    if (limiterStates[1] != 0 ) break;
    digitalWrite(STEP_PIN_Y, HIGH);
    delayMicroseconds(MICROTIME);
    digitalWrite(STEP_PIN_Y, LOW);
    delayMicroseconds(MICROTIME);
  }
}

void moveDown(uint16_t stepCount)
{
  digitalWrite(STEP_DIR_Y, LOW);
  for(int i = 0; i<stepCount; i++)
  {
    if (limiterStates[1] != 0 ) break;
    digitalWrite(STEP_PIN_Y, HIGH);
    delayMicroseconds(MICROTIME);
    digitalWrite(STEP_PIN_Y, LOW);
    delayMicroseconds(MICROTIME);
  }
}

void moveClawUp(uint16_t stepCount)
{
  digitalWrite(STEP_DIR_Z, HIGH);
  for(int i = 0; i<stepCount; i++)
  {
    digitalWrite(STEP_PIN_Z, HIGH);
    delayMicroseconds(MICROTIME);
    digitalWrite(STEP_PIN_Z, LOW);
    delayMicroseconds(MICROTIME);
  }
}

void moveClawDown(uint16_t stepCount)
{
  digitalWrite(STEP_DIR_Z, LOW);
  for(int i = 0; i<stepCount; i++)
  {
    digitalWrite(STEP_PIN_Z, HIGH);
    delayMicroseconds(MICROTIME);
    digitalWrite(STEP_PIN_Z, LOW);
    delayMicroseconds(MICROTIME);
  }
}

void loop() 
{
  if (msgMove.getX() == X_Direction::LEFT)            moveLeft(X_DIRECTION_STEP_COUNT);
  if (msgMove.getX() == X_Direction::RIGHT)           moveRight(X_DIRECTION_STEP_COUNT);
  if (msgMove.getY() == Y_Direction::UP)              moveUp(Y_DIRECTION_STEP_COUNT);
  if (msgMove.getY() == Y_Direction::DOWN)            moveDown(Y_DIRECTION_STEP_COUNT);
  if (msgMove.getClaw() == Claw_Direction::CLAW_UP)   moveClawUp(Z_DIRECTION_STEP_COUNT);
  if (msgMove.getClaw() == Claw_Direction::CLAW_DOWN) moveClawDown(Z_DIRECTION_STEP_COUNT);
  msgMove.setDefaultValues(); //making sure that if we loose connection, everything stops
}


