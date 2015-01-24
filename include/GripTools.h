/**
 * @file GripTools.h
 */
#pragma once

#include <Eigen/Core>

namespace dart {
  namespace simulation {
    class World;
  }
}

Eigen::VectorXd getWorldState( const dart::simulation::World &_world );
void setWorldState( dart::simulation::World* _world,
		     Eigen::VectorXd _state );
