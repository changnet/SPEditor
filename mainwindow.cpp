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

    ui->module_tbl->verticalHeader()->setVisible( false );
    ui->module_tbl->setSelectionBehavior( QAbstractItemView::SelectItems );
    ui->module_tbl->setSelectionMode( QAbstractItemView::SingleSelection );

    const QList<QString> &command_field = conf->get_command_field();
    ui->command_tbl->setColumnCount( command_field.length() );
    ui->command_tbl->setHorizontalHeaderLabels( conf->get_command_field() );

    ui->command_tbl->verticalHeader()->setVisible( false );
    ui->command_tbl->setSelectionBehavior( QAbstractItemView::SelectItems );
    ui->command_tbl->setSelectionMode( QAbstractItemView::SingleSelection );

    // AUTO-CONNECTION:
    // http://doc.qt.io/qt-5/designer-using-a-ui-file.html#widgets-and-dialogs-with-auto-connect
    // connect( ui->module_new,SIGNAL(clicked(bool)),this,SLOT(on_module_new(bool)) );

    connect( ui->module_tbl->itemDelegate(),
         SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),
         this,
         SLOT(module_tbl_closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint))
     );

    connect( ui->command_tbl->itemDelegate(),
         SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),
         this,
         SLOT(command_tbl_closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint))
     );
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

    int cmd_row = ui->module_tbl->currentRow();
    if ( cmd_row < 0 )
    {
        QMessageBox box;
        box.setText( "no command slected" );
        box.exec();
        return;
    }


    const QTableWidgetItem *item = ui->module_tbl->item( row,0 );
    const QTableWidgetItem *cmd_item = ui->command_tbl->item( cmd_row,0 );

    proto::instance()->del_command( item->text(),cmd_item->text() );
    ui->command_tbl->removeRow( cmd_row );
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

void MainWindow::on_module_tbl_itemSelectionChanged()
{
    _module_select.clear();
    _command_slect.clear();
    ui->statusBar->clearMessage();

    int row = ui->module_tbl->currentRow();
    if ( row < 0 ) return;

    const QTableWidgetItem *item = ui->module_tbl->item( row,0 );

    _module_select = item->text();
    QString field = "?";
    QList<QTableWidgetItem *> select_item = ui->module_tbl->selectedItems();
    if ( select_item.length() > 0 )
    {
        // as module_tbl is set in single select mode,only one item should be selected
        ui->detail_edt->setPlainText( select_item.at(0)->text() );
        int column = select_item.at(0)->column();

        const QList<QString> &module_field = config::instance()->get_module_field();
        if ( column >= 0 && column < module_field.length() )
        {
            field = module_field.at(column);
        }
    }

    update_command_view( _module_select );
    ui->statusBar->showMessage( QString("select:%1-%2").arg(_module_select).arg(field) );
}

void MainWindow::on_command_tbl_itemSelectionChanged()
{
    ui->statusBar->clearMessage();

    int row = ui->module_tbl->currentRow();
    if ( row < 0 ) return;

    const QTableWidgetItem *item = ui->module_tbl->item( row,0 );

    int cmd_row = ui->command_tbl->currentRow();
    if ( cmd_row < 0 ) return;

    const QTableWidgetItem *cmd_item = ui->command_tbl->item( cmd_row,0 );
    _command_slect = cmd_item->text();

    QString field = "?";
    QList<QTableWidgetItem *> select_item = ui->command_tbl->selectedItems();
    if ( select_item.length() > 0 )
    {
        // as module_tbl is set in single select mode,only one item should be selected
        ui->detail_edt->setPlainText( select_item.at(0)->text() );

        int column = select_item.at(0)->column();

        const QList<QString> &command_field = config::instance()->get_command_field();
        if ( column >= 0 && column < command_field.length() )
        {
            field = command_field.at(column);
        }
    }

    ui->statusBar->showMessage(
                QString("select:%1-%2-%3").arg(
                item->text()).arg(cmd_item->text()).arg(field) );
}

bool MainWindow::raw_update_module(const QString *ctx)
{
    QList<QTableWidgetItem *> select_item = ui->module_tbl->selectedItems();
    if ( select_item.length() <= 0 ) return false;

    QTableWidgetItem *item = select_item.at(0);
    int column = item->column();

    const QList<QString> &module_field = config::instance()->get_module_field();
    if ( column >= 0 && column < module_field.length() )
    {
        // 可能修改的就是module的key
        const QString &text = ctx ? *ctx : item->text();
        bool ok = proto::instance()->update_module(
            _module_select,module_field.at(column),text,0 == column );
        if ( !ok )
        {
            QMessageBox box;
            box.setText( proto::instance()->get_error_text() );
            box.exec();
        }

        return ok;
    }

    return false;
}

bool MainWindow::raw_update_command(const QString *ctx)
{
    int row = ui->module_tbl->currentRow();
    if ( row < 0 ) return false;

    QList<QTableWidgetItem *> select_item = ui->command_tbl->selectedItems();
    if ( select_item.length() <= 0 ) return false;

    // as module_tbl is set in single select mode,only one item should be selected
    QTableWidgetItem *item = select_item.at(0);
    int column = item->column();

    const QList<QString> &command_field = config::instance()->get_command_field();
    if ( column >= 0 && column < command_field.length() )
    {
        // 可能修改的就是module的key
        const QString &text = ctx ? *ctx : item->text();
        bool ok = proto::instance()->update_command(
            _module_select,_command_slect,command_field.at(column),text,0 == column );
        if ( !ok )
        {
            QMessageBox box;
            box.setText( proto::instance()->get_error_text() );
            box.exec();
        }

        return ok;
    }

    return false;
}

void MainWindow::on_submit_btn_clicked( bool check )
{
    Q_UNUSED(check);
    if ( _module_select.isEmpty() ) return;

    QTableWidget *widget = NULL;
    const QString &ctx = ui->detail_edt->toPlainText();
    if ( !_command_slect.isEmpty() )
    {
        if ( raw_update_command( &ctx ) )
        {
            widget = ui->command_tbl;
        }
    }
    else
    {
        if ( raw_update_module( &ctx ) )
        {
            widget = ui->module_tbl;
        }
    }

    if ( !widget ) return;

    QList<QTableWidgetItem *> select_item = widget->selectedItems();
    if ( select_item.length() <= 0 ) return;

    select_item.at(0)->setText( ctx );
}

void MainWindow::module_tbl_closeEditor(
        QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    Q_UNUSED(hint);
    Q_UNUSED(editor);

    bool ok = raw_update_module( NULL );
    if ( !ok )
    {
        QList<QTableWidgetItem *> select_item = ui->module_tbl->selectedItems();
        if ( select_item.length() <= 0 ) return;

        QTableWidgetItem *item = select_item.at(0);
        ui->module_tbl->editItem( item );
    }
}


void MainWindow::command_tbl_closeEditor(
        QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    Q_UNUSED(hint);
    Q_UNUSED(editor);

    bool ok = raw_update_command( NULL );
    if ( !ok )
    {
        QList<QTableWidgetItem *> select_item = ui->command_tbl->selectedItems();
        if ( select_item.length() <= 0 ) return;

        QTableWidgetItem *item = select_item.at(0);
        ui->command_tbl->editItem( item );
    }
}
