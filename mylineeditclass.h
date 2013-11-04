#ifndef MYLINEEDITCLASS_H
#define MYLINEEDITCLASS_H

#include <QtCore>
#include <QLineEdit>
#include <QFocusEvent>
class MyLineEditClass : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEditClass(QWidget *parent = 0);


signals:

public slots:

protected:
    void focusInEvent(QFocusEvent *e);
    void mousePressEvent(QMouseEvent *me);

    bool _selectOnMousePress;

};

#endif // MYLINEEDITCLASS_H
