#include "config.h"

#include <QSettings>

class config *config::_config = NULL;

config::config()
{
    QSettings rsetting( "setting.ini",QSettings::IniFormat );

    _proto_source = rsetting.value( "PATH","./proto" ).toString();

    rsetting.beginGroup( "datatypes" );

    QStringList types = rsetting.allKeys();
    QStringList::const_iterator itr = types.constBegin();
    while ( itr != types.constEnd() )
    {
        const QString k = *itr;
        int v = rsetting.value( k,-1 ).toInt();
        if ( v >= 0 )
        {
            _datatype[k] = v;
        }

        ++ itr;
    }

    rsetting.endGroup();
}

class config &config::instance()
{
    if ( !_config )
    {
        _config = new config();
    }

    return *_config;
}

void config::uninstance()
{
    if ( _config ) delete _config;

    _config = NULL;
}
