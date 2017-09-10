#include <vector>
#include <string>
#include <iostream>
#include "Car.h"
#include "physics.h"
#include "State.h"

#define FLEETSIZE 100 // cars
#define SIMLENGTH 300 // seconds
#define MAXSPEED 27.8 // m/s

int main(void)
{
    std::vector<Car *> cars;

    // Add 100 cars to the fleet
    for(int i=0; i<FLEETSIZE/4; i++)
    {
        cars.push_back(new Prius);
        cars.push_back(new Tesla3);
        cars.push_back(new Mazda3);
        cars.push_back(new Herbie);
    }
    
    // Tell cars to begin
    for (Car *car : cars)
    {
        car->accelerate(true);
    }
    
    int dt = 5;
    
    for(int t=0; t*dt<300; t++)
    {
        // Update car states
        for (Car *car : cars)
        {
            if(car->getState()->v > MAXSPEED && car->getModel() != "Herbie") car->accelerate(false);
            else    car->accelerate(true);
            
            car->drive(dt);
        }
    }
    
    // Print out car states and free their memory
    for (Car *car : cars)
    {
        std::cout << car->getModel() << " is at " << car->getState()->x << std::endl;
        delete car;
        car = nullptr;
    }
    
    return 0;
}
