#ifndef DOUBLESPINBOX_H
#define DOUBLESPINBOX_H

#include <QSlider>
 class DoubleSlider : public QSlider
{
    Q_OBJECT

public:
         DoubleSlider(QWidget *parent = 0);
         double changeTOdouble(int intvalue);
         int changeTOinteger(double doublevalue);
         double getdsvalue();
private:
         double dsvalue;

public slots:
         void setdsValue(double valueindouble);
         void setValueAndEmit(int slidervalue);

signals:
         void dsvalueChanged(double newdsvalue);

};

#endif // DOUBLESPINBOX_H
