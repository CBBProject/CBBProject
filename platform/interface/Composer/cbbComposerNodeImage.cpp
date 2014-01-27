#include <cbbComposerNodeImage.h>
#include <Core/cbbAbstractImage.h>

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

#include <dtkCore/dtkAbstractDataFactory.h>
#include <dtkCore/dtkAbstractData.h>

// /////////////////////////////////////////////////////////////////
// cbbComposerNodeImagePrivate declaration
// /////////////////////////////////////////////////////////////////

class cbbComposerNodeImagePrivate {
public:
    cbbAbstractImage* image;

    dtkComposerTransmitterReceiver<QString>         receiver_string;
    dtkComposerTransmitterEmitter<cbbAbstractImage> emitter_image;
};

// /////////////////////////////////////////////////////////////////
// cbbComposerNodeImage implementation
// /////////////////////////////////////////////////////////////////

cbbComposerNodeImage::cbbComposerNodeImage(): dtkComposerNodeLeaf(), d(new cbbComposerNodeImagePrivate) {
    d->image = NULL;

    this->appendReceiver(&(d->receiver_string));
    this->appendEmitter(&(d->emitter_image));
}

cbbComposerNodeImage::~cbbComposerNodeImage() {
    delete d;
    d = NULL;
}
   
QString cbbComposerNodeImage::type() {
    return "cbbDataImage";
}

QString cbbComposerNodeImage::titleHint() {
    return "Image";
}

void cbbComposerNodeImage::run() {
    if (!d->image) {
      d->image = dynamic_cast<cbbAbstractImage *>(dtkAbstractDataFactory::instance()->create("cbbDataImage"));
    }
    
    if (!d->receiver_string.isEmpty()) {
        const QString* file = d->receiver_string.data();
        d->image->setImage(file->toStdString());
	qDebug() << Q_FUNC_INFO << "Done" << 1;
    }

    d->emitter_image.setData(d->image);

    qDebug() << Q_FUNC_INFO << "Done" << 2;
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
