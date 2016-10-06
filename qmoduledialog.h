#ifndef QMODULEDIALOG_H
#define QMODULEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class QModuleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QModuleDialog( QWidget *parent = Q_NULLPTR );
    void setData( QString old_id,QString old_comment);
    void getData( QString &id,QString &comment,QString &old_id );
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

#endif // QMODULEDIALOG_H
