#ifndef _BASEHEADFILE_H
#define _BASEHEADFILE_H

#define EPSILON        1e-8
#define INFINITY       1e+8

//OpenMesh===============================================================================
#undef max
#undef min
#include <OpenMesh/Core/IO/MeshIO.hh>  
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>  
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh> 

struct MyTraits : OpenMesh::DefaultTraits
{
	// let point and normal be a vector of doubles 
	typedef OpenMesh::Vec3d Point;
	typedef OpenMesh::Vec3d Normal;
}; typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits>  TriMesh;
#include "SimpleCompute.h"


//Eigen====================================================================================
#include "Eigen/Core"
#include "Eigen/Sparse"
using namespace Eigen;
using namespace std;

#endif 