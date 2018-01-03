#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "config.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}
