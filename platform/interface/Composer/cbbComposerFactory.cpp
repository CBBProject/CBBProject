#include "cbbComposerFactory.h"
#include "cbbComposerNodeImage.h"
#include "cbbComposerNodeView.h"

#include <Core/cbbView.h>

#include <dtkCore/dtkAbstractView.h>
#include <dtkCore/dtkAbstractViewFactory.h>

#include <dtkLog/dtkLog.h>

class cbbComposerFactoryPrivate {
public:
    void initialize();

    QList<QString> nodes;
    QHash<QString, QString> descriptions;
    QHash<QString, QStringList> tags;
    QHash<QString, QString> types;
};

void cbbComposerFactoryPrivate::initialize() {

  dtkAbstractViewFactory::instance()->registerViewType("cbbView", cbbView::create);
  
  qRegisterMetaType<cbbView>("cbbView");
}

// /////////////////////////////////////////////////////////////////

cbbComposerFactory::cbbComposerFactory(): dtkComposerFactory(), d(new cbbComposerFactoryPrivate) {
    d->initialize();

    d->nodes = QList<QString>(dtkComposerFactory::nodes());
    d->descriptions = QHash<QString, QString>(dtkComposerFactory::descriptions());
    d->tags = QHash<QString, QStringList>(dtkComposerFactory::tags());
    d->types = QHash<QString, QString>(dtkComposerFactory::types());

    // Extend attributes

    d->nodes << "Image";
    d->descriptions["Image"] = "<p>Image.</p>";
    d->tags["Image"] = QStringList() << "image";
    d->types["Image"] = "cbbDataImage";

    d->nodes << "View";
    d->descriptions["View"] = "<p>Image View.</p>";
    d->tags["View"] = QStringList() << "image" << "view";
    d->types["View"] = "cbbView";
}

cbbComposerFactory::~cbbComposerFactory() {
    delete d;
    d = NULL;
}

dtkComposerNode *cbbComposerFactory::create(const QString& type) {

    if(type == "cbbDataImage")
        return new cbbComposerNodeImage;

    if(type == "cbbView")
        return new cbbComposerNodeView;

    return dtkComposerFactory::create(type);
}

QList<QString> cbbComposerFactory::nodes() {
    return d->nodes;
}

QHash<QString, QString> cbbComposerFactory::descriptions() {
    return d->descriptions;
}

QHash<QString, QStringList> cbbComposerFactory::tags() {
    return d->tags;
}

QHash<QString, QString> cbbComposerFactory::types() {
    return d->types;
}
