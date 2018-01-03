#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_module_new_clicked(bool check = false);
    void on_module_del_clicked(bool check = false);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
