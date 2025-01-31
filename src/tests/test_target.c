#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>

int main() {
    // Sistem bilgisi testi
    printf("\n=== Sistem Bilgisi Testi ===\n");
    struct utsname buf;
    if (uname(&buf) == 0) {
        printf("Sistem Adı: %s\n", buf.sysname);
        printf("Makine Adı: %s\n", buf.nodename);
        printf("Sürüm: %s\n", buf.release);
        printf("Versiyon: %s\n", buf.version);
        printf("Makine Tipi: %s\n", buf.machine);
    }

    // Zaman testi
    printf("\n=== Zaman Testi ===\n");
    time_t t = time(NULL);
    printf("Unix Zamanı: %ld\n", t);
    
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
        printf("clock_gettime Zamanı: %ld.%ld\n", ts.tv_sec, ts.tv_nsec);
    }

    // Analiz araçları testi
    printf("\n=== Analiz Araçları Testi ===\n");
    const char *tools[] = {
        "/usr/bin/gdb",
        "/usr/bin/strace",
        "/usr/bin/ida",
        "/usr/bin/radare2"
    };
    
    for (int i = 0; i < sizeof(tools)/sizeof(tools[0]); i++) {
        if (access(tools[i], F_OK) == 0) {
            printf("Bulundu: %s\n", tools[i]);
        } else {
            printf("Bulunamadı: %s\n", tools[i]);
        }
    }

    // Kritik dosya erişim testi
    printf("\n=== Kritik Dosya Erişim Testi ===\n");
    printf("/etc/passwd dosyasına erişim deneniyor...\n");
    FILE *f = fopen("/etc/passwd", "r");
    if (f) {
        printf("Erişim başarılı: /etc/passwd\n");
        fclose(f);
    } else {
        printf("Erişim engellendi: /etc/passwd (Permission denied)\n");
    }

    printf("/etc/shadow dosyasına erişim deneniyor...\n");
    f = fopen("/etc/shadow", "r");
    if (f) {
        printf("Erişim başarılı: /etc/shadow\n");
        fclose(f);
    } else {
        printf("Erişim engellendi: /etc/shadow (Permission denied)\n");
    }

    printf("/etc/security/access.conf dosyasına erişim deneniyor...\n");
    f = fopen("/etc/security/access.conf", "r");
    if (f) {
        printf("Erişim başarılı: /etc/security/access.conf\n");
        fclose(f);
    } else {
        printf("Erişim engellendi: /etc/security/access.conf (Permission denied)\n");
    }

    printf("/etc/sudoers dosyasına erişim deneniyor...\n");
    f = fopen("/etc/sudoers", "r");
    if (f) {
        printf("Erişim başarılı: /etc/sudoers\n");
        fclose(f);
    } else {
        printf("Erişim engellendi: /etc/sudoers (Permission denied)\n");
    }

    printf("\nTüm testler tamamlandı.\n");
    return 0;
} 