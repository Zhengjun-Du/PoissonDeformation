#include "stdafx.h"
#include "MyTriMesh.h"
#include "../Utility/SimpleCompute.h"

Vector3D g_zeroVec(0,0,0);


MyTriMesh::MyTriMesh(void)
{
	init();
}

MyTriMesh::~MyTriMesh(void)
{
	m_mesh.clear();
}

void MyTriMesh::init()
{
	m_disType = BOUNDARY_MESH;
	m_show = true;
	m_drawMesh = 1;
	m_updateDistList = true;
	boundBoxEdgeLen = 1;
}

void MyTriMesh::SetMesh(TriMesh* mesh_)
{ 
	m_mesh = *mesh_; 
	UpdateDisList(); 
};

void  MyTriMesh::DrawList()
{
	if(m_mesh.vertices_empty())
		return;

	if(m_updateDistList)
		m_updateDistList = false; 
	else
		return;

	//create display list
	glNewList(m_drawMesh,GL_COMPILE);

	TriMesh::FaceIter f_it;
	TriMesh::FaceVertexIter fv_it;

	//move object to origin
	glTranslated(-m_centerOfAABB.m_x,-m_centerOfAABB.m_y,-m_centerOfAABB.m_z);

	//draw the model with GL_LINE_LOOP
	glLineWidth(1);


	int colorIndex;

	//只绘制点
	if(m_disType == POINT_ONLY_MESH)
	{
		glDisable(GL_LIGHTING);
		glColor3f(1,0,0);
		glBegin(GL_POINTS);
		glPointSize(5);
		for (TriMesh::VertexIter vi = m_mesh.vertices_begin(); vi != m_mesh.vertices_end(); vi++)
		{
			break;
			TriMesh::Point point = m_mesh.point(vi);
			glVertex3d(point[0],point[1],point[2]);
		}
		glEnd();
		glPointSize(1);
		glEndList();
		return;
	}

	//绘制线框
	if(m_disType != SMOOTH_MESH)
	{
		glColor3f(0.2,0.1,0.2);
		glDisable(GL_LIGHTING);

		for (f_it = m_mesh.faces_begin(); f_it != m_mesh.faces_end(); f_it++)
		{
			glBegin(GL_LINE_LOOP);
			for(fv_it = m_mesh.fv_begin(f_it); fv_it != m_mesh.fv_end(f_it); fv_it++)
			{
				glVertex3d(m_mesh.point(fv_it)[0],m_mesh.point(fv_it)[1],m_mesh.point(fv_it)[2]);
			}
			glEnd();
		}	
	}

	//绘制光滑面
	{
		//get mormal
		m_mesh.request_face_normals();
		m_mesh.request_vertex_normals();
		m_mesh.update_normals();

		//offset z-depth
		glPushAttrib (GL_ALL_ATTRIB_BITS);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset (1, 1);
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

		//show the material visual color
		glDisable(GL_COLOR_MATERIAL);

		//draw the model with GL_POLYGON
		glEnable(GL_LIGHTING);
		for (f_it = m_mesh.faces_begin(); f_it != m_mesh.faces_end(); f_it++)
		{
			glBegin(GL_POLYGON);
			glNormal3d(m_mesh.normal(f_it)[0],m_mesh.normal(f_it)[1],m_mesh.normal(f_it)[2]);
			for(fv_it = m_mesh.fv_begin(f_it); fv_it != m_mesh.fv_end(f_it); fv_it++)
			{
				glNormal3d(m_mesh.normal(fv_it)[0],m_mesh.normal(fv_it)[1],m_mesh.normal(fv_it)[2]);
				//glNormal3d(m_vertexNormal[fv_it.handle().idx()].m_x,m_vertexNormal[fv_it.handle().idx()].m_y,m_vertexNormal[fv_it.handle().idx()].m_z);
				glVertex3d(m_mesh.point(fv_it)[0],m_mesh.point(fv_it)[1],m_mesh.point(fv_it)[2]);	
			}
			glEnd();
		}
		glPopAttrib();
	}
	glEndList();
}

//核心绘制函数
void MyTriMesh::Draw()
{
	if(!m_show)
		return;
	DrawList();
	glCallList(m_drawMesh);
	glFlush();
}


void MyTriMesh::GetAABBofObject() 
{
	if(m_mesh.vertices_empty())
		return;

	TriMesh::VertexIter vbegin = m_mesh.vertices_begin();
	TriMesh::Point pbegin = m_mesh.point(vbegin);

	double m_leastX  = pbegin[0], m_mostX  = pbegin[0];
	double m_leastY  = pbegin[1], m_mostY  = pbegin[1];
	double m_leastZ  = pbegin[2], m_mostZ  = pbegin[2];

	TriMesh::Point vertex;
	for(TriMesh::VertexIter vi = vbegin; vi != m_mesh.vertices_end(); vi++)
	{
		vertex = m_mesh.point(vi);

		//minx & maxx
		if(vertex[0] < m_leastX)
			m_leastX = vertex[0];
		else if(m_mostX < vertex[0])
			m_mostX = vertex[0];

		//miny & maxy
		if(vertex[1] < m_leastY)
			m_leastY = vertex[1];
		else if(m_mostY < vertex[1])
			m_mostY = vertex[1];

		//minz & maxz
		if(vertex[2] < m_leastZ)
			m_leastZ = vertex[2];
		else if(m_mostZ < vertex[2])
			m_mostZ = vertex[2];
	}

	m_centerOfAABB.m_x = (m_leastX + m_mostX)/2.0;
	m_centerOfAABB.m_y = (m_leastY + m_mostY)/2.0;
	m_centerOfAABB.m_z = (m_leastZ + m_mostZ)/2.0;

	boundBoxEdgeLen = m_mostX - m_leastX;
	if(m_mostY - m_leastY > boundBoxEdgeLen)
		boundBoxEdgeLen = m_mostY - m_leastY;
	if(m_mostZ - m_leastZ > boundBoxEdgeLen)
		boundBoxEdgeLen = m_mostZ - m_leastZ;
}

double MyTriMesh::GetAverageEdgeLength()
{
	double averageEdgeLen = 0; 
	int numEdges = 0;
	for (TriMesh::VertexIter v_it = m_mesh.vertices_begin(); v_it != m_mesh.vertices_end(); ++v_it)
	{
		TriMesh::Point point0 = m_mesh.point(v_it);
		for (TriMesh::VertexVertexIter vv_it = m_mesh.vv_iter(v_it);  vv_it;  ++vv_it)
		{
			TriMesh::Point point1 = m_mesh.point(vv_it);
			averageEdgeLen += (point0 - point1).length();
			numEdges++;
		}
	}
	averageEdgeLen /= 2*numEdges;
	return averageEdgeLen;
}


void MyTriMesh::ReComputeVertexNormal()
{
	m_mesh.request_face_normals();
	m_mesh.request_vertex_normals();
	m_mesh.update_normals();

	m_vertexNormal.resize(m_mesh.n_vertices());
	
	for (TriMesh::VertexIter vi = m_mesh.vertices_begin(); vi != m_mesh.vertices_end(); vi++)
	{
		double sumArea = 0;
		vector<double> triAreas;
		vector<Vector3D> triFaceNorm;

		for(TriMesh::VertexFaceIter vf_i = m_mesh.vf_begin(vi); vf_i != m_mesh.vf_end(vi); vf_i++)
		{
			int i = 0;
			Point3D triPoint[3];
			triFaceNorm.push_back(Vector3D(m_mesh.normal(vf_i)[0],m_mesh.normal(vf_i)[1],m_mesh.normal(vf_i)[2]));

			for(TriMesh::FaceVertexIter fv_i = m_mesh.fv_begin(vf_i); fv_i != m_mesh.fv_end(vf_i); fv_i++)
			{
				TriMesh::Point point =  m_mesh.point(fv_i);
				triPoint[i++] = Point3D(point[0],point[1],point[2]);
			}

			double area = SimpleCompute::GetTriangleArea(triPoint[0],triPoint[1],triPoint[2]);
			triAreas.push_back(area);
			sumArea += area;
		}

		Vector3D avgNorm;
		for(int i = 0; i < triAreas.size(); i++)
		{
			double rato = triAreas[i]/sumArea;
			avgNorm += triFaceNorm[i] * rato;
		}

		avgNorm.mf_normalize();
		m_vertexNormal[vi.handle().idx()] = avgNorm;
	}
}


void MyTriMesh::LoadMesh()
{
	CString m_filePath;
	CFileDialog file(true);	
	file.m_ofn.lpstrFilter =_T("Mesh File(*.obj)\0*.obj\0Off File(*.off)\0*.off\0All File(*.*)\0*.*\0\0");
	file.m_ofn.lpstrTitle  =_T("OPEN");
	if(file.DoModal()==IDOK)
	{
		m_filePath =  file.GetPathName();
		OpenMesh::IO::read_mesh(m_mesh,m_filePath.GetBuffer(0));
	}

	init();
	GetAABBofObject();
	//ReComputeVertexNormal();
}

void MyTriMesh::SaveMesh()
{
	if(m_mesh.vertices_empty())
		return;

	CString m_filePath;
	CFileDialog file(false);
	file.m_ofn.lpstrFilter =_T("mesh file(*.obj)\0*.obj\0Off File(*.off)\0*.off\0All File(*.*)\0*.*\0\0");
	file.m_ofn.lpstrTitle  =_T("SAVE");
	if(file.DoModal()==IDOK)
	{
		m_filePath =  file.GetPathName();
		OpenMesh::IO::write_mesh(m_mesh,m_filePath.GetBuffer(0));
	}
}
