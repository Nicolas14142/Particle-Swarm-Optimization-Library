#include "Space.h"
#include <cstdlib>
#include <time.h>       /* time */
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>



void Space::set_target(double target) {

	this->target = target;
}

void Space::set_n_particles(int n_particles) {

	this->n_particles = n_particles;
}


void Space::set_dest(double x, double y) {

	this->dest[0] = x;
	this->dest[1] = y;
}

void Space::set_id() {

	for (int i = 0; i < n_particles; i++) {
		particles[i].id = i;
	}
}

void Space::set_pbest() {
	for (int i = 0; i < n_particles; i++) {

		fitness_candidate = this->fitness(particles[i]);
		if (particles[i].pbest_value > fitness_candidate) {
			this->particles[i].pbest_value = fitness_candidate;
			this->particles[i].pbest_position[0] = particles[i].position[0];
			this->particles[i].pbest_position[1] = particles[i].position[1];
		}
	}
}

int Space::set_gbest() {
	gbest_value = 100000000;
	//n necessita de memória pois atualiza sempre
	for (int i = 0; i < n_particles; i++) {

		best_fitness_candidate = fitness(particles[i]);
		if (gbest_value > best_fitness_candidate) {
			this->gbest_value = best_fitness_candidate;
			this->gbest_position[0] = this->particles[i].position[0];
			this->gbest_position[1] = this->particles[i].position[1];
			this->Pbest = i;
		}
	}
	return Pbest;
}

void Space::set_const(double c1, double c2, double att, double rep, double W) {
	this->c1 = c1;
	this->c2 = c2;
	this->att = att;
	this->rep = rep;
	this->W = W;

}

void Space::print_particles() {
	for (int i = 0; i < n_particles; i++) {

		particles[i].str();
		std::cout << "Dest: " << dest[0] << ", " << dest[1] << "\n";
		std::cout << random() << "\n";
	}
}

double Space::fitness(Particle p) {

	return abs(target*target - pow((p.position[1] - dest[1]), 2) - pow((p.position[0] - dest[0]), 2));
}

void Space::PSO(double dest_x, double dest_y, int n_particles, double *last_pbest, double *positions, double c1, double c2, double att, double rep, double W) {
	//return particle next position

	set_const(c1, c2, att, rep, W);
	set_n_particles(n_particles);
	set_target(target);
	set_dest(dest_x, dest_y);
	set_id();

	set_pbest();
	int Pb = set_gbest();

	//write(search_space, i);
	move_particles(Pb);
	//std::cout << i << "\n";//<<"\t"<<" "<<Pb<<" "<<search_space.entrei<<"\t"<< search_space.fitness(search_space.particles[Pb])<<" "<< search_space.gbest_value <<"\t"<< search_space.particles[Pb].position[0]<<" "<< search_space.gbest_position[0]<<"\n";
}





void Space::move_particles(int Pb) {
	for (int i = 0; i < n_particles; i++) {

		//this->new_velocity[0] = (W*particles[i].velocity[0]) + (c1*abs(random()()))*(particles[i].pbest_position[0] - particles[i].position[0]) + (c2*abs(random()()))*(this->gbest_position[0] - particles[i].position[0]);
		//this->new_velocity[1] = (W*particles[i].velocity[1]) + (c1*abs(random()()))*(particles[i].pbest_position[1] - particles[i].position[1]) + (c2*abs(random()()))*(this->gbest_position[1] - particles[i].position[1]);

		//particle velocity.max limited

		if (false) {

			potential_field(particles[i], true, velocity_max);
		}
		else {
			double rand1 = abs(random());
			double rand2 = abs(random());
			this->new_velocity[0] = (W*particles[i].velocity[0]) + (c1*rand1)*(particles[i].pbest_position[0] - particles[i].position[0]) + (c2*rand2)*(this->gbest_position[0] - particles[i].position[0]);
			this->new_velocity[1] = (W*particles[i].velocity[1]) + (c1*rand1)*(particles[i].pbest_position[1] - particles[i].position[1]) + (c2*rand2)*(this->gbest_position[1] - particles[i].position[1]);
			potential_field(particles[i], true, velocity_max);
		}
		particles[i].velocity[0] = new_velocity[0];
		particles[i].velocity[1] = new_velocity[1];
		particles[i].limit_velocity(velocity_max);
		particles[i].move();
	}
}

void Space::potential_field(Particle p, bool attractive, double speed_limit) {

	double u[2] = { 0,0 };
	double mod;
	double dir[2] = { 0,0 };

	//Force of Attraction
	if (attractive == true) {
		u[0] = att * (this->dest[0] - p.position[0]);
		u[1] = att * (this->dest[1] - p.position[1]);
	}
	//Force of Repulsion

	for (int i = 0; i < n_particles; i++) {

		if (p.id != particles[i].id) {
			mod = (particles[i].position[0] - p.position[0])*(particles[i].position[0] - p.position[0]) + (particles[i].position[1] - p.position[1])*(particles[i].position[1] - p.position[1]);
			mod = sqrt(mod);
			if (mod != 0) {
				dir[0] = (particles[i].position[0] - p.position[0]) / mod;
				dir[1] = (particles[i].position[1] - p.position[1]) / mod;
				u[0] = u[0] - rep * dir[0] * 1 / (mod);
				u[1] = u[1] - rep * dir[1] * 1 / (mod);
			}
			else {
				u[0] = dir[0] * speed_limit;
				u[1] = dir[1] * speed_limit;
			}
		}
	}

	new_velocity[0] = new_velocity[0] + u[0];
	new_velocity[1] = new_velocity[1] + u[1];
}

double Space::random() {

	// srand(time(NULL));
	//gera aleatório de -1 a 1
	double random = ((double)rand() / (RAND_MAX)) * 2 - 1;
	//random = random * range
	return random;
}

void write(Space &search_space, int j)
{
	using namespace std;
	// file pointer 
	fstream fout;
	// opens an existing csv file or creates a new file. 

	if (j == 0) fout.open("enxame.csv", ofstream::out | ofstream::trunc);
	else fout.open("enxame.csv", ios::out | ios::app);

	int i;

	for (i = 0; i < search_space.n_particles; i++) {

		// Insert the data to file 
		fout << search_space.particles[i].position[0] << ", "
			<< search_space.particles[i].position[1] << ", ";
	}

	fout << search_space.dest[0] << ","
		<< search_space.dest[1] << "\n";

	//fout<< "\n";
}

