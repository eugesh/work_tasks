#ifndef JSONOBJECTLOADER_H
#define JSONOBJECTLOADER_H

#include <QObject>
#include "abstractobjectloader.h"

class QJsonObject;

class JsonObjectLoader : public AbstractObjectLoader
{
    Q_OBJECT
public:
    JsonObjectLoader(QObject *obj=nullptr);
    ~JsonObjectLoader();

    // QStandardItemModel * load(const QString & path);
    QList<QStringList> load(const QString & path) Q_DECL_OVERRIDE;
    QVariantMap loadAsMap(const QString & path2file) Q_DECL_OVERRIDE;

private:
    void read(const QJsonObject & obj);
};

#endif // JSONOBJECTLOADER_H
