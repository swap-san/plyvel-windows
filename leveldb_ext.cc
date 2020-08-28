// Copyright (c) Arni Mar Jonsson.
// See LICENSE for details.

// The Python 2/3 compatability code was found in cporting.rst

#include "leveldb_ext.h"

static PyMethodDef leveldb_extension_methods[] =
{
	{ (char*)"repair_db",  (PyCFunction)plyvel_repair_db,  METH_VARARGS | METH_KEYWORDS, (char*)plyvel_repair_db_doc  },
	{ (char*)"destroy_db", (PyCFunction)plyvel_destroy_db, METH_VARARGS, (char*)plyvel_destroy_db_doc },
	{NULL, NULL},
};

PyObject* leveldb_exception = 0;

#if PY_MAJOR_VERSION >= 3

struct leveldb_extension_state {
};

static int leveldb_extension_traverse(PyObject* m, visitproc visit, void* arg)
{
	return 0;
}

static int leveldb_extension_clear(PyObject* m)
{
	return 0;
}

static struct PyModuleDef leveldb_extension_def = {
	PyModuleDef_HEAD_INIT,
	"plyvel",
	NULL,
	sizeof(struct leveldb_extension_state),
	leveldb_extension_methods,
	NULL,
	leveldb_extension_traverse,
	leveldb_extension_clear,
	NULL
};

#define INITERROR return NULL

extern "C" PyObject* PyInit_plyvel(void)

#else

#define INITERROR return

extern "C" void initplyvel(void)

#endif
{
#if PY_MAJOR_VERSION >= 3
	PyObject* plyvel_module = PyModule_Create(&leveldb_extension_def);
#else
	PyObject* plyvel_module = Py_InitModule3((char*)"plyvel", leveldb_extension_methods, 0);
#endif

	if (plyvel_module == 0)
		INITERROR;

	// add custom exception
	leveldb_exception = PyErr_NewException((char*)"plyvel.Error", 0, 0);

	if (leveldb_exception == 0) {
		Py_DECREF(plyvel_module);
		INITERROR;
	}

	if (PyModule_AddObject(plyvel_module, (char*)"Error", leveldb_exception) != 0) {
		Py_DECREF(plyvel_module);
		INITERROR;
	}

	if (PyType_Ready(&Plyvel_Type) < 0) {
		Py_DECREF(plyvel_module);
		INITERROR;
	}

	if (PyType_Ready(&PlyvelSnapshot_Type) < 0) {
		Py_DECREF(plyvel_module);
		INITERROR;
	}

	if (PyType_Ready(&PyWriteBatch_Type) < 0) {
		Py_DECREF(plyvel_module);
		INITERROR;
	}

	if (PyType_Ready(&PlyvelIter_Type) < 0) {
		Py_DECREF(plyvel_module);
		INITERROR;
	}

	// add custom types to the different modules
	Py_INCREF(&Plyvel_Type);

	if (PyModule_AddObject(plyvel_module, (char*)"DB", (PyObject*)&Plyvel_Type) != 0) {
		Py_DECREF(plyvel_module);
		INITERROR;
	}

	Py_INCREF(&PlyvelSnapshot_Type);

	if (PyModule_AddObject(plyvel_module, (char*)"Snapshot", (PyObject*)&PlyvelSnapshot_Type) != 0) {
		Py_DECREF(plyvel_module);
		INITERROR;
	}

	Py_INCREF(&PyWriteBatch_Type);

	if (PyModule_AddObject(plyvel_module, (char*)"WriteBatch", (PyObject*)&PyWriteBatch_Type) != 0) {
		Py_DECREF(plyvel_module);
		INITERROR;
	}

	PyEval_InitThreads();

#if PY_MAJOR_VERSION >= 3
	return plyvel_module;
#endif
}