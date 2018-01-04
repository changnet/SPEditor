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
        _error_text = "dumplicate module key";
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
        _error_text = "no module key found";
        return false;
    }

    CmdMap &cmd_map = itr->_cmd_map;
    if ( !cmd_map.empty() && cmd_map.find( cmd ) != cmd_map.constEnd() )
    {
        _error_text = "dumplicate command key";
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
        _error_text = "no such module key found";
        return false;
    }

    Fields &fields = itr->_fields;
    Fields::ConstIterator field_itr = fields.find(key);
    if ( field_itr == fields.constEnd() )
    {
        _error_text = "no such field";
        return false;
    }

    if ( *field_itr == val ) return true; // no change

    // 如果更新Key,则不能与已有key冲突
    if (update_key)
    {
        if ( _module.find( val ) != _module.constEnd() )
        {
            _error_text = "dumplicate module new key";
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

bool proto::update_command( const QString &module_cmd,
    const QString &cmd,const QString &key,const QString &val,bool update_key )
{
    QMap<QString,struct OneModule>::Iterator itr = _module.find( module_cmd );
    if ( itr == _module.end() )
    {
        _error_text = "no such module found";
        return false;
    }

    CmdMap &cmd_map = itr->_cmd_map;
    CmdMap::Iterator cmd_itr = cmd_map.find( cmd );
    if ( cmd_itr == cmd_map.end() )
    {
        _error_text = "no such command";
        return false;
    }

    Fields &fields = *cmd_itr;
    if ( fields.find(key) == fields.constEnd() )
    {
        _error_text = "no such command field";
        return false;
    }

    if ( update_key )
    {
        if ( cmd_map.find( val ) != cmd_map.constEnd() )
        {
            _error_text = "dumplicate command new key";
            return false;
        }

        fields[key] = val;
        cmd_map[val] = fields;
        cmd_map.remove( cmd );
    }
    else
    {
        fields[key] = val;
    }

    return true;
}
