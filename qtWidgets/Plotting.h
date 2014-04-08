/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Can Erdogan <cerdogan3@gatech.edu>
 * Date: Apr 2014
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
 * \file Plotting.h
 * \brief Fields to share information among plugins and the plotting plugin
 */

#ifndef PLOTTING_H
#define PLOTTING_H

#include <map>
#include <vector>
#include <string>
#include <pthread.h>

#define NUM_PLOTTING_POINTS 101

/// The structure that a plugin uses to represent its data
struct PluginStream {
	std::vector <double> vals;				///< The circular buffer for that 
	size_t index;											///< Points to the latestdata point
	std::string label;								///< Label for the graph
	double maxVal, minVal;						///< The range for the plot
	PluginStream(const std::string& l, double mi, double ma) 
			: label(l), index(0), minVal(mi), maxVal(ma) {
		for(size_t i = 0; i < NUM_PLOTTING_POINTS; i++) vals.push_back(0.0);
	}
};

extern pthread_mutex_t plottingMutex;
extern std::map <void*, PluginStream*> pluginStreams;		///< The map from plots to streams

#endif // PLOTTING_H 

