//Motor control video: https://www.youtube.com/watch?v=7spK_BkMJys

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



const int TIME = 1500;
const int MICROTIME = 1500; //suggestion in the video: 700-3000
const int stepCount = 1000; //100 step is about 1/4 revolution, at least it seems that way


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

}

void loop() 
{

//Test mode:
  digitalWrite(STEP_DIR_X, HIGH);
  digitalWrite(STEP_DIR_Y, HIGH);                    
  digitalWrite(STEP_DIR_Z, HIGH);

  //Move all 3 at once one revolution
  for(int i = 0; i<stepCount; i++)
  {
    digitalWrite(STEP_PIN_X, HIGH);
    digitalWrite(STEP_PIN_Y, HIGH);
    digitalWrite(STEP_PIN_Z, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);

    delayMicroseconds(MICROTIME);
    digitalWrite(STEP_PIN_X, LOW);
    digitalWrite(STEP_PIN_Y, LOW);
    digitalWrite(STEP_PIN_Z, LOW);
    digitalWrite(LED_BUILTIN, LOW);

    delayMicroseconds(MICROTIME);
  }


  delay(TIME);

  digitalWrite(STEP_DIR_X, LOW);
  digitalWrite(STEP_DIR_Y, LOW);                    
  digitalWrite(STEP_DIR_Z, LOW);  

  for(int i = 0; i<stepCount; i++)
  {
    digitalWrite(STEP_PIN_X, HIGH);
    digitalWrite(STEP_PIN_Y, HIGH);
    digitalWrite(STEP_PIN_Z, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);

    delayMicroseconds(MICROTIME);
    digitalWrite(STEP_PIN_X, LOW);
    digitalWrite(STEP_PIN_Y, LOW);
    digitalWrite(STEP_PIN_Z, LOW);
    digitalWrite(LED_BUILTIN, LOW);

    delayMicroseconds(MICROTIME);
  }   

  delay(TIME);

}


