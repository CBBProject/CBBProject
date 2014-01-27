#pragma once

#include <cbbCoreExport.h>
#include <dtkCore/dtkAbstractData.h>

namespace Images {
    class Image;
}

class CBBCORE_EXPORT cbbAbstractImage: public dtkAbstractData {
    Q_OBJECT

public:
    typedef unsigned Dimension;

             cbbAbstractImage();
    virtual ~cbbAbstractImage() {}

    virtual void setImage(const std::string&);
    virtual const Images::Image* image() const;
};

Q_DECLARE_METATYPE(cbbAbstractImage)
Q_DECLARE_METATYPE(cbbAbstractImage *)
