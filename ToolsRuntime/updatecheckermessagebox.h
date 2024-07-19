// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef UPDATECHECKERMESSAGEBOX_H
#define UPDATECHECKERMESSAGEBOX_H

#include <QMessageBox>
#include "ToolsRuntime_global.h"
#include "updatechecker.h"

class TOOLSRUNTIME_EXPORT UpdateCheckerMessageBox : public QMessageBox
{
public:
    UpdateCheckerMessageBox(QWidget *parent = nullptr);
    virtual ~UpdateCheckerMessageBox();

    void setList(const CheckDataList &lst);
};

#endif // UPDATECHECKERMESSAGEBOX_H
