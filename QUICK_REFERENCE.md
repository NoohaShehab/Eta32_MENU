# ⚡ Quick Reference - Essential Commands

## الأوامر السريعة والأساسية

### 🔄 Git Commands

```bash
# استنساخ المشروع أول مرة
git clone https://github.com/NoohaShehab/Eta32_MENU.git
cd Eta32_MENU

# تحديث آخر نسخة من الريبو
git pull origin main

# إضافة الملفات المعدلة
git add .

# حفظ التعديلات (commit)
git commit -m "وصف التعديل"

# رفع التعديلات للريبو
git push origin main

# معرفة حالة المشروع
git status

# معرفة سجل التغييرات
git log --oneline -10

# إرجاع ملف لنسخة قديمة
git checkout -- filename

# حذف branch محلي
git branch -d branch-name
```

---

### 🔨 PlatformIO Commands

```bash
# بناء البرنامج (Compile)
pio run

# رفع البرنامج على الكيت
pio run --target upload

# بناء + رفع في أمر واحد
pio run && pio run --target upload

# مراقبة Serial Monitor
pio device monitor --baud 9600

# نظيف البناء السابق
pio run --target clean

# إعادة البناء من الصفر
pio run --target clean && pio run

# معرفة الأجهزة المتصلة
pio device list

# الإعدادات المحفوظة
pio run --list-envs
```

---

### 🎯 Combined Commands (متكاملة)

```bash
# تحديث + بناء + رفع
git pull && pio run --target clean && pio run --target upload

# اختبار البرنامج بعد الرفع
pio device monitor

# إعادة جميع الملفات المحلية (احذر!)
git reset --hard HEAD
```

---

### 📊 VS Code Shortcuts

| الاختصار           | الوظيفة               |
| ------------------ | --------------------- |
| `Ctrl + B`         | فتح/إغلاق Explorer    |
| `Ctrl + F`         | البحث في الملف        |
| `Ctrl + H`         | البحث والاستبدال      |
| `Ctrl + /`         | التعليق/إلغاء التعليق |
| `Ctrl + Shift + P` | Command Palette       |
| `F1`               | نفس Command Palette   |
| `Ctrl + Shift + D` | Debug Panel           |

---

### 🔌 PlatformIO Keyboard Shortcuts (في VS Code)

| الاختصار         | الوظيفة        |
| ---------------- | -------------- |
| `Ctrl + Alt + B` | Build          |
| `Ctrl + Alt + U` | Upload         |
| `Ctrl + Alt + S` | Serial Monitor |
| `Ctrl + Alt + C` | Clean          |

---

## 📋 Daily Workflow

### في البداية:

```bash
# تحديث الكود
git pull origin main

# بناء واختبار
pio run

# إذا كل شيء تمام:
# ابدأ التطوير
```

### أثناء التطوير:

```bash
# بعد التعديلات:
pio run                          # تجميع
pio run --target upload          # رفع
pio device monitor               # اختبار
```

### قبل الرفع للريبو:

```bash
# تأكد من أن الكود يعمل
pio run
pio run --target upload

# رفع التغييرات
git add .
git commit -m "وصف التعديل"
git push origin main
```

---

## 🐛 Troubleshooting Quick Fixes

| المشكلة          | الحل السريع                                       |
| ---------------- | ------------------------------------------------- |
| Build Failed     | `pio run --target clean && pio run`               |
| Upload Failed    | اضغط Reset على الكيت ثم `pio run --target upload` |
| Port Not Found   | `pio device list` وحدّث `platformio.ini`          |
| Code Not Working | `git pull` ثم `pio run`                           |
| Merge Conflict   | اسأل قائد الفريق                                  |

---

## 🔗 Useful Links

```
المشروع على GitHub:
https://github.com/NoohaShehab/Eta32_MENU

توثيق PlatformIO:
https://docs.platformio.org/

توثيق Arduino:
https://www.arduino.cc/reference/

توثيق Git:
https://git-scm.com/doc
```

---

## 📞 File Organization

```
اسأل عن هذه الملفات من الفريق:
├─ TEAM_SETUP.md ............ هذا الملف (إعداد الفريق)
├─ QUICK_REFERENCE.md ...... الأوامر السريعة (هذا)
├─ CONTRIBUTING.md ......... كيفية المساهمة
├─ SUMMARY.md .............. ملخص التحسينات
├─ DIAGNOSTICS.md ......... استكشاف الأخطاء
└─ README.md ............... معلومات المشروع
```

---

**استخدم هذا الملف كمرجع سريع! 🚀**
