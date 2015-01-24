

#include <dart/simulatin/World.h>
#include "GripTools.h"

Eigen::VectorXd getWorldState( dart::simulation::World* _world ) {

  Eigen::VectorXd worldState, skelState;

  for( size_t i = 0; i < world->getNumSkeletons(); ++i ) {
      // Get position and velocity of each skeleton
      skelState = _world->getSkeleton(i)->getState();
      for( size_t j = 0; j < skelState.size(); ++j ) {
        worldState.push_back( skelState(j) );
      }
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
