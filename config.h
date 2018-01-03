#ifndef CONFIG_H
#define CONFIG_H

#include <QList>

class config
{
public:
    static void uninstance();
    static config *instance();

    static void writeExample();
    const QString &get_module_key() const { return _module_key; }
    const QString &get_command_key() const { return _command_key; }
    const QString &get_source_path() const { return _proto_source; }
    const QList<QString> &get_module_field() const { return _module_field; }
    const QList<QString> &get_command_field() const { return _command_field; }
private:
    config();
    void read_conf();

    QList<QString> _module_field;
    QList<QString> _command_field;

    QString _module_key;
    QString _command_key;
    QString _proto_source;

    static class config *_config;
};

#endif // CONFIG_H
