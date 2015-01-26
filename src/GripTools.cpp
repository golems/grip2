/**
 * @file GripTools.cpp
 */

#include <dart/simulation/World.h>
#include <dart/dynamics/Skeleton.h>
#include "GripTools.h"

Eigen::VectorXd getWorldState( const dart::simulation::World &_world ) {

  std::vector<double> wS;
  Eigen::VectorXd worldState, skelState;

  for( size_t i = 0; i < _world.getNumSkeletons(); ++i ) {
      // Get position and velocity of each skeleton
      skelState = _world.getSkeleton(i)->getState();
      for( size_t j = 0; j < skelState.size(); ++j ) {
        wS.push_back( skelState(j) );
      }
  }

  worldState.resize(wS.size());
  for( unsigned int i = 0; i < wS.size(); ++i ) {
    worldState(i) = wS[i];
  }
  return worldState;
}


void setWorldState( dart::simulation::World* _world,
		     Eigen::VectorXd _state ) {

  for( size_t i = 0; i < _world->getNumSkeletons(); ++i ) {
    int start = 2*( _world->getIndex(i) );
    int size = 2*( _world->getSkeleton(i)->getNumDofs() );
    _world->getSkeleton(i)->setState( _state.segment( start, size ) );
  }

}
