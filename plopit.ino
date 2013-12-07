
int incomingByte=0;

int currentLevel = 1;
int waitCounter = 0;
int score = 0;
int currentEvent = 0;
boolean playing = false;
const int PLUNGE_IT = 0;
const int FLUSH_IT = 1;
const int LIFT_IT = 2;

const int MAX_WAIT_SECONDS = 10;
const int SLEEP_TIME = 200;

const int PLUNGE_PIN=2;
const int FLUSH_PIN=4;
const int LIFT_PIN=6;
const int NEW_GAME_PIN=8;

const String NEW_GAME_STRING = "plop";
const String PLUNGE_STRING = "plunge";
const String LIFT_STRING = "lift";
const String FLUSH_STRING = "flush";

void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");
  pinMode(PLUNGE_PIN, INPUT);
  pinMode(FLUSH_PIN, INPUT);
  pinMode(LIFT_PIN, INPUT);
  pinMode(NEW_GAME_PIN, INPUT);
  //newGame();
  digitalWrite(PLUNGE_PIN,1); 
  digitalWrite(FLUSH_PIN,1); 
  digitalWrite(LIFT_PIN,1); 
  digitalWrite(NEW_GAME_PIN,1); 
  delay(200);
}


void loop() {
  readInputs();
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
  if(currentEvent==PLUNGE_IT) {
    return expectPlungeIt();
  } else if(currentEvent == FLUSH_IT) {
    return expectFlushIt(); 
  } else {
    return expectLiftIt(); 
  }
}

boolean incorrectResponse() {
  if(currentEvent==PLUNGE_IT) {
    return expectFlushIt() || expectLiftIt();
  } else if(currentEvent==FLUSH_IT) {
    return expectPlungeIt() || expectLiftIt();
  } else {
    return expectPlungeIt() || expectFlushIt(); 
  }
}

boolean expectPlungeIt() {
  //Serial.print("Plunge: ");
  //Serial.println(digitalRead(PLUNGE_PIN));
  return digitalRead(PLUNGE_PIN) == LOW;
}

boolean expectFlushIt() {
  //Serial.print("Flush: ");
  //Serial.println(digitalRead(FLUSH_PIN));
  return digitalRead(FLUSH_PIN) == LOW;
}

boolean expectLiftIt() {
  //Serial.print("Lift: ");
  //Serial.println(digitalRead(LIFT_PIN));
  return digitalRead(LIFT_PIN) == HIGH;
}

boolean newGamePressed() {
  //Serial.print("NewGame: ");
  //Serial.println(digitalRead(NEW_GAME_PIN));
  return digitalRead(NEW_GAME_PIN)==LOW;
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
  if(event == PLUNGE_IT) {
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


