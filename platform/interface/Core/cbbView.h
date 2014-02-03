#pragma once

#include <cbbExport.h>
#include <dtkCore/dtkAbstractView.h>
#include <Images/ImageViewer.H>

class cbbAbstractImage;

class CBB_WINDOWS_EXPORT cbbView: public dtkAbstractView {

    Q_OBJECT

public:

    cbbView(): imageViewer(new ImageViewer) { }
    virtual ~cbbView() { delete imageViewer; }

    void addImage(const cbbAbstractImage* image);

    QWidget* widget() { return imageViewer; }

    static dtkAbstractView* create() { return new cbbView; }

private:
    
    ImageViewer* imageViewer;
};

Q_DECLARE_METATYPE(cbbView)
Q_DECLARE_METATYPE(cbbView*)
