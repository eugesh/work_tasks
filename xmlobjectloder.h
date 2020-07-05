#ifndef XMLOBJECTLODER_H
#define XMLOBJECTLODER_H

#include <QObject>

#include "abstractobjectloader.h"

class XmlObjectLoder : public AbstractObjectLoader
{
public:
    XmlObjectLoder();

    QList<QStringList> load(const QString & path) Q_DECL_OVERRIDE;
    QVariantMap loadAsMap(const QString & path2file) Q_DECL_OVERRIDE;
};

#endif // XMLOBJECTLODER_H
