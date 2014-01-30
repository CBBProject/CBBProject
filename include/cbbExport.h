//  Dealing with windows exports

#pragma once

#ifdef WIN32
    #ifdef CBB_WINDOWS_EXPORTS
        #define CBB_WINDOWS_EXPORT __declspec(dllexport) 
    #else
        #define CBB_WINDOWS_EXPORT __declspec(dllimport) 
    #endif
#else
    #define CBB_WINDOWS_EXPORT
#endif
