#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>

// Orijinal fonksiyon tipleri
typedef int (*orig_open_f_type)(const char *pathname, int flags, ...);
typedef int (*orig_openat_f_type)(int dirfd, const char *pathname, int flags, ...);

// Şüpheli davranış sayacı
static int suspicious_count = 0;

// Alert gönderme fonksiyonu
void send_alert(const char* message) {
    // Log dosyasına yaz
    FILE *log = fopen("/tmp/cyber_scarecrow.log", "a");
    if (log) {
        time_t now = time(NULL);
        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        fprintf(log, "[%s] ALERT: %s\n", timestamp, message);
        fclose(log);
    }
    
    // TODO: HTTP request ile alert gönder
    // Şimdilik sadece log'a yazıyoruz
    // İleride CURL ile HTTP request yapılacak
}

// Şüpheli davranış tespiti
void check_suspicious_behavior(const char* action) {
    suspicious_count++;
    
    char alert[512];
    snprintf(alert, sizeof(alert), 
            "Şüpheli davranış tespit edildi: %s (Toplam: %d, PID: %d)", 
            action, suspicious_count, getpid());
    send_alert(alert);
}

// Hook fonksiyonları
int open(const char* pathname, int flags, ...) {
    orig_open_f_type orig_open = dlsym(RTLD_NEXT, "open");
    mode_t mode = 0;
    
    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode = va_arg(args, mode_t);
        va_end(args);
    }
    
    // Kritik dosya erişimi kontrolü
    if (strstr(pathname, "/etc/passwd") || 
        strstr(pathname, "/etc/shadow") ||
        strstr(pathname, "/etc/security") ||
        strstr(pathname, "/etc/sudoers")) {
        check_suspicious_behavior("Kritik dosya erişimi");
        errno = EACCES;
        return -1;
    }
    
    return orig_open(pathname, flags, mode);
}

int openat(int dirfd, const char* pathname, int flags, ...) {
    orig_openat_f_type orig_openat = dlsym(RTLD_NEXT, "openat");
    mode_t mode = 0;
    
    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode = va_arg(args, mode_t);
        va_end(args);
    }
    
    // Kritik dosya erişimi kontrolü
    if (strstr(pathname, "/etc/passwd") || 
        strstr(pathname, "/etc/shadow") ||
        strstr(pathname, "/etc/security") ||
        strstr(pathname, "/etc/sudoers")) {
        check_suspicious_behavior("Kritik dosya erişimi");
        errno = EACCES;
        return -1;
    }
    
    return orig_openat(dirfd, pathname, flags, mode);
} 