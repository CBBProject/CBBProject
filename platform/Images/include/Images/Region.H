#pragma once

#include <stack>
#include <Images/Neighborood.H>
#include <Schemes/Growing.H>
#include <Schemes/Conditions.H>

namespace Images {

    namespace Tags {

        //  Marker tags to be used in the adder.

        struct Inside   { };
        struct Interior { };
        struct Bound    { };
        struct Exterior { };
        struct Outside  { };
    }

    typedef enum { Outside, Scheduled, Exterior, Bound, Inside, Interior } PixelStatus;

    template <template <typename T> class Image>
    class Region: public Image<PixelStatus> {

        typedef Image<PixelStatus> base;

    public:

        Region(const typename base::Shape& s): base(s) { base::operator=(Outside); }

        //  Some simple region metrics.

        unsigned volume() const {
            unsigned vol = 0;
            for (typename Image<PixelStatus>::template const_iterator<pixel> i=this->begin();i!=this->end();++i)
                if (*i>=Bound)
                    ++vol;
            return vol;
        }

        unsigned surface() const {
            unsigned surf = 0;
            for (typename Image<PixelStatus>::template const_iterator<pixel> i=this->begin();i!=this->end();++i)
                if (*i==Bound)
                    ++surf;
            return surf;
        }
    };

    //  This is a base class for Adders.
    //  Adders must define at least (eventually empty) methods:
    //
    //      template <typename Iterator> void operator()(const Iterator&,Inside);
    //      template <typename Iterator> void operator()(const Iterator&,Interior);
    //      template <typename Iterator> void operator()(const Iterator&,Bound);
    //      template <typename Iterator> void operator()(const Iterator&,Exterior);
    //      template <typename Iterator> bool select(const Iterator&);
    //
    //  The default here just does nothing and selects the whole image.

    template <template <typename T> class Image,typename Iterator,typename PAdder>
    class BasicAdder {

        typedef typename Image<PixelStatus>::Shape Shape;

    public:

        BasicAdder(PAdder& add,const Shape& s): adder(add),status(s) { }

        //using namespace Tags;

        void add(const Iterator& P,const Tags::Inside)   { status(P) = Inside;   adder.add(P,Tags::Inside());   }
        void add(const Iterator& P,const Tags::Interior) { status(P) = Interior; adder.add(P,Tags::Interior()); }
        void add(const Iterator& P,const Tags::Bound)    { status(P) = Bound;    adder.add(P,Tags::Bound());    }
        void add(const Iterator& P,const Tags::Exterior) { status(P) = Exterior; adder.add(P,Tags::Exterior()); }
        void add(const Iterator& P,const Tags::Outside)  { status(P) = Outside;  adder.add(P,Tags::Outside());  }

        bool select(const Iterator& P) const { return adder.select(P); }

        PAdder&       adder;
        Region<Image> status;
    };

    //  This adder maintains 3 sets of pixels (besides the inside and outside regions).
    //      - boundary: the pixels that are at the frontier between the inside and outside regions.
    //      - interior: the pixels of the inside region that are neighbors of the pixel in the boundary region.
    //      - exterior: the pixels of the outside region that are neighbors of the pixel in the boundary region.
    //
    //  The shrink and expand methods simply shift these various sets and update the regions in accordance.
    //      - shrink: interior->boundary->exterior->outside
    //      - expand: exterior->boundary->interior->inside

    template <template <typename T> class Image,typename Iterator,typename PAdder>
    class MorphoAdder: public BasicAdder<Image,Iterator,PAdder> {

        typedef BasicAdder<Image,Iterator,PAdder> base;
        typedef typename Image<PixelStatus>::Shape Shape;

        struct IsNextToBound {

            IsNextToBound(const Image<PixelStatus>& I): Im(I) { }

            typedef bool value_type;
            bool operator()(const Iterator& P) const { return Im.InRange(P) && Im(P)==Outside; }

            const Image<PixelStatus>& Im;
        };

        template <typename TAG>
        struct ChangeToStatus {

            ChangeToStatus(const Image<PixelStatus>& I,const PixelStatus old,MorphoAdder& A): Im(I),old_value(old),adder(A) { }

            typedef Iterator value_type;
            void operator()(const Iterator& P) const {
                if (Im.InRange(P) && Im(P)==old_value)
                    adder.add(P,TAG());
            }

            const PixelStatus         old_value;
            const Image<PixelStatus>& Im;
                  MorphoAdder&        adder;
        };

    public:

        MorphoAdder(PAdder& adder,const Shape& s): base(adder,s) { }

        //using namespace Tags;

        void add(const Iterator& P,const Tags::Inside)   { base::add(P,Tags::Inside());                          }
        void add(const Iterator& P,const Tags::Interior) { base::add(P,Tags::Interior()); interior.push_back(P); }
        void add(const Iterator& P,const Tags::Bound)    { base::add(P,Tags::Bound());    boundary.push_back(P); }
        void add(const Iterator& P,const Tags::Exterior) { base::add(P,Tags::Exterior()); exterior.push_back(P); }
        void add(const Iterator& P,const Tags::Outside)  { base::add(P,Tags::Outside());                         }

        //  Simple morpho-math operations.

        void shrink();
        void expand();

    private:

        typedef std::vector<Iterator> PixelCollection;

        PixelCollection exterior;
        PixelCollection boundary;
        PixelCollection interior;
    };

    template <template <typename T> class Image,typename Iterator,typename PAdder>
    void MorphoAdder<Image,Iterator,PAdder>::shrink() {
        using Numerics::Schemes::Grid::ForAll;

        for (typename PixelCollection::iterator i=exterior.begin();i!=exterior.end();++i)
            add(*i,Tags::Outside());

        exterior = boundary;
        for (typename PixelCollection::iterator i=boundary.begin();i!=boundary.end();++i)
            add(*i,Tags::Exterior());

        boundary = interior;
        for (typename PixelCollection::iterator i=interior.begin();i!=interior.end();++i)
            add(*i,Tags::Bound());

        interior.clear();
        ChangeToStatus<Tags::Interior> InteriorP(base::status,Inside,*this);
        for (typename PixelCollection::iterator i=boundary.begin();i!=boundary.end();++i)
            ForAll(*i,base::status,InteriorP);
    }

    template <template <typename T> class Image,typename Iterator,typename PAdder>
    void MorphoAdder<Image,Iterator,PAdder>::expand() {
        using Numerics::Schemes::Grid::ForAll;

        for (typename PixelCollection::iterator i=interior.begin();i!=interior.end();++i)
            add(*i,Tags::Inside());

        interior = boundary;
        for (typename PixelCollection::iterator i=boundary.begin();i!=boundary.end();++i)
            add(*i,Tags::Interior());

        boundary = exterior;
        for (typename PixelCollection::iterator i=exterior.begin();i!=exterior.end();++i)
            add(*i,Tags::Bound());

        exterior.clear();
        ChangeToStatus<Tags::Exterior> ExteriorP(base::status,Outside,*this);
        for (typename PixelCollection::iterator i=boundary.begin();i!=boundary.end();++i)
            ForAll(*i,base::status,ExteriorP);
    }
    
    template <template <typename T> class Image,typename Pixel,typename Iterator,typename PAdder,
              template <template <typename T> class Image,typename Iterator,typename PAdder> class Adder = BasicAdder>
    class RegionGrower: private std::stack<Iterator>, public Adder<Image,Iterator,PAdder> {

        typedef std::stack<Iterator>         base;
        typedef Adder<Image,Iterator,PAdder> MAdder;
        typedef RegionGrower                 self;

    public:

        using MAdder::add;
        using MAdder::status;

        RegionGrower(const Image<Pixel>& I,PAdder& adder): base(),MAdder(adder,I.shape()),image(I) { }

        Region<Image>&
        operator()(const Iterator& firstPoint) {

            using Numerics::Schemes::Grid::Growing;

            push(firstPoint);
            while (!base::empty()) {
                const Iterator P = base::top();
                base::pop();
                border = false;
                Growing(P,image,*this);
                if (border)
                    add(P,Tags::Bound());
                else
                    add(P,Tags::Inside());
            }

            return this->status;
        }

    private:

        //  This method is designed for the Stacker combiner.

        friend class Numerics::Schemes::Combiners::Stacker<self>;

        typedef Iterator value_type;

        void push(const Iterator& i) {
            if (image.InRange(i)) {
                if (MAdder::select(i)) {
                    if (status(i)==Outside) {
                        status(i) = Scheduled;
                        base::push(i);
                    }
                } else {
                    add(i,Tags::Exterior());
                    border = true;
                }
            }
        }

        const Image<Pixel>& image;
        bool                border;
    };
}
