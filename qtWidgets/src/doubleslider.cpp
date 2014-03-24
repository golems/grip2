#include <QtGui>
#include <cmath>
#include <iostream>

#include "doubleslider.h"

DoubleSlider::DoubleSlider (QWidget *parent)
  : QSlider(parent), dsvalue(0.0), min_value(-180.0), max_value(180.0), decimal_point(1)
  {
    //this->setRange(-1800,1800); //I cannot override settings here. It seems settings from the designer have priorities.

    connect (this, SIGNAL(valueChanged(int)), this, SLOT(setValueAndEmit(int)));
//    int min_value_integer = ceil((double)min_value*pow(10.0,(double)decimal_point));
//    int max_value_integer = ceil((double)max_value*pow(10.0,(double)decimal_point));
//    this->setRange(min_value_integer,max_value_integer);

}

double DoubleSlider::changeTOdouble(int intvalue)
{
    double temp = (double)intvalue;
    double doublevalue = temp*pow(10.0,-(double)decimal_point);

    return doublevalue;
}

int DoubleSlider::changeTOinteger(double doublevalue)
{
    double temp = doublevalue*pow(10.0,(double)decimal_point);
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

double DoubleSlider::getdsValue()
{
    double temp = (double)value();
    temp = temp*pow(10.0,-(double)decimal_point);
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

void DoubleSlider::setMinMaxDecimalValue(double minvalue, double maxvalue, int decimalvalue)
{
    min_value = minvalue;
    max_value = maxvalue;
    decimal_point = decimalvalue;
    int min_value_integer = ceil((double)min_value*pow(10.0,(double)decimal_point));
    int max_value_integer = ceil((double)max_value*pow(10.0,(double)decimal_point));
    //std::cerr << "base^decimal: " << (double)max_value*pow(10.0,(double)decimal_point) << std::endl;
    //std::cerr << "min value of the slider : " << min_value_integer <<" max value of the slider : " <<max_value_integer << std::endl;
    this->setRange(min_value_integer,max_value_integer);
}

double DoubleSlider::getMinValue()
{
    return min_value;
}

double DoubleSlider::getMaxValue()
{
    return max_value;
}

int DoubleSlider::getDecimalPoint()
{
    return decimal_point ;
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
