/* sislAbstractSplineBlender.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Sat Jun  2 00:23:50 2012 (+0200)
 * Version: $Id$
 * Last-Updated: 2012 Thu Nov  8 13:34:15 (+0100)
 *           By: Thibaud Kloczko, Inria.
 *     Update #: 29
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef SISLABSTRACTSPLINEBLENDER_H
#define SISLABSTRACTSPLINEBLENDER_H

#include "sislCoreExport.h"

#include <dtkCore/dtkAbstractProcess.h>

class sislAbstractSpline;
class sislAbstractSplineBlenderPrivate;

class SISLCORE_EXPORT sislAbstractSplineBlender : public dtkAbstractProcess
{
    Q_OBJECT

public:
             sislAbstractSplineBlender(void);
             sislAbstractSplineBlender(const sislAbstractSplineBlender& other);
    virtual ~sislAbstractSplineBlender(void);

public:
   virtual sislAbstractSplineBlender *clone(void);

public:
    sislAbstractSplineBlender& operator = (const sislAbstractSplineBlender& other);

protected:
   virtual void copy(const dtkAbstractObject& other);

public:
    virtual void  setLeftHandSideSpline(sislAbstractSpline *spline);
    virtual void setRightHandSideSpline(sislAbstractSpline *spline);

public:
    virtual sislAbstractSpline *blended(void);

private:
    DTK_DECLARE_PRIVATE(sislAbstractSplineBlender);
};

Q_DECLARE_METATYPE(sislAbstractSplineBlender)
Q_DECLARE_METATYPE(sislAbstractSplineBlender *)

#endif
