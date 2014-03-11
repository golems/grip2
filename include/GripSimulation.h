
#ifndef GRIP_SIMULATION_H
#define GRIP_SIMULATION_H

#include <dart/simulation/World.h>
#include <QObject>
#include "mainwindow.h"
#include <QMutex>

using namespace dart;

class GripSimulation : public QObject
{
    Q_OBJECT

public:

    GripSimulation(bool debug);

    ~GripSimulation();

    void setWorld(simulation::World* world);

//signals:


public slots:
    virtual void startSimulation();
    virtual void stopSimulation();
    virtual void simulateTimeStep();
    virtual void simulateSingleTimeStep();

protected:
    /// World object received from creator that we need to simulate
    simulation::World* _world;

    /// Bool for whether or not we are simulating
    bool _simulating;

    bool _simulateOneFrame;

    /// Bool for whether or not to print debug output to standard error
    bool _debug;
    QMutex mMutex;
    QThread* _thread;

};

#endif // GRIP_SIMULATION_H
