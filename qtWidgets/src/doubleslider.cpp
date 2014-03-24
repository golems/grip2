/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Michael X. Grey <mxgrey@gatech.edu>
 * Date: Jan 2014
 *
 * Humanoid Robotics Lab      Georgia Institute of Technology
 * Director: Mike Stilman     http://www.golems.org
 *
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *   * Neither the name of the Humanoid Robotics Lab nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

// Qt includes
#include <QtGui>

// C++ Standard includes
#include <cmath>
#include <iostream>

// Local includes
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
