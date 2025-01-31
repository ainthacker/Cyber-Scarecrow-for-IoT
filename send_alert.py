#!/usr/bin/env python3
import smtplib
import sys
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from datetime import datetime

def send_alert(subject, message):
    # E-posta ayarları
    sender_email = "huseynovu18@gmail.com"           # Gönderen e-posta adresi
    sender_password = "tnmizafjdfvcarnz"             # Uygulama şifresi
    receiver_email = "guseynof.tural18@gmail.com"    # Alıcı e-posta adresi
    
    # E-posta oluşturma
    msg = MIMEMultipart()
    msg['From'] = sender_email
    msg['To'] = receiver_email
    msg['Subject'] = f"[CYBER SCARECROW ALERT] {subject}"
    
    # Mesaj içeriği
    body = f"""
    ⚠️ GÜVENLİK UYARISI ⚠️
    
    Zaman: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}
    
    Olay: {subject}
    
    Detaylar:
    {message}
    
    Bu mail Cyber Scarecrow güvenlik sistemi tarafından otomatik olarak gönderilmiştir.
    """
    
    msg.attach(MIMEText(body, 'plain'))
    
    try:
        # Gmail SMTP sunucusuna bağlanma
        server = smtplib.SMTP('smtp.gmail.com', 587)
        server.starttls()
        server.login(sender_email, sender_password)
        
        # E-posta gönderme
        text = msg.as_string()
        server.sendmail(sender_email, receiver_email, text)
        server.quit()
        
        print("Alert başarıyla gönderildi!")
        return True
    except Exception as e:
        print(f"Hata: {str(e)}")
        return False

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Kullanım: ./send_alert.py 'konu' 'mesaj'")
        sys.exit(1)
    
    subject = sys.argv[1]
    message = sys.argv[2]
    send_alert(subject, message) 