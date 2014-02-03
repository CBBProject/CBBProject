#pragma once

#include <QtCore>

#include <cbbExport.h>
#include <dtkComposer/dtkComposerNodeLeafView.h>
#include <Core/cbbView.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

class CBB_WINDOWS_EXPORT cbbComposerNodeView : public dtkComposerNodeLeafView {
public:

     cbbComposerNodeView();
    ~cbbComposerNodeView() { }

    void run();

    QString type();
    QString titleHint();

    QString  inputLabelHint(const int port);
    QString outputLabelHint(const int port);

    virtual bool isAbstractView()      const { return false;           }
    virtual QString abstractViewType() const { return QString("view"); }

private:

    cbbView* view;

    dtkComposerTransmitterReceiver<cbbAbstractImage> receiver_image;
};
