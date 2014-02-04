#include <cbbAbstractImage.h>

#include <Image.H>
#include <Images/Maps.H>

#include <cbbView.h>

void cbbView::addImage(const cbbAbstractImage* I) {

    if (!I)
        return;

    const Images::Image* image = I->image();
    if (image && imageViewer)
        imageViewer->open(image);
}
