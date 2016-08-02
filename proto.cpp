#include "proto.h"

#include "config.h"

#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

class proto *proto::_proto = NULL;

class proto &proto::instance()
{
    if ( !_proto )
    {
        _proto = new proto();
    }

    return *_proto;
}

void proto::uninstance()
{
    if ( _proto ) delete _proto;

    _proto = NULL;
}

proto::proto()
{

}

bool proto::update_module( int module_id,QString comment )
{
    struct proto_module &module = _module_map[module_id];
    module._comments = comment;

    return save_module( module_id, module );
}

bool proto::update_function( int module_id,int function_id,int sc,QString comment )
{
    QMap< int,proto_module >::iterator module_itr = _module_map.find( module_id );
    if ( module_itr == _module_map.end() ) return false;

    QMap< int,struct proto_function >::iterator func_itr = module_itr->_function.find( function_id );
    if ( func_itr == module_itr->_function.end() ) return false;

    func_itr->_sc = sc;
    func_itr->_comments = comment;

    return save_module( module_id,*module_itr );
}

bool proto::update_proto( int module_id,int function_id,const struct proto_node *node )
{
    return true;
}

/* one module save to one file */
bool proto::save_module( int module_id,const proto_module &module )
{
    const QString &path = config::instance().source_path();

    QDir dir;
    dir.mkpath( path );

    QJsonObject md;
    md["module"] = module_id;
    md["comment"] = module._comments;

    if ( !module._function.empty() )
    {
        QJsonArray ay;
        QMap< int,struct proto_function >::const_iterator itr = module._function.constBegin();
        while ( itr != module._function.constEnd() )
        {
            const struct proto_function &fc = *itr;

            QJsonObject func;
            func["id"] = itr.key();
            func["sc"] = fc._sc;
            func["comment"] = fc._comments;

            if ( !fc._node.empty() )
            {
                QJsonArray nd_ay;

                encode_node( nd_ay,fc._node );

                func["node"] = nd_ay;
            }

            ++ itr;
        }
        md["functions"] = ay;
    }

    QString file_name = QString( "%1/%2.json" ).arg( path ).arg( module_id );

    QFile file( file_name );
    if ( !file.open( QIODevice::WriteOnly ) ) //The device is open for writing. Note that this mode implies Truncate
    {
        return false;
    }

    QJsonDocument doc( md );
    file.write( doc.toJson() );

    file.close();

    return true;
}


void proto::encode_node(QJsonArray &nd_ay, const QList<proto_node> &nds )
{
    QList< struct proto_node >::const_iterator nd_itr = nds.constBegin();
    while ( nd_itr != nds.constEnd() )
    {
        QJsonObject nd;
        nd["type"] = nd_itr->_type;
        nd["name"] = nd_itr->_name;
        nd["optional"] = nd_itr->_optional;
        nd["comment"] = nd_itr->_comments;
        if ( nd_itr->_server > 0 ) nd["server"] = nd_itr->_server; /* C2S no server specify */

        if ( !(nd_itr->_child.empty()) )
        {
            QJsonArray nd_child;
            encode_node( nd_child,nd_itr->_child );

            nd["child"] = nd_child;
        }

        nd_ay.append( nd );
    }
}
