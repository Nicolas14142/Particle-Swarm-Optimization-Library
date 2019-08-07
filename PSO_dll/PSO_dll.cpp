#include <cstdlib>
#include<stdio.h>
#include <iostream>
#include <cstdlib>
#include <time.h>       /* time */
#include <limits>

#include "PSO_dll.h"
#include "Particle.h"
#include "Space.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

DLLEXPORT void ParticleSwarmOptimization1(py::list dest_py, py::list positions_py, py::list pso_positions_py, py::list vel_py, py::list lastBestPositions_py, py::list debug_py, py::int_ n_particles_py) {
	float target = 7.5; //distance around destination
	float c1 = 10; //pbest
	float c2 = 30; //gbest
	float att = 20; //attractive potential field
	float rep = 20; //repulsive potential field
	float W = 1; //inertia

	////// Casting py List to Array ////////
	int n_particles = n_particles_py.cast<int>();

	double* positions = NULL;
	double* vel = NULL;
	double* lastBestPositions = NULL;
	double* debug = NULL;
	double* dest = NULL;
	double* pso_positions = NULL;

	positions = new double[2 * n_particles];
	pso_positions = new double[2 * n_particles];
	vel = new double[2 * n_particles];
	lastBestPositions = new double[3 * n_particles];
	debug = new double[20];
	dest = new double[2];

	dest[0] = dest_py[0].cast<double>();
	dest[1] = dest_py[1].cast<double>();

	for (int i = 0; i < n_particles; i++) {
		positions[2*i] = positions_py[2 * i].cast<double>();
		positions[2*i+1] = positions_py[2 * i + 1].cast<double>();

		vel[2 * i] = vel_py[2 * i].cast<double>();
		vel[2 * i + 1] = vel_py[2 * i + 1].cast<double>();

		lastBestPositions[3 * i] = lastBestPositions_py[3 * i].cast<double>();
		lastBestPositions[3 * i + 1] = lastBestPositions_py[3 * i + 1].cast<double>();
		lastBestPositions[3 * i + 2] = lastBestPositions_py[3 * i + 2].cast<double>();
	}

	for (int i = 0; i < 20; i++) {
		debug[i] = debug_py[i].cast<double>();
	}
	
	ParticleSwarmOptimization(
		dest[0], dest[1],
		target,
		n_particles,
		c1,
		c2,
		att,
		rep,
		W,
		positions,
		pso_positions,
		lastBestPositions,
		vel,
		debug);


	for (int i = 0; i < n_particles; i++) {

		pso_positions_py[2 * i] = pso_positions[2 * i];
		pso_positions_py[2 * i + 1] = pso_positions[2 * i + 1];

		vel_py[2 * i] = vel[2 * i];
		vel_py[2 * i + 1] = vel[2 * i + 1];

		lastBestPositions_py[3 * i] = lastBestPositions[3 * i];
		lastBestPositions_py[3 * i + 1] = lastBestPositions[3 * i + 1];
		lastBestPositions_py[3 * i + 2] = lastBestPositions[3 * i + 2];
	}
	for (int i = 0; i < 20; i++) {
		debug_py[i] = debug[i];
	}

}


DLLEXPORT void ParticleSwarmOptimization(float dest_x, float dest_y, float target, int n_particles, float c1, float c2, float att, float rep, float W, double positions[], double pso_positions[], double last_pbest[], double vel[], double debug[]) {

	using namespace std;
	srand(time(NULL));

	Space search_space;
	search_space.set_target(target);
	search_space.set_n_particles(n_particles);
	search_space.set_dest(dest_x, dest_y); //seta o destino do grupo
	search_space.set_id();
	search_space.set_const(c1, c2, att, rep, W); //seta os pesos do PSO

	debug[0] = search_space.dest[0];
	debug[1] = search_space.dest[1];
	debug[2] = search_space.n_particles;

	for (int i = 0; i < search_space.n_particles; i++) {
		//set last_pbest
		search_space.particles[i].pbest_value = last_pbest[3 * i];
		search_space.particles[i].pbest_position[0] = last_pbest[3 * i + 1];
		search_space.particles[i].pbest_position[1] = last_pbest[3 * i + 2];

		//set last_velocities
		search_space.particles[i].velocity[0] = vel[2 * i];
		search_space.particles[i].velocity[1] = vel[2 * i + 1];

		//set last_positions
		search_space.particles[i].position[0] = positions[2 * i];
		search_space.particles[i].position[1] = positions[2 * i + 1];
	}

	//execute the algorithm
	search_space.PSO(dest_x, dest_y, n_particles, last_pbest, positions, c1, c2, att, rep, W);

	debug[3] = search_space.gbest_value;
	debug[4] = search_space.gbest_position[0];
	debug[5] = search_space.gbest_position[1];
	debug[6] = search_space.c1;
	debug[7] = search_space.Pbest;

	for (int i = 0; i < search_space.n_particles; i++) {
		//return pbest
		last_pbest[3 * i] = search_space.particles[i].pbest_value;
		last_pbest[3 * i + 1] = search_space.particles[i].pbest_position[0];
		last_pbest[3 * i + 2] = search_space.particles[i].pbest_position[1];

		//return new positions
		pso_positions[2 * i] = search_space.particles[i].position[0];
		pso_positions[2 * i + 1] = search_space.particles[i].position[1];

		//return new velocities
		vel[2 * i] = search_space.particles[i].velocity[0];
		vel[2 * i + 1] = search_space.particles[i].velocity[1];
	}

	write(search_space, 1);
}

DLLEXPORT void PsoTest() {
	std::cout << "Teste de chamada da biblioteca" << std::endl;
}

DLLEXPORT void PsoTest2(double positions[], int nrParticles) {
	std::cout << "Teste de chamada da biblioteca 2" << std::endl;
	std::cout << "\t Nr Particles: " << nrParticles << std::endl;
	std::cout << "\t positions: [" << positions[0];
	for (int i = 0; i < nrParticles; i++) {
		std::cout << ", " << positions[i];
		positions[1 + i] += 100;
	}
	std::cout << "]" << std::endl;
}

