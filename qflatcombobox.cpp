#include "qflatcombobox.h"

#include <QStylePainter>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPalette>

QFlatComboBox::QFlatComboBox(QWidget *parent)
    : Super(parent)
    , flat_(true)
    , arrowAlignment_(Qt::AlignRight)
{
    setFlat(true);
    setAutoFillBackground(true);
    QPalette plt(palette());
    plt.setColor(QPalette::Background, Qt::white);
    setPalette(plt);
}

void QFlatComboBox::paintEvent(QPaintEvent *e)
{
    if (flat()) {
        QStylePainter painter(this);
        painter.setPen(palette().color(QPalette::Text));
        QStyleOptionComboBox opt;
        initStyleOption(&opt);
        QString displayText(opt.currentText);
        opt.currentText = "";
        painter.drawItemText(rect(), Qt::AlignCenter, palette(), true, displayText);
        const QRect rcOld(opt.rect);
        opt.rect = QStyle::alignedRect(Qt::LeftToRight, arrowAlignment(), QSize(16, rcOld.height()), rcOld);
        painter.drawPrimitive(QStyle::PE_IndicatorArrowDown, opt);
        opt.rect = rcOld;
        painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
    } else {
        Super::paintEvent(e);
    }
}

void QFlatComboBox::mousePressEvent(QMouseEvent *e)
{
    if (!isEditable() || !lineEdit()->rect().contains(e->pos())) {
        emit aboutToPullDown();
    }
    Super::mousePressEvent(e);
}

void QFlatComboBox::setFlat(bool flat)
{
    flat_ = flat;
}
void QFlatComboBox::setArrowAlignment(Qt::Alignment a)
{
    arrowAlignment_ = a;
}
