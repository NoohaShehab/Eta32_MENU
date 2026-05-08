// ProjectMp v3.2 – AVR/ATmega | Ghost-fix + condensed
#ifdef F_CPU
#undef F_CPU
#endif
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdio.h>
#include "MACROS.h"

// ── Shims ────────────────────────────────────────────────────
#define delay(ms) delay_ms(ms)
#define delayMicroseconds(us) _delay_us(us)
#define random(lo, hi) random_range((lo), (hi))

// ── LCD ──────────────────────────────────────────────────────
#define LCD_CLEAR(c, r)      \
  do                         \
  {                          \
    lcd.setCursor((c), (r)); \
    lcd.print(' ');          \
  } while (0)
#define LCD_DRAW(c, r, ch)   \
  do                         \
  {                          \
    lcd.setCursor((c), (r)); \
    lcd.write(byte(ch));     \
  } while (0)
#define LCD_SCREEN(a, b, ms) \
  do                         \
  {                          \
    lcd.clear();             \
    lcd.print(a);            \
    lcd.setCursor(0, 1);     \
    lcd.print(b);            \
    delay(ms);               \
  } while (0)

// ── Timing / Input ───────────────────────────────────────────
#define ELAPSED(t, ms) ((unsigned long)(get_millis() - (t)) >= (unsigned long)(ms))
#define IS_EXIT(k) ((k) == 'A' || (k) == 'D')
#define FLUSH_KEY()                     \
  do                                    \
  {                                     \
    while (getKeyNonBlocking() != '\0') \
    {                                   \
    }                                   \
  } while (0)

// ── Post-game: show score + wait for retry/menu choice ───────
#define GAME_OVER(sc, hi, st1, st2) \
  do                                \
  {                                 \
    if ((sc) > (hi))                \
      (hi) = (sc);                  \
    delay(500);                     \
    lcd.clear();                    \
    lcd.print("Score:");            \
    lcd.print(sc);                  \
    lcd.print(" Hi:");              \
    lcd.print(hi);                  \
    lcd.setCursor(0, 1);            \
    lcd.print("1:Retry  2:Menu");   \
    playGameOver();                 \
    FLUSH_KEY();                    \
    for (char _c = '\0';;)          \
    {                               \
      _c = getKeyNonBlocking();     \
      if (_c == '1')                \
      {                             \
        currentState = (st1);       \
        break;                      \
      }                             \
      if (_c == '2')                \
      {                             \
        currentState = (st2);       \
        break;                      \
      }                             \
    }                               \
  } while (0)

// ── Hardware ─────────────────────────────────────────────────
LiquidCrystal lcd(25, 26, 27, 28, 29, 30);
const uint8_t rowBits[4] = {4, 5, 6, 7};
const uint8_t colBits[4] = {2, 3, 4, 5};
#define BUZZER_PORT PORTC
#define BUZZER_DDR DDRC
#define BUZZER_PIN PC5

const char keys[4][4] = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};

// ── Custom chars (slot order matches createChar in main) ─────
uint8_t player[8] = {B00111, B00101, B00111, B10110, B11111, B01110, B01010, B00000};
uint8_t obstacle[8] = {B00100, B00101, B10101, B10111, B11100, B00100, B00100, B00000};
uint8_t alien[8] = {B10001, B01010, B11111, B10101, B11111, B01010, B10001, B00000};
uint8_t ship[8] = {B00100, B01110, B11111, B10101, B00100, B01010, B11011, B00000};
uint8_t laser[8] = {B00100, B00100, B00100, B00100, B00100, B00000, B00000, B00000};
uint8_t explode[8] = {B10001, B01010, B00100, B01010, B10001, B00000, B00000, B00000};
#define CHAR_PLAYER 0
#define CHAR_OBSTACLE 1
#define CHAR_ALIEN 2
#define CHAR_SHIP 3
#define CHAR_LASER 4
#define CHAR_EXPLODE 5

// ── State / globals ──────────────────────────────────────────
enum GameState
{
  MENU,
  TIMER_SETUP,
  GAMES_MENU,
  DINO_GAME,
  SPACE_GAME
};
GameState currentState = MENU;
int highScoreDino = 0, highScoreSpace = 0;
volatile unsigned long timer_millis = 0;
static uint32_t rng_state = 0x6D2B79F5UL;

// ── Prototypes ───────────────────────────────────────────────
void timer0_init();
void keypad_init();
void buzzer_init();
unsigned long get_millis();
void delay_ms(unsigned long ms);
void seed_rng(unsigned long s);
long random_range(long mn, long mx);
char getKey();
char getKeyNonBlocking();
void playBeep(int d);
void playGameOver();
void showMenu();
void showGamesMenu();
void runTimer();
void runDinoGame();
void runSpaceGame();
void resetAll();

// ── ISR ──────────────────────────────────────────────────────
ISR(TIMER0_COMP_vect) { timer_millis++; }

void timer0_init()
{
  TCCR0 = 0;
  TCNT0 = 0;
  OCR0 = 249;
  SET_BIT(TCCR0, WGM01);
  SET_BIT(TIMSK, OCIE0);
  SET_BIT(TCCR0, CS01);
  SET_BIT(TCCR0, CS00);
}

// ── Keypad ───────────────────────────────────────────────────
void keypad_init()
{
  for (uint8_t i = 0; i < 4; i++)
  {
    SET_BIT(DDRB, rowBits[i]);
    SET_BIT(PORTB, rowBits[i]);
    CLR_BIT(DDRD, colBits[i]);
    SET_BIT(PORTD, colBits[i]);
  }
}

char getKey()
{
  while (1)
    for (uint8_t r = 0; r < 4; r++)
    {
      CLR_BIT(PORTB, rowBits[r]);
      for (uint8_t c = 0; c < 4; c++)
        if (READ_BIT(PIND, colBits[c]) == 0)
        {
          _delay_ms(30);
          while (READ_BIT(PIND, colBits[c]) == 0)
          {
          }
          SET_BIT(PORTB, rowBits[r]);
          _delay_ms(30);
          return keys[r][c];
        }
      SET_BIT(PORTB, rowBits[r]);
    }
}

char getKeyNonBlocking()
{
  for (uint8_t r = 0; r < 4; r++)
  {
    SET_BIT(PORTB, rowBits[r]);
    CLR_BIT(PORTB, rowBits[r]);
    for (uint8_t c = 0; c < 4; c++)
      if (READ_BIT(PIND, colBits[c]) == 0)
      {
        delay_ms(30);
        if (READ_BIT(PIND, colBits[c]) == 0)
        {
          while (READ_BIT(PIND, colBits[c]) == 0)
          {
          }
          SET_BIT(PORTB, rowBits[r]);
          delay_ms(30);
          return keys[r][c];
        }
      }
    SET_BIT(PORTB, rowBits[r]);
  }
  return '\0';
}

// ── Buzzer ───────────────────────────────────────────────────
void buzzer_init()
{
  SET_BIT(BUZZER_DDR, BUZZER_PIN);
  CLR_BIT(BUZZER_PORT, BUZZER_PIN);
}
void playBeep(int d)
{
  SET_BIT(BUZZER_PORT, BUZZER_PIN);
  delay_ms(d);
  CLR_BIT(BUZZER_PORT, BUZZER_PIN);
}
void playGameOver()
{
  playBeep(100);
  delay(100);
  playBeep(100);
  delay(100);
  playBeep(250);
}

// ── Time helpers ─────────────────────────────────────────────
unsigned long get_millis()
{
  unsigned long v;
  uint8_t s = SREG;
  cli();
  v = timer_millis;
  SREG = s;
  return v;
}
void delay_ms(unsigned long ms)
{
  unsigned long t = get_millis();
  while (!ELAPSED(t, ms))
  {
  }
}

// ── RNG (xorshift32) — next_random inlined into random_range ─
void seed_rng(unsigned long s) { rng_state = s ? s : 0x6D2B79F5UL; }
long random_range(long mn, long mx)
{
  if (mx <= mn)
    return mn;
  uint32_t x = rng_state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  rng_state = x;
  return mn + (long)(x % (uint32_t)(mx - mn));
}

// ── Main ─────────────────────────────────────────────────────
int main(void)
{
  cli();
  lcd.begin(16, 2);
  lcd.createChar(CHAR_PLAYER, player);
  lcd.createChar(CHAR_OBSTACLE, obstacle);
  lcd.createChar(CHAR_ALIEN, alien);
  lcd.createChar(CHAR_SHIP, ship);
  lcd.createChar(CHAR_LASER, laser);
  lcd.createChar(CHAR_EXPLODE, explode);
  timer0_init();
  keypad_init();
  buzzer_init();
  sei();
  delay_ms(1);
  seed_rng(get_millis() ^ 0xA5A5A5A5UL);
  resetAll();
  LCD_SCREEN("ProjectMp v3.2", "Starting...", 2000);
  while (1)
    switch (currentState)
    {
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

// ── Reset ────────────────────────────────────────────────────
void resetAll()
{
  currentState = MENU;
  lcd.clear();
  lcd.home();
  CLR_BIT(PORTC, BUZZER_PIN);
  for (uint8_t i = 0; i < 4; i++)
    SET_BIT(PORTB, rowBits[i]);
  delay(100);
}

// ── Menus ────────────────────────────────────────────────────
void showMenu()
{
  LCD_SCREEN("** MAIN MENU **", "1:Timer  2:Games", 200);
  char c = getKey();
  if (c == '1')
    currentState = TIMER_SETUP;
  else if (c == '2')
    currentState = GAMES_MENU;
  delay(300);
}

void showGamesMenu()
{
  LCD_SCREEN("Select Game:", "1:Dino   2:Space", 200);
  char c = getKey();
  if (c == '1')
    currentState = DINO_GAME;
  else if (c == '2')
    currentState = SPACE_GAME;
  else if (IS_EXIT(c))
    currentState = MENU;
  delay(300);
}

// ── Countdown Timer ──────────────────────────────────────────
void runTimer()
{
  LCD_SCREEN("Timer Mode", "Enter Seconds:", 1500);
  lcd.clear();
  delay(50);
  lcd.print("Input Time:");
  lcd.setCursor(0, 1);
  lcd.print("  [ - - ]");

  char s1 = getKey();
  lcd.setCursor(4, 1);
  lcd.print(s1);
  char s2 = getKey();
  lcd.setCursor(6, 1);
  lcd.print(s2);
  delay(400);

  int total = ((s1 - '0') * 10) + (s2 - '0');
  if (total <= 0 || total > 99)
  {
    LCD_SCREEN("Invalid!", "(1-99 only)", 0);
    playBeep(200);
    delay(1500);
    currentState = MENU;
    return;
  }

  delay(500);
  LCD_SCREEN("Timer Running", "", 0);
  int val = total;
  unsigned long t = get_millis();

  while (val >= 0)
    for (int i = 0; i < 40; i++)
    {
      char k = getKeyNonBlocking();
      if (IS_EXIT(k))
      {
        LCD_SCREEN("Cancelled", "", 0);
        playBeep(100);
        delay(1000);
        currentState = MENU;
        return;
      }
      if (ELAPSED(t, 1000UL))
      {
        lcd.setCursor(0, 1);
        lcd.print("Time: ");
        if (val < 10)
          lcd.print("0");
        lcd.print(val);
        lcd.print("s  ");
        val--;
        t = get_millis();
        break;
      }
      delayMicroseconds(100);
    }

  LCD_SCREEN("** TIME UP **", "Well Done!", 0);
  playGameOver();
  delay(1000);
  currentState = MENU;
}

// ── Dino Game ────────────────────────────────────────────────
void runDinoGame()
{
  LCD_SCREEN("Dino Game!", "Any key to jump", 2000);
  lcd.clear();
  lcd.setCursor(12, 0);
  lcd.print("S:0");

  int playerRow = 1, jumpTimer = 0, score = 0, speed = 300;
  const int N = 2;
  int obs[N] = {15, 23}, prev[N] = {-1, -1}, prevRow = -1;
  unsigned long t = get_millis();
  bool running = true;

  while (running)
  {
    char k = getKeyNonBlocking();
    if (k != '\0')
    {
      if (IS_EXIT(k))
        break;
      if (playerRow == 1 && jumpTimer == 0)
      {
        playerRow = 0;
        jumpTimer = 3;
        playBeep(20);
      }
    }
    if (!ELAPSED(t, speed))
      continue;

    // erase prev positions
    for (int i = 0; i < N; i++)
      if (prev[i] >= 0 && prev[i] <= 11)
        LCD_CLEAR(prev[i], 1);
    if (prevRow != -1)
      LCD_CLEAR(1, prevRow);

    // advance obstacles
    for (int i = 0; i < N; i++)
    {
      obs[i]--;
      if (obs[i] < 0)
      {
        int o = (i == 0) ? 1 : 0, base = (obs[o] > 15) ? obs[o] : 15;
        obs[i] = base + random(5, 9);
        score++;
        lcd.setCursor(12, 0);
        lcd.print("S:");
        lcd.print(score);
        if (speed > 100 && score % 3 == 0)
          speed -= 15;
        if (score % 2 == 0)
          playBeep(50);
      }
    }

    // jump timer
    if (jumpTimer > 0)
    {
      jumpTimer--;
      if (jumpTimer == 0)
        playerRow = 1;
    }

    // collision
    for (int i = 0; i < N; i++)
      if (obs[i] == 1 && playerRow == 1)
      {
        LCD_DRAW(1, 1, CHAR_OBSTACLE);
        running = false;
        break;
      }
    if (!running)
      break;

    // draw new positions
    for (int i = 0; i < N; i++)
    {
      prev[i] = obs[i];
      if (obs[i] >= 0 && obs[i] <= 11)
        LCD_DRAW(obs[i], 1, CHAR_OBSTACLE);
    }
    LCD_DRAW(1, playerRow, CHAR_PLAYER);
    prevRow = playerRow;
    t = get_millis();
  }
  GAME_OVER(score, highScoreDino, DINO_GAME, GAMES_MENU);
}

// ── Space Invaders ───────────────────────────────────────────
void runSpaceGame()
{
  LCD_SCREEN("Space Invaders!", "4:< 6:> 5:Shoot", 2000);
  lcd.clear();

  int shipX = 7, alienX = 15, alienY = 0, prevAlienX = -1, prevAlienY = -1, alienDir = -1, alienSpeed = 700, score = 0;
  unsigned long t = get_millis();
  LCD_DRAW(shipX, 1, CHAR_SHIP);
  LCD_DRAW(alienX, alienY, CHAR_ALIEN);
  prevAlienX = alienX;
  prevAlienY = alienY;

  while (true)
  {
    char k = getKeyNonBlocking();
    int oldX = shipX;
    if (k == '4' && shipX > 0)
      shipX--;
    if (k == '6' && shipX < 15)
      shipX++;
    if (IS_EXIT(k))
      break;

    if (shipX != oldX)
    {
      LCD_CLEAR(oldX, 1);
      LCD_DRAW(shipX, 1, CHAR_SHIP);
    }

    if (k == '5')
    {
      playBeep(30);
      LCD_DRAW(shipX, 0, CHAR_LASER);
      delay(150);
      if (shipX == alienX && alienY == 0)
      {
        LCD_DRAW(alienX, alienY, CHAR_EXPLODE);
        playBeep(80);
        delay(150);
        LCD_CLEAR(alienX, alienY);
        prevAlienX = -1;
        prevAlienY = -1;
        score++;
        // Spawn alien at random position (avoid collision)
        do
        {
          alienX = random(0, 16);
          alienY = random(0, 2);
        } while (alienX == shipX && alienY == 1);
        alienDir = (random(0, 2) == 0) ? -1 : 1;
        if (alienSpeed > 150)
          alienSpeed -= 10;
        t = get_millis();
      }
      else if (shipX != alienX)
        LCD_CLEAR(shipX, 0);
    }

    if (ELAPSED(t, alienSpeed))
    {
      if (prevAlienX >= 0 && prevAlienX <= 15 && prevAlienY >= 0 && prevAlienY <= 1)
        LCD_CLEAR(prevAlienX, prevAlienY);
      alienX += alienDir;
      if (score > 3 && random(0, 10) > 7 && alienX > 3 && alienX < 12)
        alienDir = -alienDir;
      if ((alienDir == -1 && alienX < 0) || (alienDir == 1 && alienX > 15))
      {
        LCD_DRAW(shipX, 1, CHAR_EXPLODE);
        break;
      }
      // Check collision with ship
      if (alienX == shipX && alienY == 1)
      {
        LCD_DRAW(shipX, 1, CHAR_EXPLODE);
        break;
      }
      if (alienX >= 0 && alienX <= 15)
      {
        LCD_DRAW(alienX, alienY, CHAR_ALIEN);
        prevAlienX = alienX;
        prevAlienY = alienY;
      }
      t = get_millis();
    }
  }
  GAME_OVER(score, highScoreSpace, SPACE_GAME, GAMES_MENU);
}