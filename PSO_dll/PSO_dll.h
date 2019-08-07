#pragma once

#ifndef __PSO_H
#define __PSO_H

#if !defined(unix) && !defined(__unix__) && !defined(__unix)
#define DLLEXPORT
#else
#define DLLEXPORT extern "C"
#endif

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

DLLEXPORT void PsoTest();
DLLEXPORT void PsoTest2(double positions[], int nrParticles);
DLLEXPORT void ParticleSwarmOptimization1(py::list dest_py, py::list positions_py, py::list pso_positions_py, py::list vel_py, py::list lastBestPositions_py, py::list debug_py, py::int_ n_particles);
DLLEXPORT void ParticleSwarmOptimization(float dest_x, float dest_y, float target, int n_particles, float c1, float c2, float att, float rep, float W, double positions[], double pso_positions[], double last_pbest[], double vel[], double debug[]);
#endif
