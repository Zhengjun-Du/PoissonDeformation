#include "StdAfx.h"
#include "MeshLaplacianSolver.h"


MeshLaplaceSolver::MeshLaplaceSolver(void)
{
}

MeshLaplaceSolver::MeshLaplaceSolver(TriMesh mesh)
{
	m_desMesh = mesh;
}

MeshLaplaceSolver::MeshLaplaceSolver(TriMesh mesh,const vector<bool> isctrlv)
{
	m_desMesh = mesh;
	m_isControlVertex = isctrlv;
}

MeshLaplaceSolver::~MeshLaplaceSolver(void)
{
}

void MeshLaplaceSolver::SetControlVertex(const vector<bool> isctrlv)
{
	m_isControlVertex.clear();
	m_isControlVertex = isctrlv;
}

void MeshLaplaceSolver::ComputeVertexLaplacianWeight()
{
	m_desMesh.add_property(vertexLPLWeight);
	for (TriMesh::VertexIter v_it = m_desMesh.vertices_begin(); v_it!=m_desMesh.vertices_end(); ++v_it)
	{
		Vector3D currentPosition = Vector3D(m_desMesh.point(v_it)[0],m_desMesh.point(v_it)[1],m_desMesh.point(v_it)[2]);

		vector<Vector3D> neighborPoints; 
		for (TriMesh::VertexVertexIter vv_it = m_desMesh.vv_iter(v_it);  vv_it;  ++vv_it)
			neighborPoints.push_back(Vector3D(m_desMesh.point(vv_it)[0], m_desMesh.point(vv_it)[1],m_desMesh.point(vv_it)[2])); 

		bool is_boder_vertex = m_desMesh.is_boundary(v_it);
		int numNeighbors = (int)neighborPoints.size();
		vector<double> weights(numNeighbors,0);

		for (int i=0; i<numNeighbors; i++)
		{
			double w1 = CotValue(neighborPoints[(i + numNeighbors -1)%numNeighbors], currentPosition, neighborPoints[i]);
			double w2 = CotValue(neighborPoints[(i+1)%numNeighbors], currentPosition, neighborPoints[i]);

			if(is_boder_vertex)
			{
				if(i == 0)
					w1 = CotValue(neighborPoints[i], currentPosition, neighborPoints[i]);
				if(i == numNeighbors - 1)
					w2 = CotValue(neighborPoints[i], currentPosition, neighborPoints[i]);
			}
			weights[i] = 0.5*(w1+w2);
			weights[i] = MaxValue(weights[i], EPSILON);
		}
		m_desMesh.property(vertexLPLWeight, v_it) = weights;
	}
}

void MeshLaplaceSolver::ComputeLalacianMatrixA()
{
	ComputeVertexLaplacianWeight();

	assert(!m_isControlVertex.empty());

	int numVertices = m_desMesh.n_vertices();
	A.resize(numVertices,numVertices);

	for (TriMesh::VertexIter v_it = m_desMesh.vertices_begin(); v_it!=m_desMesh.vertices_end(); ++v_it)
	{
		int vindex = v_it.handle().idx();

		if (m_isControlVertex[vindex]) // control points
		{
			A.coeffRef(vindex,vindex) = 1.0;
		}
		else
		{
			double sum = 0.0;
			vector<double> weights = m_desMesh.property(vertexLPLWeight, v_it);
			int id = 0;
			for (TriMesh::VertexVertexIter vv_it = m_desMesh.vv_iter(v_it);  vv_it;  ++vv_it)
			{
				int vvindex = vv_it.handle().idx();
				A.coeffRef(vindex, vvindex) = -weights[id];
				sum +=  weights[id++];
			}
			A.coeffRef(vindex,vindex) = sum;
		}
	}

	//pre QR decompose
	A.makeCompressed();
	solver.compute(A);
}

VectorXd& MeshLaplaceSolver::LplacianSolve()
{
	X = solver.solve(B);
	return X;
}