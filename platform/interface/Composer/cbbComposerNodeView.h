#pragma once

#include <QtCore>

#include <cbbComposerExport.h>
#include <dtkComposer/dtkComposerNodeLeafView.h>

class cbbComposerNodeViewPrivate;

class CBBCOMPOSER_EXPORT cbbComposerNodeView : public dtkComposerNodeLeafView {
public:
     cbbComposerNodeView();
    ~cbbComposerNodeView();

    void run();

    QString type();
    QString titleHint();

    QString  inputLabelHint(const int port);
    QString outputLabelHint(const int port);

    virtual bool isAbstractView()      const { return false;           }
    virtual QString abstractViewType() const { return QString("view"); }

private:
    cbbComposerNodeViewPrivate *d;
};
