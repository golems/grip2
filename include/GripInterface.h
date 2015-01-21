/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Shailesh Lohia <shailesh.lohia@gatech.edu>
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
 * \file GripInterface.h
 * \brief Grip wrapper class for interfacing to python (or other languages)
 * \author Jon Scholz & Pete Viera
 */

#ifndef GRIP_INTERFACE_H
#define GRIP_INTERFACE_H

// C++ Standard includes
#include <iostream>
#include <cstdio>
#include <string>
 #include <thread>
#include "GripMainWindow.h"

/**
 * \class GripInterface GripInterface.h
 * \brief 
 */
class GripInterface
{
public:
	int some_var;

    GripInterface();

    ~GripInterface();

    /** 
     * Creates a Grip QT instance and starts its event loop.
     */
    int _create(int argc, char **argv);

    /**
     * Spins up a grip instance in a separate thread, allowing 
     * the caller to proceed and interact with the running grip
     * instance through the interface methods below.  This is 
     * the standard use-case for this interface.
     */
    int run(int argc, char **argv);

	/**
     * \brief Load the scene and renders it. This function resets everything
     * on each load.
     * \param fileName Name of scene file to load
     * \return void
     */
    void loadScene(std::string sceneFileName);

    /**
     * \brief Loads a plugin file (.so) by name. When loaded, the plugin will be
     * added to the bottom tabs area.
     * \param pluginFileName String representing the path of the plugin
     */
    void loadPluginFile(std::string pluginFileName);

    static void show_usage(); //std::ostream &ostr);

    /** 
     * Renders the current scene and executes all drawing callbacks
     * in the render queue.
     */
    void render();

    /**
     * \brief Starts the simulation
     * \return void
     */
    void startSimulation();

    /**
     * \brief Stops the simulation
     * \return void
     */
    void stopSimulation();

    /**
     * \brief Simulates a single time step of the simulation
     * \return void
     */
    void simulateSingleStep();

    /**
     * \brief Returns the state of the world as std::vector, which can be
     *        coerced to numpy array in cython bindings.
     * \return An std (not Eigen) vector containing the current state
     */
    std::vector<double> getState();

    /**
     * \brief Sets the state of the world from an std::vector, which can be
     *        coerced from a numpy array in cython bindings.
     * \return 
     */
    void setState(const std::vector<double> &state);

    /**
     * \brief Pass-through call to world collision checker
     * \return Boolean, true if a collision is detected, false otherwise
     */
    bool checkCollision();

protected:
	QApplication * _app;
	GripMainWindow *_window;
    std::thread *_gripthread; // used for linux thread solution only
};

#endif // GRIP_INTERFACE_H
