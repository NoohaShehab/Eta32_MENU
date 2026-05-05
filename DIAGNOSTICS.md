# 🔧 ملف التشخيص والصيانة - ProjectMp

## 📋 فحص ما قبل التشغيل (Pre-Flight Checklist)

### المكونات المادية:

- [ ] LCD 16x2 متصل بشكل صحيح
- [ ] Keypad 4x4 متصل بشكل صحيح
- [ ] Buzzer متصل بالدبس 21
- [ ] 7-Segment العرض متصل
- [ ] جميع الأسلاك GND موصولة
- [ ] جميع التوصيلات آمنة وليست فضفاضة

### البرنامج:

- [ ] لا توجد أخطاء تجميع (Compilation Errors)
- [ ] لا توجد تحذيرات (Warnings)
- [ ] الملفات كاملة (main.cpp و MACROS.h)

---

## 🐛 استكشاف الأخطاء الشائعة

### المشكلة 1: LCD لا يعمل

#### الأعراض:

- الشاشة سوداء تماماً
- لا يوجد نص على LCD

#### التشخيص:

```
1. تحقق من توصيل 5V و GND
2. قم بتعديل التباين (Contrast) على اللوحة
3. تحقق من الدبابيس:
   - RS (Pin 25)
   - EN (Pin 26)
   - D4-D7 (Pins 27-30)
```

#### الحل:

```cpp
// اختبر LCD:
void test_LCD() {
    lcd.clear();
    lcd.print("LCD Test OK!");
    delay(2000);
}
// أضفه في setup() أو loop()
```

---

### المشكلة 2: لا توجد استجابة من الأزرار

#### الأعراض:

- الأزرار لا تستجيب
- البرنامج معلق في getKey()

#### التشخيص:

```
1. تحقق من توصيل Keypad:
   - الصفوف: 20, 21, 22, 23
   - الأعمدة: 10, 11, 12, 13
2. تأكد من عدم قصر الأسلاك
3. اختبر Keypad بشكل منفصل
```

#### الحل:

```cpp
// اختبر المفاتيح:
void test_Keypad() {
    lcd.clear();
    lcd.print("Press any key");
    char key = getKey();
    lcd.clear();
    lcd.print("You pressed: ");
    lcd.print(key);
    delay(2000);
}
```

---

### المشكلة 3: Buzzer لا يصدر صوتاً

#### الأعراض:

- لا توجد أصوات على الإطلاق
- Buzzer صامت

#### التشخيص:

```
1. تحقق من توصيل Buzzer:
   - موجب → Pin 21 (PC5)
   - سالب → GND
2. تحقق من قطبية Buzzer
3. اختبر Buzzer بشكل منفصل مع 5V
```

#### الحل:

```cpp
// اختبر Buzzer:
void test_Buzzer() {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);
}
// استدعه في loop() عدة مرات
```

---

### المشكلة 4: 7-Segment لا يعمل

#### الأعراض:

- العرض الرقمي فارغ أو معطل
- أرقام غريبة أو مخطوطة

#### التشخيص:

```
1. تحقق من توصيل Port A
2. تحقق من تفعيل الأرقام (Pins 22, 23)
3. اختبر 7-Segment بشكل منفصل
```

#### الحل:

```cpp
// اختبر 7-Segment:
void test_7Segment() {
    for (int i = 0; i < 100; i++) {
        displayOn7Seg(i);
        delay(50);
    }
}
```

---

### المشكلة 5: تداخل الشاشات

#### الأعراض:

- نصوص من لعبة على شاشة المؤقت
- رموز غريبة في وسط الشاشة

#### التشخيص:

```
السبب الرئيسي: عدم وجود lcd.clear() كافي
أو عدم تحديث البيانات بشكل صحيح
```

#### الحل:

```cpp
// تأكد من وجود clear() في كل دالة:
void runTimer() {
    lcd.clear();  // ✓ مهم جداً
    lcd.print("Timer Running...");
    // ...
}

void runDinoGame() {
    lcd.clear();  // ✓ مهم جداً
    // ...
}
```

---

### المشكلة 6: البرنامج معلق

#### الأعراض:

- البرنامج يتوقف فجأة
- LCD متجمد
- لا يمكن الخروج

#### التشخيص:

```
السبب: getKey() blocking بدون خروج
```

#### الحل:

```cpp
// استخدم getKeyNonBlocking() في اللعبة:
char key = getKeyNonBlocking();  // لا يعلق البرنامج
if (key == 'A' || key == 'D') {
    // خروج
}
```

---

## 🧪 اختبارات الوحدة (Unit Tests)

### اختبار الدوال الفردية:

#### 1. اختبار LCD:

```cpp
void test_all() {
    // اختبر LCD
    lcd.clear();
    lcd.print("Test 1: LCD");
    delay(1000);

    // اختبر الأحرف المخصصة
    lcd.write(byte(0));  // الكائن
    lcd.write(byte(1));  // العقبة
    delay(1000);

    // اختبر 7-Segment
    displayOn7Seg(42);
    delay(1000);

    // اختبر Buzzer
    playBeep(100);
    delay(500);
}
```

#### 2. اختبار الكيبورد:

```cpp
void test_keypad() {
    while (true) {
        lcd.clear();
        lcd.print("Press key...");
        char key = getKey();
        lcd.clear();
        lcd.print("Key: ");
        lcd.print(key);
        delay(2000);
    }
}
```

#### 3. اختبار المؤقت:

```cpp
void test_timer() {
    runTimer();  // استدعه مباشرة
}
```

#### 4. اختبار اللعبة:

```cpp
void test_game() {
    runDinoGame();  // استدعه مباشرة
}
```

---

## 📊 جدول استكشاف الأخطاء

| المشكلة     | السبب المحتمل     | الحل                            |
| ----------- | ----------------- | ------------------------------- |
| LCD فارغ    | توصيل أو تباين    | التحقق من الأسلاك أو التباين    |
| أزرار معلقة | Pin خاطئ          | التحقق من الدبابيس 20-23, 10-13 |
| لا صوت      | Pin خاطئ          | التحقق من Pin 21                |
| 7-Seg معطل  | Port A            | التحقق من توصيل Port A          |
| تداخل شاشات | LCD لم ينظف       | إضافة lcd.clear()               |
| برنامج معلق | getKey() blocking | استخدام Non-Blocking            |

---

## 🔍 قراءة السجلات

### إذا كنت تستخدم Serial Monitor:

```cpp
// أضف في setup():
Serial.begin(9600);

// أضف في الدوال:
void runTimer() {
    Serial.println("Timer started");
    // ...
    Serial.print("Time: ");
    Serial.println(totalSeconds);
}
```

### الرسائل المهمة:

- `Timer started` - المؤقت يعمل
- `Game started` - اللعبة تعمل
- `Collision detected` - اصطدام
- `Invalid input` - إدخال خاطئ

---

## 🔧 تصحيح الأخطاء الشائعة

### خطأ: أرقام غريبة على LCD

**الحل:**

```cpp
// تأكد من أن الدالة تطبع الأرقام بشكل صحيح:
void print_score(int score) {
    lcd.setCursor(6, 0);
    if (score < 10) lcd.print("0");  // أضف صفر
    lcd.print(score);
}
```

### خطأ: المفاتيح تتكرر

**الحل:**

```cpp
// أضف debounce كافي:
delay(30);  // بعد الكشف
while (digitalRead(colPins[c]) == LOW);  // انتظر الإفراج
delay(30);  // بعد الإفراج
```

### خطأ: اللعبة بطيئة جداً

**الحل:**

```cpp
// قلل GAME_SPEED:
const int GAME_SPEED = 150;  // بدلاً من 250
```

### خطأ: اللعبة سريعة جداً

**الحل:**

```cpp
// زد GAME_SPEED:
const int GAME_SPEED = 350;  // بدلاً من 250
```

---

## 📈 قائمة الصيانة الدورية

### يومياً:

- [ ] اختبر جميع الأزرار
- [ ] تأكد من عمل LCD
- [ ] اختبر الأصوات

### أسبوعياً:

- [ ] افحص جميع التوصيلات
- [ ] اختبر اللعبة والمؤقت
- [ ] تحقق من الأسلاك المفكوكة

### شهرياً:

- [ ] نظف المقاطع المعدنية
- [ ] استبدل البطاريات إن لزم
- [ ] احفظ نسخة من الكود

---

## 💾 النسخ الاحتياطية والاستعادة

### حفظ الكود:

```
حفظ في مجلد الاختبار:
ProjectMp_Backup_2026-05-05.cpp
```

### استعادة الكود:

```
1. افتح النسخة الاحتياطية
2. انسخ محتويات main.cpp
3. الصق في الكود الحالي
4. أعد التجميع والتحميل
```

---

## 📞 الدعم التقني

### إذا استمرت المشاكل:

1. **اطبع معلومات التشخيص**:

```cpp
void print_diagnostics() {
    Serial.println("=== DIAGNOSTICS ===");
    Serial.print("Game State: ");
    Serial.println(currentState);
    Serial.print("Player Pos: ");
    Serial.println(playerPos);
    Serial.print("Obstacle Pos: ");
    Serial.println(obsPos);
}
```

2. **تحقق من الذاكرة**:

```cpp
void check_memory() {
    int freeRam = freeRam();
    Serial.print("Free RAM: ");
    Serial.println(freeRam);
}
```

3. **أعد تحميل البرنامج**:

```
1. استخدم PlatformIO
2. اختر "Build & Upload"
3. تحقق من الرسائل
```

---

**آخر تحديث**: 5 مايو 2026  
**النسخة**: 2.0  
**الحالة**: ✅ محدثة وشاملة
