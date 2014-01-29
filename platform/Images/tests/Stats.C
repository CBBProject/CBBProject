#include <iostream>
#include <fstream>
#include <exception>
#include <Utils/IOUtils.H>
#include <Image.H>

//  Example: ./Stats images/bear.pgm

using namespace std;
using namespace Images;

//  The user writes any function template depending upon the Pixel type
//  and the Dimension of the image.

template <typename Pixel,unsigned Dimension>
void Stats(const Image* im)
{
    typedef typename ImageType<Dimension,Pixel>::type IMAGE;
    
    const IMAGE image = *reinterpret_cast<const IMAGE*>(im);

    std::cerr << "Min: " << static_cast<double>(min(image)) << " Max: " << static_cast<double>(max(image)) << std::endl;
}

//  Declare  a handle, MainHandle, to that function template.

DeclarePolymorphicHandle(Stats,void (*prototype)(const Image*));

void Usage(const char* name) { cerr << "Usage: " << name << " image" << endl; }

int
main(int argc,char *argv[]) try
{
    if (argc!=2) {
        Usage(argv[0]);
        return 1;
    }

    //  Read the images.

    ifstream ifs(argv[1],std::ios::binary);
    Image *image;
    ifs >> image;

    //  Authorized types for this program.

    //typedef Polymorphic::Types<Polymorphic::All>::Dimensions<1,2,3> Switch;
    typedef Polymorphic::Types<float,double,unsigned char,unsigned short,unsigned,char,short,int>::Dimensions<1,2,3> Switch;

    // Instanciate for the effective type of image1 (which is equal to type of image2).

    Switch item(image->pixel_id(),image->dimension());

    // Instanciate correctly the Main function template and call it.

    (*item.Function<StatsHandle>())(image);

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
