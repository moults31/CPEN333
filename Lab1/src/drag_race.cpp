// OBJECT-ORIENTED CAR SIMULATOR
#include <iostream>
#include <string>
#include "State.h"
#include "Car.h"

#define QUARTERMILE 402.3

int main()
{
    Car car1((std::string)"Mazda 3", 1600, 790, 0.61);
    Car car2((std::string)"Toyota Prius", 1450, 740, 0.58);
    
    Car *leader = new Car((std::string)"Mazda 3", 1600, 790, 0.61);
    
    // Define time step size in seconds
    double dt = 0.01;
    
    // GO!!!!
    car1.accelerate(true);
    car2.accelerate(true);
    do
    {
        if(car1.getState()->x <= QUARTERMILE) car1.drive(dt);
        if(car1.getState()->x <= QUARTERMILE) car2.drive(dt);
        
        // keep track of leader at each time step
        *leader = car1.getState()->x > car2.getState()->x ? car1 : car2;
        
        std::cout << car1.getModel() << " is at " << car1.getState()->x << std::endl;
        std::cout << car2.getModel() << " is at " << car2.getState()->x << std::endl << std::endl;
        
    } while(car2.getState()->x < QUARTERMILE || car1.getState()->x < QUARTERMILE);
    
    // print out winner
    std::cout << "Winner is " << leader->getModel() << " finishing in "
              << leader->getState()->t << " seconds" << std::endl;
    
    return 0;
}
