#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <stdint.h>

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

// ── LCD Pins (4-bit mode) ────────────────────────────────────
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

#ifdef __cplusplus
extern "C" {
#endif

void lcd_init(void);
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_clear(void);
void lcd_home(void);
void lcd_setCursor(uint8_t col, uint8_t row);
void lcd_print(const char *str);
void lcd_printInt(int num);
void lcd_write(uint8_t ch);
void lcd_createChar(uint8_t loc, uint8_t *charmap);

#ifdef __cplusplus
}
#endif

#endif