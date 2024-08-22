#include "jsonsettings.h"
#include "toolsruntime.h"
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QMapIterator>

bool readJsonFile(QIODevice &device, QSettings::SettingsMap &map)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(device.readAll(), &err);

    qInfo(logSettings()) << "Begin parse settings...";
    if (err.error == QJsonParseError::NoError)
    {
        qCritical(logSettings()) << "Error parse settings file:" << err.errorString();
        return false;
    }

    QJsonArray root = doc.array();
    for (const QJsonValue &groups : root)
    {
        QJsonObject obj = groups.toObject();
        QString key = obj["key"].toString();

        QVariant value = obj["value"].toVariant();
        map.insert(key, value);
    }

    qInfo(logSettings()) << "End parse settings...";
    return true;
}

bool writeJsonFile(QIODevice &device, const QSettings::SettingsMap &map)
{
    qInfo(logSettings()) << "Begin save settings...";
    QJsonArray root;

    QMapIterator it(map);
    while (it.hasNext())
    {
        it.next();
        QJsonObject obj;
        obj["key"] = it.key();
        obj["value"] = QJsonValue::fromVariant(it.value());
        root.append(obj);
    }

    QJsonDocument doc;
    doc.setArray(root);
    device.write(doc.toJson());
    qInfo(logSettings()) << "End save settings...";
    return true;
}

int registerJsonSettingsFormat()
{
    QSettings::Format format = QSettings::registerFormat("json", readJsonFile, writeJsonFile);
    return (int)format;
}
