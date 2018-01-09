#ifndef QPROTODELEGATE_H
#define QPROTODELEGATE_H

#include <QStyledItemDelegate>

class QModuleDelegate : public QStyledItemDelegate
{
public:
    QModuleDelegate(QObject *parent = Q_NULLPTR);
protected:
    bool eventFilter(QObject *obj, QEvent* e);
};

#endif // QPROTODELEGATE_H
