#pragma once

#include <ios>
#include <stdexcept>

namespace io_utils {

    //  IO manipulations for error reporting and efficiency.

    template <typename STREAM>
    struct IOInit {

        typedef std::ios_base::iostate iostate;

        IOInit(STREAM& s): ios(s),sync(std::ios::sync_with_stdio(false)), state(ios.exceptions()) {
            ios.exceptions(state|std::ios_base::badbit|std::ios_base::eofbit|std::ios_base::failbit);
        }

        IOInit(STREAM& s,const iostate& mask): ios(s),sync(std::ios::sync_with_stdio(false)), state(ios.exceptions()) {
            ios.exceptions(state|mask);
        }
        
        ~IOInit() {
            ios.exceptions(state);
            std::ios::sync_with_stdio(sync);
        }

        STREAM& ios;

        const bool    sync;
        const iostate state;
    };
}
