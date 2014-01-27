#ifndef VERBOSE_H
#define VERBOSE_H

#include <iostream>

namespace io_utils {

    struct VerboseStream {

        VerboseStream(const bool verb,std::ostream& s=std::cerr): verbose(verb),os(s) { }

        const bool verbose;
        std::ostream& os;
    };

    template <typename T>
    inline VerboseStream&
    operator<<(VerboseStream& vs,const T& t) {
        if (vs.verbose)	
            vs.os << t;
        return vs;
    }

    //  This is ugly but replacing std::basic_ostream<char> with a template parameter fails
    //  to match and the net result is that manipulators such as endl cause errors !!!

    inline VerboseStream&
    operator<<(VerboseStream& vs,std::basic_ostream<char>& (*pf)(std::basic_ostream<char>&)) {
        if (vs.verbose)	
            vs.os << pf;
        return vs;
    }
}

#endif  //  !VERBOSE_H
