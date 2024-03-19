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
