# 🏗️ Architecture & Code Structure Guide

## فهم البنية الداخلية للمشروع

هذا الملف يشرح كيفية تنظيم الكود والعلاقات بين الأجزاء المختلفة.

---

## 📊 الهيكل العام

```
main()
   ├─ setup() - تهيئة أول مرة
   │  ├─ LCD initialization
   │  ├─ Keypad setup
   │  ├─ Buzzer setup
   │  └─ Custom characters
   │
   └─ loop() - الحلقة الرئيسية
      ├─ showMenu()
      ├─ getKey()
      ├─ runTimer()
      └─ runDinoGame()
```

---

## 🎯 State Machine

المشروع يعتمد على **State Machine** بثلاث حالات:

```
         ┌─────────────────┐
         │      MENU       │
         └────────┬────────┘
                  │
          ┌───────┼───────┐
          │       │       │
          ▼       ▼       ▼
      TIMER  DINO_GAME  (others)
      (1)       (2)
      │       │
      └───────┴──────────┐
              │          │
              ▼          ▼
            Done  Return to MENU
```

### رمز الـ State Machine:

```cpp
enum GameState { MENU, TIMER_SETUP, TIMER_RUN, DINO_GAME };
GameState currentState = MENU;

// في loop():
switch(currentState) {
    case MENU:
        showMenu();
        break;
    case TIMER_SETUP:
        runTimer();
        break;
    case DINO_GAME:
        runDinoGame();
        break;
}
```

---

## 📁 الملفات الرئيسية

### 1. `src/main.cpp` (~500 سطر)

```cpp
// ===== INCLUDES =====
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "MACROS.h"

// ===== HARDWARE SETUP =====
LiquidCrystal lcd(25, 26, 27, 28, 29, 30);
// Pin definitions...

// ===== STATE VARIABLES =====
enum GameState { MENU, TIMER_SETUP, TIMER_RUN, DINO_GAME };
GameState currentState = MENU;

// ===== GAME VARIABLES =====
int playerPos = 0;
int gameScore = 0;
int obsPos = 15;

// ===== FUNCTIONS =====
void setup() { ... }
void loop() { ... }
void showMenu() { ... }
void runTimer() { ... }
void runDinoGame() { ... }
void getKey() { ... }
void playBeep() { ... }
void displayOn7Seg() { ... }
```

---

### 2. `src/MACROS.h` (Utility Macros)

```cpp
// Bit manipulation macros
#define set_bit(reg, bit)    (reg |= (1 << bit))
#define clear_bit(reg, bit)  (reg &= ~(1 << bit))
#define toggle_bit(reg, bit) (reg ^= (1 << bit))
#define read_bit(reg, bit)   ((reg >> bit) & 1)
```

---

## 🔌 Pin Mapping

### LCD Pins (4-bit Mode):

```
LCD RS   → Pin 25 (PA1)
LCD EN   → Pin 26 (PA2)
LCD D4   → Pin 27 (PA3)
LCD D5   → Pin 28 (PA4)
LCD D6   → Pin 29 (PA5)
LCD D7   → Pin 30 (PA6)
```

### Keypad Matrix:

```
Rows (Output):
Row 0 → Pin 20 (PC4)
Row 1 → Pin 21 (PC5)
Row 2 → Pin 22 (PC6)
Row 3 → Pin 23 (PC7)

Columns (Input):
Col 0 → Pin 10 (PD2)
Col 1 → Pin 11 (PD3)
Col 2 → Pin 12 (PD4)
Col 3 → Pin 13 (PD5)
```

### Other Outputs:

```
Buzzer       → Pin 21 (PC5)
7-Seg Dig1   → Pin 22 (PC6)
7-Seg Dig2   → Pin 23 (PC7)
7-Seg Data   → Port A (PA1-PA7)
```

---

## 🎮 Game Logic Details

### Timer Mode

```cpp
void runTimer() {
    // Step 1: Get input (2 digits)
    char s1 = getKey();
    char s2 = getKey();
    int totalSeconds = ((s1 - '0') * 10) + (s2 - '0');

    // Step 2: Validate
    if (totalSeconds <= 0 || totalSeconds > 99) {
        // Error handling
        return;
    }

    // Step 3: Count down
    while (displayValue >= 0) {
        // Multiplex 7-Segment 40x/sec
        for (int i = 0; i < 40; i++) {
            displayOn7Seg(displayValue);
            // Check for exit keys (A/D)
        }
        // Decrement every 1000ms
        if (millis() - lastUpdate >= 1000) {
            displayValue--;
        }
    }

    // Step 4: Time's up sound
    playGameOver();
}
```

### Game Mode

```cpp
void runDinoGame() {
    playerPos = 0;      // Start on ground
    gameScore = 0;
    obsPos = 15;        // Obstacle on right

    while (gameRunning) {
        // Check input (non-blocking)
        char key = getKeyNonBlocking();
        if (key != '\0' && playerPos == 0) {
            playerPos = 1;  // Jump
        }

        // Update every 250ms
        if (millis() - lastGameUpdate >= GAME_SPEED) {
            obsPos--;  // Move obstacle left

            if (obsPos < 0) {
                obsPos = 15;  // Wrap around
                gameScore++;  // Increment score
            }

            playerPos = 0;  // Fall back down

            // Collision detection
            if (obsPos == 0 && playerPos == 0) {
                gameRunning = false;
            }
        }

        // Draw screen
        drawGame();
    }

    // Game over
    playGameOver();
}
```

---

## 🔑 Key Functions

### Input Functions

#### `getKey()` - Blocking (قراءة مع انتظار)

```cpp
char getKey() {
    while (true) {
        for (int r = 0; r < 4; r++) {
            digitalWrite(rowPins[r], LOW);
            for (int c = 0; c < 4; c++) {
                if (digitalRead(colPins[c]) == LOW) {
                    delay(30);  // Debounce
                    while (digitalRead(colPins[c]) == LOW);
                    return keys[r][c];
                }
            }
            digitalWrite(rowPins[r], HIGH);
        }
    }
}
```

#### `getKeyNonBlocking()` - Non-blocking (قراءة بدون انتظار)

```cpp
char getKeyNonBlocking() {
    for (int r = 0; r < 4; r++) {
        digitalWrite(rowPins[r], LOW);
        for (int c = 0; c < 4; c++) {
            if (digitalRead(colPins[c]) == LOW) {
                // ... debounce logic ...
                return keys[r][c];
            }
        }
        digitalWrite(rowPins[r], HIGH);
    }
    return '\0';  // No key pressed
}
```

---

### Display Functions

#### `displayOn7Seg(int val)` - 7-Segment Multiplexing

```cpp
void displayOn7Seg(int val) {
    if (val > 99) val = 99;
    if (val < 0) val = 0;

    int tens = val / 10;
    int units = val % 10;

    // Show tens
    PORTA = seg_map[tens] << 1;
    digitalWrite(DIG1_ENABLE, HIGH);
    digitalWrite(DIG2_ENABLE, LOW);
    delayMicroseconds(500);

    // Show units
    PORTA = seg_map[units] << 1;
    digitalWrite(DIG1_ENABLE, LOW);
    digitalWrite(DIG2_ENABLE, HIGH);
    delayMicroseconds(500);
}
```

---

### Sound Functions

```cpp
void playBeep(int duration) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
}

void playGameOver() {
    playBeep(100);  // Beep 1
    delay(100);
    playBeep(100);  // Beep 2
    delay(100);
    playBeep(150);  // Beep 3 (longer)
}
```

---

## ⏱️ Timing Diagram

```
TIMER MODE:
├─ Input Phase: Blocking (انتظار 2 رقم)
├─ Display Phase: LCD update كل 1000ms
├─ Multiplex Phase: 7-Segment 40x/sec
└─ Buzzer: عند الانتهاء

GAME MODE:
├─ Input Phase: Non-blocking (فحص سريع)
├─ Logic Phase: كل 250ms
│  ├─ Move obstacle
│  ├─ Check collision
│  ├─ Update score
│  └─ Update physics
└─ Draw Phase: كل 10ms
   ├─ Draw player
   └─ Draw obstacle
```

---

## 🎨 Custom Characters

```cpp
// Player Character
byte player[8] = {
    B00100,  // row 0
    B01010,  // row 1
    B11111,  // row 2
    B10101,  // row 3
    B11111,  // row 4
    B01010,  // row 5
    B00100,  // row 6
    B00000   // row 7
};

// Obstacle Character (Tree)
byte obstacle[8] = {
    B00111,  // row 0
    B01111,  // row 1
    B11111,  // row 2
    B11111,  // row 3
    B11111,  // row 4
    B01111,  // row 5
    B00111,  // row 6
    B00000   // row 7
};

// تحميل الأحرف في setup():
lcd.createChar(0, player);
lcd.createChar(1, obstacle);

// استخدام الأحرف في loop():
lcd.write(byte(0));  // يطبع الكائن
lcd.write(byte(1));  // يطبع العقبة
```

---

## 🔄 Data Flow

```
┌─────────────────────────────────────┐
│        User Input (Keypad)          │
└──────────────┬──────────────────────┘
               │
               ▼
        getKey() / getKeyNonBlocking()
               │
               ▼
        Update Game State
        (currentState, playerPos, etc.)
               │
               ▼
        Game Logic Update
        (Check collision, increment score)
               │
               ▼
        Display Output
        (LCD, 7-Segment, Buzzer)
               │
               ▼
┌─────────────────────────────────────┐
│      Physical Output (LCD, etc.)    │
└─────────────────────────────────────┘
```

---

## 🐛 Debug Tips

### طريقة 1: Serial Monitor

```cpp
// في setup():
Serial.begin(9600);

// في loop():
Serial.print("playerPos: ");
Serial.println(playerPos);
Serial.print("obsPos: ");
Serial.println(obsPos);
Serial.print("score: ");
Serial.println(gameScore);
```

### طريقة 2: LED Indicators

```cpp
// إضافة LED للتشخيص
#define DEBUG_LED 13

void debug_blink() {
    digitalWrite(DEBUG_LED, HIGH);
    delay(100);
    digitalWrite(DEBUG_LED, LOW);
}
```

---

## 📈 Performance Optimization Tips

1. **تقليل الـ LCD writes**: استخدم `setCursor` فقط عند الضرورة
2. **تحسين الـ 7-Segment**: زيادة تكرار الـ multiplexing
3. **تحسين الـ Game Loop**: استخدم ساعة دقيقة (`millis()`)
4. **تقليل الـ debounce**: من 30ms إلى 20ms إذا كان مستقراً

---

## 🔐 Memory Usage

```
Approx. Memory Usage:
├─ Global variables: ~400 bytes
├─ Stack (local variables): ~100 bytes
├─ Code (program): ~3-4 KB
└─ Total: ~5-6 KB (من 30 KB المتاح)
```

---

## 🚀 Future Extension Points

إذا أردت إضافة ميزات جديدة، هذه نقاط التوسع:

1. **إضافة صعوبة جديدة**: زيادة `GAME_SPEED`
2. **إضافة ميزات**: تعديل `runDinoGame()`
3. **إضافة أصوات**: تعديل `playBeep()`
4. **إضافة نقاط محفوظة**: استخدام `EEPROM`

---

**هذا الملف سيساعدك على فهم وتطوير المشروع بسهولة! 🚀**
