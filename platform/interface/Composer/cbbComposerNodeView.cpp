#include <cbbComposerNodeView.h>

#include <Core/cbbAbstractImage.h>
#include <Core/cbbView.h>

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

#include <dtkCore/dtkAbstractViewFactory.h>
#include <dtkCore/dtkAbstractView.h>

#include <dtkMath/dtkVector.h>

// /////////////////////////////////////////////////////////////////
// cbbComposerNodeViewPrivate declaration
// /////////////////////////////////////////////////////////////////

class cbbComposerNodeViewPrivate {
public:

    cbbView *view;

    dtkComposerTransmitterReceiver<cbbAbstractImage> receiver_image;
};

// /////////////////////////////////////////////////////////////////
// cbbComposerNodeView implementation
// /////////////////////////////////////////////////////////////////

cbbComposerNodeView::cbbComposerNodeView(): dtkComposerNodeLeafView() {\
  d = new cbbComposerNodeViewPrivate;
  d->view = 0;
  appendReceiver(&(d->receiver_image));
}

cbbComposerNodeView::~cbbComposerNodeView() {
    delete d;
    d = NULL;
}

void cbbComposerNodeView::run() {

  qDebug() << Q_FUNC_INFO;

  if(!d->view)
    d->view = dynamic_cast<cbbView *>(dtkAbstractViewFactory::instance()->create("cbbView"));

  foreach(cbbAbstractImage *image, d->receiver_image.allData())
    d->view->addImage(image);

  d->view->widget()->show();
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
