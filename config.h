#ifndef CONFIG_H
#define CONFIG_H

#include <QMap>

class config
{
public:
    static void uninstance();
    static class config &instance();

    const QString &source_path() { return _proto_source; }
private:
    config();

    QMap< QString,int > _datatype;
    QString _proto_source;

    static class config *_config;
};

#endif // CONFIG_H
