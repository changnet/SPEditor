#ifndef QFUNCTIONDIALOG_H
#define QFUNCTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class QFunctionDialog : public QDialog
{
    Q_OBJECT
public:
    QFunctionDialog();
private:
    QLineEdit _id;
    QLineEdit _comment;

    QPushButton _ok;
    QPushButton _cancel;

    QString _old_id;
    QString _old_comment;
public slots:
    void onOk();
};

#endif // QFUNCTIONDIALOG_H
