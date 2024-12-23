#include "mainwindow.h"
#include <QCommandLineParser>
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QDateTime>
#include <QDir>

static QScopedPointer<QFile> m_logFile;
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream out(m_logFile.data());
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    switch (type)
    {
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
    }

    out << context.category << ": "
        << msg << Qt::endl;

    out.flush();
}

bool InitLogging(const QString &prefix, const QString &rules)
{
    QDir logDir(qApp->applicationDirPath());

    if (!logDir.cd("logs"))
    {
        logDir.mkdir("logs");
        logDir.cd("logs");
    }

    QString baseName = prefix + QString("_") + QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss_zzz");
    QString logFileName = QString("%1.txt")
                              .arg(logDir.absoluteFilePath(baseName))
                              .arg(prefix);

    m_logFile.reset(new QFile(logFileName));

    if (m_logFile->open(QFile::Append | QFile::Text))
    {
        //toolSetLoggingRules("*=true");
        QLoggingCategory::setFilterRules(rules);
        qInstallMessageHandler(messageHandler);
        //m_fLogging = true;
    }
    else
    {
        m_logFile.reset(Q_NULLPTR);
        //m_fLogging = false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InitLogging("rsldbg", "rsldbg.*=true");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption hostOption(QStringList() << "host",
                                  QCoreApplication::translate("main", "host to connect <port>."),
                                  QCoreApplication::translate("main", "host"));
    QCommandLineOption portOption(QStringList() << "p" << "port",
                                  QCoreApplication::translate("main", "port to connect <port>."),
                                  QCoreApplication::translate("main", "port"));

    parser.addOption(hostOption);
    parser.addOption(portOption);

    parser.process(a);

    if (!parser.isSet(hostOption) || !parser.isSet(portOption))
    {
        QMessageBox::critical(nullptr, QCoreApplication::translate("main", "Error"),
                              QCoreApplication::translate("main", "Сonnection parameters not specified"));
        a.quit();
        return -1;
    }

    MainWindow w;
    if (!w.connectToHost(parser.value(hostOption), parser.value(portOption).toShort()))
    {
        QMessageBox::critical(nullptr, QCoreApplication::translate("main", "Error"),
                              w.lastError());
        a.quit();
    }

    w.show();

    return a.exec();
}
