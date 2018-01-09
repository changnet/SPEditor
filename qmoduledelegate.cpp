#include "qmoduledelegate.h"

#include <QKeyEvent>

QModuleDelegate::QModuleDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

bool QModuleDelegate::eventFilter(QObject *obj, QEvent* e)
{
    if (QEvent::KeyPress == e->type())
    {
        QKeyEvent* key = static_cast<QKeyEvent*>( e );
        // tab and shift + tab
        if ( key->key()== Qt::Key_Tab || key->key() == Qt::Key_Backtab )
        {
            return true;
        }
    }

    return QStyledItemDelegate::eventFilter(obj,e);
}
