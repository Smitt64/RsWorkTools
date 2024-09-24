#include "iostaticmodule.h"
#include "rsscript/registerobjlist.hpp"
#include "rslexecutor.h"
#include "rslmodule/io/iodevice.h"
#include "rslmodule/io/iofile.h"
#include "rslmodule/io/iobuffer.h"
#include "rslmodule/io/iotextstream.h"
#include <QIODevice>
#include <QTextStream>

IoStaticModule::IoStaticModule()
{
    RegisterObjList::inst()->RegisterRslObject<IoDevice>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<IoFile>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<IoBuffer>(GenInfoUseParentProps | GenInfoUseParentMeths);
    RegisterObjList::inst()->RegisterRslObject<IoTextStream>(GenInfoUseParentProps | GenInfoUseParentMeths);
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

    RegisterObjList::inst()->AddObject<IoDevice>(false);
    RegisterObjList::inst()->AddObject<IoFile>();
    RegisterObjList::inst()->AddObject<IoBuffer>();
    RegisterObjList::inst()->AddObject<IoTextStream>();
    //RegisterObjList::inst()->AddStdProc("uiLoadFile", Rsl_uiLoadFile);
    //RegisterObjList::inst()->AddStdProc("uiFindChild", Rsl_uiFindChild);
}
