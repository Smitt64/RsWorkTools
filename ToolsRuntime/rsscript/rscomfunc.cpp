#include "rslexecutor.h"
#include "statvars.h"
#include "rsl/isymbol.h"
#include "playrep.h"
#include "statvars.h"
#include "typeinfo_p.h"
#include "registerinfobase.h"
#include "rscom/appintf.hpp"
#include "toolsruntime.h"
#include <QGlobalStatic>
#include <QLibrary>

Q_GLOBAL_STATIC_WITH_ARGS(QLibrary, fs32stw, ("fs32stw.dll"))

typedef int(*LibInitRsCom)();
typedef Qt::HANDLE(*LibGetRsComAppIntf)(void);

LibInitRsCom _LibInitRsCom = nullptr;
LibGetRsComAppIntf _LibGetRsComAppIntf = nullptr;

template<class T>
T FS32STW_Resolve(const char *name)
{
    T func = (T)fs32stw->resolve(name);

    if (!func)
        qWarning(logRsl()) << "Can't resolve RsCom function:" << name;

    return func;
}

void FS32STW_Init()
{
    if (fs32stw->isLoaded())
        return;

    if (fs32stw->load())
    {
        _LibInitRsCom = FS32STW_Resolve<LibInitRsCom>("InitRsCom");
        _LibGetRsComAppIntf = FS32STW_Resolve<LibGetRsComAppIntf>("GetRsComAppIntf");
    }
    else
        qWarning(logRsl()) << "Can't resolve RsCom functions";
}

int rslInitRsCom()
{
    FS32STW_Init();

    if (!_LibInitRsCom)
        return 1;

    int stat = _LibInitRsCom();

    if (stat)
        qWarning(logRsl()) << "Can't initialize RsCom";
    else
        qInfo(logRsl()) << "RsCom success initialize";

    Qt::HANDLE app = _LibGetRsComAppIntf();
    qInfo(logRsl()) << "GetRsComAppIntf: " << app;

    return stat;
}

Qt::HANDLE rslGetAppIntf()
{
    FS32STW_Init();

    if (!_LibInitRsCom)
        return nullptr;

    return _LibGetRsComAppIntf();
}
