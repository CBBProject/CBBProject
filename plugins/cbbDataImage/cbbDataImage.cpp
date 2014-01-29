#include <cbbDataImage.h>

#include <Image.H>
#include <dtkCore/dtkAbstractDataFactory.h>
#include <dtkCore/dtkAbstractData_p.h>

// cbbDataImage implementation

cbbDataImage::cbbDataImage(): cbbAbstractImage(),image_(0) { }

cbbDataImage::~cbbDataImage() {
    if (image_!=0)
        delete image_;
    image_ = 0;
}

bool cbbDataImage::registered() {
    return dtkAbstractDataFactory::instance()->registerDataType(id,create,"cbbAbstractImage");
}

const char cbbDataImage::id[] = "cbbDataImage";
