#include <Arduino.h>
#include <LiquidCrystal.h>
#include "MACROS.h"
#include <stdio.h>
#include <util/delay.h>
#define F_CPU 16000000UL

// --- Correct Mapping for Eta32mini + MightyCore Standard Pinout ---
LiquidCrystal lcd(25, 26, 27, 28, 29, 30);
// Keypad Pins
const int rowPins[4] = {4, 5, 6, 7};
const int colPins[4] = {10, 11, 12, 13};
char keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
#define BUZZER_PIN 21
// ===== CUSTOM CHARACTERS =====
byte player[8] = {B00111, B00101, B00111, B10110, B11111, B01110, B01010, B00000};   // Dino
byte obstacle[8] = {B00100, B00101, B10101, B10111, B11100, B00100, B00100, B00000}; // Cactus
byte alien[8] = {B10001, B01010, B11111, B10101, B11111, B01010, B10001, B00000};    // Classic Alien
byte ship[8] = {B00100, B01110, B11111, B10101, B00100, B01010, B11011, B00000};     // Cool Space Ship
byte laser[8] = {B00100, B00100, B00100, B00100, B00100, B00000, B00000, B00000};    // Laser Beam
byte explode[8] = {B10001, B01010, B00100, B01010, B10001, B00000, B00000, B00000};  // Explosion 💥

// ===== STATE VARIABLES =====
enum GameState{
  MENU,
  TIMER_SETUP,
  GAMES_MENU,
  DINO_GAME,
  SPACE_GAME
};
GameState currentState = MENU;

// ===== GAME VARIABLES =====
int highScoreDino = 0;
int highScoreSpace = 0;

// --- Function Prototypes ---
char getKey();
char getKeyNonBlocking();
void runTimer();
void showGamesMenu();
void runDinoGame();
void runSpaceGame();
void showMenu();
void playBeep(int duration);
void playGameOver();
void resetAll();

void setup(){
  lcd.begin(16, 2);
  lcd.createChar(0, player);
  lcd.createChar(1, obstacle);
  lcd.createChar(2, alien);
  lcd.createChar(3, ship);
  lcd.createChar(4, laser);
  lcd.createChar(5, explode);

  pinMode(BUZZER_PIN, OUTPUT);

  for (int i = 0; i < 4; i++){
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
    pinMode(colPins[i], INPUT_PULLUP);
  }

  randomSeed(analogRead(0));

  resetAll();

  lcd.clear();
  delay(100);
  lcd.print("ProjectMp v3.1");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
}

void loop()
{
  switch (currentState){
  case MENU:
    showMenu();
    break;
  case TIMER_SETUP:
    runTimer();
    break;
  case GAMES_MENU:
    showGamesMenu();
    break;
  case DINO_GAME:
    runDinoGame();
    break;
  case SPACE_GAME:
    runSpaceGame();
    break;
  default:
    currentState = MENU;
  }
}

// ===== MAIN MENU =====
void showMenu(){
  lcd.clear();
  lcd.print("** MAIN MENU **");
  lcd.setCursor(0, 1);
  lcd.print("1:Timer  2:Games");
  delay(200);

  char choice = getKey();

  if (choice == '1')
    currentState = TIMER_SETUP;
  else if (choice == '2')
    currentState = GAMES_MENU;

  delay(300);
}

// ===== GAMES SUB-MENU =====
void showGamesMenu(){
  lcd.clear();
  lcd.print("Select Game:");
  lcd.setCursor(0, 1);
  lcd.print("1:Dino   2:Space");
  delay(200);

  char choice = getKey();

  if (choice == '1')
    currentState = DINO_GAME;
  else if (choice == '2')
    currentState = SPACE_GAME;
  else if (choice == 'A' || choice == 'D')
    currentState = MENU;

  delay(300);
}

// --- Keypad Scanning Logic ---
char getKey(){
  while (true){
    for (int r = 0; r < 4; r++){
      digitalWrite(rowPins[r], LOW);
      for (int c = 0; c < 4; c++){
        if (digitalRead(colPins[c]) == LOW){
          delay(30);
          while (digitalRead(colPins[c]) == LOW);
          digitalWrite(rowPins[r], HIGH);
          delay(30);
          return keys[r][c];
        }
      }
      digitalWrite(rowPins[r], HIGH);
    }
  }
}

char getKeyNonBlocking(){
  for (int r = 0; r < 4; r++){
    digitalWrite(rowPins[r], LOW);
    for (int c = 0; c < 4; c++){
      if (digitalRead(colPins[c]) == LOW){
        delay(30);
        if (digitalRead(colPins[c]) == LOW){
          while (digitalRead(colPins[c]) == LOW);
          digitalWrite(rowPins[r], HIGH);
          delay(30);
          return keys[r][c];
        }
      }
    }
    digitalWrite(rowPins[r], HIGH);
  }
  return '\0';
}

void playBeep(int duration){
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

void playGameOver(){
  playBeep(100);
  delay(100);
  playBeep(100);
  delay(100);
  playBeep(250);
}

// ===== Mode 1: Countdown Timer =====
void runTimer(){
  lcd.clear();
  delay(100);
  lcd.print("Timer Mode");
  lcd.setCursor(0, 1);
  lcd.print("Enter Seconds:");
  delay(1500);

  lcd.clear();
  delay(50);
  lcd.print("Input Time:");
  lcd.setCursor(0, 1);
  lcd.print("  [ - - ]");

  char s1 = getKey();
  lcd.setCursor(6, 1);
  lcd.print(s1);

  char s2 = getKey();
  lcd.setCursor(8, 1);
  lcd.print(s2);

  delay(400);

  int totalSeconds = ((s1 - '0') * 10) + (s2 - '0');

  if (totalSeconds <= 0 || totalSeconds > 99){
    lcd.clear();
    lcd.print("Invalid!");
    lcd.setCursor(0, 1);
    lcd.print("(1-99 only)");
    playBeep(200);
    delay(1500);
    currentState = MENU;
    return;
  }

  delay(500);
  lcd.clear();
  lcd.print("Timer Running");
  int displayValue = totalSeconds;
  unsigned long lastUpdate = millis();

  while (displayValue >= 0){
    for (int i = 0; i < 40; i++){
      char key = getKeyNonBlocking();
      if (key == 'A' || key == 'D'){
        lcd.clear();
        lcd.print("Cancelled");
        playBeep(100);
        delay(1000);
        currentState = MENU;
        return;
      }

      if (millis() - lastUpdate >= 1000){
        lcd.setCursor(0, 1);
        lcd.print("Time: ");
        if (displayValue < 10)
          lcd.print("0");
        lcd.print(displayValue);
        lcd.print("s  ");
        displayValue--;
        lastUpdate = millis();
        break;
      }
      delayMicroseconds(100);
    }
  }

  lcd.clear();
  lcd.print("** TIME UP **");
  lcd.setCursor(0, 1);
  lcd.print("Well Done!");
  playGameOver();
  delay(1000);
  currentState = MENU;
}

// ===== Game 1: Dino Game =====
void runDinoGame(){
  lcd.clear();
  lcd.print("Dino Game!");
  lcd.setCursor(0, 1);
  lcd.print("Any key to jump");
  delay(2000);

  lcd.clear();
  lcd.setCursor(12, 0);
  lcd.print("S:0");

  int playerPos = 0, jumpTimer = 0, gameScore = 0;
  int currentSpeed = 150;
  int obsPos[2] = {15, 23};
  unsigned long lastGameUpdate = millis();
  boolean gameRunning = true;

  while (gameRunning){
    char key = getKeyNonBlocking();
    if (key != '\0'){
      if (key == 'A' || key == 'D')
        break;
      else if (playerPos == 0 && jumpTimer == 0){
        playerPos = 1;
        jumpTimer = 3;
        playBeep(20);
      }
    }

    if (millis() - lastGameUpdate >= currentSpeed){
      for (int i = 0; i < 2; i++){
        if (obsPos[i] >= 0 && obsPos[i] < 16){
          lcd.setCursor(obsPos[i], 1);
          lcd.print(" ");
        }
        obsPos[i]--;

        if (obsPos[i] < 0){
          int otherObs = (i == 0) ? 1 : 0;
          obsPos[i] = max(15, obsPos[otherObs]) + random(5, 9);
          gameScore++;
          lcd.setCursor(12, 0);
          lcd.print("S:");
          lcd.print(gameScore);

          if (currentSpeed > 100 && gameScore % 3 == 0)
            currentSpeed -= 15;
          if (gameScore % 2 == 0)
            playBeep(50);
        }

        if (obsPos[i] >= 0 && obsPos[i] < 16){
          lcd.setCursor(obsPos[i], 1);
          lcd.write(byte(1));
        }
      }

      if (jumpTimer > 0){
        jumpTimer--;
        if (jumpTimer == 0){
          lcd.setCursor(1, 0);
          lcd.print(" ");
          playerPos = 0;
        }
      }

      if ((obsPos[0] == 1 || obsPos[1] == 1) && playerPos == 0){
        lcd.setCursor(1, 1);
        lcd.write(byte(1));
        gameRunning = false;
        break;
      }

      if (playerPos == 1){
        lcd.setCursor(1, 0);
        lcd.write(byte(0));
        lcd.setCursor(1, 1);
        lcd.print(" ");
      }
      else{
        lcd.setCursor(1, 1);
        lcd.write(byte(0));
      }
      lastGameUpdate = millis();
    }
  }

  if (gameScore > highScoreDino) highScoreDino = gameScore;

  delay(500);
  lcd.clear();
  lcd.print("Score:");
  lcd.print(gameScore);
  lcd.print(" Hi:");
  lcd.print(highScoreDino);
  lcd.setCursor(0, 1);
  lcd.print("1:Retry  2:Menu");
  playGameOver();

  while (getKeyNonBlocking() != '\0');
  while (true){
    char choice = getKeyNonBlocking();
    if (choice == '1'){
      currentState = DINO_GAME;
      break;
    }
    else if (choice == '2'){
      currentState = GAMES_MENU;
      break;
    }
  }
}

//===== Game 2: Space Invaders (Unpredictable Alien) ====
void runSpaceGame(){
  lcd.clear();
  lcd.print("Space Invaders!");
  lcd.setCursor(0, 1);
  lcd.print("4:< 6:> 5:Shoot");
  delay(2000);
  lcd.clear();

  int shipX = 7, score = 0;
  int alienX = 15;
  int alienDir = -1;
  int alienSpeed = 400;
  unsigned long lastAlienMove = millis();

  lcd.setCursor(shipX, 1);
  lcd.write(byte(3)); // رسم السفينة
  lcd.setCursor(alienX, 0);
  lcd.write(byte(2)); // رسم الفضائي

  while (true){
    char k = getKeyNonBlocking();
    int oldX = shipX;

    if (k == '4' && shipX > 0)
      shipX--; // move left
    if (k == '6' && shipX < 15)
      shipX++; // move right
    if (k == 'A' || k == 'D')
      break;

    // update ship position if changed
    if (shipX != oldX){
      lcd.setCursor(oldX, 1);
      lcd.print(" ");
      lcd.setCursor(shipX, 1);
      lcd.write(byte(3));
    }

    // shooting logic
    if (k == '5'){
      playBeep(30);
      lcd.setCursor(shipX, 0);
      lcd.write(byte(4)); // DRAW LASER
      delay(80);          //delay for laser to be visible

      if (shipX == alienX){ // if hit
        lcd.setCursor(alienX, 0);
        lcd.write(byte(5)); // draw explosion
        playBeep(80);
        delay(150);
        lcd.setCursor(alienX, 0);
        lcd.print(" ");

        score++;

        // --- logic for new alien ---
        if (random(0, 2) == 0){
          alienX = 15;
          alienDir = -1; // show from right and move left
        }
        else{
          alienX = 0;
          alienDir = 1; // show from left and move right
        }

        if (alienSpeed > 100)  alienSpeed -= 20; // increase speed after each hit
      }
      else{// missed shot
        lcd.setCursor(shipX, 0);
        lcd.print(" ");
      }

      if (shipX != alienX && alienX >= 0 && alienX <= 15){
        lcd.setCursor(alienX, 0);
        lcd.write(byte(2));
      }
    }

    // move alien
    if (millis() - lastAlienMove > alienSpeed){
      lcd.setCursor(alienX, 0);
      lcd.print(" "); // remove old alien position

      // shift the alien
      alienX += alienDir;

      // --- new logic for unpredictable movement ---
      // if the score is higher than 3, the alien might suddenly change direction with a certain probability while on screen
      if (score > 3 && random(0, 10) > 7 && alienX > 3 && alienX < 12)
        alienDir = -alienDir; // change direction unpredictably

      // lose condition: if the alien reaches the ship's row
      if (alienX < 0 || alienX > 15){
        lcd.setCursor(shipX, 1);
        lcd.write(byte(5)); // explosion on ship
        break;
      }

      lcd.setCursor(alienX, 0);
      lcd.write(byte(2)); // draw alien in new position
      lastAlienMove = millis();
    }
  }

  if (score > highScoreSpace)   highScoreSpace = score;

  delay(500);
  lcd.clear();
  lcd.print("Score:");
  lcd.print(score);
  lcd.print(" Hi:");
  lcd.print(highScoreSpace);
  lcd.setCursor(0, 1);
  lcd.print("1:Retry  2:Menu");
  playGameOver();

  while (getKeyNonBlocking() != '\0');
  while (true){
    char choice = getKeyNonBlocking();
    if (choice == '1'){
      currentState = SPACE_GAME;
      break; }
    else if (choice == '2'){
      currentState = GAMES_MENU;
      break;}
  }
}
// ===== RESET FUNCTION =====
void resetAll(){
  currentState = MENU;
  lcd.clear();
  lcd.home();
  digitalWrite(BUZZER_PIN, LOW);
  for (int i = 0; i < 4; i++) digitalWrite(rowPins[i], HIGH);
  delay(100);}