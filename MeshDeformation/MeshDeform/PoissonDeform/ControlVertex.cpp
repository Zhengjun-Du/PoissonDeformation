#include "stdafx.h"
#include "ControlVertex.h"
#include "../../Utility/MeshLaplacianSolver.h"
#include "../../Utility/MyAfxFunction.h"
#include "glut.h"

void ControlVertex::LoadHandVertex()
{
	m_HandVindex.clear();
	int vindex = 0;

	CFileDialog file(true);
	file.m_ofn.lpstrFilter =_T("txt file(*.txt)\0*.txt\0*.*\0\0");
	file.m_ofn.lpstrTitle  =_T("OPEN");
	if(file.DoModal()==IDOK)
	{
		CString m_filePath =  file.GetPathName();
		ifstream  read_(m_filePath);
		while(!read_.eof())
		{
			read_>>vindex;
			m_HandVindex.push_back(vindex);
		}
	}
	
	m_posOfHand.clear();
	for(int i = 0; i < m_HandVindex.size(); i++)
	{
		TriMesh::VertexHandle vh  = m_mesh->vertex_handle(m_HandVindex[i]);
		Point3D point(m_mesh->point(vh)[0],m_mesh->point(vh)[1],m_mesh->point(vh)[2]);
		m_posOfHand.push_back(point);
	}
}

void ControlVertex::Draw()
{
	if(!m_mesh)
		return;

	glDisable(GL_LIGHTING);

	//draw hand points
	glColor4f(1,0, 0, 1);
	glPointSize(6);
	glBegin(GL_POINTS);
	for(int i = 0; i < m_posOfHand.size(); i++)
		glVertex3d(m_posOfHand[i].m_x,m_posOfHand[i].m_y,m_posOfHand[i].m_z);
	glEnd();
	glPointSize(1);


	//draw fixed vertex
	glColor4f(1, 1, 1, 1);
	glPointSize(6);
	glBegin(GL_POINTS);
	for(int i = 0; i < m_fixedVindex.size(); i++)
	{
		TriMesh::Point point = m_mesh->point(TriMesh::VertexHandle(m_fixedVindex[i]));
		glVertex3d(point[0],point[1],point[2]);
	}
	glEnd();
	glPointSize(1);

	glEnable(GL_LIGHTING);
}

void ControlVertex::HandTransform(Matrix4d mt)
{
	Point3D center = afxGetMainMesh()->m_centerOfAABB;
	Matrix4d mt1 = SimpleCompute::Translate2Matrix(-center.m_x,-center.m_y,-center.m_z);
	Matrix4d mt2 = SimpleCompute::Translate2Matrix(center.m_x,center.m_y,center.m_z);

	Matrix4d mat = mt2*mt*mt1;
	
	for(int i = 0; i < m_HandVindex.size(); i++)
	{
		TriMesh::Point point = m_mesh->point(TriMesh::VertexHandle(m_HandVindex[i]));
		m_posOfHand[i] =   SimpleCompute::ComputeMatrixMultiPoint(mat,Point3D(m_posOfHand[i].m_x,m_posOfHand[i].m_y,m_posOfHand[i].m_z));
		//m_posOfHand[i] =   SimpleCompute::ComputeMatrixMultiPoint(mat,Point3D(point[0],point[1],point[2]));
	}
}

void  ControlVertex::UpdateHandPos()
{
	for(int i = 0; i < m_HandVindex.size(); i++)
	{
		TriMesh::VertexHandle vh  = m_mesh->vertex_handle(m_HandVindex[i]);
		Point3D point(m_mesh->point(vh)[0],m_mesh->point(vh)[1],m_mesh->point(vh)[2]);
		m_posOfHand[i] = point;
	}
}

vector<bool> ControlVertex::GetVertexProperty()
{
	vector<bool> isControlVertex(m_mesh->n_vertices(),false);
	for(int i = 0; i <m_HandVindex.size(); i++)
		isControlVertex[m_HandVindex[i]] = true;

	for(int j = 0; j < m_fixedVindex.size(); j++)
		isControlVertex[m_fixedVindex[j]] = true;

	return isControlVertex;
}

void ControlVertex::Clear()
{
	m_HandVindex.clear();
	m_fixedVindex.clear();
	m_posOfHand.clear();
	m_freeVertexWeight.clear();
}

void ControlVertex::Reset()
{
	m_posOfHand.clear();
	for(int i = 0; i < m_HandVindex.size(); i++)
	{
		TriMesh::VertexHandle vh  = m_mesh->vertex_handle(m_HandVindex[i]);
		Point3D point(m_mesh->point(vh)[0],m_mesh->point(vh)[1],m_mesh->point(vh)[2]);
		m_posOfHand.push_back(point);
	}
}

void ControlVertex::ComputeFreeVertexWeight()
{	
	vector<bool> isControlVertex = GetVertexProperty();

	MeshLaplaceSolver LPLslover(*m_mesh);
	LPLslover.SetControlVertex(isControlVertex);
	LPLslover.ComputeLalacianMatrixA();

	int nvertex = m_mesh->n_vertices();
	VectorXd b(nvertex); b.setZero();

	for(int i = 0; i < m_HandVindex.size(); i++)
	{
		int handIndex = m_HandVindex[i];
		b[handIndex]  =  1;
	}
	LPLslover.SetRightHandB(b);

	VectorXd  x = LPLslover.LplacianSolve();
	for(int i = 0; i < nvertex; i++)
		m_freeVertexWeight.push_back(1-x[i]);
} 