#include "config.h"

#include <QFile>
#include <QSettings>

#define SETTING "setting.ini"

class config *config::_config = NULL;

config::config()
{
}

void config::read_conf()
{
    QSettings rsetting( SETTING,QSettings::IniFormat );

    _proto_source = rsetting.value( "PATH","./proto" ).toString();

    int length = rsetting.beginReadArray( "modulefield" );
    for ( int idx = 0;idx < length;idx ++ )
    {
        rsetting.setArrayIndex( idx );
        _module_field.append( rsetting.value("field").toString() );
    }
    rsetting.endArray();

    length = rsetting.beginReadArray( "commandfield" );
    for ( int idx = 0;idx < length;idx ++ )
    {
        rsetting.setArrayIndex( idx );
        _command_field.append( rsetting.value("field").toString() );
    }
    rsetting.endArray();

    if ( _module_field.length() > 0 )
    {
        _module_key = _module_field.at(0);
    }
    if ( _command_field.length() > 0 )
    {
        _command_key = _command_field.at(0);
    }
}

void config::writeExample()
{
    QSettings wsetting( SETTING,QSettings::IniFormat );
    wsetting.setValue( "PATH","./proto" );

    QList<QString> module_field;
    module_field << "command" << "module" << "comment";
    wsetting.beginWriteArray("modulefield");
    for ( int idx = 0;idx < module_field.length();idx ++ )
    {
        wsetting.setArrayIndex(idx);
        wsetting.setValue("field",module_field.at(idx));
    }
    wsetting.endArray();

    QList<QString> command_field;
    command_field << "command" << "field" << "comment" << "server" << "client";
    wsetting.beginWriteArray("commandfield");
    for ( int idx = 0;idx < command_field.length();idx ++ )
    {
        wsetting.setArrayIndex(idx);
        wsetting.setValue("field",command_field.at(idx));
    }
    wsetting.endArray();

    // This function is called automatically from QSettings's destructor and by the
    // event loop at regular intervals, so you normally don't need to call it yourself.
    wsetting.sync();
}

class config *config::instance()
{
    if ( !_config )
    {
        QFile file(SETTING);
        if ( !file.exists() )
        {
            writeExample();
        }
        _config = new config();
        _config->read_conf();
    }

    return _config;
}

void config::uninstance()
{
    if ( _config ) delete _config;

    _config = NULL;
}
