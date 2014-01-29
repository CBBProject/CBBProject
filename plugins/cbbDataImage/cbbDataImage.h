#pragma once

#include <fstream>
#include <Image.H>
#include <Core/cbbAbstractImage.h>
#include "cbbDataImagePluginExport.h"

// cbbDataImage interface

class CBBDATAIMAGEPLUGIN_EXPORT cbbDataImage: public cbbAbstractImage {
public:

    cbbDataImage();
    virtual ~cbbDataImage();

public:

    virtual QString description() const { return "Image data"; }
    virtual QString identifier()  const { return id;           }

    static bool registered();
    
    void setImage(const std::string& filename) {
        std::cerr << "Setting Image." << std::endl;
        if (image_)
            delete image_;
        std::cerr << "Reading image " <<  filename << std::endl;
        std::ifstream ifs(filename.c_str(),std::ios::binary);
        ifs >> image_;
        qDebug() << Q_FUNC_INFO << "Done";
    }

    void setData(void *data) {
        std::cerr << "Setting data." << std::endl;
        const char* image_name = static_cast<const char*>(data);
        setImage(image_name);
        qDebug() << Q_FUNC_INFO << "Done";
    }

    void* data() { return image_; }

private:
    static const char id[];

    static dtkAbstractData* create() { return new cbbDataImage(); }

    Images::Image* image_;
};
