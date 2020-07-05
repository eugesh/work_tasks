#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>

#include "jsonobjectloader.h"

JsonObjectLoader::JsonObjectLoader(QObject * obj) : AbstractObjectLoader(obj)
{

}

JsonObjectLoader::~JsonObjectLoader() {

}

// QStandardItemModel *
QList<QStringList>
JsonObjectLoader::load(QString const & path) {
    QList<QStringList> objects_list;

    return objects_list;
}

QVariantMap
JsonObjectLoader::loadAsMap(const QString & path2file) {
    QFile loadFile(path2file);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return QVariantMap();
    }

    QByteArray readData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(readData));

    // read(loadDoc.object());

    /* QTextStream(stdout) << "Loaded save for "
                        << loadDoc["player"]["name"].toString()
                        << " using JSON...\n";*/

    return loadDoc.object().toVariantMap();
}

void
JsonObjectLoader::read(const QJsonObject & obj) {

}
