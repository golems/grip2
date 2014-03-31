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

/**
 * \file DoubleSlider.h
 * \brief Class that creates a double valued slider
 */

#ifndef DOUBLESPINBOX_H
#define DOUBLESPINBOX_H

#include <QSlider>

/**
 * \class DoubleSlider DoubleSlider.h
 * \brief Class that creates a double valued slider
 */
class DoubleSlider : public QSlider
{
    /// MetaObject macro for using signals and slots
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

}; // end DoubleSlider class

#endif // DOUBLESPINBOX_H
