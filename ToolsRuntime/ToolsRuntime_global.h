// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef TOOLSRUNTIME_GLOBAL_H
#define TOOLSRUNTIME_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TOOLSRUNTIME_LIBRARY)
#define TOOLSRUNTIME_EXPORT Q_DECL_EXPORT
#else
#define TOOLSRUNTIME_EXPORT Q_DECL_IMPORT
#endif

#endif // TOOLSRUNTIME_GLOBAL_H
