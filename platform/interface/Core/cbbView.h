#pragma once

#include <cbbCoreExport.h>

#include <dtkCore/dtkAbstractView.h>

class cbbAbstractImage;
class cbbViewPrivate;

class CBBCORE_EXPORT cbbView: public dtkAbstractView {
    Q_OBJECT

public:
             cbbView();
    virtual ~cbbView() { }

    void addImage(cbbAbstractImage *image);

    QWidget *widget();

    static dtkAbstractView* create() { return new cbbView; }

private:
    DTK_DECLARE_PRIVATE(cbbView);
};

Q_DECLARE_METATYPE(cbbView)
Q_DECLARE_METATYPE(cbbView *)
