#ifndef STATE
#define STATE

#include <iostream>

class State
{
public:
    // Properties
    double x;
    double v;
    double a;
    double t;
    
    // Constructor
    State() { x = v = a = t = 0; }
    State(double pos, double vel, double acc, double time) :
            x(pos), v(vel), a(acc), t(time) {};
    
    // Methods
    void set(double pos, double vel, double acc, double time);
};

// prints out a State class information
inline std::ostream& operator<<(std::ostream& os, State& state)
{
    os << "t: " << state.t << ", x: " << state.x
    << ", v: " << state.v << ", a: " << state.a;
    return os;
}

// sets properties of State
inline void State::set(double pos, double vel, double acc, double time)
{
    x = pos;
    v = vel;
    a = acc;
    t = time;
}

#endif //STATE
