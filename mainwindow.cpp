#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStringList>

#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->resize( 860,640 );
    setWindowTitle( "Stream Protocol Editor" );

    _search_button.setText( "search" );
    _module_add.setText( "new" );
    _module_del.setText( "delete" );
    _function_add.setText( "new" );
    _function_del.setText( "delete" );

    _module_table.setRowCount( 1 );
    _module_table.setColumnCount( 2 );
    QStringList module_labels;
    module_labels << "module id" << "comments";
    _module_table.setHorizontalHeaderLabels( module_labels );
    _module_table.verticalHeader()->setVisible( false );
    _module_table.horizontalHeader()->setStretchLastSection( true );
    _module_table.setSelectionBehavior( QAbstractItemView::SelectRows );

    _function_table.setColumnCount( 2 );
    _function_table.setRowCount( 1 );
    QStringList function_labels;
    function_labels << "function id" << "comments";
    _function_table.setHorizontalHeaderLabels( function_labels );
    _function_table.verticalHeader()->setVisible( false );
    _function_table.horizontalHeader()->setStretchLastSection( true );
    _function_table.setSelectionBehavior( QAbstractItemView::SelectRows );

    QRadioButton *proto_s2c = new QRadioButton();
    QRadioButton *proto_c2s = new QRadioButton();
    QRadioButton *proto_all = new QRadioButton();
    proto_s2c->setText( "S2C" );
    proto_c2s->setText( "C2S" );
    proto_all->setText( "All" );
    proto_all->setChecked( true );

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

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout( search_layout );
    vbox->addLayout( table_layout );

    QWidget *widget = new QWidget();
    widget->setLayout( vbox );

    this->setCentralWidget( widget );
}

MainWindow::~MainWindow()
{
    delete ui;
}
