#ifndef EXPRAW_GLOBAL_H
#define EXPRAW_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef _WIN32
#if defined(EXPRAW_LIBRARY)
#  define EXPRAWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EXPRAWSHARED_EXPORT Q_DECL_IMPORT
#endif
#else
#  define EXPRAWSHARED_EXPORT
#endif
#endif // EXPRAW_GLOBAL_H
