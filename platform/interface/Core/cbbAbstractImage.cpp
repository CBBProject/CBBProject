#include <typeinfo>
#include <cbbAbstractImage.h>
#include <cbbAbstractImage_p.h>

cbbAbstractImage::cbbAbstractImage(): dtkAbstractData() { 
    std::cerr << "Creating cbbAbstractImage with " << "me" << std::endl;
}

void cbbAbstractImage::setImage(const std::string&) {
    std::cerr << "Default SetImage" << std::endl;
    DTK_DEFAULT_IMPLEMENTATION;
}

const Images::Image* cbbAbstractImage::image() const {
    DTK_DEFAULT_IMPLEMENTATION;
    return 0;
}
