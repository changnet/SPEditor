#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "proto.h"
#include "config.h"

#include <QMessageBox>

#define DEF_FIELD "undefine"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 在designer中的layout与centralwidget没有建立联系，手动建立
    ui->centralWidget->setLayout(ui->gridLayout);

    class config *conf = config::instance();

    const QList<QString> &module_field = conf->get_module_field();
    ui->module_tbl->setColumnCount( module_field.length() );
    ui->module_tbl->setHorizontalHeaderLabels( module_field );

    const QList<QString> &command_field = conf->get_command_field();
    ui->command_tbl->setColumnCount( command_field.length() );
    ui->command_tbl->setHorizontalHeaderLabels( conf->get_command_field() );

    // AUTO-CONNECTION:
    // http://doc.qt.io/qt-5/designer-using-a-ui-file.html#widgets-and-dialogs-with-auto-connect
    // connect( ui->module_new,SIGNAL(clicked(bool)),this,SLOT(on_module_new(bool)) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_module_new_clicked(bool check)
{
    Q_UNUSED(check);

    // 由于采用了双击编辑，很难确定用户什么时候编辑完成一个module的各个子项
    // 因此，在创新module时，自动填充所有子项。以后的编辑，均采用更新方式
    class config *conf = config::instance();
    const QList<QString> &module_field = conf->get_module_field();

    Fields fields;
    for ( int idx = 0;idx < module_field.length();idx ++ )
    {
        fields[module_field.at(idx)] = DEF_FIELD;
    }
    // 防止用户拼命点添加
    if ( !proto::instance()->new_module(DEF_FIELD,fields) )
    {
        QMessageBox box;
        box.setText( "dumplicate module" );
        box.exec();
        return;
    }

    int rows = ui->module_tbl->rowCount();
    ui->module_tbl->setRowCount( rows + 1 );
    for ( int idx = 0;idx < module_field.length();idx ++ )
    {
        ui->module_tbl->setItem( rows,idx,new QTableWidgetItem(DEF_FIELD) );
    }
}

void MainWindow::on_module_del_clicked(bool check)
{
    Q_UNUSED(check);

    int row = ui->module_tbl->currentRow();
    if ( row < 0 ) return;  /* no select */

    const QTableWidgetItem *item = ui->module_tbl->item( row,0 );
    QString cmd = item->text();

    ui->module_tbl->removeRow( row );
    proto::instance()->del_module( cmd );
}

void MainWindow::on_command_new_clicked(bool check)
{
    Q_UNUSED(check);

    int row = ui->module_tbl->currentRow();
    if ( row < 0 )
    {
        QMessageBox box;
        box.setText( "no module slected" );
        box.exec();
        return;
    }

    const QTableWidgetItem *item = ui->module_tbl->item( row,0 );
    QString module_cmd = item->text();

    class config *conf = config::instance();
    const QList<QString> &command_field = conf->get_command_field();

    Fields fields;
    for ( int idx =0;idx < command_field.length();idx ++ )
    {
        fields[command_field.at(idx)] = DEF_FIELD;
    }

    if ( !proto::instance()->new_command(module_cmd,DEF_FIELD,fields) )
    {
        QMessageBox box;
        box.setText( "no module selected or dumplicate command" );
        box.exec();
        return;
    }

    update_command_view( module_cmd);
}

void MainWindow::on_command_del_clicked(bool check)
{
    Q_UNUSED(check);

    int row = ui->module_tbl->currentRow();
    if ( row < 0 )
    {
        QMessageBox box;
        box.setText( "no module slected" );
        box.exec();
        return;
    }

    const QTableWidgetItem *item = ui->module_tbl->item( row,0 );
    QString cmd = item->text();

}

void MainWindow::update_command_view( QString &cmd )
{
    ui->command_tbl->clearContents();
    const CmdMap *cmd_map = proto::instance()->get_module_cmd( cmd );
    if ( !cmd_map ) return;

    class config *conf = config::instance();
    const QList<QString> &command_field = conf->get_command_field();

    int rows = 0;
    ui->command_tbl->setRowCount( cmd_map->size() );
    for ( CmdMap::ConstIterator itr = cmd_map->constBegin();itr != cmd_map->constEnd();itr ++ )
    {
        const Fields &fields = *(itr);
        for ( int idx = 0;idx < command_field.length();idx ++ )
        {
            Fields::ConstIterator cmd_itr = fields.find( command_field.at(idx) );
            if ( cmd_itr != fields.constEnd() )
            {
                ui->command_tbl->setItem( rows,idx,new QTableWidgetItem(*cmd_itr) );
            }
        }
        rows ++;
    }
}
