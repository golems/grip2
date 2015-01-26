/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Pete Vieira <pete.vieira@gatech.edu>
 * Date: Feb 2014
 *
 * Humanoid skeletonics Lab      Georgia Institute of Technology
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
 * \file GripTimeslice.h
 * \brief Class for the objects in the timeline
 */

#ifndef GRIP_TIMESLICE_H
#define GRIP_TIMESLICE_H

#include <dart/simulation/World.h>
#include <Eigen/Geometry>

/**
 * \class GripTimeslice GripTimeslice.h
 * \brief Class for storing a slice of the timeline. This contains a
 * time parameter and a world state parameter.
 */
class GripTimeslice
{
public:
    /**
     * \brief Default constructor for GripTimeslice object
     */
    GripTimeslice();

    /**
     * \brief Convenience constructor for GripTimeslice object
     * \param world World object from whic to create a GripTimeslice object
     */
    GripTimeslice(const dart::simulation::World &world);

    /**
     * \brief Destructs a GripTimeslice object
     */
    ~GripTimeslice();

    /**
     * \brief Sets the protected time variable of the GripTimeslice
     * \param time Time value
     */
    void setTime(double time);

    /**
     * \brief Sets the protected world state variable of the GripTimeslice
     * \param state State of the World
     */
    void setState(const Eigen::VectorXd &state);

    /**
     * \brief Gets the time stored in the GripTimeslice
     * \return Double value of the time
     */
    double getTime();

    /**
     * \brief Gets the state stored in the GripTimeslice
     * \return Eigen::VectorXd representing the world state
     */
    const Eigen::VectorXd& getState();

protected:
    double _time; ///< Timestamp for the world state
    Eigen::VectorXd _state; ///< State of the world at this time

}; // end class GripTimeslice

#endif // GRIP_TIMESLICE_H
