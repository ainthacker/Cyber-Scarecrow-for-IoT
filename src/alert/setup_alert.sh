#!/bin/bash

# Alert sistemi dizinini oluştur
sudo mkdir -p /usr/local/bin/cyber_scarecrow

# Python betiğini kopyala
sudo cp send_alert.py /usr/local/bin/cyber_scarecrow/
sudo chmod +x /usr/local/bin/cyber_scarecrow/send_alert.py

# Gerekli Python paketlerini yükle
sudo apt-get update
sudo apt-get install -y python3-pip
pip3 install secure-smtplib

echo "Alert sistemi kuruldu!"
echo "Lütfen /usr/local/bin/cyber_scarecrow/send_alert.py dosyasını düzenleyerek e-posta ayarlarınızı yapın." 