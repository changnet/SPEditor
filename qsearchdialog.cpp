#include "qsearchdialog.h"
#include "ui_qsearchdialog.h"

QSearchDialog::QSearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSearchDialog)
{
    ui->setupUi(this);

    QStringList header;
    header << "module" << "command" << "field" << "content";
    ui->search_tbl->setColumnCount( header.length() );
    ui->search_tbl->setHorizontalHeaderLabels( header );
}

QSearchDialog::~QSearchDialog()
{
    delete ui;
}

void QSearchDialog::update_content( const QString &ctx,const QList<search_ctx> &content )
{
    setWindowTitle( QString("search: %1").arg(ctx) );

    show();

    ui->search_tbl->clearContents();
    if ( content.length()<= 0 ) return;

    ui->search_tbl->setRowCount( content.length() );
    for ( int idx = 0;idx < content.length();idx ++ )
    {
        const struct search_ctx &sctx = content.at(idx);
        ui->search_tbl->setItem( idx,0,new QTableWidgetItem(sctx._module) );
        ui->search_tbl->setItem( idx,1,new QTableWidgetItem(sctx._command) );
        ui->search_tbl->setItem( idx,2,new QTableWidgetItem(sctx._field) );
        ui->search_tbl->setItem( idx,3,new QTableWidgetItem(sctx._content) );
    }
}

void QSearchDialog::on_search_tbl_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    const QTableWidgetItem *module_item = ui->search_tbl->item( row,0 );
    const QTableWidgetItem *command_item = ui->search_tbl->item( row,1 );
    const QTableWidgetItem *field_item = ui->search_tbl->item( row,2 );

    emit result_double_click(
        module_item->text(),command_item->text(),field_item->text() );
}
