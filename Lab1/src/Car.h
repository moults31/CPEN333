#ifndef CAR
#define CAR

#include <string>
#include "State.h"

class Car
{
private:
    // Properties
    std::string model;
    double mass;
    double engine_force;
    double drag_area;
    State state;
    
public:
    // Constructor
    Car(std::string argmodel, double argmass, double argengine_force, double argdrag_area) :
            model(argmodel), mass(argmass),
            engine_force(argengine_force), drag_area(argdrag_area)
    {
        state.set(0, 0, 0 ,0);
    };
    
    // Methods
    std::string getModel(void);
    double getMass(void);
    void accelerate(bool on);
    void drive(double dt);
    State getState(void);
};

#endif
