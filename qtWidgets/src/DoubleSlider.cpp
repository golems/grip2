/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Sungmoon Joo <sungmoon.joo@gmail.com>
 * Date: Feb 2014
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
#include "DoubleSlider.h"

// DoubleSlider is used to connect an integer valued slider to double valued doublespinbox

DoubleSlider::DoubleSlider (QWidget *parent)
  : QSlider(parent), dsvalue(0.0), min_value(-180.0), max_value(180.0), decimal_point(1)
{
 
    connect (this, SIGNAL(valueChanged(int)), this, SLOT(setValueAndEmit(int)));

}

double DoubleSlider::changeTOdouble(int intvalue)
{

    double doublevalue = ((double)intvalue)*pow(10.0,-(double)decimal_point);

    return doublevalue;
}

int DoubleSlider::changeTOinteger(double doublevalue)
{

    int integervalue = round(doublevalue*pow(10.0,(double)decimal_point)); 
    
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
    double temp = ((double)value())*pow(10.0,-(double)decimal_point);
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

     emit dsvalueChanged(dsvalue);
}

void DoubleSlider::getValueAndEmit(double spinboxvalue)
{
     double temp = changeTOinteger(spinboxvalue);
     if (temp != value()) {
         setValue(temp);
         }

     emit valueChanged(temp);
}

void DoubleSlider::setMinMaxDecimalValue(double minvalue, double maxvalue, int decimalvalue)
{
    min_value = minvalue;
    max_value = maxvalue;
    decimal_point = decimalvalue;
    int min_value_integer = round((double)min_value*pow(10.0,(double)decimal_point));
    int max_value_integer = round((double)max_value*pow(10.0,(double)decimal_point));

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

