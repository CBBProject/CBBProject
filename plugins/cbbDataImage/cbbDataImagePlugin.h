// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#pragma once

#include <dtkCore/dtkPlugin.h>

#include "cbbExport.h"

class CBB_WINDOWS_EXPORT cbbDataImagePluginPrivate;

// /////////////////////////////////////////////////////////////////
// cbbDataImagePlugin interface
// /////////////////////////////////////////////////////////////////

class CBB_WINDOWS_EXPORT cbbDataImagePlugin : public dtkPlugin
{
    Q_OBJECT
    Q_INTERFACES(dtkPlugin)

public:
     cbbDataImagePlugin(QObject *parent = 0);
     cbbDataImagePlugin(const cbbDataImagePlugin& other);
    ~cbbDataImagePlugin();

public:
    virtual bool initialize();
    virtual bool uninitialize();

    virtual QString name() const;
    virtual QString description() const;

    virtual QStringList tags() const;
    virtual QStringList types() const;

private:

    DTK_DECLARE_PRIVATE(cbbDataImagePlugin);
};
