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
 * \file PlaybackWidget.h
 * \brief Class for simulation and kinematic playback
 */

#ifndef PLAYBACK_WIDGET_H
#define PLAYBACK_WIDGET_H

// Local includes
namespace Ui { class PlaybackWidget; }

#include "grip/MainWindow.h"

/**
 * \class PlaybackWidget PlaybackWidget.h
 * \brief Class for simulation and kinematic playback
 */
class PlaybackWidget : public QWidget {

    /// MetaObject macro for using signals and slots
    Q_OBJECT

public:
    /**
     * \brief Constructs a PlaybackWidget object
     * \param parent Object that creates the PlaybackWidget object
     */
    PlaybackWidget(MainWindow *parent);

    /**
     * \brief Destructs this PlaybackWidget object
     */
    ~PlaybackWidget();

    /**
     * \brief Resets the widgets
     */
    void reset();

    /**
     * \brief Sets the value of the slider object
     * \param value Value to set slider to
     * \return void
     */
    void setSliderValue(int value);

    /**
     * \brief Gets the current value of the slider
     * \return Integer value of the slider
     */
    int getSliderValue();

    /**
     * \brief returns the playback speed from the spinbox
     * \return int
     */
    size_t getPlaybackSpeed();

    /// UI object which holds all the widgets
    Ui::PlaybackWidget *ui;

public slots:

    /**
     * \brief Updates the min and max of the slider widget
     * \param min The new minimum value of the slider
     * \param max The new maximum value of the slider
     * \return void
     */
    void slotUpdateSliderMinMax(int min, int max);

    /**
     * \brief Updates the time displays of the playback widget
     * \param sim_time Current simulation time for playback
     * \param rel_time Simulation time relative to real time
     * \return void
     */
    void slotSetTimeDisplays(double sim_time, double rel_time);

private:
    /// Parent's base class to access its slots and signals
    MainWindow *_parent;

}; // end class PlaybackWidget

#endif // PLAYBACK_WIDGET_H
