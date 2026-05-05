#include <Arduino.h>
#include <LiquidCrystal.h>
#include "MACROS.h"
#include <stdio.h>
// freq 1600000
#define F_CPU 16000000UL
#include <util/delay.h>

// --- Correct Mapping for Eta32mini + MightyCore Standard Pinout ---
// LCD Pins: RS=25(PA1), EN=26(PA2), D4=27(PA3), D5=28(PA4), D6=29(PA5), D7=30(PA6)
LiquidCrystal lcd(25, 26, 27, 28, 29, 30);

// Keypad Pins (Eta32mini):
// Rows (PB4-PB7) -> Arduino Pins {12, 13, 14, 15}
// Cols (PD2-PD5) -> Arduino Pins {2, 3, 4, 5}
const int rowPins[4] = {12, 13, 14, 15};
const int colPins[4] = {2, 3, 4, 5};

char keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

// Output Pins (Eta32mini):
#define BUZZER_PIN 19   // Buzzer on PC5 (Arduino Pin 19) - with DIP switch

// ===== CUSTOM CHARACTERS =====
// Player character (bird-like)
byte player[8] = {B00100, B01010, B11111, B10101, B11111, B01010, B00100, B00000};

// Obstacle character (tree/block)
byte obstacle[8] = {B00111, B01111, B11111, B11111, B11111, B01111, B00111, B00000};

// ===== STATE VARIABLES =====
enum GameState
{
  MENU,
  TIMER_SETUP,
  TIMER_RUN,
  DINO_GAME
};
GameState currentState = MENU;
volatile boolean keyPressFlag = false;
volatile char pressedKey = '\0';

// ===== GAME VARIABLES =====
int playerPos = 0; // 0 = ground, 1 = jumping
int gameScore = 0;
int obsPos = 15;
unsigned long lastGameUpdate = 0;
const int GAME_SPEED = 250; // milliseconds
const int JUMP_HEIGHT = 1;

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

  // Initializing Outputs
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize Keypad Pins
  for (int i = 0; i < 4; i++)
  {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
    pinMode(colPins[i], INPUT_PULLUP);
  }

  // Reset all variables
  resetAll();

  // Show welcome message
  lcd.clear();
  delay(100); // Additional delay for LCD stability
  lcd.print("ProjectMp v2.0");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000); // Longer delay for LCD initialization
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
  lcd.print("==== MAIN MENU ====");
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
    playerPos = 0; // Reset player position
    gameScore = 0; // Reset score
    obsPos = 15;   // Reset obstacle position
    lastGameUpdate = millis();
  }
  delay(300);
}

// --- Keypad Scanning Logic (Blocking) ---
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
          delay(30); // Debounce
          while (digitalRead(colPins[c]) == LOW)
            ;
          digitalWrite(rowPins[r], HIGH);
          delay(30); // Debounce on release
          return keys[r][c];
        }
      }
      digitalWrite(rowPins[r], HIGH);
    }
  }
}

// --- Keypad Scanning Logic (Non-Blocking) ---
char getKeyNonBlocking()
{
  for (int r = 0; r < 4; r++)
  {
    digitalWrite(rowPins[r], LOW);
    for (int c = 0; c < 4; c++)
    {
      if (digitalRead(colPins[c]) == LOW)
      {
        delay(30); // Debounce
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
  playBeep(150);
}

// ===== Mode 1: Countdown Timer =====
void runTimer()
{
  // Input time
  lcd.clear();
  lcd.print("Enter Seconds:");
  lcd.setCursor(0, 1);
  lcd.print("(0-99) Max 2 Dig");
  delay(1500);

  lcd.clear();
  lcd.print("Press Keys: ");
  lcd.setCursor(0, 1);

  char s1 = getKey();
  lcd.print(s1);
  char s2 = getKey();
  lcd.print(s2);

  int totalSeconds = ((s1 - '0') * 10) + (s2 - '0');

  if (totalSeconds <= 0 || totalSeconds > 99)
  {
    lcd.clear();
    lcd.print("Invalid Input!");
    playBeep(200);
    delay(1500);
    currentState = MENU;
    return;
  }

  delay(500);

  // Timer running
  lcd.clear();
  lcd.print("Timer Running...");

  unsigned long lastUpdate = millis();
  int displayValue = totalSeconds;

  while (displayValue >= 0)
  {
    // Keep checking for key press
    for (int i = 0; i < 40; i++)
    { // Check input ~40 times per second
      // Check for key press to cancel
      char key = getKeyNonBlocking();
      if (key == 'A' || key == 'D')
      {
        lcd.clear();
        delay(50); // Ensure LCD clears properly
        lcd.print("Timer Cancelled!");
        playBeep(100);
        delay(1000);
        currentState = MENU;
        return;
      }

      // Check if 1 second has passed
      if (millis() - lastUpdate >= 1000)
      {
        // Clear and rewrite the time display
        lcd.setCursor(0, 1);
        lcd.print("Time: ");
        if (displayValue < 10)
          lcd.print("0");
        lcd.print(displayValue);
        lcd.print("s  "); // Extra spaces to clear old text
        delay(100);       // Delay to let LCD update properly

        displayValue--;
        lastUpdate = millis();
        break;
      }
      delayMicroseconds(100);
    }
  }

  // Time's up!
  lcd.clear();
  lcd.print("TIME'S UP!");

  playGameOver(); // 3 beeps
  delay(500);
  playBeep(200);
  delay(500);

  currentState = MENU;
}

// ===== Mode 2: Dino Game =====
void runDinoGame()
{
  lcd.clear();
  lcd.print("Dinosaur Game!");
  lcd.setCursor(0, 1);
  lcd.print("Jump with Btn");
  delay(1500);

  // Game loop
  playerPos = 0; // Player starts on ground
  gameScore = 0;
  obsPos = 15;
  lastGameUpdate = millis();

  boolean gameRunning = true;

  while (gameRunning)
  {
    // Check for non-blocking key press for jumping
    char key = getKeyNonBlocking();

    // Jump with any key, or 'A'/'D' to quit
    if (key != '\0')
    {
      if (key == 'A' || key == 'D')
      {
        // Quit game
        gameRunning = false;
        break;
      }
      // Jump!
      if (playerPos == 0)
      {
        playerPos = 1;
        playBeep(50); // Jump sound
      }
    }

    // Update game state every GAME_SPEED ms
    if (millis() - lastGameUpdate >= GAME_SPEED)
    {
      // Move obstacle left
      obsPos--;

      // Obstacle wraps around
      if (obsPos < -1)
      {
        obsPos = 15;
        gameScore++;
      }

      // Player gravity (always falling)
      if (playerPos > 0)
      {
        playerPos = 0; // Fall back to ground
      }

      // Collision detection
      if (obsPos == 0 && playerPos == 0)
      {
        // Hit! Game Over
        gameRunning = false;
        break;
      }

      lastGameUpdate = millis();
    }

    // Draw game
    lcd.setCursor(0, 0);
    lcd.print("Score:");
    if (gameScore < 10)
      lcd.print("0");
    lcd.print(gameScore);
    lcd.print("      ");

    // Draw player (position 1)
    lcd.setCursor(1, 1);
    if (playerPos == 0)
    {
      lcd.write(byte(0)); // Player on ground
    }
    else
    {
      lcd.print(" "); // Player jumping
    }

    // Draw player in air if jumping
    if (playerPos > 0)
    {
      lcd.setCursor(1, 0);
      lcd.write(byte(0)); // Player in air
    }

    // Draw obstacle
    if (obsPos >= 0 && obsPos < 16)
    {
      lcd.setCursor(obsPos, 1);
      lcd.write(byte(1)); // Obstacle
    }

    // Clear the display for next iteration
    delay(10);
  }

  // Game Over Screen
  lcd.clear();
  lcd.print("GAME OVER!");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  if (gameScore < 10)
    lcd.print("0");
  lcd.print(gameScore);

  playGameOver();
  delay(1500);

  currentState = MENU;
}

// ===== RESET FUNCTION =====
// This function is called when the device is reset (power on or RESET button)
// It ensures all variables and hardware are properly initialized
void resetAll()
{
  // Reset all game variables
  currentState = MENU;
  playerPos = 0;
  gameScore = 0;
  obsPos = 15;
  lastGameUpdate = millis();
  keyPressFlag = false;
  pressedKey = '\0';

  // Clear LCD display completely
  lcd.clear();
  lcd.home(); // Move cursor to (0,0)

  // Ensure buzzer is OFF
  digitalWrite(BUZZER_PIN, LOW);

  // Reset keypad pins to initial state
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(rowPins[i], HIGH);
  }

  delay(100); // Small delay for all hardware to stabilize
}