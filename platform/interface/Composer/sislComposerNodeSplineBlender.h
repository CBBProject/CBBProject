/* sislComposerNodeSplineBlender.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Sat Jun  2 01:50:32 2012 (+0200)
 * Version: $Id$
 * Last-Updated: 2012 Thu Nov  8 10:38:23 (+0100)
 *           By: Thibaud Kloczko, Inria.
 *     Update #: 20
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef SISLCOMPOSERNODESPLINEBLENDER_H
#define SISLCOMPOSERNODESPLINEBLENDER_H

#include "sislComposerExport.h"

#include <dtkComposer/dtkComposerNodeLeafProcess.h>

#include <QtCore>

class sislComposerNodeSplineBlenderPrivate;

class SISLCOMPOSER_EXPORT sislComposerNodeSplineBlender : public dtkComposerNodeLeafProcess
{
public:
     sislComposerNodeSplineBlender(void);
    ~sislComposerNodeSplineBlender(void);

public:
    bool isAbstractProcess(void) const;

    QString abstractProcessType(void) const;

public:
    void run(void);

public:
    QString type(void);
    QString titleHint(void) ;

public:
    QString  inputLabelHint(int port);
    QString outputLabelHint(int port);

private:
    sislComposerNodeSplineBlenderPrivate *d;
};

#endif
