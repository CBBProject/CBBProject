#pragma once

#include <dtkCore/dtkAbstractView_p.h>

class QGraphicsScene;
class QGraphicsView;

class cbbView;

class cbbViewPrivate : public dtkAbstractViewPrivate
{
public:
    cbbViewPrivate(cbbView *q = 0) : dtkAbstractViewPrivate(q) {}
    cbbViewPrivate(const dtkAbstractViewPrivate& other) : dtkAbstractViewPrivate(other) {}

public:
    virtual ~cbbViewPrivate(void) {}

public:
    QGraphicsScene *scene;
    QGraphicsView *view;
};

DTK_IMPLEMENT_PRIVATE(cbbView, dtkAbstractView);
