
#include <dart/simulation/World.h>
#include <QObject>
#include "mainwindow.h"

using namespace dart;

class GripSimulation : public QObject
{
    Q_OBJECT

public:

    GripSimulation(MainWindow* gripWindow, bool debug=false);

    ~GripSimulation();

    void setWorld(simulation::World* world);

//signals:
//    void signalDoBeforeSimulationTimeStep();
//    void signalDoAfterSimulationTimeStep();

public slots:
    virtual void startSimulation();
    virtual void stopSimulation();
    virtual void simulateTimeStep();

protected:
    /// World object received from creator that we need to simulate
    simulation::World* _world;

    /// Bool for whether or not we are simulating
    bool _simulating;

    /// Bool for whether or not to print debug output to standard error
    bool _debug;

    QThread* _thread;

    MainWindow* _gripWindow;



};
