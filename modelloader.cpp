#include <QFile>
#include <QList>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>

#include "modelloader.h"

ModelLoader::ModelLoader(QObject *parent) : QObject(parent)
{

}

QVariantMap
ModelLoader::loadAsMap(const QString & path2file) {
    QFile loadFile(path2file);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return QVariantMap();
    }

    QByteArray readData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(readData));

    /*QTextStream(stdout) << "Loaded save for "
                        << loadDoc["player"]["name"].toString()
                        << " using JSON...\n";*/

    return loadDoc.object().toVariantMap();
}

// int
// ModelLoader::load ()
DataModel * ModelLoader::load(const QString & path) {
    DataModel * data = new DataModel();

    QFile loadFile(path);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return nullptr;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    read(data, loadDoc.object());

    QTextStream(stdout) << "Loaded save for "
                        << loadDoc["player"]["name"].toString()
                        << " using JSON...\n";

    return data;
}

int
ModelLoader::read(DataModel * model, const QJsonObject & obj) {
    // QStringList::const_iterator iter = obj.keys().begin();
    // while (iter != dataMap->constEnd()) {
    foreach (auto key, obj.keys()) {
            if (key.contains("types")) {
                QJsonArray arr = obj["types"].toArray();
                foreach (auto val, arr)
                    model->EnumAirspaceType.push_back(val.toString());
            }
            else if (key.contains("enumerations")) {
                QJsonArray arr = obj["enumerations"].toArray();
                // foreach(auto val, arr)
                   // model->EnumRouteDirection.push_back(obj["types"].toString());
            }
    }
    
    return 0;
}
