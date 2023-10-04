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



const int TIME = 500;
const int MICROTIME = 1500; //suggestion in the video: 700-3000
const int stepCount = 1000; //100 step is about 1/4 revolution, at least it seems that way

void myDelay(unsigned long ms)
{
  unsigned long startTime = millis();
  unsigned long deltaTime = 0;
  unsigned long currentTime = 0;
 
  CHECK_AGAIN:
  currentTime = millis();
  if(startTime > currentTime) //in case myDelay was called near the end of its max capacity
  {
    return; //we just simply return
  }
  deltaTime = currentTime - startTime;

  if(deltaTime > ms)
  {
    return;
  }
  else
  {
    goto CHECK_AGAIN;
  }

}


unsigned long currentTime()
{
  return millis();
}


void setup() 
{
  Serial.begin(9600);
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
  Serial.write("SETUP");

}

void loop() 
{
  Serial.write("A");

//Test mode:
  digitalWrite(STEP_DIR_X, HIGH);
  digitalWrite(STEP_DIR_Y, HIGH);                    
  digitalWrite(STEP_DIR_Z, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);

  //Move all 3 at once one revolution
  for(int i = 0; i<stepCount; i++)
  {
    digitalWrite(STEP_PIN_X, HIGH);
    digitalWrite(STEP_PIN_Y, HIGH);
    digitalWrite(STEP_PIN_Z, HIGH);
    delayMicroseconds(MICROTIME);
    digitalWrite(STEP_PIN_X, LOW);
    digitalWrite(STEP_PIN_Y, LOW);
    digitalWrite(STEP_PIN_Z, LOW);
    delayMicroseconds(MICROTIME);
  }
  Serial.println("B");
  Serial.println(millis());
  myDelay(TIME);
  Serial.println(millis());

  Serial.write("C");
  digitalWrite(STEP_DIR_X, LOW);
  digitalWrite(STEP_DIR_Y, LOW);                    
  digitalWrite(STEP_DIR_Z, LOW);
  digitalWrite(LED_BUILTIN, LOW);


  for(int i = 0; i<stepCount; i++)
  {
    digitalWrite(STEP_PIN_X, HIGH);
    digitalWrite(STEP_PIN_Y, HIGH);
    digitalWrite(STEP_PIN_Z, HIGH);
    delayMicroseconds(MICROTIME);
    digitalWrite(STEP_PIN_X, LOW);
    digitalWrite(STEP_PIN_Y, LOW);
    digitalWrite(STEP_PIN_Z, LOW);
    delayMicroseconds(MICROTIME);
  }   

  Serial.println("D");
  myDelay(TIME);
  Serial.println(millis());


}


