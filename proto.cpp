#include "proto.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class proto *proto::_proto = NULL;

void proto::uninstance()
{
    if ( _proto )
    {
        delete _proto;
        _proto = NULL;
    }
}
class proto *proto::instance()
{
    if ( !_proto )
    {
        _proto = new proto();
    }

    return _proto;
}

proto::proto()
{

}

bool proto::new_module( const QString &cmd,const Fields &fields )
{
    if ( !_module.empty() && _module.find( cmd ) != _module.constEnd() )
    {
        return false;
    }

    _module[cmd]._fields = fields;

    return true;
}

void proto::del_module( const QString &cmd )
{
    _module.remove( cmd );

    // TODO: remove file from disk
}
