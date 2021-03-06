
//-
// ==========================================================================
// Copyright (C) 2008 Autodesk, Inc. and/or its licensors.  All 
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its 
// licensors, which is protected by U.S. and Canadian federal copyright 
// law and by international treaties.
//
// The Data is provided for use exclusively by You. You have the right 
// to use, modify, and incorporate this Data into other products for 
// purposes authorized by the Autodesk software license agreement, 
// without fee.
//
// The copyright notices in the Software and this entire statement, 
// including the above license grant, this restriction and the 
// following disclaimer, must be included in all copies of the 
// Software, in whole or in part, and all derivative works of 
// the Software, unless such copies or derivative works are solely 
// in the form of machine-executable object code generated by a 
// source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. 
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED 
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF 
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
// PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE, OR 
// TRADE PRACTICE. IN NO EVENT WILL AUTODESK AND/OR ITS LICENSORS 
// BE LIABLE FOR ANY LOST REVENUES, DATA, OR PROFITS, OR SPECIAL, 
// DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK 
// AND/OR ITS LICENSORS HAS BEEN ADVISED OF THE POSSIBILITY 
// OR PROBABILITY OF SUCH DAMAGES.
//
// ==========================================================================
//+

#include <Python.h>

#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MSelectionList.h>

static PyObject* cppCentroid_mesh(PyObject*, PyObject* args)
{
	//	Retrieve the name of the mesh from the argument list.
	const char*	meshName;
	if (!PyArg_ParseTuple(args, "s:mesh", &meshName))
	{
		return NULL;
	}

	// Get the mesh's dag path.
	MSelectionList	slist;
	slist.add(meshName);

	MDagPath		path;
	slist.getDagPath(0, path);

	if (!path.hasFn(MFn::kMesh))
	{
		return PyErr_Format(PyExc_ValueError, "'%s' is not a mesh.", meshName);
	}

	// Get the mesh's vertex positions.
	MFnMesh		meshFn(path);
	MPointArray	verts;

	meshFn.getPoints(verts, MSpace::kWorld);

	unsigned int numVerts = verts.length();
	if (numVerts == 0)
	{
		return PyErr_Format(PyExc_ValueError, "Mesh '%s' has no vertices.", meshName);
	}

	// Add all the vertex positions together.
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

	for (unsigned int i = 0; i < numVerts; ++i)
	{
		MPoint& p = verts[i];
		x += p.x;
		y += p.y;
		z += p.z;
	}

	// Calculate the average position.
	double divisor = 1.0 / (double)numVerts;
	
	x *= divisor;
	y *= divisor;
	z *= divisor;

	//	Return a tuple containing the average of the vertex positions.
	return Py_BuildValue("(ddd)", x, y, z);
}


static PyMethodDef cppCentroidMethods[] =
{
	{ "mesh", cppCentroid_mesh, METH_VARARGS, "Find the centroid of a mesh." },
	{ NULL, NULL, 0, NULL }
};


extern "C" PyMODINIT_FUNC initcppCentroid()
{
	Py_InitModule("cppCentroid", cppCentroidMethods);
}
