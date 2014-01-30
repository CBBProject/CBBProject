// Posix implementation of dlfcn.h on Windows.
// Taken from http://www.refcode.net/2013/02/posix-dynamic-library-loading-calls-for.html
// And modified for its use with C++.

#pragma once

#define RTLD_LAZY       1
#define RTLD_NOW        2
#define RTLD_GLOBAL     3
#define RTLD_LOCAL      4

extern "C" {

    struct Dl_info {
        const char * dli_fname;
        void * dli_fbase;
        const char * dli_sname;
        void * dli_saddr;
    };

    void *dlopen(const char *file, int mode);
    void *dlsym(void *handle, const char *name);
    char *dlerror(void);
    int dlclose(void *handle);
    int dladdr(void *address, struct Dl_info *dlip);
}
