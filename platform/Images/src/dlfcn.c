// Posix implementation of dlfcn.h on Windows.
// Taken from http://www.refcode.net/2013/02/posix-dynamic-library-loading-calls-for.html
// And modified for its use with C++.

#include <windows.h>
#include <string.h>
#include <stdio.h>

#include "dlfcn.h"

/// dlopen: opens a dll file

void* dlopen(const char* module_name,int mode) {
    UINT err_code;
    HINSTANCE dll_handle;
  
    err_code = SetErrorMode(SEM_FAILCRITICALERRORS);
    dll_handle = LoadLibrary(module_name);
    if (!dll_handle) {
        dll_handle = LoadLibraryEx(module_name,NULL,0);
		if (!dll_handle)
			return (void *) dll_handle;
    }

	//  Clear the last error.

    SetLastError(0); 
    return (void *) dll_handle;
}

/// dlerror: reports last error occured

char* dlerror() {

    int len, error_code;
    static char errstr[128];
        
    if ((error_code = GetLastError())==0)
        return ((char *)0);

	/*POSIX dlerror call needs to report no error(null) 
	when it is called 2nd time consequently, so clear error*/
    SetLastError(0); 

	/*format the error string*/
    len = sprintf(errstr, "Error <%d>: ", error_code);
    len += FormatMessage( 
            FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            error_code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /* Default language */
            (LPTSTR) errstr + len,
            sizeof(errstr) - len,
            NULL 
        );
    
    //  Replace \r\n

    if (len>1 && errstr[len-2]=='\r' && errstr[len-1]=='\n') {
        if (len>2 && errstr[len-3]=='.')
            len--;
    }

    return errstr;
}

/// dlclose: closes handle

int dlclose(void* handle) {

	//  POSIX call returns zero for success, non-zero for failure.

	return (!FreeLibrary(handle)); 
}

/// dlsym: get a symbol from dll

void* dlsym(void* handle,const char* name) {
    return GetProcAddress(handle, name);
}

/// dladdr: translates an address to symbolic information.

int dladdr(void *address, struct Dl_info *dlip) {
	//  Right now windows doesn't support this.
	return 0;
}
