# car simulator project
bin/car_sim: bin/car_simulator.o
	g++ -Wall bin/car_simulator.o -o bin/car_sim

bin/car_simulator.o: src/car_simulator.cpp
	g++ -Wall -c src/car_simulator.cpp -o bin/car_simulator.o

# drag race project
bin/drag_race: bin/drag_race.o bin/Car.o
	g++ -Wall bin/drag_race.o bin/Car.o -o bin/drag_race

bin/drag_race.o: src/drag_race.cpp
	g++ -Wall -c src/drag_race.cpp -o bin/drag_race.o

bin/Car.o: src/Car.cpp
	g++ -Wall -c src/Car.cpp -o bin/Car.o
