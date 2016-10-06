#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTreeWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void add_module_item( int id,QString comment );
private:
    Ui::MainWindow *ui;

    QLineEdit _search_text;
    QPushButton _search_button;

    QPushButton _module_add;
    QPushButton _module_del;
    QTableWidget _module_table;

    QPushButton _function_add;
    QPushButton _function_del;
    QTableWidget _function_table;

    QWidget _proto_group;

    QTreeWidget _node_tree;
    QWidget _server_group;
    QLabel _node_label;
    QPushButton _field_add;
    QPushButton _field_del;
public slots:
    void module_add();
    void module_del();
    void field_add();
    void moduleDoubleClicked(QTableWidgetItem *item);
    void initialize();
};

#endif // MAINWINDOW_H
