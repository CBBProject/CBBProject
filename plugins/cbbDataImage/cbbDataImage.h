#pragma once

#include <cbbExport.h>

#include <fstream>
#include <Image.H>
#include <Core/cbbAbstractImage.h>

// cbbDataImage interface

class CBB_WINDOWS_EXPORT cbbDataImage: public cbbAbstractImage {
public:

    cbbDataImage();
    virtual ~cbbDataImage();

public:

    virtual QString description() const { return "Image data"; }
    virtual QString identifier()  const { return id;           }

    static bool registered();
    
    void setImage(const std::string& filename) {
        if (image_) {
            delete image_;
            image_ = 0;
        }
        std::ifstream ifs(filename.c_str(),std::ios::binary);
        ifs >> image_;
    }

    void setData(void *data) {
        const char* image_name = static_cast<const char*>(data);
        setImage(image_name);
    }

    Images::Image* image() const { std::cerr << "Getting the image !!" << std::endl; return image_; }
    void* data()  { std::cerr << "Getting the image !!" << std::endl; return image_; }

private:

    static const char id[];

    static dtkAbstractData* create() { return new cbbDataImage(); }

    Images::Image* image_;
};
