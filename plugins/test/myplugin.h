#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <libStronghold/stronghold.h>

class MyPlugin : public QObject, public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.xsoftware.stronghold.plugin.myplugin")
    Q_INTERFACES(Plugin)
public:

    int use();
};

#endif //MYPLUGIN_H
