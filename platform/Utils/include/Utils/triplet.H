#ifndef TRIPLET_H
#define TRIPLET_H

#include <iostream>

//  This is superceded by std::tr1::tuple....
//  Soon to be removed....

namespace stdext {

    template <typename T1,typename T2,typename T3>
    struct triplet {

        typedef T1 first_type;
        typedef T2 second_type;
        typedef T3 third_type;

        T1 first;
        T2 second;
        T3 third;
     
        triplet():first(T1()),second(T2()),third(T3()) { }
        triplet(const T1& t1,const T2& t2,const T3& t3):first(t1),second(t2),third(t3) { }

        ~triplet() { }

        template <typename U1,typename U2,typename U3>
        triplet(const triplet<U1,U2,U3>& t):first(t.first),second(t.second),third(t.third) { }
    };

    template<typename T1,typename T2,typename T3>
    inline bool operator==(const stdext::triplet<T1,T2,T3>& x,const stdext::triplet<T1,T2,T3>& y) {
        return x.first==y.first && x.second==y.second && x.third==y.third;
    }

    template<typename T1,typename T2,typename T3>
    inline bool operator<(const stdext::triplet<T1,T2,T3>& x,const stdext::triplet<T1,T2,T3>& y) {
        return x.first<y.first ||
               (!(y.first<x.first) && x.second<y.second) ||
               (!(y.first<x.first) && !(y.second<x.second) && x.third<y.third);
    }

    template<typename T1,typename T2,typename T3>
    inline bool operator!=(const stdext::triplet<T1,T2,T3>& x,const stdext::triplet<T1,T2,T3>& y) { return !(x==y); }
    template<typename T1,typename T2,typename T3>
    inline bool operator>(const stdext::triplet<T1,T2,T3>& x,const stdext::triplet<T1,T2,T3>& y)  { return y<x;     }
    template<typename T1,typename T2,typename T3>
    inline bool operator<=(const stdext::triplet<T1,T2,T3>& x,const stdext::triplet<T1,T2,T3>& y) { return !(y<x);  }
    template<typename T1,typename T2,typename T3>
    inline bool operator>=(const stdext::triplet<T1,T2,T3>& x,const stdext::triplet<T1,T2,T3>& y) { return !(x<y);  }
}

template <typename T1,typename T2,typename T3>
inline std::istream&
operator>>(std::istream& is,stdext::triplet<T1,T2,T3>& t) {
    return is >> t.first >> t.second >> t.third;
}

template <typename T1,typename T2,typename T3>
inline std::ostream&
operator<<(std::ostream& os,const stdext::triplet<T1,T2,T3>& t) {
    return os << t.first << t.second  << t.third;
}

#endif  //  !TRIPLET_H
