#pragma once

namespace Iterators {

    //  Markers used to delay the begin/end computation at iterator construction.

    template <typename T>
    struct BEGIN {
        BEGIN(T t): val(t) { }
        T val;
    };

    template <typename T>
    struct END {
        END(T t):val(t) { }
        T val;
    };

    //  Remove this version ???? TODO
    //  Currently not possible as image uses this (and it is not clear it can be changed).
    //  Maybe we should keep both versions and find better names...

    template <typename Iterator>
    struct generic: public Iterator {
        typedef Iterator Base;
        template <typename T> generic(BEGIN<T> b): Base(b) { }
        template <typename T> generic(END<T> e):   Base(e) { }

        //  Check that this does not incur any problems. (TODO)

        generic(const Base& b): Base(b) { }

        template <typename T>
        bool operator==(const END<T>& e) const {
            const generic end(e);
            return Base::operator==(end);
        }
        template <typename T>
        bool operator!=(const END<T>& e) const {
            const generic end(e);
            return Base::operator!=(end);
        }

        //  reverse iterators also want comparison with begin.

        template <typename T>
        bool operator==(const BEGIN<T>& b) const {
            const generic beg(b);
            return Base::operator==(beg);
        }
        template <typename T>
        bool operator!=(const BEGIN<T>& b) const {
            const generic beg(b);
            return Base::operator!=(beg);
        }

        using Base::operator==;
        using Base::operator!=;
    };

    template <typename Iterator,typename Container>
    struct generic_new: public Iterator {
        typedef Iterator Base;
        template <typename T> generic_new(BEGIN<T> b): Base(b.val.template begin<Container>()) { }
        template <typename T> generic_new(END<T> e):   Base(e.val.template end<Container>())   { }

        //  Check that this does not incur any problems. (TODO)

        generic_new(const Base& b): Base(b) { }

        template <typename T>
        bool operator==(const END<T>& e) const {
            const generic_new end(e);
            return *this==end;
        }
        template <typename T>
        bool operator!=(const END<T>& e) const {
            const generic_new end(e);
            return *this!=end;
        }
    };

    template <typename Iterator,typename Container>
    bool operator!=(const generic_new<Iterator,Container>& it1,const generic_new<Iterator,Container>& it2) {
        return static_cast<const Iterator&>(it1)!=static_cast<const Iterator&>(it2);
    }
}
