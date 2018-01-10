#ifndef QSEARCHDIALOG_H
#define QSEARCHDIALOG_H

#include <QList>
#include <QDialog>

namespace Ui {
class QSearchDialog;
}

struct search_ctx
{
    QString _module;
    QString _command;
    QString _field;
    QString _content;
};

class QSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QSearchDialog(QWidget *parent = 0);
    ~QSearchDialog();

    void update_content(const QString &ctx, const QList<search_ctx> &content );
signals:
    void result_double_click(const QString &module,const QString &command,const QString &field);
private:
    Ui::QSearchDialog *ui;
};

#endif // QSEARCHDIALOG_H
