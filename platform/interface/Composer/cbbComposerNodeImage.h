#pragma once

#include <QtCore>

#include <cbbExport.h>
#include <Core/cbbAbstractImage.h>
#include <dtkComposer/dtkComposerNodeLeaf.h>
#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

class CBB_WINDOWS_EXPORT cbbComposerNodeImage: public dtkComposerNodeLeaf {
public:

     cbbComposerNodeImage();
    ~cbbComposerNodeImage() { }

    void run();

    QString type();
    QString titleHint();

    QString  inputLabelHint(const int port);
    QString outputLabelHint(const int port);

private:

    cbbAbstractImage* image;

    dtkComposerTransmitterReceiver<QString>         receiver_string;
    dtkComposerTransmitterEmitter<cbbAbstractImage> emitter_image;
};
