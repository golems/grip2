
#ifndef GRIP_SIMULATION_H
#define GRIP_SIMULATION_H

#include <dart/simulation/World.h>
#include <QObject>
#include "mainwindow.h"

/**
 * \class GripTimeslice
 * \brief Class for storing a slice of the timeline. This contains a
 * time parameter and a world state parameter.
 */
class GripTimeslice
{
public:
    double time; ///< Timestamp for the world state
    Eigen::VectorXd state; ///< State of the world at this time
};

/**
 * \class GripSimulation GripSimulation.h
 * \brief Class that handles the simulation loop, simulation timeline, timing
 * and communicates with the main GUI thread via slots and signals. This class
 * subclass QObject and is used by creating an instance of the class and then
 * moving it to a thread with the "moveToThread(QThread*)" function.
 */
class GripSimulation : public QObject
{
    Q_OBJECT

public:

    /**
     * \brief Constructs a GripSimulation object
     * \param parent Pointer to the parent widget. Default is 0
     * \param debug Flag for whether or not to output debug statements
     */
    GripSimulation(dart::simulation::World* world, MainWindow *parent=0, bool debug=false);

    /**
     * \brief Destroys the GripSimulation object
     */
    ~GripSimulation();

    /**
     * \brief Set the world object for the simulation
     * \param world A pointer to the dart::simulation::World to be simulated
     * \return void
     */
    void setWorld(dart::simulation::World* world);

    /**
     * \brief Reset the GripSimulation object
     * \return void
     */
    void reset();

signals:
    /**
     * \brief Signal to tell parent widget that the simulation loop is done. This is
     * used to make sure the simulation loop is done using the world object before the
     * parent deletes it.
     * \return void
     */
    void simulationStoppedd();

public slots:
    /**
     * \brief Slot that checks if there's a world object. If so, initialize time variables
     * and start the simulation loop. Otherwise exit.
     * \return void
     */
    virtual void startSimulation();

    /**
     * \brief Slot that sets the simulating flag to false to stop the simulation loop.
     * \return void
     */
    virtual void stopSimulation();

    /**
     * \brief Slot that simulates a single time step. This function calls itself, unless
     * the flag "simulateOneFrame" is set to true.
     * \return void
     */
    virtual void simulateTimeStep();

    /**
     * \brief Slot that sets the "simulateOneFrame" to true and simulate a time step
     * \return void
     */
    virtual void simulateSingleTimeStep();

protected:
    /**
     * \brief Adds a GripTimeslice to the timeline vector in order to store the
     * world time and state in the timeline for playback or movie saving at a later time.
     * \param worldToAdd dart::simulation::World object of which to save the time and state.
     * \return void
     */
    void addWorldToTimeline(const dart::simulation::World& worldToAdd);

    /// World object received from creator that we need to simulate
    dart::simulation::World* _world;

    /// Local thread to move object into
    QThread* _thread;

    /// Vector to hold timeslice objects for the slider and playback
    std::vector<GripTimeslice> _timeline;

    double _simulationDuration;  ///< Simulation time in realtime
    double _simulationStartTime; ///< Initial system clock time when simulation started
    double _simTimeRelToRealTimeInstantaneous; ///< Simulation time relative to realtime (ie. 1.0 is realtime. 0.5 is half the speed of realtime)
    double _simTimeRelToRealTimeOverall; ///< Simulation time relative to realtime (ie. 1.0 is realtime. 0.5 is half the speed of realtime)
    double _prevTime; ///< Real time on the last time step

    bool _simulating; /// Bool for whether or not we are simulating
    bool _simulateOneFrame; /// Bool for whether or not to simulate only one frame
    bool _debug; /// Bool for whether or not to print debug output to standard error
};

#endif // GRIP_SIMULATION_H
