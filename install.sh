#!/bin/bash

# Root yetkisi kontrolü
if [ "$EUID" -ne 0 ]; then 
    echo "Bu script root yetkisi gerektirir. Lütfen 'sudo' ile çalıştırın."
    exit 1
fi

echo "Cyber Scarecrow IoT Security System kurulumu başlıyor..."

# Gerekli paketlerin kontrolü ve kurulumu
echo "Gerekli paketler kontrol ediliyor..."
apt-get update
apt-get install -y build-essential

# Hook kütüphanesinin derlenmesi
echo "Hook kütüphanesi derleniyor..."
gcc -Wall -fPIC -shared src/hook/hook.c -o hook.so -ldl -O2

if [ ! -f hook.so ]; then
    echo "Derleme hatası! hook.so oluşturulamadı."
    exit 1
fi

# Kütüphanenin sistem dizinine kopyalanması
echo "Kütüphane sistem dizinine kopyalanıyor..."
install -m 755 hook.so /usr/lib/cyber_scarecrow.so

# Servis dosyasının kopyalanması
echo "Servis dosyası kopyalanıyor..."
cp cyber-scarecrow.service /etc/systemd/system/

# Systemd servisinin yeniden yüklenmesi ve aktif edilmesi
echo "Servis aktif ediliyor..."
systemctl daemon-reload
systemctl enable cyber-scarecrow
systemctl start cyber-scarecrow

# Kurulum durumunun kontrolü
if systemctl is-active --quiet cyber-scarecrow; then
    echo "Kurulum başarıyla tamamlandı!"
    echo "Servis durumu:"
    systemctl status cyber-scarecrow | grep Active
else
    echo "Kurulum tamamlandı ancak servis başlatılamadı."
    echo "Lütfen 'systemctl status cyber-scarecrow' komutu ile detayları kontrol edin."
fi 