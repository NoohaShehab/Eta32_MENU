# 🤝 Contributing Guide - كيفية المساهمة في المشروع

## شكراً لرغبتك في المساهمة! 💡

هذا الملف يوضح كيفية إضافة ميزات جديدة أو إصلاح أخطاء في المشروع.

---

## 📋 قبل البدء

تأكد من:

- [ ] قرأت `README.md`
- [ ] قرأت `TEAM_SETUP.md` و اتبعت الخطوات
- [ ] لديك فهم أساسي للكود في `src/main.cpp`
- [ ] لديك git مثبت

---

## 🔄 خطوات المساهمة الأساسية

### 1️⃣ أنشئ Branch جديد

**قاعدة الأسماء:**

```
feature/feature-name        (ميزة جديدة)
bugfix/bug-name             (إصلاح خطأ)
docs/documentation-name     (توثيق)
refactor/refactor-name      (إعادة هيكلة)
```

**الأمر:**

```bash
# تحديث main أولاً
git pull origin main

# أنشئ branch جديد
git checkout -b feature/your-feature-name
```

---

### 2️⃣ اعمل التعديلات

عدّل الملفات المطلوبة وتأكد من:

- ✅ الكود يعمل بدون أخطاء
- ✅ لا توجد Compilation Warnings
- ✅ تبعت معايير الترميز (أسفل الملف)

---

### 3️⃣ اختبر التعديلات

```bash
# بناء البرنامج
pio run

# إذا كانت هناك أخطاء:
pio run --target clean
pio run

# رفع على الكيت
pio run --target upload

# اختبر على الكيت وتأكد من أن كل شيء يعمل
```

---

### 4️⃣ احفظ التعديلات (Commit)

```bash
# إضافة الملفات المعدلة
git add .

# أو إضافة ملفات محددة
git add src/main.cpp

# حفظ مع رسالة واضحة
git commit -m "Add new feature: description"
```

**معايير رسالة الـ Commit:**

```
Format: "Type: Description"

Examples:
✅ "Feature: Add difficulty levels to game"
✅ "Bugfix: Fix LCD display flickering"
✅ "Docs: Update README with new features"
✅ "Refactor: Optimize 7-segment display function"
```

---

### 5️⃣ رفع التعديلات (Push)

```bash
# رفع البرانش
git push origin feature/your-feature-name
```

---

### 6️⃣ اطلب دمج التعديلات (Pull Request)

على GitHub:

1. اذهب إلى الريبو
2. سيظهر زر "Compare & Pull Request"
3. أكمل المعلومات:
   - **Title**: وصف واضح
   - **Description**: شرح التغييرات
   - **Links**: أي issues متعلقة

---

## 📝 معايير الترميز (Coding Standards)

### 1. أسماء المتغيرات والدوال

**✅ صحيح:**

```cpp
int playerPosition = 0;
void displayGameScore();
bool isCollisionDetected();
const int GAME_SPEED = 250;
```

**❌ خطأ:**

```cpp
int pp = 0;              // اسم غير واضح
void dsply();            // مختصر جداً
bool collision;          // غير واضح ما تفعل
const int speed = 250;   // constant بدون caps
```

---

### 2. التعليقات والتوثيق

**✅ صحيح:**

```cpp
// Initialize player position to ground
int playerPos = 0;

// Update game state every 250ms
if (millis() - lastGameUpdate >= GAME_SPEED) {
    updateGameLogic();
}

/*
 * Collision detection:
 * Returns true if player hits obstacle
 */
bool detectCollision() {
    return (obsPos == 0 && playerPos == 0);
}
```

**❌ خطأ:**

```cpp
int p = 0;  // لا تعليق

// Update
if (m - l >= s) {  // تعليق غير واضح
    u();
}
```

---

### 3. Formatting والمسافات

**✅ صحيح:**

```cpp
void runTimer() {
    lcd.clear();
    lcd.print("Timer Running...");

    int timeLeft = totalSeconds;
    while (timeLeft >= 0) {
        displayTime(timeLeft);
        timeLeft--;
        delay(1000);
    }
}
```

**❌ خطأ:**

```cpp
void runTimer(){
lcd.clear();lcd.print("Timer Running...");
int timeLeft=totalSeconds;while(timeLeft>=0){displayTime(timeLeft);
timeLeft--;delay(1000);}}
```

---

### 4. المسافات البادئة (Indentation)

استخدم **4 spaces** لكل level:

```cpp
if (condition) {
    doSomething();
    if (anotherCondition) {
        doSomethingElse();
    }
}
```

---

### 5. طول السطر

الحد الأقصى **100 character** لكل سطر:

```cpp
// ✅ صحيح
void displayOn7Seg(int val) {
    if (val > 99) val = 99;
    if (val < 0) val = 0;
}

// ❌ خطأ (سطر طويل جداً)
void displayOn7Seg(int val) { if (val > 99) val = 99; if (val < 0) val = 0; }
```

---

## 🎯 أنواع المساهمات

### 1. إضافة ميزة جديدة

```bash
git checkout -b feature/new-feature
# اعمل التعديلات
git push origin feature/new-feature
```

**يجب أن تشمل:**

- [ ] كود نظيف وموثق
- [ ] اختبارات شاملة
- [ ] تحديث التوثيق إن أمكن
- [ ] رسالة commit واضحة

---

### 2. إصلاح خطأ (Bug Fix)

```bash
git checkout -b bugfix/bug-name
# اصلح الخطأ
git push origin bugfix/bug-name
```

**يجب أن تشمل:**

- [ ] وصف الخطأ بوضوح
- [ ] خطوات تكرار الخطأ
- [ ] الحل المقترح
- [ ] اختبار الحل

---

### 3. تحسينات الأداء

```bash
git checkout -b refactor/optimization-name
# حسّن الأداء
git push origin refactor/optimization-name
```

**يجب أن تشمل:**

- [ ] قياس الأداء قبل وبعد
- [ ] وثيقة توضح التحسينات
- [ ] اختبار شامل

---

## 🔍 قائمة الفحص (Checklist) قبل الـ PR

- [ ] البرنامج يُترجم بدون أخطاء
- [ ] لا توجد تحذيرات (Warnings)
- [ ] تبعت معايير الترميز
- [ ] اختبرت على الكيت بالكامل
- [ ] كتبت تعليقات واضحة
- [ ] حديثت التوثيق إن لزم
- [ ] رسالة الـ commit واضحة
- [ ] الـ branch مُحدّث مع main

---

## 🚫 ما لا تفعله

❌ **لا تعدّل على main مباشرة**

```bash
# خطأ!
git checkout main
git commit -m "Fixed bug"
```

❌ **لا ترفع ملفات غير ضرورية**

```bash
# حذفها من git:
git rm --cached file.o
git rm --cached .vscode/settings.json
```

❌ **لا تجمع عدة تغييرات مختلفة في commit واحد**

```bash
# خطأ: يجب أن تكون commits منفصلة
# ✅ صحيح:
git commit -m "Feature: Add difficulty levels"
# (commit منفصل)
git commit -m "Bugfix: Fix display flickering"
```

❌ **لا تكتب رسائل commit غير واضحة**

```bash
# خطأ
git commit -m "Fixed stuff"

# صحيح
git commit -m "Bugfix: Fix 7-segment display updating"
```

---

## 🤖 Git Workflow الكامل

```bash
# 1. تحديث المشروع
git pull origin main

# 2. أنشئ branch
git checkout -b feature/awesome-feature

# 3. اعمل التعديلات
# (عدّل الملفات)

# 4. اختبر
pio run
pio run --target upload

# 5. احفظ التغييرات
git add .
git commit -m "Feature: Add awesome feature"

# 6. رفع للريبو
git push origin feature/awesome-feature

# 7. على GitHub: أنشئ Pull Request
# (من فضلك اتبع قالب الـ PR)

# بعد الموافقة والدمج:
# 8. عد للـ main
git checkout main
git pull origin main

# 9. احذف البرانش المحلي
git branch -d feature/awesome-feature

# 10. احذف البرانش من الريبو
git push origin --delete feature/awesome-feature
```

---

## 📞 الدعم والأسئلة

إذا كان لديك أسئلة:

1. **ابحث أولاً** في Issues السابقة
2. **اسأل** في Discussions إن وجدت
3. **تواصل** مع قائد الفريق مباشرة

---

## 🎉 شكراً!

مساهمتك مهمة جداً! كل مساهمة تحسّن المشروع 💪

---

**آخر تحديث**: 5 مايو 2026  
**الحالة**: ✅ جاهز للفريق
