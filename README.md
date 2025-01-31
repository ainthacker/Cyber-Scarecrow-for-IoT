# Cyber Scarecrow IoT Security Project

IoT cihazları için geliştirilmiş bir aldatma (deception) tabanlı güvenlik sistemi.

## Özellikler

- Sistem çağrılarını hook'lama
- Kritik dosya erişim kontrolü
- Sistem bilgisi manipülasyonu (QEMU/vbox görünümü)
- Zaman manipülasyonu
- Analiz araçları simülasyonu

## Kurulum

1. Gereksinimleri yükleyin:
```bash
sudo apt-get update
sudo apt-get install build-essential
```

2. Projeyi derleyin:
```bash
gcc -Wall -fPIC -shared src/hook/hook.c -o hook.so -ldl -O2
```

3. Sistemde kurun:
```bash
sudo install -m 755 hook.so /usr/lib/cyber_scarecrow.so
sudo cp cyber-scarecrow.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable cyber-scarecrow
sudo systemctl start cyber-scarecrow
```

## Test

Test programını derleyin ve çalıştırın:
```bash
gcc test_target.c -o test_target
./test_target
```

## Özellikler

1. **Sistem Bilgisi Manipülasyonu**
   - Sistem: QEMU
   - Makine: vbox
   - Versiyon: 4.9.0-virtual

2. **Zaman Manipülasyonu**
   - Sabit zaman: 2024-03-01

3. **Analiz Araçları Simülasyonu**
   - gdb, strace, ida, radare2 vb.

4. **Kritik Dosya Erişim Kontrolü**
   - /etc/passwd
   - /etc/shadow
   - /etc/sudoers

## Güvenlik Notları

- Root yetkisi gerektirir
- Sistem genelinde etkilidir
- Recovery mode'da devre dışı bırakılabilir

## Lisans

Bu proje MIT lisansı altında lisanslanmıştır. 