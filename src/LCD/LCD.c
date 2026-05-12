#include "LCD.h"
#include "../MACROS.h"
#include <stdio.h>
#include <util/delay.h>

// ── LCD Control (4-bit mode, simplified) ─────────────────────────
static uint8_t lcd_row = 0, lcd_col = 0;

static void lcd_write_nibble(uint8_t nibble)
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

void lcd_init(void)
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

void lcd_clear(void)
{
  lcd_command(0x01);
  _delay_ms(2);
  lcd_row = 0;
  lcd_col = 0;
}

void lcd_home(void)
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