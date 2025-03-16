#include "iostaticmodule.h"
#include "rsscript/registerobjlist.hpp"
#include "rslexecutor.h"
#include "rslmodule/io/iodevice.h"
#include "rslmodule/io/iofile.h"
#include "rslmodule/io/iobuffer.h"
#include "rslmodule/io/iotextstream.h"
#include "rslmodule/io/temporaryfile.h"
#include "rslmodule/io/process.h"
#include "rslmodule/io/ionamespace.h"
#include "rslmodule/io/iofileinfo.h"
#include "toolsruntime.h"
#include <QIODevice>
#include <QTextStream>

Q_GLOBAL_STATIC(IoNamespace, pIoNamespace)

static void Rsl_toolStartProcess()
{
    enum
    {
        prm_exe = 0,
        prm_program,
        prm_arguments,
        prm_waitForFinished,
        prm_waitForStarted,
        prm_timeout,
        prm_waitForReadyRead
    };

    bool waitForFinished = false;
    bool waitForStarted = false;
    bool waitForReadyRead = false;
    int timeout = 30000;

    Process *proc = GetFuncParam<Process*>(prm_exe);
    if (!proc)
        ThrowParamTypeError<Process>(prm_exe);

    if (GetFuncParamType(prm_program) != QVariant::String)
        ThrowParamTypeError<QString>(prm_program);

    if (GetFuncParamType(prm_arguments) != QVariant::StringList)
        ThrowParamTypeError<QStringList>(prm_arguments);

    if (GetFuncParamType(prm_waitForFinished) == QVariant::Bool)
        waitForFinished = GetFuncParam(prm_waitForFinished).toBool();

    if (GetFuncParamType(prm_waitForStarted) == QVariant::Bool)
        waitForStarted = GetFuncParam(prm_waitForStarted).toBool();

    if (GetFuncParamType(prm_waitForReadyRead) == QVariant::Bool)
        waitForReadyRead = GetFuncParam(prm_waitForReadyRead).toBool();

    if (GetFuncParamType(prm_timeout) == QVariant::Int)
        timeout = GetFuncParam(prm_timeout).toBool();


    QString pattern = GetFuncParam(0).toString();
    QString str = GetFuncParam(1).toString();
    int pos = GetFuncParam(2).toInt();

    QStringList lst = GetFuncParam(prm_arguments, true).toStringList();
    int result = toolStartProcess(proc->proc(), GetFuncParam(prm_program).toString(),
        lst, waitForFinished, waitForStarted, timeout, waitForReadyRead);

    SetReturnVal(result);
}

IoStaticModule::IoStaticModule()
{
    RegisterObjList::inst()->RegisterRslObject<IoDevice>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<IoFile>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<IoBuffer>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<IoTextStream>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<TemporaryFile>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<Process>(GenInfoUseParentProps | GenInfoUseParentMeths);

    RegisterObjList::inst()->RegisterRslObject<ProcessEnvironment>();
    RegisterObjList::inst()->RegisterRslObject<TemporaryDir>();

    RegisterObjList::inst()->RegisterRslObject<IoFileInfo>();
    RegisterObjList::inst()->RegisterRslObject<IoNamespace>();
}

void IoStaticModule::Init()
{

}

void IoStaticModule::Proc()
{
    addConstant("IoNotOpen", QIODevice::NotOpen);
    addConstant("IoReadOnly", QIODevice::ReadOnly);
    addConstant("IoWriteOnly", QIODevice::WriteOnly);
    addConstant("IoReadWrite", QIODevice::ReadWrite);
    addConstant("IoAppend", QIODevice::Append);
    addConstant("IoTruncate", QIODevice::Truncate);
    addConstant("IoText", QIODevice::Text);
    addConstant("IoUnbuffered", QIODevice::Unbuffered);
    addConstant("IoNewOnly", QIODevice::NewOnly);
    addConstant("IoExistingOnly", QIODevice::ExistingOnly);

    addConstant("StandardOutput", QProcess::StandardOutput);
    addConstant("StandardError", QProcess::StandardError);

    addConstant("FieldAlignLeft", QTextStream::AlignLeft);
    addConstant("FieldAlignRight", QTextStream::AlignRight);
    addConstant("FieldAlignCenter", QTextStream::AlignCenter);
    addConstant("FieldAlignAccountingStyle", QTextStream::AlignAccountingStyle);

    addConstant("NumberShowBase", QTextStream::ShowBase);
    addConstant("NumberForcePoint", QTextStream::ForcePoint);
    addConstant("NumberForceSign", QTextStream::ForceSign);
    addConstant("NumberUppercaseBase", QTextStream::UppercaseBase);
    addConstant("NumberUppercaseDigits", QTextStream::UppercaseDigits);

    addConstant("RealNumberScientificNotation", QTextStream::ScientificNotation);
    addConstant("RealNumberFixedNotation", QTextStream::FixedNotation);
    addConstant("RealNumberSmartNotation", QTextStream::SmartNotation);

    addConstant("FailedToStart", QProcess::FailedToStart);
    addConstant("Crashed", QProcess::Crashed);
    addConstant("Timedout", QProcess::Timedout);
    addConstant("WriteError", QProcess::WriteError);
    addConstant("ReadError", QProcess::ReadError);
    addConstant("UnknownError", QProcess::UnknownError);

    addConstant("NormalExit", QProcess::NormalExit);
    addConstant("CrashExit", QProcess::CrashExit);

    addConstant("ManagedInputChannel", QProcess::ManagedInputChannel);
    addConstant("ForwardedInputChannel", QProcess::ForwardedInputChannel);

    addConstant("SeparateChannels", QProcess::SeparateChannels);
    addConstant("MergedChannels", QProcess::MergedChannels);
    addConstant("ForwardedChannels", QProcess::ForwardedChannels);
    addConstant("ForwardedErrorChannel", QProcess::ForwardedErrorChannel);
    addConstant("ForwardedOutputChannel", QProcess::ForwardedOutputChannel);

    addConstant("DirFilterDirs", QDir::Dirs);
    addConstant("DirFilterAllDirs", QDir::AllDirs);
    addConstant("DirFilterFiles", QDir::Files);
    addConstant("DirFilterDrives", QDir::Drives);
    addConstant("DirFilterNoSymLinks", QDir::NoSymLinks);
    addConstant("DirFilterNoDotAndDotDot", QDir::NoDotAndDotDot);
    addConstant("DirFilterNoDot", QDir::NoDot);
    addConstant("DirFilterNoDotDot", QDir::NoDotDot);
    addConstant("DirFilterAllEntries", QDir::AllEntries);
    addConstant("DirFilterReadable", QDir::Readable);
    addConstant("DirFilterWritable", QDir::Writable);
    addConstant("DirFilterExecutable", QDir::Executable);
    addConstant("DirFilterModified", QDir::Modified);
    addConstant("DirFilterHidden", QDir::Hidden);
    addConstant("DirFilterSystem", QDir::System);
    addConstant("DirFilterCaseSensitive", QDir::CaseSensitive);

    addConstant("SortName", QDir::Name);
    addConstant("SortTime", QDir::Time);
    addConstant("SortSize", QDir::Size);
    addConstant("SortType", QDir::Type);
    addConstant("SortUnsorted", QDir::Unsorted);
    addConstant("SortNoSort", QDir::NoSort);
    addConstant("SortDirsFirst", QDir::DirsFirst);
    addConstant("SortDirsLast", QDir::DirsLast);
    addConstant("SortReversed", QDir::Reversed);
    addConstant("SortIgnoreCase", QDir::IgnoreCase);
    addConstant("SortLocaleAware", QDir::LocaleAware);

    RegisterObjList::inst()->AddObject<IoDevice>(false);
    RegisterObjList::inst()->AddObject<IoNamespace>(false);
    RegisterObjList::inst()->AddObject<IoFile>();
    RegisterObjList::inst()->AddObject<IoBuffer>();
    RegisterObjList::inst()->AddObject<IoTextStream>();
    RegisterObjList::inst()->AddObject<TemporaryFile>();
    RegisterObjList::inst()->AddObject<TemporaryDir>();
    RegisterObjList::inst()->AddObject<ProcessEnvironment>();
    RegisterObjList::inst()->AddObject<Process>();
    RegisterObjList::inst()->AddObject<IoFileInfo>();
    //RegisterObjList::inst()->AddStdProc("uiLoadFile", Rsl_uiLoadFile);
    //RegisterObjList::inst()->AddStdProc("uiFindChild", Rsl_uiFindChild);
    RegisterObjList::inst()->AddStdProc("toolStartProcess", Rsl_toolStartProcess);

    addConstant("Io", QVariant::fromValue((QObject*)pIoNamespace));
}
