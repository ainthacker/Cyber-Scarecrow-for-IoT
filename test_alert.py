#!/usr/bin/env python3

print("Cyber Scarecrow Alert Test Programı")
print("-----------------------------------")

# Kritik dosyalar listesi
critical_files = [
    "/etc/passwd",
    "/etc/shadow",
    "/etc/sudoers",
    "/etc/security/access.conf"
]

# Her dosya için erişim testi yap
for file_path in critical_files:
    print(f"\nDosya erişim testi: {file_path}")
    try:
        with open(file_path, 'r') as f:
            print("✓ Dosya açıldı (root yetkisi var)")
            # Sadece ilk satırı oku
            first_line = f.readline().strip()
            print(f"İlk satır: {first_line}")
    except PermissionError:
        print("✗ Erişim engellendi! (Alert tetiklenmeli)")
    except FileNotFoundError:
        print("? Dosya bulunamadı")
    except Exception as e:
        print(f"! Hata: {str(e)}")

print("\nTest tamamlandı. Lütfen alert log dosyasını kontrol edin:") 