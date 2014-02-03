#include <cbbComposerNodeView.h>

#include <Core/cbbAbstractImage.h>
#include <Core/cbbView.h>

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

#include <dtkCore/dtkAbstractViewFactory.h>
#include <dtkCore/dtkAbstractView.h>

#include <dtkMath/dtkVector.h>

// /////////////////////////////////////////////////////////////////
// cbbComposerNodeView implementation
// /////////////////////////////////////////////////////////////////

cbbComposerNodeView::cbbComposerNodeView(): dtkComposerNodeLeafView(),view(0) {
    appendReceiver(&(receiver_image));
}

void cbbComposerNodeView::run() {

    qDebug() << Q_FUNC_INFO;

    if (!view)
        view = dynamic_cast<cbbView*>(dtkAbstractViewFactory::instance()->create("cbbView"));

    foreach (cbbAbstractImage* image,receiver_image.allData())
        view->addImage(image);

    view->widget()->show();
}

QString cbbComposerNodeView::type() {
    return "cbbView";
}

QString cbbComposerNodeView::titleHint() {
    return "View";
}

QString cbbComposerNodeView::inputLabelHint(const int port) {
    switch(port) {
        case 0:
            return "image";
        default:
            return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString cbbComposerNodeView::outputLabelHint(int port) {
    return dtkComposerNodeLeaf::outputLabelHint(port);
}
