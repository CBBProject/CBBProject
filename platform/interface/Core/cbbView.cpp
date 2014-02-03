#include <cbbAbstractImage.h>

#include <Image.H>
#include <Images/Maps.H>

#include <cbbView.h>

void cbbView::addImage(const cbbAbstractImage* I) {

    if (!I)
        return;

    std::cerr << "HERE" << std::endl;
    const Images::Image* image = I->image();
    std::cerr << "HERE 1" << std::endl;
    if (image && imageViewer) {
        std::cerr << "HERE 12" << std::endl;
        imageViewer->open(image);
    }
    std::cerr << "HERE 2" << std::endl;
}
