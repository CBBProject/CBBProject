// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#include "cbbDataImage.h"
#include "cbbDataImagePlugin.h"

#include <dtkCore/dtkPlugin_p.h>

#include <dtkLog/dtkLog.h>

// /////////////////////////////////////////////////////////////////
// cbbDataImagePluginPrivate interface
// /////////////////////////////////////////////////////////////////

class cbbDataImagePluginPrivate : public dtkPluginPrivate
{
public:
    cbbDataImagePluginPrivate(cbbDataImagePlugin *q = 0): dtkPluginPrivate(q) {}
    cbbDataImagePluginPrivate(const cbbDataImagePluginPrivate& other): dtkPluginPrivate(other) {} // Complete copy ctror with your local members.

public:
    ~cbbDataImagePluginPrivate() {}
    
public:
    // Class variables go here.
};

////////////////////////////////////////////////////
// cbbDataImagePlugin protected constructors
////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(cbbDataImagePlugin, dtkPlugin);

// /////////////////////////////////////////////////////////////////
// cbbDataImagePlugin implementation
// /////////////////////////////////////////////////////////////////

cbbDataImagePlugin::cbbDataImagePlugin(QObject *parent): dtkPlugin(*new cbbDataImagePluginPrivate,parent) { }

cbbDataImagePlugin::cbbDataImagePlugin(const cbbDataImagePlugin& other): dtkPlugin(*new cbbDataImagePluginPrivate(*other.d_func()),other) { }

cbbDataImagePlugin::~cbbDataImagePlugin() { }

bool cbbDataImagePlugin::initialize() {
    if (!cbbDataImage::registered()) {
        dtkWarn() << "Unable to register cbbDataImage type";
    }
    return true;
}

bool cbbDataImagePlugin::uninitialize() {
    return true;
}

QString cbbDataImagePlugin::name() const {
    return "cbbDataImagePlugin";
}

QString cbbDataImagePlugin::description() const {
    return "Image data";
}

QStringList cbbDataImagePlugin::tags() const {
    return QStringList("Image");
}

QStringList cbbDataImagePlugin::types() const {
    return QStringList() << "cbbDataImage";
}

Q_EXPORT_PLUGIN2(cbbDataImagePlugin,cbbDataImagePlugin)
