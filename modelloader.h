#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <QObject>
#include <QMap>

// Field name - value.
typedef QMap<QString, QString> ObjectClass;

// Dynamic loadable types and items.
struct DataModel {
    QStringList EnumAirspaceType;
    QStringList EnumRouteDirection;
    // Key: Class name, Value - class.
    // QMap<QString, ObjectClass> objects_classes;
    // Key - Class name, value - names of fields in the Class.
    QMap<QString, QStringList> objects_classes;
};

class ModelLoader : public QObject
{
    Q_OBJECT
public:
    ModelLoader(QObject * parent=nullptr);

    DataModel * load(const QString & path); //!< Returns 0 on success.
    QVariantMap loadAsMap(const QString & path2file);

private:
    int read(DataModel * data, const QJsonObject & obj);
};

#endif // MODELLOADER_H
