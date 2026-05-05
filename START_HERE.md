# 🎯 Team Member Quick Start - للعضو الجديد

## بدون أي تعقيدات - الخطوات الأساسية فقط! 🚀

---

## ⏱️ 5 دقائق - الإعداد الأساسي

### 1️⃣ استنسخ المشروع (Clone)

```bash
git clone https://github.com/NoohaShehab/Eta32_MENU.git
cd Eta32_MENU
```

### 2️⃣ افتح في VS Code

```bash
code .
```

### 3️⃣ ثبّت PlatformIO

- VS Code سيطلب منك تثبيتها (اضغط Install)
- أو: من Extensions اكتب `PlatformIO` وثبّته

---

## ⏱️ 5 دقائق - أول تشغيل

### 4️⃣ صحح الـ COM Port (إذا لزم)

1. افتح `platformio.ini`
2. غيّر `COM3` لـ `COM` الصحيح (حسب جهازك)
3. أو شوف: PlatformIO → Devices

### 5️⃣ بناء + رفع

```bash
pio run && pio run --target upload
```

### ✅ كل شيء تمام؟ البرنامج يعمل على الكيت!

---

## 🎮 الاستخدام الأساسي

```
اضغط 1: المؤقت
  - أدخل الثواني
  - شوف الوقت ينزل

اضغط 2: اللعبة
  - اضغط أي زر للقفز
  - تجنب العقبات

اضغط A أو D: خروج من أي مكان
```

---

## 📚 الملفات المهمة (شاركها أعضاء الفريق معك)

### لأول مرة:

- [ ] `TEAM_SETUP.md` - تفاصيل الإعداد
- [ ] `QUICK_START.md` - شرح الاستخدام

### قبل التطوير:

- [ ] `CONTRIBUTING.md` - معايير المساهمة
- [ ] `QUICK_REFERENCE.md` - أوامر سريعة

### للفهم العميق:

- [ ] `ARCHITECTURE.md` - شرح الكود
- [ ] `LOCAL_DOCS.md` - معلومات الملفات

---

## ⚡ الأوامر اليومية

```bash
# بعد تحديثات من الفريق
git pull origin main

# بناء البرنامج
pio run

# رفع على الكيت
pio run --target upload

# مراقبة الأخطاء (Serial Monitor)
pio device monitor --baud 9600

# بعد تعديلاتك
git add .
git commit -m "وصف التعديل"
git push origin main
```

---

## 🚀 الخطوة التالية

```
اختر واحد:

أ) أريد أفهم الكود أكتر
   → اقرأ ARCHITECTURE.md

ب) أريد أضيف ميزة جديدة
   → اقرأ CONTRIBUTING.md

ج) واجهت مشكلة
   → اقرأ DIAGNOSTICS.md

د) أنا مستعد!
   → ابدأ الكود في main.cpp
```

---

## 🆘 مشاكل شائعة

```
المشكلة: "Build Failed"
الحل: pio run --target clean && pio run

المشكلة: "COM Port Not Found"
الحل: شوف الـ COM الصحيح في PlatformIO Devices

المشكلة: "Git Push Failed"
الحل: git pull أولاً ثم git push
```

---

## ✅ قائمة الفحص (Checklist)

- [ ] استنسخت المشروع
- [ ] فتحته في VS Code
- [ ] ثبتّ PlatformIO
- [ ] بنيت البرنامج بنجاح
- [ ] رفعته على الكيت
- [ ] جرّبته - يعمل! ✅
- [ ] قرأت CONTRIBUTING.md
- [ ] أنا جاهز للتطوير 🚀

---

## 📞 احتاج ساعدة؟

```
1. اقرأ LOCAL_DOCS.md (معلومات الملفات)
2. ابحث في DIAGNOSTICS.md (المشاكل الشائعة)
3. اسأل قائد الفريق
```

---

## 🎉 هلا وسهلا في الفريق!

الآن أنت جاهز! 🚀

احفظ هذا الملف وارجع له متى احتجت!

---

**Happy Coding! 💻**
