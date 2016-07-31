#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QRadioButton>
#include <QGroupBox>

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
public slots:
    void module_add();
    void module_del();
    void module_sort( int column );
};

#endif // MAINWINDOW_H
