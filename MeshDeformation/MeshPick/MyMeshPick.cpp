#include "StdAfx.h"
#include "MyMeshPick.h"
#include "glut.h"

MyMeshPick::MyMeshPick(void)
{
	m_desMesh = NULL;
	m_nVertices = 0;
}


MyMeshPick::~MyMeshPick(void)
{
	if(m_desMesh)
		delete[] m_desMesh;
}

void MyMeshPick::SetDesMesh(TriMesh* mesh)
{
	if(!mesh)return;
	if(m_desMesh) delete[] m_desMesh;

	m_pickedSet.clear();

	m_nVertices = mesh->n_vertices();
	m_desMesh = new M3DVector3f[m_nVertices];
	int vid = 0;
	for (TriMesh::VertexIter v_it = mesh->vertices_begin(); v_it != mesh->vertices_end(); v_it++)
	{
		vid = v_it.handle().idx();
		m_desMesh[vid][0] = mesh->point(v_it)[0];
		m_desMesh[vid][1] = mesh->point(v_it)[1];
		m_desMesh[vid][2] = mesh->point(v_it)[2];
	}
}

void MyMeshPick::PickMesh() 
{
	if(!m_desMesh)
		return;
	m_pickedSet.clear();

	if(m_singleSelect)
	{
		int r = 5;
		m3dLoadVector2(m_left_bottom, m_mouseDownPoint.x-r, m_viewHeight-(m_mouseDownPoint.y-r));
		m3dLoadVector2(m_right_top, m_mouseDownPoint.x+r, m_viewHeight-(m_mouseDownPoint.y+r));
	}
	else
	{
		m3dLoadVector2(m_left_bottom, m_mouseDownPoint.x, m_viewHeight-(m_mouseDownPoint.y));
		m3dLoadVector2(m_right_top, m_mouseUpPoint.x, m_viewHeight-(m_mouseUpPoint.y));
	}
	int m_viewport[4];
	glViewport (0, 0, (GLsizei)m_viewWidth, (GLsizei)m_viewHeight); 
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	// get the projection matrix
	glGetFloatv(GL_PROJECTION_MATRIX, m_mat_proj);
	glMatrixMode(GL_MODELVIEW);

	// get the modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX, m_mat_modelview);
	
	// pick vertex
	selection.set_config(&m_desMesh[0], m_nVertices, m_left_bottom, m_right_top, m_mat_modelview, m_mat_proj, m_viewport);
	selection.highlight_selected_pts();

	//get the picked vertex index
	if(selection.vec_selected_pts_index.size() != 0)
		m_pickedSet = selection.vec_selected_pts_index;
}


void MyMeshPick::DrawSelectedVertex()
{
	glPointSize(5);
	glColor4f(0, 1, 0, 1);
	glDisable(GL_LIGHTING);
	glBegin(GL_POINTS);
	for(int i = 0; i < m_pickedSet.size(); i++)
		glVertex3fv(m_desMesh[m_pickedSet[i]]);
	glEnable(GL_LIGHTING);
	glEnd();
}

void MyMeshPick::Update(TriMesh* mesh)
{
	SetDesMesh(mesh);
}

void MyMeshPick::Clear()
{
	if(m_desMesh) 
	{
		delete[] m_desMesh; 
		m_desMesh = NULL;
	}
	m_pickedSet.clear();
}