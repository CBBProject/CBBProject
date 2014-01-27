#pragma once

#include <algorithm>
#include <Images/Defs.H>
#include <Images/Index.H>
#include <Images/Shape.H>

namespace Images {

    template <Coord N>
    struct ConstantBounds {

        ConstantBounds() { }

        Coord  operator[](const unsigned) const { return N; } 
    };

    template <unsigned N>
    struct ImageBounds {

        ImageBounds(const Shape<N>& shape) {
            for (unsigned i=1;i<=N;++i)
                bounds[i-1] = shape(i)-1;
        }

        Coord operator[](const unsigned i) const { return bounds[i]; }

        const Coord* begin() const { return bounds;   }
        const Coord* end()   const { return bounds+N; }

    private: 

        Coord bounds[N];
    };

    template <unsigned N>
    struct Bounds {

        template <typename ARRAY>
        Bounds(const ARRAY& bounds) { std::copy(bounds.begin(),bounds.end(),tab); }

        Bounds(const Coord bounds[N]) { std::copy(bounds,bounds+N,tab); }

        template <Coord M>
        Bounds(const ConstantBounds<M>& bounds) {
            for (unsigned i=0;i<N;++i)
                (*this)[i] = bounds[i];
        }

        template <typename ARRAY>
        Bounds(const int dim[],const ARRAY& bounds) {
            for (unsigned i=0;i<N;++i)
                (*this)[i] = bounds[dim[i]];
        }

        Coord& operator[](const unsigned i)       { return tab[i]; }
        Coord  operator[](const unsigned i) const { return tab[i]; }

        const Coord* begin() const { return tab;   }
        const Coord* end()   const { return tab+N; }

    private: 

        Coord tab[N];
    };

    template <unsigned N,typename UBOUNDS=Bounds<N>,typename LBOUNDS=ConstantBounds<0> >
    class MultiDimCounter {

        typedef Images::Index<N> MultiIndex;

    public:

        typedef Bounds<N>       BOUNDS;
        typedef MultiDimCounter self;
        typedef MultiDimCounter value_type;
        typedef int             difference_type;

        MultiDimCounter(const UBOUNDS& hi):     UBound(hi),current(LBound),valid(true) {                 }
        MultiDimCounter(const UBOUNDS& hi,int): UBound(hi),current(UBound),valid(true) { ++current[N-1]; }

        MultiDimCounter(const LBOUNDS& low,const UBOUNDS& hi):    LBound(low),UBound(hi),current(LBound),valid(true) { }
        MultiDimCounter(const LBOUNDS& low,const UBOUNDS& hi,int):LBound(low),UBound(hi),current(UBound),valid(true) { ++current[N-1]; }

        void operator++() {
            for (unsigned i=0;i<N;++i) {
                if (++current[i]<=UBound[i])
                    return;
                current[i] = LBound[i];
            }
            current = UBound;
            ++current[N-1];
        }
        
        void operator--() {
            for (unsigned i=0;i<N;++i) {
                if (--current[i]>=LBound[i])
                    return;
                current[i] = UBound[i];
            }
            current = LBound;
            --current[0];
        }

        self& operator+=(const difference_type n) { add(n);  return *this; }
        self& operator-=(const difference_type n) { add(-n); return *this; }

        self& operator+=(const int Offset[N]) {
            for (unsigned i=0;i<N;++i)
                current[i] += Offset[i];
        }

        self& operator-=(const int Offset[N]) {
            for (unsigned i=0;i<N;++i)
                current[i] -= Offset[i];
        }

        self operator+(const difference_type n) const {
            self res = *this;
            res += n;
            return res;
        }

        self operator- (const difference_type n) const {
            self res = *this;
            res -= n;
            return res;
        }

        const self& BestIndex() const { return *this; }
        
        MultiIndex position() const { return MultiIndex(current.begin()); }

        bool operator==(const self& it) const {
            for (int i=0;i<N;++i)
                if (current[i]!=it.current[i])
                    return false;
            return true;
        }

        bool operator!=(const self& it) const {
            for (unsigned i=0;i<N;++i)
                if (current[i]!=it.current[i])
                    return true;
            return false;
        }

        //  Make the non const version private with Images being friend ??

        Coord& operator[](const unsigned i)       { return current[i];   }
        Coord  operator[](const unsigned i) const { return current[i];   }
        Coord& operator()(const unsigned i)       { return current[i-1]; }
        Coord  operator()(const unsigned i) const { return current[i-1]; }

        const self neighbor(const unsigned i,const int offset) const {
            self res = *this;
            res.current[i] += offset;
            if (res.current[i]>UBound[i]) {
                res.current[i] = UBound[i];
                res.valid = false;
            } else if (res.current[i]<LBound[i]) {
                res.current[i] = LBound[i];
                res.valid = false;
            }
            return res; 
        }

        const bool is_valid() const { return valid && (current[0]>=LBound[0]) && (current[N-1]<=UBound[N-1]); }
        
    private:

        void add(int n) {

            for (unsigned i=0;i<N;++i) {
                const int ni = current[i]-LBound[i]+n;
                current[i] = LBound[i]+ni%(UBound[i]-LBound[i]+1);
                if ((n = ni/(UBound[i]-LBound[i]+1))==0)
                    return;
            }

            if (n>0) {
                current = UBound;
                ++current[N-1]; 
            } else if (n<0) {
                current = LBound;
                --current[0];
            }
        }

        LBOUNDS LBound;  //  Array of lower bounds.
        UBOUNDS UBound;  //  Array of upper bounds.
        BOUNDS  current; //  The current index.
        bool    valid;   //  A boolean indicating whether the counter is in a valid state.
    };

    template <unsigned N,typename LBOUNDS,typename UBOUNDS>
    std::ostream& operator<<(std::ostream& os,const MultiDimCounter<N,LBOUNDS,UBOUNDS>& mdc) {
        for (unsigned i=0;i<N-1;++i)
            os << mdc[i] << ' ';
        return os << mdc[N-1];
    }
}
