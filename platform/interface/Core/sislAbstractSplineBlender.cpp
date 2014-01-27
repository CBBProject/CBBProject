/* sislAbstractSplineBlender.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Sat Jun  2 00:27:35 2012 (+0200)
 * Version: $Id$
 * Last-Updated: 2012 Thu Nov  8 13:35:54 (+0100)
 *           By: Thibaud Kloczko, Inria.
 *     Update #: 22
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "sislAbstractSplineBlender.h"
#include "sislAbstractSplineBlender_p.h"

sislAbstractSplineBlender::sislAbstractSplineBlender(void) : dtkAbstractProcess(*new sislAbstractSplineBlenderPrivate(this), 0)
{

}

sislAbstractSplineBlender::sislAbstractSplineBlender(const sislAbstractSplineBlender& other) : dtkAbstractProcess(*new sislAbstractSplineBlenderPrivate(*other.d_func()), other)
{

}

sislAbstractSplineBlender::~sislAbstractSplineBlender(void)
{

}

sislAbstractSplineBlender *sislAbstractSplineBlender::clone(void)
{
    return new sislAbstractSplineBlender(*this);
}

sislAbstractSplineBlender& sislAbstractSplineBlender::operator = (const sislAbstractSplineBlender& other)
{
    this->copy(other);
    return *(this);
}

void sislAbstractSplineBlender::copy(const dtkAbstractObject& other)
{
    dtkAbstractProcess::copy(other);
}

void sislAbstractSplineBlender::setLeftHandSideSpline(sislAbstractSpline *spline)
{
    DTK_UNUSED(spline);

    DTK_DEFAULT_IMPLEMENTATION;
}

void sislAbstractSplineBlender::setRightHandSideSpline(sislAbstractSpline *spline)
{
    DTK_UNUSED(spline);

    DTK_DEFAULT_IMPLEMENTATION;
}

sislAbstractSpline *sislAbstractSplineBlender::blended(void)
{
    DTK_DEFAULT_IMPLEMENTATION;

    return NULL;
}
