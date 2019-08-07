#pragma once

#ifndef __Space
#define __Space

#include "Particle.h"

class Space
{
public:
	int n_particles;
	double target;
	int Pbest = 0;
	double gbest_value;
	double gbest_position[2];
	double dest[2];
	double new_velocity[2];
	double velocity_max = 50;

	double fitness_candidate, best_fitness_candidate;
	double range = 50;
	double c1 = 0.1, c2 = 4, att = 5, rep = 1, W = 1;

	Particle particles[12];

	//algoritmo
	void PSO(double dest_x, double dest_y, int n_particles, double *last_pbest, double *positions, double c1, double c2, double att, double rep, double W);

	void print_particles();
	double fitness(Particle); // função fitness, depende do target(distancia até o dest)

	void set_target(double);
	void set_n_particles(int);
	void set_dest(double, double); //seta o destino do grupo
	void set_id();
	void set_const(double, double, double, double, double); //seta os pesos do PSO

	void set_pbest(); // calcula melhor posição da particula
	int set_gbest(); //calcula melhor posição do grupo

	void move_particles(int);
	double random(); //gerador de números aleatórios
	void potential_field(Particle, bool, double); //campo potencial

};
void write(Space &search_space, int j);

#endif