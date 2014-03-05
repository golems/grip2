
#include <dart/simulation/World.h>
#include <QObject>

using namespace dart;

class GripSimulation : public QObject
{
    Q_OBJECT

public:

    GripSimulation();

    ~GripSimulation();

    void setWorld(simulation::World* world);

public slots:
    virtual void startSimulation();
    virtual void stopSimulation();
    virtual void doBeforeSimulationTimeStep();
    virtual void doAfterSimulationTimeStep();

protected:
    simulation::World* mWorld;
    bool mSimulating;
};
