// ProjectMp v3.2 – AVR/ATmega | Ghost-fix + condensed
#ifdef F_CPU
#undef F_CPU
#endif
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
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
    lcd_setCursor((c), (r)); \
    lcd_print(" ");          \
  } while (0)
#define LCD_DRAW(c, r, ch)    \
  do                          \
  {                           \
    lcd_setCursor((c), (r));  \
    lcd_write((uint8_t)(ch)); \
  } while (0)
#define LCD_SCREEN(a, b, ms) \
  do                         \
  {                          \
    lcd_clear();             \
    lcd_print(a);            \
    lcd_setCursor(0, 1);     \
    lcd_print(b);            \
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
    lcd_clear();                    \
    lcd_print("Score:");            \
    lcd_printInt(sc);               \
    lcd_print(" Hi:");              \
    lcd_printInt(hi);               \
    lcd_setCursor(0, 1);            \
    lcd_print("1:Retry  2:Menu");   \
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
// LCD pins (4-bit mode): RS=PA3, E=PA4, D4-D7=PA5-PA7, D7_ALT=PC7
#define LCD_RS_PORT PORTA
#define LCD_RS_DDR  DDRA
#define LCD_RS_PIN  PA1

#define LCD_E_PORT  PORTA
#define LCD_E_DDR   DDRA
#define LCD_E_PIN   PA2

#define LCD_D4_PORT PORTA
#define LCD_D4_DDR  DDRA
#define LCD_D4_PIN  PA3

#define LCD_D5_PORT PORTA
#define LCD_D5_DDR  DDRA
#define LCD_D5_PIN  PA4

#define LCD_D6_PORT PORTA
#define LCD_D6_DDR  DDRA
#define LCD_D6_PIN  PA5

#define LCD_D7_PORT PORTA
#define LCD_D7_DDR  DDRA
#define LCD_D7_PIN  PA6
const uint8_t rowBits[4] = {4, 5, 6, 7};
const uint8_t colBits[4] = {2, 3, 4, 5};
#define BUZZER_PORT PORTC
#define BUZZER_DDR DDRC
#define BUZZER_PIN PC5

const char keys[4][4] = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};

// ── Custom chars (slot order matches createChar in main) ─────
uint8_t player[8] = {0x07, 0x05, 0x07, 0x16, 0x1F, 0x0E, 0x0A, 0x00};
uint8_t obstacle[8] = {0x04, 0x05, 0x15, 0x17, 0x1C, 0x04, 0x04, 0x00};
uint8_t alien[8] = {0x11, 0x0A, 0x1F, 0x15, 0x1F, 0x0A, 0x11, 0x00};
uint8_t ship[8] = {0x04, 0x0E, 0x1F, 0x15, 0x04, 0x0A, 0x1B, 0x00};
uint8_t laser[8] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00};
uint8_t explode[8] = {0x11, 0x0A, 0x04, 0x0A, 0x11, 0x00, 0x00, 0x00};
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
void lcd_init();
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_clear();
void lcd_home();
void lcd_setCursor(uint8_t col, uint8_t row);
void lcd_print(const char *str);
void lcd_printInt(int num);
void lcd_write(uint8_t ch);
void lcd_createChar(uint8_t loc, uint8_t *charmap);
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

// ── LCD Control (4-bit mode, simplified) ─────────────────────────
static uint8_t lcd_row = 0, lcd_col = 0;

void lcd_write_nibble(uint8_t nibble)
{
  (nibble & 1) ? SET_BIT(LCD_D4_PORT, LCD_D4_PIN) : CLR_BIT(LCD_D4_PORT, LCD_D4_PIN);
  (nibble & 2) ? SET_BIT(LCD_D5_PORT, LCD_D5_PIN) : CLR_BIT(LCD_D5_PORT, LCD_D5_PIN);
  (nibble & 4) ? SET_BIT(LCD_D6_PORT, LCD_D6_PIN) : CLR_BIT(LCD_D6_PORT, LCD_D6_PIN);
  (nibble & 8) ? SET_BIT(LCD_D7_PORT, LCD_D7_PIN) : CLR_BIT(LCD_D7_PORT, LCD_D7_PIN);
  SET_BIT(LCD_E_PORT, LCD_E_PIN);
  _delay_us(1);
  CLR_BIT(LCD_E_PORT, LCD_E_PIN);
  _delay_us(50);
}

void lcd_command(uint8_t cmd)
{
  CLR_BIT(LCD_RS_PORT, LCD_RS_PIN);
  _delay_us(10);
  lcd_write_nibble((cmd >> 4) & 0x0F);
  lcd_write_nibble(cmd & 0x0F);
  _delay_ms(2);
}

void lcd_data(uint8_t data)
{
  SET_BIT(LCD_RS_PORT, LCD_RS_PIN);
  _delay_us(10);
  lcd_write_nibble((data >> 4) & 0x0F);
  lcd_write_nibble(data & 0x0F);
  _delay_us(50);
}

void lcd_init()
{
  SET_BIT(LCD_RS_DDR, LCD_RS_PIN);
  SET_BIT(LCD_E_DDR, LCD_E_PIN);
  SET_BIT(LCD_D4_DDR, LCD_D4_PIN);
  SET_BIT(LCD_D5_DDR, LCD_D5_PIN);
  SET_BIT(LCD_D6_DDR, LCD_D6_PIN);
  SET_BIT(LCD_D7_DDR, LCD_D7_PIN);

  CLR_BIT(LCD_RS_PORT, LCD_RS_PIN);
  CLR_BIT(LCD_E_PORT, LCD_E_PIN);
  
  _delay_ms(20);
  lcd_write_nibble(0x03);
  _delay_ms(5);
  lcd_write_nibble(0x03);
  _delay_ms(1);
  lcd_write_nibble(0x03);
  _delay_ms(1);
  lcd_write_nibble(0x02);
  _delay_ms(1);
  
  lcd_command(0x28); // 4-bit mode, 2 lines, 5x7 dots
  lcd_command(0x0C); // Display ON, Cursor OFF
  lcd_command(0x06); // Entry mode
  lcd_command(0x01); // Clear display
  _delay_ms(2);
  lcd_row = 0;
  lcd_col = 0;
}

void lcd_clear()
{
  lcd_command(0x01);
  _delay_ms(2);
  lcd_row = 0;
  lcd_col = 0;
}

void lcd_home()
{
  lcd_command(0x02);
  _delay_ms(2);
  lcd_row = 0;
  lcd_col = 0;
}

void lcd_setCursor(uint8_t col, uint8_t row)
{
  lcd_command(0x80 | ((row ? 0x40 : 0) + col));
  _delay_us(50);
  lcd_row = row;
  lcd_col = col;
}

void lcd_print(const char *str)
{
  while (*str)
  {
    if (lcd_col >= 16)
    {
      lcd_col = 0;
      lcd_row = !lcd_row;
      lcd_setCursor(0, lcd_row);
    }
    lcd_data(*str++);
    lcd_col++;
  }
}

void lcd_printInt(int num)
{
  char buf[6];
  snprintf(buf, sizeof(buf), "%d", num);
  lcd_print(buf);
}

void lcd_write(uint8_t ch)
{
  if (lcd_col >= 16)
  {
    lcd_col = 0;
    lcd_row = !lcd_row;
    lcd_setCursor(0, lcd_row);
  }
  lcd_data(ch);
  lcd_col++;
}

void lcd_createChar(uint8_t loc, uint8_t *charmap)
{
  lcd_command(0x40 | ((loc & 7) << 3));
  for (int i = 0; i < 8; i++)
    lcd_data(charmap[i]);
  lcd_setCursor(0, 0);
}

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
  lcd_init();
  lcd_createChar(CHAR_PLAYER, player);
  lcd_createChar(CHAR_OBSTACLE, obstacle);
  lcd_createChar(CHAR_ALIEN, alien);
  lcd_createChar(CHAR_SHIP, ship);
  lcd_createChar(CHAR_LASER, laser);
  lcd_createChar(CHAR_EXPLODE, explode);
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
  lcd_clear();
  lcd_home();
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
  lcd_clear();
  delay(50);
  lcd_print("Input Time:");
  lcd_setCursor(0, 1);
  lcd_print("  [ - - ]");

  char s1 = getKey();
  lcd_setCursor(4, 1);
  char s1_str[2] = {s1, '\0'};
  lcd_print(s1_str);
  char s2 = getKey();
  lcd_setCursor(6, 1);
  char s2_str[2] = {s2, '\0'};
  lcd_print(s2_str);
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
        lcd_setCursor(0, 1);
        lcd_print("Time: ");
        if (val < 10)
          lcd_print("0");
        lcd_printInt(val);
        lcd_print("s  ");
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
  lcd_clear();
  lcd_setCursor(12, 0);
  lcd_print("S:0");

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
        lcd_setCursor(12, 0);
        lcd_print("S:");
        lcd_printInt(score);
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
  lcd_clear();

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