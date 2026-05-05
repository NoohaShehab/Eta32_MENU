# 👥 For Team Members - ما يجب أن تفعله؟

## السيناريو الكامل: عضو جديد يريد يشتغل على المشروع

---

## 📋 الخطوات بالتفصيل

### المرحلة 1: الإعداد الأولي (15 دقيقة)

#### الخطوة 1: استنساخ المشروع

```bash
# افتح Terminal
git clone https://github.com/NoohaShehab/Eta32_MENU.git
cd Eta32_MENU
```

#### الخطوة 2: تثبيت الأدوات

```bash
# إذا لم تثبّت PlatformIO بعد:
pip install platformio

# أو: من VS Code Extensions (أسهل)
# اكتب: PlatformIO وثبّت
```

#### الخطوة 3: فتح المشروع

```bash
code .
```

---

### المرحلة 2: الاختبار الأول (10 دقائق)

#### الخطوة 4: التحقق من الإعدادات

1. افتح ملف `platformio.ini`
2. تحقق من `upload_port = COM3` (غيّره إذا لزم)
3. لمعرفة الـ COM الصحيح: PlatformIO → Devices

#### الخطوة 5: بناء البرنامج

```bash
pio run
```

**يجب أن ترى:**

```
BUILD SUCCESSFUL
```

#### الخطوة 6: رفع على الكيت

```bash
pio run --target upload
```

**يجب أن ترى:**

```
UPLOAD SUCCESSFUL
```

---

### المرحلة 3: الاختبار على الكيت (5 دقائق)

#### الخطوة 7: اختبر البرنامج

على الكيت:

```
اضغط 1: المؤقت
  - جرّبه: أدخل 05 ثانية
  - شوف الوقت ينزل على LCD و 7-Segment

اضغط 2: اللعبة
  - اضغط أي زر للقفز
  - تجنب العقبات الزرقاء
  - اجمّع النقاط
```

#### الخطوة 8: استمتع! 🎉

```
البرنامج يعمل بدون مشاكل!
أنت الآن جزء من الفريق! 👋
```

---

## 📚 الملفات المهمة (اطلبها من الفريق)

لا تنسى نسخ هذه الملفات من جهازك أو من أي عضو:

```
📁 Local Documentation (ملفات محلية):
├── START_HERE.md ..................... ابدأ هنا!
├── TEAM_SETUP.md .................... إعداد مفصل
├── QUICK_START.md ................... استخدام البرنامج
├── QUICK_REFERENCE.md .............. أوامر سريعة
├── CONTRIBUTING.md ................. معايير المساهمة
├── ARCHITECTURE.md ................. فهم الكود
├── DIAGNOSTICS.md .................. حل المشاكل
├── LOCAL_DOCS.md ................... معلومات الملفات
├── SUMMARY.md ...................... ملخص التحسينات
├── IMPROVEMENTS.md ................. قائمة التحسينات
├── FUTURE_UPDATES.md .............. خطة التطوير
└── INDEX.md ........................ فهرس شامل
```

---

## 🎯 ماذا بعد الإعداد؟

### إذا أردت فهم الكود:

```
1. اقرأ src/main.cpp
2. اقرأ ARCHITECTURE.md
3. اقرأ CONTRIBUTING.md (معايير الترميز)
4. ابدأ التطوير!
```

### إذا أردت إضافة ميزة:

```
1. اقرأ CONTRIBUTING.md (الخطوات)
2. أنشئ branch جديد: git checkout -b feature/my-feature
3. اعمل التعديل
4. اختبره على الكيت
5. Push وأطلب merge
```

### إذا واجهت مشكلة:

```
1. اقرأ DIAGNOSTICS.md
2. جرّب الحل
3. إذا استمرت المشكلة: اسأل الفريق
```

---

## ⚡ الأوامر الأساسية اليومية

### في البداية:

```bash
git pull origin main    # آخر نسخة من الفريق
pio run                 # بناء
pio run --target upload # رفع على الكيت
```

### أثناء العمل:

```bash
# تعديل الملفات
# ... تطوير ...

# اختبر
pio run
pio run --target upload

# إذا كل شيء تمام:
git add .
git commit -m "وصف التعديل"
git push origin main
```

---

## 🚨 المشاكل الشائعة والحلول

### ❌ "Build Failed"

```bash
pio run --target clean
pio run
```

### ❌ "COM Port Not Found"

```bash
# شوف الأجهزة المتصلة
pio device list

# حدّث platformio.ini بـ COM الصحيح
```

### ❌ "Upload Failed"

```bash
# اضغط Reset على الكيت
# ثم حاول مجدداً
pio run --target upload
```

### ❌ "Git Push Failed"

```bash
git pull origin main    # شد آخر التحديثات أولاً
git push origin main    # ثم رفع
```

---

## 📊 ملخص الملفات والأدوار

| الملف                  | الدور           | متى تستخدمه     |
| ---------------------- | --------------- | --------------- |
| **START_HERE.md**      | ملخص سريع       | أول مرة         |
| **TEAM_SETUP.md**      | إعداد مفصل      | عند الإعداد     |
| **QUICK_START.md**     | الاستخدام       | لتشغيل البرنامج |
| **QUICK_REFERENCE.md** | أوامر سريعة     | يومياً          |
| **CONTRIBUTING.md**    | معايير المساهمة | قبل التطوير     |
| **ARCHITECTURE.md**    | فهم الكود       | لفهم التصميم    |
| **DIAGNOSTICS.md**     | حل المشاكل      | عند المشاكل     |
| **README.md** (الريبو) | معلومات المشروع | معلومات عامة    |

---

## 🎓 نصائح مهمة

### ✅ افضل الممارسات:

1. **اقرأ قبل العمل**
   - اقرأ CONTRIBUTING.md قبل أي تعديل
   - اقرأ معايير الترميز
   - اتبع النمط الموجود

2. **اختبر قبل الرفع**
   - بنّي البرنامج: `pio run`
   - رفع على الكيت: `pio run --target upload`
   - اختبره على الكيت نفسه
   - لا توجد أخطاء؟ OK للرفع

3. **استخدم Git صحيح**
   - اسحب الآخر: `git pull`
   - أنشئ branch: `git checkout -b ...`
   - اعمل commit واضح: `git commit -m "..."`
   - ادفع لـ GitHub: `git push`

4. **وثّق تعديلاتك**
   - اكتب تعليقات واضحة في الكود
   - صف الـ commit بوضوح
   - أضف معلومات في PR description

---

## 🔗 الروابط المهمة

```
GitHub Repo:
https://github.com/NoohaShehab/Eta32_MENU

PlatformIO Docs:
https://docs.platformio.org/

Arduino Reference:
https://www.arduino.cc/reference/

Git Guide:
https://git-scm.com/book/en/v2/
```

---

## 📝 قائمة الفحص النهائية

قبل أن تبدأ:

- [ ] استنسخت المشروع
- [ ] ثبتّ PlatformIO
- [ ] فتحت المشروع في VS Code
- [ ] بنيت البرنامج: ✅
- [ ] رفعته على الكيت: ✅
- [ ] اختبرته: ✅
- [ ] قرأت CONTRIBUTING.md
- [ ] أنا فاهم معايير الترميز
- [ ] أنا جاهز للتطوير: 🚀

---

## 🎉 الخلاصة

```
الخطوات البسيطة:

1. استنسخ: git clone
2. افتح: code .
3. ثبّت: PlatformIO
4. بنّي: pio run
5. رفع: pio run --target upload
6. اختبر: على الكيت ✅
7. إذا كل شيء تمام → أنت جاهز!

```

---

## 📞 تحتاج ساعدة؟

1. **البحث أولاً**:
   - LOCAL_DOCS.md (معلومات الملفات)
   - DIAGNOSTICS.md (المشاكل الشائعة)
   - CONTRIBUTING.md (الأسئلة الشائعة)

2. **اسأل الفريق**:
   - قائد الفريق
   - أي عضو أقدم
   - في Discussions على GitHub

---

**🚀 أهلا وسهلا في الفريق!**

**استمتع بالعمل! 💻**
