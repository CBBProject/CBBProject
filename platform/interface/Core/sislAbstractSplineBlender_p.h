/* sislAbstractSplineBlender_p.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Sat Jun  2 00:29:19 2012 (+0200)
 * Version: $Id$
 * Last-Updated: Sat Jun  2 00:44:17 2012 (+0200)
 *           By: Julien Wintz
 *     Update #: 9
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef SISLABSTRACTSPLINEBLENDER_P_H
#define SISLABSTRACTSPLINEBLENDER_P_H

#include <dtkCore/dtkAbstractProcess_p.h>

class sislAbstractSplineBlender;

class sislAbstractSplineBlenderPrivate : public dtkAbstractProcessPrivate
{
public:
    sislAbstractSplineBlenderPrivate(sislAbstractSplineBlender *q = 0) : dtkAbstractProcessPrivate(q) {}
    sislAbstractSplineBlenderPrivate(const dtkAbstractProcessPrivate& other) : dtkAbstractProcessPrivate(other) {}

public:
    virtual ~sislAbstractSplineBlenderPrivate(void) {}
};

DTK_IMPLEMENT_PRIVATE(sislAbstractSplineBlender, dtkAbstractProcess);

#endif
