#include <typeinfo>
#include <cbbAbstractImage.h>
#include <cbbAbstractImage_p.h>

cbbAbstractImage::cbbAbstractImage(): dtkAbstractData() { }

void cbbAbstractImage::setImage(const std::string&) {
    DTK_DEFAULT_IMPLEMENTATION;
}

const Images::Image* cbbAbstractImage::image() const {
    DTK_DEFAULT_IMPLEMENTATION;
    return 0;
}
