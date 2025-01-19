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

#ifdef _DEBUG
#define API_LOCALHOST
#endif

#ifdef API_LOCALHOST
#define API_URL_BASE "http://127.0.0.1:5000"
#else
#define API_URL_BASE ""
#endif

#endif // TOOLSRUNTIME_GLOBAL_H
