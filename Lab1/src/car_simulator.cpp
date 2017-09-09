// CAR_SIMULATOR
#include <iostream>
#include "physics.h"
#include "State.h"

#define RHO_AIR 1.225

int main()
{
    // read in car mass
    std::cout << "Enter the mass of the car (kg): ";
    double mass;
    std::cin >> mass;
    
    // read in engine force
    std::cout << "Enter the net force of the engine (N): ";
    double engine_force;
    std::cin >> engine_force;
    
    // read in drag area coefficient
    std::cout << "Enter the car's drag area (m^2): ";
    double drag_area;
    std::cin >> drag_area;
    
    // read in time step
    std::cout << "Enter the simulation time step (s): ";
    double dt;
    std::cin >> dt;
    
    // read in total number of simulation steps
    std::cout << "Enter the number of time steps (int): ";
    int N;
    std::cin >> N;
    
    // initialize the car's state
    double x = 0;  // initial position
    double v = 0;  // initial velocity
    double a = 0;  // initial acceleration
    double t = 0;  // initial time
    
    // keep track of drag force
    double fd = 0;
    
    // run the simulation
    for (int i=0; i<N; ++i)
    {
        // Update drag force
        fd = 0.5*RHO_AIR*drag_area*v*v;
        
        // Compute updated state (x,v,a,t)
        a = physics::compute_acceleration(engine_force - fd, mass);
        v = physics::compute_velocity(v, a, dt);
        x = physics::compute_position(x, v, dt);
        t += dt;  // increment time
        
        // print the time and current state
        std::cout << "t: " << t << ", a: " << a
        << ", v: " << v << ", x: " << x << ", fd: " << fd << std::endl;
    }
    
    State::State s1;
    std::cout << s1 << std::endl;
    State::State s2(1.0, 2.0, 3.0, 4.0);
    std::cout << s2 << std::endl;
    
    return 0;
}
