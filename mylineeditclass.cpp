#include "mylineeditclass.h"

MyLineEditClass::MyLineEditClass(QWidget *parent) : QLineEdit(parent)
{
}

void MyLineEditClass::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    this->selectAll();
    _selectOnMousePress=true;
}

void MyLineEditClass::mousePressEvent(QMouseEvent *me)
{
    QLineEdit::mousePressEvent(me);
    if (_selectOnMousePress)
    {
        selectAll();
        _selectOnMousePress = false;
    }
    qDebug() << selectedText();
}

