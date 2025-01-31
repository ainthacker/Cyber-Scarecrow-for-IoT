#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "../detector/detector.h"

// Orijinal fonksiyon tipleri
typedef int (*orig_uname_f_type)(struct utsname *buf);
typedef int (*orig_clock_f_type)(clockid_t clk_id, struct timespec *tp);
typedef int (*orig_access_f_type)(const char *pathname, int mode);
typedef FILE* (*orig_fopen_f_type)(const char *pathname, const char *mode);
typedef int (*orig_system_f_type)(const char *command);
typedef int (*orig_execve_f_type)(const char *pathname, char *const argv[], char *const envp[]);
typedef time_t (*orig_time_f_type)(time_t *t);

// Zaman manipülasyon stratejileri
#define TIME_STRATEGY_FIXED     1  // Sabit zaman
#define TIME_STRATEGY_PAST      2  // Geçmiş zaman
#define TIME_STRATEGY_FUTURE    3  // Gelecek zaman
#define TIME_STRATEGY_RANDOM    4  // Rastgele zaman

static int current_time_strategy = TIME_STRATEGY_FIXED;
static time_t fixed_time = 1709251200;  // 2024-03-01 00:00:00

// Zaman manipülasyonu yardımcı fonksiyonları
time_t get_manipulated_time(time_t real_time) {
    switch(current_time_strategy) {
        case TIME_STRATEGY_FIXED:
            return fixed_time;
        
        case TIME_STRATEGY_PAST:
            return real_time - (365 * 24 * 3600);  // 1 yıl öncesi
            
        case TIME_STRATEGY_FUTURE:
            return real_time + (365 * 24 * 3600);  // 1 yıl sonrası
            
        case TIME_STRATEGY_RANDOM:
            return fixed_time + (rand() % (10 * 365 * 24 * 3600));  // 0-10 yıl arası rastgele
            
        default:
            return real_time;
    }
}

// Analiz araçları listesi
const char *analysis_tools[] = {
    "/usr/bin/gdb",
    "/usr/bin/strace",
    "/usr/bin/ltrace",
    "/usr/bin/ida",
    "/usr/bin/radare2",
    "/usr/bin/ghidra",
    "/usr/bin/objdump",
    "/usr/bin/nm",
    "/usr/bin/strings",
    NULL
};

// Analiz kütüphaneleri listesi
const char *analysis_libs[] = {
    "libpython",
    "libdbg",
    "libanalysis",
    "libsandbox",
    "libmonitoring",
    "libdebug",
    "libtracing",
    NULL
};

// Sahte sistem bilgileri
int uname(struct utsname *buf) {
    orig_uname_f_type orig_uname = dlsym(RTLD_NEXT, "uname");
    if (!buf) {
        errno = EFAULT;
        return -1;
    }
    
    orig_uname(buf);
    
    strcpy(buf->sysname, "QEMU");
    strcpy(buf->machine, "vbox");
    strcpy(buf->version, "4.9.0-virtual");
    
    check_suspicious_behavior("Sistem bilgisi sorgusu");
    return 0;
}

// Sahte zaman - clock_gettime
int clock_gettime(clockid_t clk_id, struct timespec *tp) {
    orig_clock_f_type orig_clock = dlsym(RTLD_NEXT, "clock_gettime");
    if (!tp) {
        errno = EFAULT;
        return -1;
    }
    
    orig_clock(clk_id, tp);
    
    // Zaman manipülasyonu
    time_t manipulated = get_manipulated_time(tp->tv_sec);
    tp->tv_sec = manipulated;
    
    check_suspicious_behavior("Zaman sorgusu");
    return 0;
}

// Sahte zaman - time
time_t time(time_t *t) {
    orig_time_f_type orig_time = dlsym(RTLD_NEXT, "time");
    
    time_t real_time = orig_time(NULL);
    time_t fake_time = get_manipulated_time(real_time);
    
    if (t) *t = fake_time;
    
    check_suspicious_behavior("Zaman sorgusu (time)");
    return fake_time;
}

// Sahte access çağrıları
int access(const char *pathname, int mode) {
    orig_access_f_type orig_access = dlsym(RTLD_NEXT, "access");
    if (!pathname) {
        errno = EFAULT;
        return -1;
    }
    
    // Analiz araçları için her zaman "var" döndür
    for(int i = 0; analysis_tools[i] != NULL; i++) {
        if(strstr(pathname, analysis_tools[i])) {
            check_suspicious_behavior("Analiz aracı kontrolü");
            return 0; // Dosya var gibi davran
        }
    }
    
    return orig_access(pathname, mode);
}

// Sahte fopen çağrıları
FILE* fopen(const char *pathname, const char *mode) {
    orig_fopen_f_type orig_fopen = dlsym(RTLD_NEXT, "fopen");
    if (!pathname || !mode) {
        errno = EFAULT;
        return NULL;
    }
    
    // Analiz kütüphaneleri için sahte dosya döndür
    for(int i = 0; analysis_libs[i] != NULL; i++) {
        if(strstr(pathname, analysis_libs[i])) {
            check_suspicious_behavior("Analiz kütüphanesi kontrolü");
            static char fake_content[] = "Fake Library Content";
            size_t content_size = sizeof(fake_content) - 1;  // NULL karakteri hariç
            return fmemopen(fake_content, content_size, mode);
        }
    }
    
    return orig_fopen(pathname, mode);
}

// Komut çalıştırma kontrolü
int system(const char *command) {
    orig_system_f_type orig_system = dlsym(RTLD_NEXT, "system");
    if (!command) {
        return orig_system(command);  // shell varlığını kontrol et
    }
    
    // Debug komutlarını engelle
    if(strstr(command, "gdb") || strstr(command, "strace") || 
       strstr(command, "ltrace") || strstr(command, "debug") ||
       strstr(command, "objdump") || strstr(command, "nm")) {
        check_suspicious_behavior("Debug komutu çalıştırma denemesi");
        errno = EPERM;
        return -1;
    }
    
    return orig_system(command);
}

// execve hook
int execve(const char *pathname, char *const argv[], char *const envp[]) {
    orig_execve_f_type orig_execve = dlsym(RTLD_NEXT, "execve");
    if (!pathname || !argv || !envp) {
        errno = EFAULT;
        return -1;
    }
    
    // Debug araçlarını engelle
    for(int i = 0; analysis_tools[i] != NULL; i++) {
        if(strstr(pathname, analysis_tools[i])) {
            check_suspicious_behavior("Debug aracı çalıştırma denemesi");
            errno = EPERM;
            return -1;
        }
    }
    
    return orig_execve(pathname, argv, envp);
} 