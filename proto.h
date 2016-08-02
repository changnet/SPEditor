#ifndef PROTO_H
#define PROTO_H

/* all stream protocol store here */

#include <QMap>
#include <QList>
#include <QJsonArray>

struct proto_node
{
    int _type;
    int _server;
    int _optional;
    QString _name;
    QString _comments;

    QList< struct proto_node > _child;
};

struct proto_function
{
    int _sc;
    QString _comments;

    QList< struct proto_node > _node;
};

struct proto_module
{
    QString _comments;

    QMap< int,struct proto_function > _function;
};

class proto
{
public:
    static void uninstance();
    static class proto &instance();

    bool update_module( int module_id,QString comment );
    bool update_function( int module_id,int function_id,int sc,QString comment );
    bool update_proto( int module_id,int function_id,const struct proto_node *node );
private:
    proto();
    bool serailize();
    bool deserialize();
    void encode_node(QJsonArray &nd_ay, const QList< struct proto_node > &nds );
    bool save_module( int module_id,const struct proto_module &module );
private:
    QMap< int,proto_module > _module_map;

    static class proto *_proto;
};

#endif // PROTO_H
