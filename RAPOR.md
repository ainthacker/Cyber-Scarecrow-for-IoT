# Cyber Scarecrow: IoT Güvenlik Projesi Raporu

## 1. Proje Özeti

Cyber Scarecrow, IoT cihazlarını kötü amaçlı yazılımlardan (malware) korumak için geliştirilmiş yenilikçi bir güvenlik çözümüdür. Proje, sistem çağrılarını manipüle ederek ve yanıltıcı bir ortam sunarak zararlı yazılımları etkisiz hale getirmeyi amaçlamaktadır.

## 2. Teknik Özellikler

### 2.1. Temel Bileşenler
- **Hook Modülü**: Sistem çağrılarını yakalar ve manipüle eder
- **Kandırma Modülü**: Sahte sistem bilgileri ve ortam sunar
- **Tespit Modülü**: Şüpheli aktiviteleri izler ve raporlar
- **Log Modülü**: Sistem aktivitelerini kaydeder

### 2.2. Kullanılan Teknolojiler
- **Programlama Dili**: C
- **Sistem Çağrıları**: LD_PRELOAD mekanizması
- **Derleme Araçları**: gcc, make
- **Kütüphaneler**: 
  * libdl (dinamik yükleme)
  * libcurl (ağ iletişimi - planlanan)

## 3. Özellikler ve Yetenekler

### 3.1. Sistem Bilgileri Manipülasyonu
- **Sahte Sistem Bilgileri**:
  * Sistem Adı: QEMU
  * Makine Tipi: vbox
  * Versiyon: 4.9.0-virtual
- **Amaç**: Malware'in sanal/analiz ortamında olduğunu düşünmesini sağlamak

### 3.2. Zaman Manipülasyonu
- **Stratejiler**:
  * Sabit Zaman (2024-03-01)
  * Geçmiş Zaman (1 yıl öncesi)
  * Gelecek Zaman (1 yıl sonrası)
  * Rastgele Zaman
- **Etkilenen Fonksiyonlar**:
  * time()
  * clock_gettime()

### 3.3. Analiz Araçları Simülasyonu
- **Sahte Araç Varlığı**:
  * Debugger'lar (gdb, strace, ltrace)
  * Disassembler'lar (ida, radare2, ghidra)
  * Analiz Araçları (objdump, nm, strings)
- **Sahte Kütüphaneler**:
  * libanalysis.so
  * libdbg.so
  * libmonitoring.so
  * libsandbox.so

### 3.4. Güvenlik Önlemleri
- **Kritik Dosya Koruması**:
  * /etc/passwd
  * /etc/shadow
  * /etc/security/access.conf
  * /etc/sudoers
- **Debug Koruması**:
  * Debug komutlarının engellenmesi
  * Analiz araçlarının çalıştırılmasının engellenmesi

## 4. Test Sonuçları

### 4.1. Sistem Bilgileri Testi
- ✅ Sahte sistem bilgileri başarıyla döndürüldü
- ✅ Tutarlı QEMU/VBox ortamı simüle edildi

### 4.2. Zaman Manipülasyonu Testi
- ✅ Sabit zaman başarıyla uygulandı
- ✅ Zaman tutarlılığı sağlandı
- ✅ Farklı zaman fonksiyonları arasında uyum sağlandı

### 4.3. Analiz Araçları Testi
- ✅ Tüm analiz araçları "mevcut" gösterildi
- ✅ Sahte kütüphane içerikleri başarıyla sunuldu
- ✅ Debug komutları başarıyla engellendi

### 4.4. Güvenlik Testleri
- ✅ Kritik dosyalara erişim engellendi
- ✅ Şüpheli aktiviteler loglandı
- ✅ Debug araçları çalıştırılamadı

## 5. Güvenlik Analizi

### 5.1. Güçlü Yönler
1. Çoklu kandırma stratejisi
2. Tutarlı sahte ortam
3. Düşük sistem kaynağı kullanımı
4. Modüler yapı
5. Genişletilebilir mimari

### 5.2. Geliştirme Alanları
1. Ağ trafiği manipülasyonu
2. Daha fazla sistem çağrısı desteği
3. Konfigürasyon yönetimi
4. Uzaktan izleme özellikleri

## 6. Gelecek Planları

### 6.1. Kısa Vadeli Hedefler
- [ ] Konfigürasyon sistemi
- [ ] Web arayüzü
- [ ] Servis yapısı
- [ ] Kurulum scripti

### 6.2. Uzun Vadeli Hedefler
- [ ] Mobil uygulama desteği
- [ ] Makine öğrenmesi entegrasyonu
- [ ] Dağıtık yapı desteği
- [ ] Zero-day tespit sistemi

## 7. Teknik Detaylar

### 7.1. Dosya Yapısı
```
src/
├── detector/
│   ├── detector.c
│   └── detector.h
├── deception/
│   └── deception.c
└── tests/
    └── test_target.c
```

### 7.2. Derleme ve Kurulum
```bash
# Derleme
make clean && make

# Çalıştırma
LD_PRELOAD=./scarecrow.so ./test_target
```

### 7.3. Bağımlılıklar
- gcc
- make
- libdl
- (planlanan) libcurl

## 8. Sonuç

Cyber Scarecrow projesi, IoT güvenliği alanında yenilikçi bir yaklaşım sunmaktadır. Kandırma tabanlı güvenlik stratejisi, özellikle malware analizi ve korunma konusunda etkili sonuçlar vermektedir. Projenin modüler yapısı ve genişletilebilir mimarisi, gelecekteki geliştirmeler için güçlü bir temel oluşturmaktadır.

## 9. Ekler

### 9.1. Test Çıktıları
```
=== Sistem Bilgileri Testi ===
Sistem: QEMU
Makine: vbox
Versiyon: 4.9.0-virtual

=== Zaman Manipülasyonu Testi ===
time() sonucu: Thu Feb 29 19:00:00 2024
clock_gettime() sonucu: 1709251200 saniye

... (diğer test sonuçları)
```

### 9.2. Performans Metrikleri
- Bellek Kullanımı: Minimal
- CPU Kullanımı: Düşük
- Sistem Çağrısı Gecikme Süresi: İhmal edilebilir

### 9.3. Katkıda Bulunanlar
- Proje Ekibi
- Test Ekibi
- Güvenlik Danışmanları 