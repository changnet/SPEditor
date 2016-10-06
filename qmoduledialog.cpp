#include "proto.h"
#include "qmoduledialog.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

QModuleDialog::QModuleDialog(QWidget *parent)
    :QDialog(parent)
{
    this->resize( 480,320 );

    _ok.setText( "Ok" );
    _cancel.setText( "Cancel" );

    QHBoxLayout *button_layout = new QHBoxLayout();
    button_layout->addWidget( &_cancel );
    button_layout->addWidget( &_ok );
    button_layout->setSpacing( 40 );
    button_layout->setContentsMargins( 40,0,40,0 );


    QHBoxLayout *id_layout = new QHBoxLayout();
    id_layout->addWidget( new QLabel("id:") ); //  will set label's parent and auto delete
    id_layout->addWidget( &_id );

    QHBoxLayout *comment_layout = new QHBoxLayout();
    comment_layout->addWidget( new QLabel("comment:") );
    comment_layout->addWidget( &_comment );

    QVBoxLayout *ly = new QVBoxLayout();
    ly->addLayout( id_layout );
    ly->addLayout( comment_layout );
    ly->addLayout( button_layout );
    ly->setContentsMargins( 80,80,80,80 );

    this->setLayout( ly );

    connect( &_cancel,SIGNAL(clicked()),this,SLOT(reject()) );
    connect( &_ok,SIGNAL(clicked()),this,SLOT(onOk()) );

}

void QModuleDialog::onOk()
{
    QString id = _id.text();
    QString comment = _comment.text();
    if ( id.isEmpty() || id.isNull() )
    {
        QMessageBox box;
        box.setText( "module id can not be NULL" );
        box.exec();
        return;
    }

    if ( comment.isEmpty() || comment.isNull() )
    {
        QMessageBox box;
        box.setText( "module comment can not be NULL" );
        box.exec();
        return;
    }

    bool ok;
    int num_id = id.toInt( &ok,10 );
    float fid = id.toFloat();
    if ( !ok || num_id != fid )
    {
        QMessageBox box;
        box.setText( "module id must be integer" );
        box.exec();
        return;
    }

    if ( _old_id != id ) // check id dumplicate
    {
        const class proto &pt = proto::instance();
        if ( pt.is_id_exist( num_id ) )
        {
            QMessageBox box;
            box.setText( "module id already exist" );
            box.exec();
            return;
        }
    }

    this->accept();
}

void QModuleDialog::setData(QString old_id, QString old_comment)
{
    _old_id = old_id;
    _old_comment = old_comment;

    _id.setText( _old_id );
    _comment.setText( _old_comment );
}

void QModuleDialog::getData( QString &id,QString &comment,QString &old_id )
{
    old_id = _old_id;
    id     = _id.text();
    comment = _comment.text();
}
