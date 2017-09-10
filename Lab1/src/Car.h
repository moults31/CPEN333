#ifndef CAR
#define CAR

#include <string>
#include "State.h"

class Car
{
protected:
    // Properties
    std::string model;
    double mass;
    double engine_force;
    double drag_area;
    bool accel_on;
    State state;
    
public:
    // Constructor
    Car(std::string argmodel, double argmass, double argengine_force, double argdrag_area) :
            model(argmodel), mass(argmass),
    engine_force(argengine_force), drag_area(argdrag_area) {};
    virtual ~Car() {};
    
    // Methods
    std::string getModel(void);
    double getMass(void);
    void accelerate(bool on);
    virtual void drive(double dt);
    State * getState(void);
};


// Derived classes of Car

class Prius : public Car
{
public:
    // Constructor
    Prius() : Car((std::string)"Toyota Prius", 1450, 740, 0.58) {}
};

class Mazda3 : public Car
{
public:
    // Constructor
    Mazda3() : Car((std::string)"Mazda 3", 1600, 790, 0.61) {}
};

class Tesla3 : public Car
{
public:
    // Constructor
    Tesla3() : Car((std::string)"Tesla Model 3", 1400, 850, 0.5) {}
};

class Herbie : public Car
{
public:
    // Constructor
    Herbie() : Car((std::string)"Herbie", 1500, 750, 0.55) {};
    void drive(double dt);
};

#endif
