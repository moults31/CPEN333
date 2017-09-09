// OBJECT-ORIENTED CAR SIMULATOR
#include <iostream>
#include <string>
#include "State.h"
#include "Car.h"

int main()
{
    Car car1((std::string)"Mazda 3", 1600, 790, 0.61);
    Car car2((std::string)"Toyota Prius", 1450, 740, 0.58);
    
    Car *leader = new Car((std::string)"Mazda 3", 1600, 790, 0.61);
    
    // drive for 60 seconds
    double dt = 0.01;
    
    // GO!!!!
    car1.accelerate(true);
    car2.accelerate(true);
    for (double t = 0; t <= 60; t += dt)
    {
        car1.drive(dt);
        car2.drive(dt);
        
        // print out who's in the lead
        *leader = car1.getState().x > car2.getState().x ? car1 : car2;
        std::cout << leader->getModel() << std::endl;
    }
    
    return 0;
}
