
# 💣 Mayın Tarlası (Minesweeper) - C++ & SDL3

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![SDL3](https://img.shields.io/badge/SDL-3.0-orange.svg)
![CMake](https://img.shields.io/badge/CMake-3.20+-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)

## 📖 İçindekiler
1. [Proje Hakkında](#-proje-hakkında)
2. [Oyun Mekanikleri ve Özellikler](#-oyun-mekanikleri-ve-özellikler)
3. [Algoritma ve Tasarım Kararları](#-algoritma-ve-tasarım-kararları)
4. [Proje Mimarisi (Taşınabilir Yapı)](#-proje-mimarisi-taşınabilir-yapı)
5. [Kurulum ve Çalıştırma](#-kurulum-ve-çalıştırma)
6. [Geliştirici](#-geliştirici)

---

## 🎮 Proje Hakkında
Bu proje, **Kocaeli Üniversitesi Programlama 2 dersi dönem projesi** kapsamında geliştirilmiştir. 

Klasik Mayın Tarlası (Minesweeper) oyununun **C++17** ve modern **SDL3** grafik motoru kullanılarak sıfırdan tasarlanmış, performans odaklı bir versiyonudur. Proje, sadece bir oyun olmakla kalmayıp; C++ ile bellek yönetimi, CMake ile taşınabilir derleme sistemleri (build systems) ve harici kütüphane entegrasyonu konularında akademik beklentileri karşılayacak standartlarda hazırlanmıştır.
Bu proje, klasik Mayın Tarlası oyununun **C++17** ve modern **SDL3** grafik motoru kullanılarak sıfırdan geliştirilmiş, performans odaklı bir versiyonudur. Proje, sadece bir oyun olmakla kalmayıp; C++ ile bellek yönetimi, CMake ile derleme sistemleri (build systems) ve harici kütüphane (external library) entegrasyonu konularında best-practice'leri göstermek amacıyla tasarlanmıştır.

## ✨ Oyun Mekanikleri ve Özellikler
* **Kazanma/Kaybetme Durumları (Game States):** Oyun `MENU`, `PLAYING`, `GAME_OVER` ve `GAME_WIN` olmak üzere 4 farklı state (durum) üzerinden State Machine mantığıyla yönetilmektedir. Tüm mayınsız alanlar açıldığında oyun başarıyla tamamlanır.
* **Zamana Karşı Yarış (Countdown Timer):** `SDL_GetTicks()` kullanılarak oyuna 60 saniyelik bir zaman sınırı entegre edilmiştir. Süre bitiminde oyuncu otomatik olarak kaybeder.
* **Flood-Fill Algoritması:** Mayın olmayan boş bir kutuya tıklandığında, etrafındaki diğer boş kutuları özyineli (recursive) olarak açan algoritma entegre edilmiştir.
* **Native Görsel Yönetimi:** Dışa bağımlılığı (SDL_image vb.) azaltmak amacıyla, sprite'lar (bayrak ve mayın) oyun içine standart `.bmp` formatında dahil edilmiş ve native `SDL_LoadBMP` fonksiyonu ile belleğe alınmıştır.

---

## 🛠️ Kullanılan Teknolojiler
* **Dil:** C++17
* **Grafik Motoru:** SDL3
* **Yazı (Font) Motoru:** SDL3_ttf
* **Derleme Sistemi:** CMake & MinGW


## 🚀 Kurulum ve Çalıştırma

### 🎯 Yöntem 1: Sadece Oynamak İsteyenler İçin (Derlenmiş Sürüm)
1. Bu GitHub deposunun **[Releases]** sekmesine gidin.
2. En son sürüm olan `.zip` dosyasını indirin ve klasöre çıkartın.
3. Hiçbir kurulum yapmadan `mayin_tarlasi.exe` dosyasına çift tıklayarak oynamaya başlayın!

### 💻 Yöntem 2: Geliştiriciler İçin (Kaynak Koddan Derleme)
1. Repoyu bilgisayarınıza klonlayın:
```bash
   git clone https://github.com/kerem-kuru/mayin_tarlasi.git
```

2. **CLion**, **Visual Studio** veya CMake destekli herhangi bir IDE ile projeyi açın.
3. Proje yüklendiğinde `CMakeLists.txt` dosyası otomatik olarak `external/` klasöründeki SDL3 kütüphanelerini bağlayacaktır (Linker).
4. Projeyi Build (Derle) veya Run (Çalıştır) komutuyla başlattığınızda, CMake gerekli olan `.dll` ve `.bmp` dosyalarını otomatik olarak derleme klasörüne (örn. `cmake-build-debug`) kopyalayacak ve oyunu hatasız başlatacaktır.

---

## 👨‍💻 Geliştirici
* **Kerem**
* *Yazılım Mühendisliği, Kocaeli Üniversitesi*
