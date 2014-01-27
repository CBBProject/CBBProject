#include <iostream>
#include <fstream>
#include <exception>

#include <Image.H>

//  Example: ./Shift images/irm.float.inr 50 shifted_irm.inr ### Null.output results/shifted_irm.inr

int
main(int argc,char *argv[]) try
{
    using namespace Images;

    const Image3D<float> image1(argv[1]);
    const double shiftvalue = atof(argv[2]);

    const Image3D<float> res = image1-shiftvalue;

    std::ofstream ofs(argv[3]);
    ofs << res;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
