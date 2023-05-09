#ifndef XSLIBPASSWD_GLOBAL_H
#define XSLIBPASSWD_GLOBAL_H

#include <QtCore/qglobal.h>
#include <xslib.h>
#include <QDir>

#define XSLIB_STRONGHOLD "0.11"

#ifdef WIN32
    #define SH_DIR PROGRAMPATH + "/xsoftware/Stronghold/"
#else
    #define SH_DIR QString("/usr/share/xsoftware/Stronghold/")
#endif
#define LOGINFILE SH_DIR + "Stronghold.db"


#if defined(STRONGHOLD_LIBRARY)
#  define STRONGHOLDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define STRONGHOLDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XSLIBPASSWD_GLOBAL_H
