#include <fstream>
#include <Image.H>

//  Example: ./FormatConverter images/irm.inr irm.inr5 ## results/Null.output results/irm.inr5
//  Example: ./FormatConverter images/irm.inr irm.vti  ## results/Null.output results/irm.vti

int
main(const int argc,const char* argv[]) try {

    using namespace Images;

    Image* image;

    bool FromSuffixForInput = false;
    if (argc==4) {
        if (std::string(argv[1])=="--fromSuffix") {
            FromSuffixForInput = true;
            ++argv;
        }
    }

    std::ifstream ifs(argv[1],std::ios::binary);
    if (FromSuffixForInput)
        ifs >> Images::format(argv[1],Images::format::FromSuffix);
    ifs >> image;

    std::ofstream ofs(argv[2],std::ios::binary);
    ofs << Images::format(argv[2],Images::format::FromSuffix) << image;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
