#ifndef QNUMBERTABLEWIDGETITEM_H
#define QNUMBERTABLEWIDGETITEM_H

#include <QDebug>
#include <QTableWidgetItem>

class QNumberTableWidgetItem : public QTableWidgetItem
{
public:
    QNumberTableWidgetItem();
    bool operator <(const QTableWidgetItem &other) const
    {
        if ( other.text().isEmpty() ) return true; /* new item insert in end */
        return text().toInt() < other.text().toInt();
    }
};

#endif // QNUMBERTABLEWIDGETITEM_H
