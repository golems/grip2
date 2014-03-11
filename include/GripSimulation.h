
#ifndef GRIP_SIMULATION_H
#define GRIP_SIMULATION_H

#include <dart/simulation/World.h>
#include <QObject>

class timeslice
{
public:
    double time;
    Eigen::VectorXd state;
};

class GripSimulation : public QObject
{
    Q_OBJECT

public:

    GripSimulation(bool debug);

    ~GripSimulation();

    void setWorld(dart::simulation::World* world);

public slots:
    virtual void startSimulation();
    virtual void stopSimulation();
    virtual void simulateTimeStep();
    virtual void simulateSingleTimeStep();

protected:
    void addWorldToTimeline(const dart::simulation::World& worldToAdd);

    /// World object received from creator that we need to simulate
    dart::simulation::World* _world;

    /// Bool for whether or not we are simulating
    bool _simulating;

    bool _simulateOneFrame;

    /// Bool for whether or not to print debug output to standard error
    bool _debug;

    /// Local thread to move object into
    QThread* _thread;

    /// Vector to hold timeslice objects for the slider and playback
    std::vector<timeslice> _timeline;

    /// Simulation time, taken from the dart::simulation::World object
    double _simulationDuration;
    double _simulationStartTime;

    /// Simulation time relative to realtime (ie. 1.0 is realtime. 0.5 is half the speed of realtime)
    double _simTimeRelToRealTime;

    double _prevTime;

};

#endif // GRIP_SIMULATION_H
