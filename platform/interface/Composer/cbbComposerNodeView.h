#pragma once

#include <QtCore>

#include <cbbComposerExport.h>
#include <dtkComposer/dtkComposerNodeLeafView.h>

class cbbComposerNodeViewPrivate;

class CBBCOMPOSER_EXPORT cbbComposerNodeView : public dtkComposerNodeLeaf {
public:
     cbbComposerNodeView();
    ~cbbComposerNodeView();

    void run();

    QString type();
    QString titleHint();

    QString  inputLabelHint(const int port);
    QString outputLabelHint(const int port);

private:
    cbbComposerNodeViewPrivate *d;
};
