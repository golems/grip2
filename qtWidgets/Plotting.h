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
#include <Eigen/Dense>

#define NUM_PLOTTING_POINTS 101

/// Type of markers
enum PlotMarkerType {
	marker_point = 0,
	marker_circle,
	marker_xmark,
	marker_plus,
	marker_star
};

/// The structure that a plugin uses to represent its data
struct PluginStream {
	PluginStream(const std::string& l, double mi, double ma, Eigen::Vector3i* c = NULL, PlotMarkerType* mark = NULL)
	: index(0), label(l), color(c), marker(mark), minVal(mi), maxVal(ma)
	{
		for(size_t i = 0; i < NUM_PLOTTING_POINTS; i++) vals.push_back(0.0);
	}

	std::vector <double> vals;				///< The circular buffer for that 
	size_t index;											///< Points to the latestdata point
	std::string label;								///< Label for the graph
	Eigen::Vector3i* color;
	PlotMarkerType* marker;
	double minVal, maxVal;						///< The range for the plot
};

/// Represents a drawing plugin
struct Plotter {
	Plotter () {};
	pthread_mutex_t plottingMutex;							///< To regulate the data access
	std::vector <PluginStream*> streams;	///< The map from plots to streams
	virtual void update () = 0;
};

/// Sets of plotters available to the pluggins
extern std::vector <Plotter*> plotters;		

#endif // PLOTTING_H 

