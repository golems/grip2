#include <QtGui>
#include <cmath>
#include <iostream>

#include "doubleslider.h"

DoubleSlider::DoubleSlider (QWidget *parent)
  : QSlider(parent)
{
    //this->setRange(-1800,1800); //I cannot override settings here. It seems settings from the designer have priorities.
    dsvalue = 0.0;
    connect (this, SIGNAL(valueChanged(int)), this, SLOT(setValueAndEmit(int)));

}

double DoubleSlider::changeTOdouble(int intvalue)
{
    double temp = (double)intvalue;
    double doublevalue = temp/10.0;

    return doublevalue;
}

int DoubleSlider::changeTOinteger(double doublevalue)
{
    double temp = doublevalue*10.0;
    int integervalue = ceil(temp);
    return integervalue;
}
void DoubleSlider::setdsValue(double valueindouble)
 {
     if (valueindouble != dsvalue) {
         dsvalue = valueindouble;
         }
     emit valueChanged(changeTOinteger(dsvalue));
}

double DoubleSlider::getdsvalue()
{
    double temp = (double)value();
    temp = temp/10.0;
    if (temp != dsvalue) {
        dsvalue = temp;
        }
    return dsvalue;
}

void DoubleSlider::setValueAndEmit(int slidervalue)
{
     double temp = changeTOdouble(slidervalue);
     if (temp != dsvalue) {
         dsvalue = temp;
         }
     //std::cout << slidervalue << " " << temp << " " << dsvalue << std::endl;
     emit dsvalueChanged(dsvalue);
}

void DoubleSlider::getValueAndEmit(double spinboxvalue)
{
     double temp = changeTOinteger(spinboxvalue);
     if (temp != value()) {
         setValue(temp);
         }
     //std::cout << slidervalue << " " << temp << " " << dsvalue << std::endl;
     emit valueChanged(temp);
}

/*
 class DoubleSlider : public QSlider
{
    Q_OBJECT

public:
         DoubleSlider(QWidget *parent = 0);
         double changeTOdouble(int intvalue);
         int changeTOinteger(double doublevalue);
         double getdsvalue();
         void setValueAndEmit(int slidervalue);
private:
         double dsvalue;

public slots:
         void setdsValue(double valueindouble);

signals:
         void dsvalueChanged(double newdsvalue);

};
*/
