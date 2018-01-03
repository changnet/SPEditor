#ifndef PROTO_H
#define PROTO_H

#include <QMap>

typedef QMap<QString,QString> Fields;
typedef QList<Fields> CmdList;

struct OneModule
{
    Fields _fields;
    CmdList _cmdList;
};

class proto
{
public:
    static void uninstance();
    static class proto *instance();

    void del_module( const QString &cmd );
    bool new_module( const QString &cmd,const Fields &fields );
private:
    proto();
    QMap<QString,struct OneModule> _module;

    static class proto *_proto;
};

#endif // PROTO_H
