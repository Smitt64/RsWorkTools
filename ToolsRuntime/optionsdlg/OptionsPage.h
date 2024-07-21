#ifndef OPTIONSPAGE_H
#define OPTIONSPAGE_H

#include <QWidget>
#include "ToolsRuntime_global.h"

class TOOLSRUNTIME_EXPORT OptionsPage : public QWidget
{
public:
    OptionsPage(QWidget *parent) :
        QWidget(parent)
    {

    }

    virtual ~OptionsPage()
    {

    }
};

#endif // OPTIONSPAGE_H
