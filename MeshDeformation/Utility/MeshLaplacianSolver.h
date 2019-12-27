#pragma once
#include "baseHeadFile.h"
#include "Eigen/SparseLU"

class MeshLaplaceSolver
{
public:
	MeshLaplaceSolver(void);
	MeshLaplaceSolver(TriMesh mesh);
	MeshLaplaceSolver(TriMesh mesh,const vector<bool> isctrlv);
	~MeshLaplaceSolver(void);

public:
	TriMesh m_desMesh;
	OpenMesh::VPropHandleT<vector<double>> vertexLPLWeight;
	SparseMatrix<double> A;

private:
	vector<bool> m_isControlVertex;
	Eigen::SparseLU<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<int> > solver;
	
	VectorXd B,X;

public:
	void SetControlVertex(const vector<bool> isctrlv);
	void SetDesMesh(TriMesh& mesh_){m_desMesh = mesh_;};
	VectorXd& LplacianSolve();
	void ComputeVertexLaplacianWeight();
	void ComputeLalacianMatrixA();
	void SetRightHandB(VectorXd& rightHand){ B = rightHand;}
}; 