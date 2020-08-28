// Copyright (c) Arni Mar Jonsson.
// See LICENSE for details.

#ifndef __PLYVEL__MODULE__H__
#define __PLYVEL__MODULE__H__

extern "C" {
#include <Python.h>

#include "structmember.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
}

#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <leveldb/comparator.h>
#include <leveldb/cache.h>

#include <vector>

typedef struct {
	PyObject_HEAD

		// object is open if all of these are non-null,
		// once an object has been closed, it can not be re-opened
		leveldb::DB* _db;
	leveldb::Options* _options;
	leveldb::Cache* _cache;
	const leveldb::Comparator* _comparator;

	// number of open snapshots, associated with LevelDB object
	int n_snapshots;

	// number of open iterators, associated with LevelDB object
	int n_iterators;
} Plyvel;

typedef struct {
	PyObject_HEAD

		// the associated LevelDB object
		Plyvel* db;

	// the snapshot
	const leveldb::Snapshot* snapshot;
} PlyvelSnapshot;

typedef struct {
	PyObject_HEAD

		// the associated LevelDB object or snapshot
		PyObject* ref;

	// the associated db object
	Plyvel* db;

	// the iterator
	leveldb::Iterator* iterator;

	// upper/lower limit, inclusive, if any
	std::string* bound;

	// iterator direction
	int is_reverse;

	// if 1: return (k, v) 2-tuples, otherwise just k
	int include_value;
} PlyvelIter;

typedef struct {
	bool is_put;
	std::string key;
	std::string value;
} PyWriteBatchEntry;

typedef struct {
	PyObject_HEAD
		std::vector<PyWriteBatchEntry>* ops;
} PyWriteBatch;

// custom types
extern PyTypeObject Plyvel_Type;
extern PyTypeObject PlyvelSnapshot_Type;
extern PyTypeObject PyWriteBatch_Type;
extern PyTypeObject PlyvelIter_Type;

#define Plyvel_Check(op) PyObject_TypeCheck(op, &Plyvel_Type)
#define PlyvelSnapshotCheck(op) PyObject_TypeCheck(op, &PlyvelSnapshot_Type)
#define PyWriteBatch_Check(op) PyObject_TypeCheck(op, &PyWriteBatch_Type)

extern PyObject* leveldb_exception;

extern const char plyvel_repair_db_doc[];
extern const char plyvel_destroy_db_doc[];

extern PyObject* plyvel_repair_db(Plyvel* self, PyObject* args, PyObject* kwds);
extern PyObject* plyvel_destroy_db(PyObject* self, PyObject* args);

extern "C" void initplyvel(void);

#endif