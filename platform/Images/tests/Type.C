#include <iostream>
#include <exception>

#include <Image.H>
#include <Images/Polymorphic.H>

using namespace Images;

// The user writes any function template depending upon the Pixel type
// and the Dimension of the image.

template <typename Pixel,unsigned Dimension>
void Main(void*,int argc,char **argv)
{
    // Here comes the code, since the type of the image is known.
    // for example:

    std::cout << "type = " << typeid(Pixel).name() << " and dimension=" << Dimension << std::endl; 
}

// Declare  a handle, MainHandle, to that function template.

DeclarePolymorphicHandle(Main,void (*prototype)(void*,int,char**));

int main(int argc,char **argv) try
{
    using namespace Polymorphic;

    // Read images from the disk.

    Image *image1 = new Image2D<double>;
    Image *image2 = new Image2D<char>;

    // What are the images that should be accepted in this program ?

    // typedef Polymorphic::Types<UnsignedIntegers,Floats>::Dimensions<1, 2, 3> Accepted;
    typedef Polymorphic::Types<char,unsigned char,short,unsigned short,int,unsigned,float,double>::Dimensions<1,2,3> Accepted;

    // Create a template instanciator for the effective type of image1.

    Accepted Instanciate1(image1->pixel_id(),image1->dimension());

    // Instanciate correctly the Main function template and call it.

    (*Instanciate1.Function<MainHandle>())(image1,argc,argv); // OK.

    // Try also with the second image.

    Accepted Instanciate2(image2->pixel_id(),image2->dimension());
    (*Instanciate2.Function<MainHandle>())(image1,argc,argv); // no OK, exception thrown.
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
