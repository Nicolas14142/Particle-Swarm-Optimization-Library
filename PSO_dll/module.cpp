#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "PSO_dll.h"

//O código onde as funções que serão chamadas pelo python tem sua macro definida!

namespace py = pybind11;

////// FUNÇÕES DE TESTE /////////

struct DynamicData {
	double* position;
	double* velocity;
};

void test_with_obj_1(py::int_ x) {
	x = 5;
}

void test_with_obj_2(double a[], size_t s) { // recebe cópia do valor
	std::cout << "C++ test_with_obj_2 " << a << std::endl;
	a[0] = 2;
	std::cout << "C++ test_with_obj_2 " << a << std::endl;
}

void test_with_obj_3(py::list a) { // recebe o próprio objeto python
	std::cout << "C++ test_with_obj_3 " << a << std::endl;
	a[0] = 3; // Tb altera no python
	int c = a[0].cast<int>();
	std::cout << "C++ test_with_obj_3 " << c << std::endl;
	//FUNCIONANDO
}

void test_with_obj_4(py::array_t<double> a) { // recebe cópia da lista python e referencia do numpy array
	std::cout << "C++ test_with_obj_4 " << a << std::endl;
	double * d = a.mutable_data();
	d[0] = 4.1;	 // Tb altera no python, se for numpy array
	std::array<double, 3> b;
	std::cout << "C++ test_with_obj_4 " << a << std::endl;
}

PyObject* pso_with_defaults(PyObject* self, PyObject* args) {
	PyObject *positionsListObj;
	PyObject *velocityListObj;
	int nrParticles;
	if (!PyArg_ParseTuple(args, "OOi", &positionsListObj, &velocityListObj, &nrParticles)) {
		return NULL;
	}
	return Py_BuildValue("i", 0);
}

void castListToArray(py::list l, int size) { // recebe o próprio objeto python
	// Tb altera no python
	double* a = NULL;
	a = new double[size];
	for (int i = 0; i < size; i++) {
		a[i] = l[i].cast<double>();
	}
	//std::cout << "C++ test_with_obj_3 " << a << std::endl;
	//FUNCIONANDO
}



////////// DEFINIÇÃO DAS MACROS ///////////

PYBIND11_MODULE(PSO_dll, m) {
	
	m.def("test", &PsoTest, R"pbdoc(
        Teste de chamada da biblioteca.
    )pbdoc");
	
	m.def("data_test", &test_with_obj_1, R"pbdoc(
        Teste de chamada da biblioteca.
    )pbdoc");

	m.def("ParticleSwarmOptimization", &ParticleSwarmOptimization1, R"pbdoc(
		Particle Swarm Optimization Algorithm
    )pbdoc");


#ifdef VERSION_INFO
	m.attr("__version__") = VERSION_INFO;
#else
	m.attr("__version__") = "dev";
#endif;
}


//static PyMethodDef pso_methods[] = {
//	// The first property is the name exposed to Python, fast_tanh, the second is the C++
//	// function name that contains the implementation.
//	{ "tanh", (PyCFunction)tanh_impl, METH_O, nullptr },
//	{ "test", (PyCFunction)test, METH_NOARGS, nullptr },
//	{ "test_with_obj_1", (PyCFunction)test_with_obj_1, METH_VARARGS, nullptr },
//	{ "pso_with_defaults", (PyCFunction)pso_with_defaults, METH_VARARGS, nullptr },
//
//	// Terminate the array with an object containing nulls.
//	{ nullptr, nullptr, 0, nullptr }
//};
//
//static PyModuleDef pso_module = {
//	PyModuleDef_HEAD_INIT,
//	"pso",                        // Module name to use with Python import statements
//	"Particle Swarm Optimization",  // Module description
//	0,
//	pso_methods                   // Structure that defines the methods of the module
//};
//
//PyMODINIT_FUNC PyInit_pso() {
//	return PyModule_Create(&pso_module);
//}
