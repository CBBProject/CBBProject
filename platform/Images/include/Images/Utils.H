#pragma once

#include <iostream>
#include <sstream>
#include <algorithm>
#include <Images/Defs.H>
#include <Utils/IOUtils.H>

namespace Images {
    namespace Utils {

        template <typename TYPE=Dimension>
        class ImageSize {
        public:

            typedef TYPE Type;

            ImageSize(): dim(0),sz(0) { }

            ImageSize(const Dimension d): dim(d),sz(new Type[dim]) { }

            ImageSize(const Dimension d,const Type s[]): dim(d),sz(new Type[dim]) {
                std::copy(&s[0],&s[dim],&sz[0]);
            }

            ImageSize(const ImageSize& ims): dim(ims.dim),sz(new Type[dim]) {
                std::copy(&ims.sz[0],&ims.sz[dim],&sz[0]);
            }

            ImageSize(const std::string& str): dim(0),sz(0) {
                std::istringstream iss(str.c_str());
                iss >> *this;
            }

            ~ImageSize() {
                if (sz)
                    delete[] sz;
            }

            ImageSize& operator=(const ImageSize& ims) {
                this->~ImageSize();
                dim = ims.dim;
                sz  = new Type[dim];
                std::copy(&ims.sz[0],&ims.sz[dim],&sz[0]);
                return *this;
            }

            Dimension dimension() const { return dim; }

            const Dimension* size() const { return sz; }

            template <unsigned DIM>
            Shape<DIM> shape() const {
                if (dim!=DIM)
                    throw BadSizeSpec(DIM);
                return Shape<DIM>(sz);    
            }

            Type& operator[](const Dimension i)       { return sz[i]; }
            Type  operator[](const Dimension i) const { return sz[i]; }

            template <typename T> friend std::istream& operator>>(std::istream&,ImageSize<T>&);

            template <typename T>
            T scale(const T& v) const {
                T res = v;
                typename T::iterator it = res.begin();
                for (Dimension i=0;i<dim;++i,++it)
                    *it *= sz[i];
                return res;
            }

        private:

            Dimension  dim;
            Type*      sz;
        };

        template <typename TYPE>
        std::istream& operator>>(std::istream& is,ImageSize<TYPE>& s) {
            std::vector<TYPE> v;
            TYPE i;
            
            bool cont = true;
            while (cont) {
                is >> i >> io_utils::match_optional("x",cont);
                v.push_back(i);
            }

            if (s.dim!=0 && s.dim!=v.size()) {
                s.dim = 0;
                delete[] s.sz;
            }

            if (s.dim==0) {
                s.dim = v.size();
                s.sz = new TYPE[s.dim];
            }

            std::copy(v.begin(),v.end(),s.sz);

            return is;
        }

        template <typename TYPE>
        std::ostream& operator<<(std::ostream& os,const ImageSize<TYPE>& s) {
            os << s[0];
            for (Dimension i=1;i<s.dimension();++i)
                os << 'x' << s[i];
            return os;
        }
    }
}
