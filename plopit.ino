
int incomingByte=0;

int currentLevel = 1;
int waitCounter = 0;
int score = 0;
int currentEvent = 0;
int lastEvent = -1;
boolean playing = false;
int lastButtonDown = -1;
const int PLUNGE_IT = 0;
const int FLUSH_IT = 1;
const int LIFT_IT = 2;
const int NEW_GAME = 3;

const int MAX_WAIT_SECONDS = 10;
const int SLEEP_TIME = 200;

const int PLUNGE_PIN=2;
const int FLUSH_PIN=4;
const int LIFT_PIN=6;
const int NEW_GAME_PIN=8;
const boolean logging=false;

const String NEW_GAME_STRING = "plop";
const String PLUNGE_STRING = "plunge";
const String LIFT_STRING = "lift";
const String FLUSH_STRING = "flush";
const String USER_PLUNGED_STRING = "plunged";
const String USER_LIFTED_STRING = "lifted";
const String USER_FLUSHED_STRING = "flushed";

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(PLUNGE_PIN, INPUT);
  pinMode(FLUSH_PIN, INPUT);
  pinMode(LIFT_PIN, INPUT);
  pinMode(NEW_GAME_PIN, INPUT);
  digitalWrite(PLUNGE_PIN,1); 
  digitalWrite(FLUSH_PIN,1); 
  digitalWrite(LIFT_PIN,1); 
  digitalWrite(NEW_GAME_PIN,1); 
  delay(200);
  //newGame();
}


void loop() {
  //readInputs();
  playGame();
}

void playGame() {
  if(newGamePressed()) {
    newGame(); 
  }
  if(gameOn()) {
    if(waitCounter==0) {
      currentEvent = generateEvent();
      returnEvent(currentEvent);
    }
    waitCounter++;
    userFeedback();
    checkForTimeOut();
    checkForCorrectResponse();
    checkForIncorrectResponse();
  }
  delay(SLEEP_TIME);
  
}

void readInputs() {
 //expectPlungeIt();
 //expectLiftIt();
 //expectFlushIt();
 //newGamePressed();
 
 if(expectPlungeIt()) {
  Serial.println(PLUNGE_STRING); 
 } else if(!expectLiftIt()) {
   Serial.println(LIFT_STRING);
 } else if(expectFlushIt()) {
   Serial.println(FLUSH_STRING);
 } else if(newGamePressed()) {
   Serial.println(NEW_GAME_STRING);
 }
 
 delay(10*100); 
}


void checkForTimeOut() {
  if(waitTimeExceeded(waitCounter)) {
    Serial.println("Time exceeded!");
    gameOverActions();
  }
}

void checkForCorrectResponse() {
  if(correctResponse()) {
    correctResponseActions(); 
  }
}

void userFeedback() {
  if(lastButtonDown==PLUNGE_IT) {
    Serial.println(USER_PLUNGED_STRING);
  } else if(lastButtonDown==FLUSH_IT) {
    Serial.println(USER_FLUSHED_STRING);
  } else if(lastButtonDown==LIFT_IT) {
    Serial.println(USER_LIFTED_STRING);
  }
}

void checkForIncorrectResponse() {
  if(incorrectResponse()) {
    userFeedback();
    //Serial.print(lastButtonDown);
    //Serial.println(" Wrong input!");
    gameOverActions(); 
  }
}

void correctResponseActions() {
  waitCounter = 0;
  lastEvent = currentEvent;
  score++; 
  correctResponseDelay();
}

void correctResponseDelay() {
  int baseDelay = 2000;
  int speedUp = score / 5;
  delay(baseDelay - speedUp*200);
}

void gameOverActions() {
  gameOver(score);
  waitCounter = 0;
  delay(4000);
}

void newGame() {
  waitCounter = 0;
  score = 0;
  playing = true;
  Serial.println(NEW_GAME_STRING);
  delay(4000);
}

boolean gameOn() {
  return playing;
}

boolean correctResponse() {
  if(stillHoldingPreviousButton()) {
    return false;
  }
  if(currentEvent==PLUNGE_IT) {
    return expectPlungeIt();
  } else if(currentEvent == FLUSH_IT) {
    return expectFlushIt(); 
  } else {
    return expectLiftIt(); 
  }
}

boolean incorrectResponse() {
  if(stillHoldingPreviousButton()) {
    return false;
  }
  if(currentEvent==PLUNGE_IT) {
    return expectFlushIt() || expectLiftIt();
  } else if(currentEvent==FLUSH_IT) {
    return expectPlungeIt() || expectLiftIt();
  } else {
    return expectPlungeIt() || expectFlushIt(); 
  }
}

boolean stillHoldingPreviousButton() {
  if(noButtonDown()) return false;
  return (lastButtonDown == lastEvent);
}

boolean noButtonDown() {
  if(!expectPlungeIt() && !expectFlushIt() && !expectLiftIt() && !newGamePressed()) {
    lastButtonDown = -1;
  }
}

boolean expectPlungeIt() {
  if(logging) {
    Serial.print("Plunge: ");
    Serial.println(digitalRead(PLUNGE_PIN));
  }
  boolean isDown = digitalRead(PLUNGE_PIN) == LOW;
  if(isDown) {
    lastButtonDown = PLUNGE_IT;
  }
  return isDown;
}

boolean expectFlushIt() {
  if(logging) {
    Serial.print("Flush: ");
    Serial.println(digitalRead(FLUSH_PIN));
  }
  boolean isDown = digitalRead(FLUSH_PIN) == HIGH;
  if(isDown) {
    lastButtonDown = FLUSH_IT;
  }
  return isDown;
}

boolean expectLiftIt() {
  if(logging) {
    Serial.print("Lift: ");
    Serial.println(digitalRead(LIFT_PIN));
  }
  boolean isDown = digitalRead(LIFT_PIN) == HIGH;
  if(isDown) {
    lastButtonDown = LIFT_IT;
  }
  return isDown;
}

boolean newGamePressed() {
  //if(logging) {
  //  Serial.print("NewGame: ");
  //  Serial.println(digitalRead(NEW_GAME_PIN));
  //}
  boolean isDown = digitalRead(NEW_GAME_PIN)==LOW;
  if(isDown) {
    lastButtonDown = NEW_GAME;
  }
  return isDown;
}

boolean waitTimeExceeded(int count) {
  false;
  //return ((MAX_WAIT_SECONDS*1000)-(count*SLEEP_TIME)) <= 0; 
}

void gameOver(int score) {
  //if (Serial.available() > 0) {
    //Serial.print("Game OVER! Your score is ");
    Serial.println(score); 
  //}
  playing = false;
}

int generateEvent() {
  return random(3);
}

void returnEvent(int event) {
  //Serial.println(event);
  if(event == PLUNGE_IT) {
    Serial.println(PLUNGE_STRING);
  } else if(event == FLUSH_IT) {
    Serial.println(FLUSH_STRING);
  } else {
    Serial.println(LIFT_STRING);
  }
}


