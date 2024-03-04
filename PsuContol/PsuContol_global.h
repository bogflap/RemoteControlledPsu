#ifndef PSUCONTOL_GLOBAL_H
#define PSUCONTOL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PSUCONTOL_LIBRARY)
#define PSUCONTOL_EXPORT Q_DECL_EXPORT
#else
#define PSUCONTOL_EXPORT Q_DECL_IMPORT
#endif

#endif // PSUCONTOL_GLOBAL_H
