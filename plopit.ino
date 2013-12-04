
int incomingByte=0;

int currentLevel = 1;
int waitCounter = 0;
int score = 0;
int currentEvent = 0;
boolean playing = false;
const int PLOP_IT = 0;
const int FLUSH_IT = 1;
const int LIFT_IT = 2;

const int MAX_WAIT_SECONDS = 10;
const int SLEEP_TIME = 200;

const int PLOP_PIN=2;
const int FLUSH_PIN=4;
const int LIFT_PIN=6;
const int NEW_GAME_PIN=8;

void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");
  pinMode(PLOP_PIN, INPUT);
  pinMode(FLUSH_PIN, INPUT);
  pinMode(LIFT_PIN, INPUT);
  pinMode(NEW_GAME_PIN, INPUT);
  newGame(); 

}


void loop() {
  if(newGamePressed()) {
    newGame(); 
  }
  if(gameOn()) {
    if(waitCounter==0) {
      currentEvent = generateEvent();
      returnEvent(currentEvent);
    }
    waitCounter++;
    checkForTimeOut();
    checkForCorrectResponse();
    checkForIncorrectResponse();
  }
  delay(SLEEP_TIME);
}


void checkForTimeOut() {
  if(waitTimeExceeded(waitCounter)) {
    Serial.println("Time exceeded!");
    gameOverActions();
  }
}

void checkForCorrectResponse() {
  if(correctResponse()) {
    Serial.println("Right input!");
    correctResponseActions(); 
  }
}

void checkForIncorrectResponse() {
  if(incorrectResponse()) {
    Serial.println("Wrong input!");
    gameOverActions(); 
  }
}

void correctResponseActions() {
  waitCounter = 0;
  score++; 
  delay(5000);
}

void gameOverActions() {
  gameOver(score);
  waitCounter = 0;
  delay(4000);
}

boolean newGamePressed() {
  digitalRead(NEW_GAME_PIN)==HIGH;
}

void newGame() {
  waitCounter = 0;
  score = 0;
  playing = true;
  Serial.println("GameStarting!");
  delay(4000);
}

boolean gameOn() {
  return playing;
}

boolean correctResponse() {
  if(currentEvent==PLOP_IT) {
    return expectPlopIt();
  } else if(currentEvent == FLUSH_IT) {
    return expectFlushIt(); 
  } else {
    return expectLiftIt(); 
  }
}

boolean incorrectResponse() {
  if(currentEvent==PLOP_IT) {
    return expectFlushIt() || expectLiftIt();
  } else if(currentEvent==FLUSH_IT) {
    return expectPlopIt() || expectLiftIt();
  } else {
    return expectPlopIt() || expectFlushIt(); 
  }
}

boolean expectPlopIt() {
  return digitalRead(PLOP_PIN) == HIGH;
  //return true;
}

boolean expectFlushIt() {
  return digitalRead(FLUSH_PIN) == HIGH;
}

boolean expectLiftIt() {
  return digitalRead(LIFT_PIN) == HIGH;
}

boolean waitTimeExceeded(int count) {
  false;
  //return ((MAX_WAIT_SECONDS*1000)-(count*SLEEP_TIME)) <= 0; 
}

void gameOver(int score) {
  //if (Serial.available() > 0) {
    Serial.print("Game OVER! Your score is ");
    Serial.println(score); 
  //}
  playing = false;
}

int generateEvent() {
  return random(3);
}

void returnEvent(int event) {
  //Serial.println(event);
  if(event == PLOP_IT) {
    sendMessage("PlopIt");
  } else if(event == FLUSH_IT) {
    sendMessage("FlushIt");
  } else {
    sendMessage("LiftIt");
  }
}

void sendMessage(String message) {
  //if (Serial.available() > 0) {
    Serial.println(message); 
  //}
}


