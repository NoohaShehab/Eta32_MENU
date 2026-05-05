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
byte player[8] = {B00111, B00101, B00111, B10110, B11111, B01110, B01010, B00000};
byte obstacle[8] = {B00100, B00101, B10101, B10111, B11100, B00100, B00100, B00000};

// ===== STATE VARIABLES =====
enum GameState
{
  MENU,
  TIMER_SETUP,
  TIMER_RUN,
  DINO_GAME
};
GameState currentState = MENU;

// ===== GAME VARIABLES =====
int playerPos = 0;
int jumpTimer = 0;
int gameScore = 0;
int highScore = 0;
int obsPos[2] = {15, 23};
unsigned long lastGameUpdate = 0;
int currentSpeed = 250;

// --- Function Prototypes ---
char getKey();
char getKeyNonBlocking();
void runTimer();
void runDinoGame();
void showMenu();
void playBeep(int duration);
void playGameOver();
void resetAll();

void setup()
{
  lcd.begin(16, 2);
  lcd.createChar(0, player);
  lcd.createChar(1, obstacle);

  pinMode(BUZZER_PIN, OUTPUT);

  for (int i = 0; i < 4; i++)
  {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
    pinMode(colPins[i], INPUT_PULLUP);
  }

  resetAll();

  lcd.clear();
  delay(100);
  lcd.print("ProjectMp v2.0");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
}

void loop()
{
  switch (currentState)
  {
  case MENU:
    showMenu();
    break;
  case TIMER_SETUP:
    runTimer();
    break;
  case DINO_GAME:
    runDinoGame();
    break;
  default:
    currentState = MENU;
  }
}

// ===== MENU SCREEN =====
void showMenu()
{
  lcd.clear();
  lcd.print("** MAIN MENU **");
  lcd.setCursor(0, 1);
  lcd.print("1:Timer  2:Game");
  delay(200);

  char choice = getKey();

  if (choice == '1')
  {
    currentState = TIMER_SETUP;
  }
  else if (choice == '2')
  {
    currentState = DINO_GAME;
  }
  delay(300);
}

// --- Keypad Scanning Logic ---
char getKey()
{
  while (true)
  {
    for (int r = 0; r < 4; r++)
    {
      digitalWrite(rowPins[r], LOW);
      for (int c = 0; c < 4; c++)
      {
        if (digitalRead(colPins[c]) == LOW)
        {
          delay(30);
          while (digitalRead(colPins[c]) == LOW)
            ;
          digitalWrite(rowPins[r], HIGH);
          delay(30);
          return keys[r][c];
        }
      }
      digitalWrite(rowPins[r], HIGH);
    }
  }
}

char getKeyNonBlocking()
{
  for (int r = 0; r < 4; r++)
  {
    digitalWrite(rowPins[r], LOW);
    for (int c = 0; c < 4; c++)
    {
      if (digitalRead(colPins[c]) == LOW)
      {
        delay(30);
        if (digitalRead(colPins[c]) == LOW)
        {
          while (digitalRead(colPins[c]) == LOW)
            ;
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

// --- Buzzer Functions ---
void playBeep(int duration)
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

void playGameOver()
{
  playBeep(100);
  delay(100);
  playBeep(100);
  delay(100);
  playBeep(250);
}

// ===== Mode 1: Countdown Timer (UNCHANGED) =====
void runTimer()
{
  lcd.clear();
  delay(100);
  lcd.print("Timer Mode");
  lcd.setCursor(0, 1);
  lcd.print("Enter Seconds:");
  delay(1500);

  lcd.clear();
  delay(50);
  lcd.print("Input: ");
  lcd.setCursor(0, 1);

  char s1 = getKey();
  lcd.print(s1);
  char s2 = getKey();
  lcd.print(s2);

  int totalSeconds = ((s1 - '0') * 10) + (s2 - '0');

  if (totalSeconds <= 0 || totalSeconds > 99)
  {
    lcd.clear();
    delay(50);
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
  delay(50);
  lcd.print("Timer Running");
  lcd.setCursor(0, 1);
  int displayValue = totalSeconds;
  unsigned long lastUpdate = millis();

  while (displayValue >= 0)
  {
    for (int i = 0; i < 40; i++)
    {
      char key = getKeyNonBlocking();
      if (key == 'A' || key == 'D')
      {
        lcd.clear();
        delay(50);
        lcd.print("Cancelled");
        lcd.setCursor(0, 1);
        lcd.print("Going to Menu");
        playBeep(100);
        delay(1000);
        currentState = MENU;
        return;
      }

      if (millis() - lastUpdate >= 1000)
      {
        lcd.setCursor(0, 1);
        lcd.print("Time: ");
        if (displayValue < 10)
          lcd.print("0");
        lcd.print(displayValue);
        lcd.print("s  ");
        delay(100);

        displayValue--;
        lastUpdate = millis();
        break;
      }
      delayMicroseconds(100);
    }
  }

  lcd.clear();
  delay(50);
  lcd.print("** TIME UP **");
  lcd.setCursor(0, 1);
  lcd.print("Well Done!");

  playGameOver();
  delay(500);
  playBeep(200);
  delay(500);

  currentState = MENU;
}

// ===== Mode 2: Dino Game (UPDATED) =====
void runDinoGame()
{
  lcd.clear();
  lcd.print("Dino Game!");
  lcd.setCursor(0, 1);
  lcd.print("Any key to jump");
  delay(2000);

  lcd.clear();
  lcd.setCursor(12, 0);
  lcd.print("S:0");

  playerPos = 0;
  jumpTimer = 0;
  gameScore = 0;
  currentSpeed = 150;
  obsPos[0] = 15;
  obsPos[1] = 23;
  lastGameUpdate = millis();

  boolean gameRunning = true;

  while (gameRunning)
  {
    char key = getKeyNonBlocking();

    if (key != '\0')
    {
      if (key == 'A' || key == 'D')
      {
        gameRunning = false;
        break;
      }
      else if (playerPos == 0 && jumpTimer == 0)
      {
        playerPos = 1;
        jumpTimer = 3;
        playBeep(20);
      }
    }

    if (millis() - lastGameUpdate >= currentSpeed)
    {

      for (int i = 0; i < 2; i++)
      {
        // 1. Clear old obstacle
        if (obsPos[i] >= 0 && obsPos[i] < 16)
        {
          lcd.setCursor(obsPos[i], 1);
          lcd.print(" ");
        }

        // 2. Move obstacle
        obsPos[i]--;

        // 3. Redraw obstacle and update score
        if (obsPos[i] < 0)
        {
          // Calculate random distance for new obstacle based on other obstacle position
          int otherObs = (i == 0) ? 1 : 0;
          int gap = random(5, 9); // Random gap between 5 and 8 steps
          obsPos[i] = max(15, obsPos[otherObs]) + gap;

          gameScore++;
          lcd.setCursor(12, 0);
          lcd.print("S:");
          lcd.print(gameScore);

          // Improvement: Gradually increase speed
          if (currentSpeed > 150 && gameScore % 3 == 0)
          {
            currentSpeed -= 15;
          }

          // Improvement: Celebratory beep every 5 points
          if (gameScore % 2 == 0)
          {
            playBeep(50);
          }
        }

        // 4. Draw obstacle at its new position
        if (obsPos[i] >= 0 && obsPos[i] < 16)
        {
          lcd.setCursor(obsPos[i], 1);
          lcd.write(byte(1));
        }
      }

      // 5. Jump physics
      if (jumpTimer > 0)
      {
        jumpTimer--;
        if (jumpTimer == 0)
        {
          lcd.setCursor(1, 0);
          lcd.print(" ");
          playerPos = 0;
        }
      }

      // 6. Collision detection (if hit any of the obstacles)
      if ((obsPos[0] == 1 || obsPos[1] == 1) && playerPos == 0)
      {
        lcd.setCursor(1, 1);
        lcd.write(byte(1));
        gameRunning = false;
        break;
      }

      // 7. Draw dinosaur
      if (playerPos == 1)
      {
        lcd.setCursor(1, 0);
        lcd.write(byte(0));
        lcd.setCursor(1, 1);
        lcd.print(" ");
      }
      else
      {
        lcd.setCursor(1, 1);
        lcd.write(byte(0));
      }

      lastGameUpdate = millis();
    }
  }

  // --- Game Over Screen ---
  if (gameScore > highScore)
  {
    highScore = gameScore; // Save high score
  }

  delay(500);
  lcd.clear();
  lcd.print("GAME OVER! S:");
  lcd.print(gameScore);
  lcd.setCursor(0, 1);
  lcd.print("High Score: ");
  lcd.print(highScore);

  playGameOver();

  delay(1000); // delay before showing retry options
  lcd.clear();
  lcd.print("Score:");
  lcd.print(gameScore);
  lcd.print(" High:");
  lcd.print(highScore);

  lcd.setCursor(0, 1);
  lcd.print("1:Retry  2:Menu ");

  // wait for any key release to avoid immediate re-triggering
  while (getKeyNonBlocking() != '\0')
    ;

  // retry or menu
  while (true)
  {
    char choice = getKeyNonBlocking();
    if (choice == '1')
    {
      currentState = DINO_GAME; //  start new game
      break;
    }
    else if (choice == '2')
    {
      currentState = MENU; // Return to main menu
      break;
    }
  }
}

// ===== RESET FUNCTION =====
void resetAll()
{
  currentState = MENU;
  lcd.clear();
  lcd.home();
  digitalWrite(BUZZER_PIN, LOW);
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(rowPins[i], HIGH);
  }
  delay(100);
}