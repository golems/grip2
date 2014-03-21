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
         double getdsValue(); //get doubleslider value
         void setMinMaxDecimalValue(double minvalue, double maxvalue, int decimalvalue);
         double getMinValue();
         double getMaxValue();
         int getDecimalPoint();
private:
         double dsvalue; //doubleslider value
         double max_value; //max value of the doubeslider
         double min_value; //min value of the doubleslider
         int    decimal_point; //decimal poit of the doublevalue: ex. decimal point = 1 --> x.x

public slots:
         void setdsValue(double valueindouble); //set doubleslider value
         void setValueAndEmit(int slidervalue);
         void getValueAndEmit(double spinboxvalue);

signals:
         void dsvalueChanged(double newdsvalue); //emit signal "doubleslider value changed"

};

#endif // DOUBLESPINBOX_H
