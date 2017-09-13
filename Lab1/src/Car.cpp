#include <iostream>
#include <stdlib.h>
#include <string>
#include "Car.h"
#include "physics.h"

#define RHO_AIR 1.225

// Return a string containing the model of the car
std::string Car::getModel(void)
{
    return model;
}

// Return the mass of the car
double Car::getMass(void)
{
    return mass;
}

// Set initial acceleration if on is true
void Car::accelerate(bool on)
{
    accel_on = on;
}

// Update car's state after one time step
void Car::drive(double dt)
{
    //std::cout << state.v << " " << accel_on << std::endl;
    // Update drag force
    double fd = 0.5*RHO_AIR*drag_area*state.v*state.v;
    
    // Compute updated state (x,v,a,t)
    double acc = accel_on ? physics::compute_acceleration(engine_force - fd, mass) : 0;
    double vel = physics::compute_velocity(state.v, acc, dt);
    double pos = physics::compute_position(state.x, vel, dt);
    double time = state.t + dt;
    
    // Store results in car's state member
    state.set(pos, vel, acc, time);
}

// Return a pointer to the car's current state
State * Car::getState(void)
{
    return &state;
}

// Drive but ignore air resistance
void Herbie::drive(double dt)
{
    // Compute updated state (x,v,a,t)
    double acc = physics::compute_acceleration(engine_force, mass);
    double vel = physics::compute_velocity(state.v, acc, dt);
    double pos = physics::compute_position(state.x, vel, dt);
    double time = state.t + dt;
    
    // Store results in car's state member
    state.set(pos, vel, acc, time);
}
