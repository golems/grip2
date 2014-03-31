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


#include "PlaybackWidget.h"
#include <cmath>
PlaybackWidget::PlaybackWidget (MainWindow *parent)
 : QWidget(parent), ui(new Ui::PlaybackWidget)
{
    _parent = parent;
    ui->setupUi(this);
    connect(ui->sliderMain, SIGNAL(sliderMoved(int)), _parent, SLOT(slotSetWorldFromPlayback(int)));
    connect(ui->buttonPlay, SIGNAL(released()), _parent, SLOT(slotPlaybackStart()));
    connect(ui->buttonPause, SIGNAL(released()), _parent, SLOT(slotPlaybackPause()));
    connect(ui->buttonReverse, SIGNAL(released()), _parent, SLOT(slotPlaybackReverse()));
    connect(ui->buttonBeginning, SIGNAL(released()), _parent, SLOT(slotPlaybackBeginning()));
}

PlaybackWidget::~PlaybackWidget()
{
}

void PlaybackWidget::setSliderValue(int value)
{
    ui->sliderMain->setValue(value);
}

int PlaybackWidget::getSliderValue()
{
    return ui->sliderMain->value();
}

void PlaybackWidget::slotUpdateSliderMinMax(int max)
{
    ui->sliderMain->setMinimum(0);
    ui->sliderMain->setMaximum(max);
}

void PlaybackWidget::slotSetTimeDisplays(double sim_time, double rel_time)
{
    ui->editSimTime->setText(QString("%1").arg(sim_time));
    ui->editSimRelTime->setText(QString("%1").arg(round(rel_time*100)/100)); // decimal point 2
}
