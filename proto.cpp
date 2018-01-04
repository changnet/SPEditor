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

bool proto::new_command( const QString &module_cmd,const QString &cmd,const Fields &Fields )
{
    QMap<QString,struct OneModule>::iterator itr = _module.find( module_cmd );
    if ( itr == _module.end() )
    {
        return false;
    }

    CmdMap &cmd_map = itr->_cmd_map;
    if ( !cmd_map.empty() && cmd_map.find( cmd ) != cmd_map.constEnd() )
    {
        return false;
    }

    cmd_map[cmd] = Fields;

    return true;
}

void proto::del_command( const QString &module_cmd,const QString &cmd )
{
    QMap<QString,struct OneModule>::iterator itr = _module.find( module_cmd );
    if ( itr == _module.end() )
    {
        return;
    }

    itr->_cmd_map.remove( cmd );
}

const CmdMap *proto::get_module_cmd( const QString &cmd ) const
{
    QMap<QString,struct OneModule>::ConstIterator itr = _module.find( cmd );
    if ( itr == _module.constEnd() )
    {
        return NULL;
    }

    return &(itr->_cmd_map);
}

bool proto::update_module( const QString &cmd,const QString &key,const QString &val,bool update_key )
{
    QMap<QString,struct OneModule>::Iterator itr = _module.find( cmd );
    if ( itr == _module.end() )
    {
        return false;
    }

    Fields &fields = itr->_fields;
    Fields::ConstIterator field_itr = fields.find(key);
    if ( field_itr == fields.constEnd() || *field_itr == val )
    {
        return false;
    }

    // 如果更新Key,则不能与已有key冲突
    if (update_key)
    {
        if ( _module.find( val ) != _module.constEnd() )
        {
            return false;
        }

        fields[key] = val;
        _module[val] = *itr;
        _module.remove( cmd );
    }
    else
    {
        fields[key] = val;
    }

    return true;
}

bool proto::update_command( const QString &module_cmd,const QString &cmd,const QString &key,QString &val )
{
    QMap<QString,struct OneModule>::Iterator itr = _module.find( module_cmd );
    if ( itr == _module.end() )
    {
        return false;
    }

    CmdMap &cmd_map = itr->_cmd_map;
    CmdMap::Iterator cmd_itr = cmd_map.find( cmd );
    if ( cmd_itr == cmd_map.end() )
    {
        return false;
    }

    Fields &fields = *cmd_itr;
    if ( fields.find(key) == fields.constEnd() )
    {
        return false;
    }

    fields[key] = val;

    return true;
}
