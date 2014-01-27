#include <fstream>
#include <Image.H>

//  Test 3D image conversion.

//  Example: ./Convert3D images/irm.float.inr C ### images/irm.inr
//  Example: ./Convert3D images/irm.float.inr s ### results/irm.short-le.inr

void Usage(const char* name) { std::cerr << "Usage: " << name << "-n float_image [c|s|S]" << std::endl; }

//  Test image conversion pixel wise by producing a tridimensional float INRIMAGE (a ramp)
//  and convert it before outputting it on stdout.

int
main(int argc,char* argv[]) try
{
    using namespace Images;

    if ((argc!=2) && (argc!=3)) {
        Usage(argv[0]);
        return 1;
    }

    //  Read the image.

    Image3D<float> image;
    std::ifstream ifs(argv[1]);

    ifs >> image;

    const char type = (argc==3) ? argv[2][0] : 's';
    
    switch (type) {
        case 'C': {
            const float minval = min(image);
            const float maxval = max(image);
            const float factor = (minval==maxval) ? 128.0/maxval : 255.0/(maxval-minval);
            std::cout << static_cast<Image3D<unsigned char> >(factor*(image-minval));
            break;
        }
        case 'S': {
            const float minval = min(image);
            const float maxval = max(image);
            const float factor = (minval==maxval) ? 32767.0/maxval : 65535.0/(maxval-minval);
            std::cout << static_cast<Image3D<unsigned short> >(factor*(image-minval));
            break;
        }
        case 'c':
            std::cout << static_cast<Image3D<unsigned char> >(image);
            break;
        case 's':
        default:
            std::cout << static_cast<Image3D<unsigned short> >(image);
    }

    return 0;
}
catch(const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
