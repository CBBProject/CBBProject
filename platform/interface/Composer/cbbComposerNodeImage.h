#pragma once

#include <QtCore>

#include <cbbComposerExport.h>
#include <dtkComposer/dtkComposerNodeLeaf.h>

class cbbComposerNodeImagePrivate;

class CBBCOMPOSER_EXPORT cbbComposerNodeImage: public dtkComposerNodeLeaf {
public:
     cbbComposerNodeImage();
    ~cbbComposerNodeImage();

    void run();

    QString type();
    QString titleHint();

    QString  inputLabelHint(const int port);
    QString outputLabelHint(const int port);

private:
    cbbComposerNodeImagePrivate* d;
};
