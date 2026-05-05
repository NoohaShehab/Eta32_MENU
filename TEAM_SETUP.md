# 🚀 Team Member Setup Guide - Eta32 Menu System

## للأعضاء الجدد - كيفية البدء بالمشروع

إذا كنت عضواً جديداً في الفريق وتريد العمل على المشروع، اتبع هذه الخطوات:

---

## 1️⃣ استنساخ المشروع (Clone)

### الخطوة الأولى: فتح Terminal أو PowerShell

على Windows:

```powershell
# انتقل للمجلد الذي تريد حفظ المشروع فيه
cd Documents

# استنسخ المشروع
git clone https://github.com/NoohaShehab/Eta32_MENU.git

# ادخل المجلد
cd Eta32_MENU
```

### النتيجة:

```
Eta32_MENU/
├── src/main.cpp
├── src/MACROS.h
├── README.md
├── platformio.ini
└── ...
```

---

## 2️⃣ فتح المشروع في VS Code

### الطريقة 1: من Terminal

```powershell
code .
```

### الطريقة 2: يدوياً

1. فتح VS Code
2. اختر `File → Open Folder`
3. اختر مجلد `Eta32_MENU`

---

## 3️⃣ تثبيت الإضافات المطلوبة (Extensions)

VS Code سيقترح عليك تثبيت الإضافات المطلوبة.  
اضغط **"Install All"** أو ثبت يدوياً:

### الإضافات المهمة:

1. **PlatformIO IDE** (ضروري جداً)
   - للتحميل والتجميع
   - ID: `platformio.platformio-ide`

2. **C/C++ IntelliSense** (اختياري)
   - لفهم الكود بشكل أفضل

---

## 4️⃣ فهم هيكل المشروع

```
📁 Eta32_MENU/
│
├─ 📄 README.md ..................... توثيق المشروع (اقرأه أولاً!)
├─ 📄 platformio.ini ............... إعدادات PlatformIO
│
├─ 📁 src/
│  ├─ main.cpp ................... البرنامج الرئيسي
│  └─ MACROS.h ................... الماكروهات والدوال المساعدة
│
├─ 📁 include/ .................... ملفات الاشتمال
├─ 📁 lib/ ........................ المكتبات الخارجية
└─ 📁 test/ ....................... ملفات الاختبار
```

---

## 5️⃣ اقرأ الكود

### ابدأ من هنا:

#### أ. الملف الرئيسي: `src/main.cpp`

- ~500 سطر
- منظم جداً
- مقسم لـ functions واضحة

#### ب. الماكروهات: `src/MACROS.h`

```cpp
# define set_bit(reg, bit) (reg |= (1 << bit))
# define clear_bit(reg, bit) (reg &= ~(1 << bit))
# define toggle_bit(reg, bit) (reg ^= (1 << bit))
#define read_bit(reg, bit) ((reg >> bit) & 1)
```

---

## 6️⃣ اتصل بالكيت (Eta32mini)

### قبل البدء:

- ✅ تأكد من توصيل الكيت بـ USB
- ✅ تثبيت Drivers (إذا لزم)
- ✅ تحديد Board في PlatformIO

### الإعدادات في VS Code:

1. افتح ملف `platformio.ini`:

```ini
[env:default]
platform = atmelavr
board = ATmega328P
framework = arduino
upload_port = COM3  ; غيره حسب جهازك
monitor_speed = 9600
```

2. تحديد COM Port:
   - اضغط على PlatformIO icon (يسار الشاشة)
   - اختر `Devices`
   - شوف ال COM port الخاص بـ Eta32

---

## 7️⃣ تجميع البرنامج (Build)

### من Terminal:

```powershell
# تجميع فقط
pio run

# أو من الواجهة الرسومية:
# اضغط على Build icon (أسفل الشاشة)
```

### يجب أن تقول:

```
BUILD SUCCESSFUL
```

---

## 8️⃣ رفع البرنامج على الكيت (Upload)

### من Terminal:

```powershell
pio run --target upload
```

### أو من الواجهة:

- اضغط على Upload icon (أسفل الشاشة)
- أو اضغط `Ctrl + Shift + P` واكتب `Upload`

### يجب أن تقول:

```
Leaving... Uploading [###] 100%
UPLOAD SUCCESSFUL
```

---

## 9️⃣ اختبر البرنامج على الكيت

### شغّل اللعبة:

1. **اضغط الزر 1**: وضع المؤقت
   - ادخل الثواني (00-99)
   - شوف الوقت ينزل على الـ LCD
   - شوف الأرقام على الـ 7-Segment

2. **اضغط الزر 2**: لعبة الديناصور
   - اضغط أي زر للقفز
   - تجنب العقبات
   - جمّع النقاط

3. **اضغط A أو D**: للخروج من أي وضع

---

## 🔟 المراقبة (Serial Monitor)

للاطلاع على الرسائل والأخطاء:

```powershell
# من Terminal:
pio device monitor

# أو من الواجهة:
# اضغط على Monitor icon (أسفل الشاشة)
```

---

## 📚 الملفات الإضافية (محلية عندك فقط)

إذا كنت تريد ملفات التوثيق الإضافية:

```
الملفات الموجودة محلياً على الآخرين:
├─ SUMMARY.md ..................... ملخص التحسينات
├─ QUICK_START.md ................ دليل الاستخدام
├─ IMPROVEMENTS.md ............... قائمة التحسينات
├─ DIAGNOSTICS.md ............... استكشاف الأخطاء
├─ FUTURE_UPDATES.md ............ خطة التطوير
└─ INDEX.md ..................... فهرس المشروع
```

اطلب منهم نسخ هذه الملفات إلى جهازك يدوياً.

---

## 🎯 خطوات سريعة (Summary)

```bash
# 1. استنسخ المشروع
git clone https://github.com/NoohaShehab/Eta32_MENU.git
cd Eta32_MENU

# 2. افتح في VS Code
code .

# 3. ثبّت PlatformIO
# (سيطلب VS Code تثبيتها)

# 4. تأكد من COM port في platformio.ini

# 5. بناء البرنامج
pio run

# 6. رفع للكيت
pio run --target upload

# 7. اختبر على الكيت!
```

---

## 🐛 المشاكل الشائعة

### ❌ مشكلة: "Build Failed"

**الحل:**

```bash
# نظّف البناء السابق
pio run --target clean

# حاول مجدداً
pio run
```

### ❌ مشكلة: "COM Port Not Found"

**الحل:**

- تأكد من توصيل الكيت بـ USB
- غيّر رقم COM في `platformio.ini`
- إذا فشل: `pio device list` لمعرفة COM الصحيح

### ❌ مشكلة: "UPLOAD FAILED"

**الحل:**

1. اضغط Reset على الكيت
2. حاول الرفع مجدداً
3. تأكد من الـ Board صحيح

### ❌ مشكلة: PlatformIO لا يتثبت

**الحل:**

```bash
# ثبّت يدوياً
pip install platformio
```

---

## 📞 الدعم والمساعدة

### إذا واجهت مشكلة:

1. 📖 اقرأ `README.md` للفهم العام
2. 🔧 شوف `DIAGNOSTICS.md` إذا كان متوفراً
3. 💬 اسأل أعضاء الفريق الآخرين
4. 🔍 ابحث عن الخطأ في Google

---

## ✅ قائمة التحقق (Checklist)

عندما تنهي الإعداد، تأكد من:

- [ ] استنسخت المشروع من GitHub
- [ ] فتحته في VS Code
- [ ] ثبتّ PlatformIO
- [ ] قرأت `README.md`
- [ ] حددت COM Port الصحيح
- [ ] بنيت المشروع بنجاح (`pio run`)
- [ ] رفعت البرنامج على الكيت (`pio run --target upload`)
- [ ] اختبرت البرنامج على الكيت
- [ ] البرنامج يعمل بدون أخطاء
- [ ] استطعت تشغيل المؤقت واللعبة

---

## 🚀 الخطوة التالية

بعد ما تخلصت من الإعداد:

1. **فهم الكود**: اقرأ `src/main.cpp` بتمعن
2. **التطوير**: أضف ميزات جديدة إذا أردت
3. **الاختبار**: اختبر التغييرات على الكيت
4. **Push للريبو**: رفع التعديلات على GitHub

---

## 📝 نصائح مهمة

### ✨ نصيحة 1: الـ Git

```bash
# قبل أي تعديل، اسحب آخر النسخة
git pull origin main

# بعد التعديل، رفع لـ GitHub
git add .
git commit -m "وصف التعديل"
git push origin main
```

### ✨ نصيحة 2: الـ Branches

لا تعدّل على `main` مباشرة:

```bash
# أنشئ branch جديد
git checkout -b feature/your-feature

# اعمل التعديلات
# ...

# رفع البرانش
git push origin feature/your-feature
```

### ✨ نصيحة 3: الـ Serial Monitor

استخدمه لتتبع الأخطاء:

```bash
pio device monitor --baud 9600
```

---

## 🎉 بالتوفيق!

الآن أنت جاهز للعمل على المشروع! 🚀

إذا واجهت أي مشكلة، لا تتردد في السؤال!

---

**آخر تحديث**: 5 مايو 2026  
**الإصدار**: 2.0  
**الحالة**: ✅ جاهز للفريق
