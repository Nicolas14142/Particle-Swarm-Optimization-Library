#pragma once
#ifndef __Particle
#define __Particle


class Particle
{
public:
	double position[2];
	double pbest_position[2];
	double pbest_value;
	double velocity[2];
	double range = 5;
	int id;

	void set_position(double, double);
	void str();
	void move();
	double random();
	double velocity_mod();
	void limit_velocity(double speed_limit);

	//Particle & operator=(const Particle & p);
	//Particle & operator+(const Particle & p) const;
};
#endif