#ifndef QFLATCOMBOBOX_H
#define QFLATCOMBOBOX_H

#include <QComboBox>

/* copy from http://engineer-works.blogspot.com/2010/02/flat-qcombobox.html */

class QFlatComboBox : public QComboBox
{
    typedef QComboBox Super;

private:
    Q_OBJECT;

public:
    QFlatComboBox(QWidget *parent = 0);
    bool flat() const { return flat_; }
    void setFlat(bool flat);

    Qt::Alignment arrowAlignment() const { return arrowAlignment_; }
    void setArrowAlignment(Qt::Alignment a);

protected:
    virtual void paintEvent(QPaintEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);

signals:
    void aboutToPullDown();

private:
    bool flat_;
    Qt::Alignment arrowAlignment_;
};

#endif // QFLATCOMBOBOX_H
