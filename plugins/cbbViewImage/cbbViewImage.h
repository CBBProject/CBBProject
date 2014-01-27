#pragma once

#include <dtkCore/dtkAbstractView.h>
#include "cbbViewImagePluginExport.h"

class cbbViewImagePrivate;

// /////////////////////////////////////////////////////////////////
// cbbViewImage interface
// /////////////////////////////////////////////////////////////////

class CBBVIEWIMAGEPLUGIN_EXPORT cbbViewImage : public dtkAbstractView
{
    Q_OBJECT

public:
             cbbViewImage();
             cbbViewImage(const cbbViewImage& other);
    virtual ~cbbViewImage();

    cbbViewImage& operator = (const cbbViewImage& other);

    virtual QString description() const;
    virtual QString identifier()  const { return id; }

    void setData(dtkAbstractData *data);

    static bool registered();

    QWidget* widget();

private:

    DTK_DECLARE_PRIVATE(cbbViewImage);

    static dtkAbstractView *create() { return new cbbViewImage; }
    static const char id[];
};
