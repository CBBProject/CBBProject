/* sislComposerNodeSplineBlender.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Sat Jun  2 01:51:59 2012 (+0200)
 * Version: $Id$
 * Last-Updated: 2012 Thu Nov  8 11:48:49 (+0100)
 *           By: Thibaud Kloczko, Inria.
 *     Update #: 69
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "sislComposerNodeSplineBlender.h"

#include <sislCore/sislAbstractSpline.h>
#include <sislCore/sislAbstractSplineBlender.h>

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

#include <dtkCore/dtkAbstractProcessFactory.h>
#include <dtkCore/dtkAbstractProcess.h>

#include <dtkMath/dtkVector.h>

// /////////////////////////////////////////////////////////////////
// sislComposerNodeSplineBlenderPrivate declaration
// /////////////////////////////////////////////////////////////////

class sislComposerNodeSplineBlenderPrivate
{
public:
    sislAbstractSplineBlender *blender;

public:
    dtkComposerTransmitterReceiver<sislAbstractSpline> receiver_lhs;
    dtkComposerTransmitterReceiver<sislAbstractSpline> receiver_rhs;

public:    
    dtkComposerTransmitterEmitter<sislAbstractSpline> emitter_spline;
};

// /////////////////////////////////////////////////////////////////
// sislComposerNodeSplineBlender implementation
// /////////////////////////////////////////////////////////////////

sislComposerNodeSplineBlender::sislComposerNodeSplineBlender(void) : dtkComposerNodeLeafProcess(), d(new sislComposerNodeSplineBlenderPrivate)
{
    d->blender = NULL;

    this->appendReceiver(&(d->receiver_lhs));
    this->appendReceiver(&(d->receiver_rhs));

    this->appendEmitter(&(d->emitter_spline));
}

sislComposerNodeSplineBlender::~sislComposerNodeSplineBlender(void)
{
    delete d;
    
    d = NULL;
}

bool sislComposerNodeSplineBlender::isAbstractProcess(void) const
{
    return true;
}

QString sislComposerNodeSplineBlender::abstractProcessType(void) const
{
    return "sislAbstractSplineBlender";
}

void sislComposerNodeSplineBlender::run(void)
{
    if (this->implementationHasChanged()) {
        d->blender = reinterpret_cast<sislAbstractSplineBlender*>(this->process());
    }

    if (!d->blender) {
        dtkWarn() << "no spline blender, abort "<< this->currentImplementation();
        return;
    }
    
    if(d->receiver_lhs.isEmpty() || d->receiver_rhs.isEmpty())
        return;

    d->blender->setLeftHandSideSpline(d->receiver_lhs.data());
    d->blender->setRightHandSideSpline(d->receiver_rhs.data());

    if (d->emitter_spline.receiverCount())
        d->emitter_spline.setData(d->blender->blended());
}

QString sislComposerNodeSplineBlender::type(void)
{
    return "sislSplineBlender";
}

QString sislComposerNodeSplineBlender::titleHint(void)
{
    return "SplineBlender";
}

QString sislComposerNodeSplineBlender::inputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "lhs";
    case 1:
        return "rhs";
    default:
        return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString sislComposerNodeSplineBlender::outputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "spline";
    default:
        return dtkComposerNodeLeaf::outputLabelHint(port);
    }
}
