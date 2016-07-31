#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qnumbertablewidgetitem.h"

#include <QDebug>
#include <QCheckBox>
#include <QComboBox>
#include <QStringList>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->resize( 1024,640 );
    setWindowTitle( "Stream Protocol Editor" );

    _search_button.setText( "search" );
    _module_add.setText( "new" );
    _module_del.setText( "delete" );
    _function_add.setText( "new" );
    _function_del.setText( "delete" );
    _node_label.setText( "no proto select" );
    _node_label.setAlignment( Qt::AlignCenter );
    _field_add.setText( "new" );
    _field_del.setText( "delete" );

    _module_table.setColumnCount( 2 );
    QStringList module_labels;
    module_labels << "module id" << "comments";
    _module_table.setHorizontalHeaderLabels( module_labels );
    _module_table.setSortingEnabled( true );
    _module_table.verticalHeader()->setVisible( false );
    _module_table.setSelectionBehavior( QAbstractItemView::SelectRows );
    _module_table.setSelectionMode( QAbstractItemView::SingleSelection );

    QHeaderView *module_header = _module_table.horizontalHeader();
    module_header->setSectionsClickable( true );
    module_header->setStretchLastSection( true );
    module_header->setSortIndicator( 0,Qt::AscendingOrder );
    connect( module_header,SIGNAL(sectionClicked(int)),this,SLOT(module_sort(int)) );

    _function_table.setColumnCount( 3 );
    QStringList function_labels;
    function_labels << "function id" << "S&C" << "comments";
    _function_table.setHorizontalHeaderLabels( function_labels );
    _function_table.verticalHeader()->setVisible( false );
    _function_table.setSortingEnabled( true );
    _function_table.horizontalHeader()->setStretchLastSection( true );
    _function_table.setSelectionBehavior( QAbstractItemView::SelectRows );
    _function_table.setSelectionMode( QAbstractItemView::SingleSelection );

    QRadioButton *proto_s2c = new QRadioButton();
    QRadioButton *proto_c2s = new QRadioButton();
    QRadioButton *proto_all = new QRadioButton();
    proto_s2c->setText( "S2C" );
    proto_c2s->setText( "C2S" );
    proto_all->setText( "All" );
    proto_all->setChecked( true );

    QStringList tree_labels;
    tree_labels << "field" << "data type" << "optional" << "comments";
    _node_tree.setHeaderLabels( tree_labels );

    QRadioButton *srv_router = new QRadioButton( "Router" );
    QRadioButton *srv_player = new QRadioButton( "Player" );
    QRadioButton *srv_world  = new QRadioButton( "world"  );
    QHBoxLayout *srv_layout = new QHBoxLayout();
    srv_layout->addWidget( srv_router );
    srv_layout->addWidget( srv_player );
    srv_layout->addWidget( srv_world  );
    srv_layout->addWidget( &_field_add );
    srv_layout->addWidget( &_field_del );
    _server_group.setLayout( srv_layout );

    QHBoxLayout *proto_layout = new QHBoxLayout();
    proto_layout->addWidget( proto_s2c );
    proto_layout->addWidget( proto_c2s );
    proto_layout->addWidget( proto_all );
    _proto_group.setLayout( proto_layout );

    QHBoxLayout *search_layout = new QHBoxLayout();
    search_layout->addWidget( &_search_text );
    search_layout->addWidget( &_search_button );

    QHBoxLayout *module_button_layout = new QHBoxLayout();
    module_button_layout->addWidget( &_module_add );
    module_button_layout->addWidget( &_module_del );

    QVBoxLayout *module_layout = new QVBoxLayout();
    module_layout->addLayout( module_button_layout );
    module_layout->addWidget( &_module_table );

    QHBoxLayout *function_button_layout = new QHBoxLayout();
    function_button_layout->addWidget( &_proto_group );
    function_button_layout->addWidget( &_function_add );
    function_button_layout->addWidget( &_function_del );

    QVBoxLayout *function_layout = new QVBoxLayout();
    function_layout->addLayout( function_button_layout );
    function_layout->addWidget( &_function_table );

    QHBoxLayout *table_layout = new QHBoxLayout();
    table_layout->addLayout( module_layout );
    table_layout->addLayout( function_layout );

    QVBoxLayout *node_layout = new QVBoxLayout();
    node_layout->addWidget( &_node_label );
    node_layout->addWidget( &_server_group );
    node_layout->addWidget( &_node_tree );

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout( search_layout );
    vbox->addLayout( table_layout );

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addLayout( vbox );
    hbox->addLayout( node_layout );

    QWidget *widget = new QWidget();
    widget->setLayout( hbox );

    this->setCentralWidget( widget );

    connect( &_module_add,SIGNAL(clicked()),this,SLOT(module_add()) );
    connect( &_module_del,SIGNAL(clicked()),this,SLOT(module_del()) );
    connect( &_field_add ,SIGNAL(clicked()),this,SLOT(field_add() ) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::module_add()
{
    int row_count = _module_table.rowCount();

    /* before adding new module,you must finish last module */
    if ( row_count > 0 )
    {
        QTableWidgetItem *item = _module_table.item( row_count - 1,0 );
        if ( !item || item->text() == "" ) return;
    }

    _module_table.setRowCount( row_count + 1 );
    _module_table.selectRow( row_count );

    QNumberTableWidgetItem *item = new QNumberTableWidgetItem();
    _module_table.setItem( row_count,0,item );
    _module_table.editItem( item );
}

void MainWindow::module_del()
{
    int row = _module_table.currentRow();
    if ( row < 0 ) return;  /* no select */

    _module_table.removeRow( row );
}

void MainWindow::module_sort(int column)
{
    Q_UNUSED(column);
    _module_table.sortByColumn( 0,Qt::AscendingOrder );
}

void MainWindow::field_add()
{
    QStringList types;
    types << "int8" << "uint8" << "int16";
    QComboBox *datatype = new QComboBox();
    datatype->addItems( types );

    QCheckBox *optional = new QCheckBox( "optonal" );

    QTreeWidgetItem *item = new QTreeWidgetItem();
    for ( int i = 0;i < 5;i ++ )
    {
        QStringList test;
        test << "1" << "22" << "33";
        QTreeWidgetItem *child = new QTreeWidgetItem( test );
        child->setFlags( item->flags() | Qt::ItemIsEditable );

        item->addChild( child );
    }

    _node_tree.addTopLevelItem( item );
    _node_tree.setItemWidget( item,1,datatype );
    _node_tree.setItemWidget( item,2,optional );

    item->setFlags( item->flags() | Qt::ItemIsEditable );
    _node_tree.editItem( item );
}
