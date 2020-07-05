#ifndef ABSTRACTOBJECTLOADER_H
#define ABSTRACTOBJECTLOADER_H

#include <QObject>
#include <QVariantMap>

class QStandardItemModel;

class AbstractObjectLoader : public QObject
{
    Q_OBJECT
public:
    explicit AbstractObjectLoader(QObject *parent = nullptr);

    virtual ~AbstractObjectLoader() {}

    // virtual QStandardItemModel * load(QString path2file)=0;
    virtual QList<QStringList> load(const QString & path2file)=0;
    virtual QVariantMap loadAsMap(const QString & path2file)=0;
    
signals:
    
};

#endif // ABSTRACTOBJECTLOADER_H
