#ifndef PROTO_H
#define PROTO_H

#include <QMap>

typedef QMap<QString,QString> Fields;
typedef QMap<QString,Fields> CmdMap;

struct OneModule
{
    Fields _fields;
    CmdMap _cmd_map;
};

class proto
{
public:
    static void uninstance();
    static class proto *instance();

    void del_module( const QString &cmd );
    bool new_module( const QString &cmd,const Fields &fields );
    bool update_module(const QString &cmd,
        const QString &key, const QString &val , bool update_key = false);

    void del_command( const QString &module_cmd,const QString &cmd );
    bool new_command( const QString &module_cmd,const QString &cmd,const Fields &Fields );
    bool update_command( const QString &module_cmd,const QString &cmd,const QString &key,QString &val );

    const CmdMap *get_module_cmd( const QString &cmd ) const;
private:
    proto();
    QMap<QString,struct OneModule> _module;

    static class proto *_proto;
};

#endif // PROTO_H
