#include <iostream>

#include <Image.H>
#include <Images/Region.H>
#include <ImageViewer.H>

//  Example: ./DisplayRegion images/scully.pgm 185 419 147

template <typename Image,typename Iterator>
class Adder: public Interfaces::Image {
public:

    Adder(const Image& I,const unsigned char l): plotter(I.dimx(),I.dimy()),image(I),limit(l) {
        plotter.draw(I);
    }
    ~Adder() { }

    bool isInside(const Iterator& P) const { return image(P)>=limit; }

    void add(const Iterator& P,Images::Tags::Inside)   { plotter.colorname("red");   plotter.draw(P); }
    void add(const Iterator& P,Images::Tags::Interior) { plotter.colorname("cyan");  plotter.draw(P); }
    void add(const Iterator& P,Images::Tags::Bound)    { plotter.colorname("green"); plotter.draw(P); }
    void add(const Iterator& P,Images::Tags::Exterior) { plotter.colorname("blue");  plotter.draw(P); }
    void add(const Iterator& P,Images::Tags::Outside)  {                                              }

private:

    typedef Figures::Plotters::Image2D<XPlotter> Plotter;

    Plotter         plotter;
    const Image&    image;
    const unsigned  limit;
};

int
main(int argc,char* argv[]) try
{
    using namespace Figures;
    using namespace Images;
    using namespace std;

    typedef Image2D<unsigned char>  Image;
    typedef Image::iterator<domain> iterator;
    typedef Adder<Image,iterator>   Adder;

    const Image image(argv[1]);

    const iterator start = image.position(atoi(argv[2]),atoi(argv[3]));

    Adder adder(image,atoi(argv[4]));
    Images::RegionGrower<Image2D,unsigned char,iterator,Adder,MorphoAdder> region(image,adder);

    region(start);
    for (unsigned i=0;i<10;++i)
        region.expand();
    for (unsigned i=0;i<10;++i)
        region.shrink();

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
