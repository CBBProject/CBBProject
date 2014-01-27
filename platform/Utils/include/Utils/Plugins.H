#ifndef PLUGINS_H
#define PLUGINS_H

#include <list>

namespace Plugins {

    enum { FILENAME = 2 };

    struct plugin {

        plugin(const char*);

        const char* identity() const { return name; }

        void*       handle;
        const char* name;
    };

    struct plugins: public std::list<plugin> {
        plugins(const char*,const unsigned=0);
        ~plugins();
    };
}

#endif  //  ! PLUGINS_H
