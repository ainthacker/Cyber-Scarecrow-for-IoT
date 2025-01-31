#define _GNU_SOURCE
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/utsname.h>
#include <stdlib.h>

// Original function types
typedef int (*orig_open_f_type)(const char *pathname, int flags, ...);
typedef int (*orig_openat_f_type)(int dirfd, const char *pathname, int flags, ...);
typedef time_t (*orig_time_f_type)(time_t *tloc);
typedef int (*orig_clock_gettime_f_type)(clockid_t clk_id, struct timespec *tp);
typedef int (*orig_uname_f_type)(struct utsname *buf);
typedef int (*orig_access_f_type)(const char *pathname, int mode);
typedef FILE* (*orig_fopen_f_type)(const char *pathname, const char *mode);

// Fake system information
static const char *FAKE_SYSNAME = "QEMU";
static const char *FAKE_NODENAME = "virtual-machine";
static const char *FAKE_RELEASE = "4.9.0-virtual";
static const char *FAKE_VERSION = "Virtual Linux";
static const char *FAKE_MACHINE = "vbox";

// Fake time values
static const time_t FIXED_TIME = 1709251200; // 2024-03-01

// Fake analysis tools
static const char *ANALYSIS_TOOLS[] = {
    "/usr/bin/gdb",
    "/usr/bin/strace",
    "/usr/bin/ltrace",
    "/usr/bin/ida",
    "/usr/bin/radare2",
    "/usr/bin/ghidra",
    "/usr/bin/objdump",
    "/usr/bin/nm",
    "/usr/bin/strings"
};

static const char *ANALYSIS_LIBS[] = {
    "libanalysis.so",
    "libdbg.so",
    "libmonitoring.so",
    "libsandbox.so"
};

// Alert sending function
void send_security_alert(const char* pathname, int is_root) {
    char subject[256];
    char details[512];
    
    if (strcmp(pathname, "/etc/passwd") == 0) {
        snprintf(subject, sizeof(subject), "Access Attempt to User Database");
        snprintf(details, sizeof(details), 
            "Access attempt detected to system user database (%s).\n"
            "This file contains user account information.\n"
            "Access %s", 
            pathname,
            is_root ? "was made with ROOT privileges!" : "was blocked.");
    }
    else if (strcmp(pathname, "/etc/shadow") == 0) {
        snprintf(subject, sizeof(subject), "Access Attempt to Password Database");
        snprintf(details, sizeof(details), 
            "Access attempt detected to system password database (%s).\n"
            "This file contains password hashes.\n"
            "Access %s", 
            pathname,
            is_root ? "was made with ROOT privileges!" : "was blocked.");
    }
    else if (strcmp(pathname, "/etc/sudoers") == 0) {
        snprintf(subject, sizeof(subject), "Access Attempt to Sudo Configuration");
        snprintf(details, sizeof(details), 
            "Access attempt detected to sudo configuration file (%s).\n"
            "This file contains sudo privileges and configuration.\n"
            "Access %s", 
            pathname,
            is_root ? "was made with ROOT privileges!" : "was blocked.");
    }
    else if (strcmp(pathname, "/etc/security/access.conf") == 0) {
        snprintf(subject, sizeof(subject), "Access Attempt to Security Configuration");
        snprintf(details, sizeof(details), 
            "Access attempt detected to system security configuration (%s).\n"
            "This file contains system access controls.\n"
            "Access %s", 
            pathname,
            is_root ? "was made with ROOT privileges!" : "was blocked.");
    }
    
    char command[1024];
    snprintf(command, sizeof(command),
             "python3 /usr/local/bin/cyber_scarecrow/send_alert.py '%s' '%s\nUser ID: %d\nProcess ID: %d\nTime: %ld'",
             subject, details, getuid(), getpid(), time(NULL));
    system(command);
}

// Log function
void log_access(const char* action, const char* path) {
    FILE *log = fopen("/tmp/cyber_scarecrow.log", "a");
    if (log) {
        fprintf(log, "%s: %s (UID: %d)\n", action, path, getuid());
        fclose(log);
        
        // Send alert for suspicious access
        send_security_alert(path, 0);  // Send as normal user access
    }
}

// System information manipulation
int uname(struct utsname *buf) {
    orig_uname_f_type orig_uname = (orig_uname_f_type)dlsym(RTLD_NEXT, "uname");
    int result = orig_uname(buf);
    
    if (result == 0) {
        strncpy(buf->sysname, FAKE_SYSNAME, sizeof(buf->sysname) - 1);
        strncpy(buf->nodename, FAKE_NODENAME, sizeof(buf->nodename) - 1);
        strncpy(buf->release, FAKE_RELEASE, sizeof(buf->release) - 1);
        strncpy(buf->version, FAKE_VERSION, sizeof(buf->version) - 1);
        strncpy(buf->machine, FAKE_MACHINE, sizeof(buf->machine) - 1);
    }
    
    return result;
}

// Time manipulation
time_t time(time_t *tloc) {
    time_t fake_time = FIXED_TIME;
    
    if (tloc != NULL) {
        *tloc = fake_time;
    }
    return fake_time;
}

int clock_gettime(clockid_t clk_id, struct timespec *tp) {
    orig_clock_gettime_f_type orig_clock_gettime = 
        (orig_clock_gettime_f_type)dlsym(RTLD_NEXT, "clock_gettime");
    
    int result = orig_clock_gettime(clk_id, tp);
    if (result == 0) {
        tp->tv_sec = FIXED_TIME;
        tp->tv_nsec = 0;
    }
    return result;
}

// Analysis tools simulation
int access(const char *pathname, int mode) {
    orig_access_f_type orig_access = (orig_access_f_type)dlsym(RTLD_NEXT, "access");
    
    // Fake existence check for analysis tools
    for (size_t i = 0; i < sizeof(ANALYSIS_TOOLS) / sizeof(ANALYSIS_TOOLS[0]); i++) {
        if (strcmp(pathname, ANALYSIS_TOOLS[i]) == 0) {
            return 0; // Show file as existing
        }
    }
    
    // Fake existence check for analysis libraries
    for (size_t i = 0; i < sizeof(ANALYSIS_LIBS) / sizeof(ANALYSIS_LIBS[0]); i++) {
        if (strstr(pathname, ANALYSIS_LIBS[i]) != NULL) {
            return 0; // Show library as existing
        }
    }
    
    return orig_access(pathname, mode);
}

// Hook open function
int open(const char *pathname, int flags, ...) {
    orig_open_f_type orig_open = (orig_open_f_type)dlsym(RTLD_NEXT, "open");
    
    // Critical files access control
    const char *critical_files[] = {
        "/etc/passwd", 
        "/etc/shadow", 
        "/etc/sudoers",
        "/etc/security/access.conf"
    };
    
    for (size_t i = 0; i < sizeof(critical_files) / sizeof(critical_files[0]); i++) {
        if (strcmp(pathname, critical_files[i]) == 0) {
            if (getuid() != 0) {  // Block if not root
                send_security_alert(pathname, 0);  // Normal user access
                errno = EACCES;
                return -1;
            } else {
                send_security_alert(pathname, 1);  // Root access
            }
        }
    }
    
    return orig_open(pathname, flags);
}

// Hook openat function
int openat(int dirfd, const char *pathname, int flags, ...) {
    orig_openat_f_type orig_openat = (orig_openat_f_type)dlsym(RTLD_NEXT, "openat");
    
    // Critical files access control
    const char *critical_files[] = {
        "/etc/passwd", 
        "/etc/shadow", 
        "/etc/sudoers",
        "/etc/security/access.conf"
    };
    
    for (size_t i = 0; i < sizeof(critical_files) / sizeof(critical_files[0]); i++) {
        if (strcmp(pathname, critical_files[i]) == 0) {
            if (getuid() != 0) {  // Block if not root
                send_security_alert(pathname, 0);  // Normal user access
                errno = EACCES;
                return -1;
            } else {
                send_security_alert(pathname, 1);  // Root access
            }
        }
    }
    
    return orig_openat(dirfd, pathname, flags);
}

// Hook fopen function
FILE* fopen(const char *pathname, const char *mode) {
    orig_fopen_f_type orig_fopen = (orig_fopen_f_type)dlsym(RTLD_NEXT, "fopen");
    
    // Critical files access control
    const char *critical_files[] = {
        "/etc/passwd", 
        "/etc/shadow", 
        "/etc/sudoers",
        "/etc/security/access.conf"
    };
    
    for (size_t i = 0; i < sizeof(critical_files) / sizeof(critical_files[0]); i++) {
        if (strcmp(pathname, critical_files[i]) == 0) {
            if (getuid() != 0) {  // Block if not root
                send_security_alert(pathname, 0);  // Normal user access
                errno = EACCES;
                return NULL;
            } else {
                send_security_alert(pathname, 1);  // Root access
            }
        }
    }
    
    return orig_fopen(pathname, mode);
} 