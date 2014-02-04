#include <cbbComposerNodeImage.h>

#include <dtkCore/dtkAbstractDataFactory.h>
#include <dtkCore/dtkAbstractData.h>

// /////////////////////////////////////////////////////////////////
// cbbComposerNodeImage implementation
// /////////////////////////////////////////////////////////////////

cbbComposerNodeImage::cbbComposerNodeImage(): dtkComposerNodeLeaf() {
    image = NULL;

    appendReceiver(&(receiver_string));
    appendEmitter(&(emitter_image));
}

QString cbbComposerNodeImage::type() {
    return "cbbDataImage";
}

QString cbbComposerNodeImage::titleHint() {
    return "Image";
}

void cbbComposerNodeImage::run() {
    if (!image)
        image = dynamic_cast<cbbAbstractImage *>(dtkAbstractDataFactory::instance()->create("cbbDataImage"));
    
    if (!receiver_string.isEmpty()) {
        const QString* file = receiver_string.data();
        image->setImage(file->toStdString());
    }

    emitter_image.setData(image);
}

QString cbbComposerNodeImage::inputLabelHint(const int port) {
    switch (port) {
    case 0:
        return "file";
    default:
        return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString cbbComposerNodeImage::outputLabelHint(const int port) {
    switch (port) {
    case 0:
        return "image";
    default:
        return dtkComposerNodeLeaf::outputLabelHint(port);
    }
}
