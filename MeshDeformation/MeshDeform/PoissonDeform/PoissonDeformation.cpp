#include "stdafx.h"
#include "PoissonDeformation.h"
#include "../../Utility/MyAfxFunction.h"
#include "../../Utility/baseHeadFile.h"
#include "../../Utility/CP_PointVector.h"

PoissonDeformation::PoissonDeformation(TriMesh* mesh)
{
	m_mesh = *mesh;
	m_static_mesh = *mesh;
	m_handTransMat = Matrix4d::Identity();
	poissonCtrl.m_mesh = mesh;
	m_quater_fixed.R2Q(0,1,0,0);
}

void PoissonDeformation::reset()
{
	poissonCtrl.Clear();
	m_mesh = m_static_mesh;
	m_handTransMat = Matrix4d::Identity();
}

void PoissonDeformation::InteracTransform(const Matrix4d mat,bool localTransform)
{ 
	m_handTransMat = mat*m_handTransMat;
	poissonCtrl.HandTransform(mat);
	m_quater_hand.RotationMatrix2Qua(m_handTransMat);
}

void PoissonDeformation::DeformReset()
{
	if(!m_static_mesh.vertices_empty())
	{
		m_mesh = m_static_mesh;
		afxGetMainMesh()->UpdateDisList(&m_static_mesh);
		poissonCtrl.Reset();
		m_handTransMat = Matrix4d::Identity();
	}
}

//compute laplacian matrix
void PoissonDeformation::ComputeCoefficientMatrix()
{
	vector<bool> isControlVertex = poissonCtrl.GetVertexProperty();
	myLPLslover.SetDesMesh(m_mesh);
	myLPLslover.SetControlVertex(isControlVertex);
	myLPLslover.ComputeLalacianMatrixA();

	poissonCtrl.ComputeFreeVertexWeight();
}

//compute divergence
Vector3D PoissonDeformation::ComputeTriangleDiv(const Point3D& source,const Point3D& vleft,const Point3D& vright,int l,int r)
{
	Vector3D s_l = source - vleft;
	Vector3D s_r = source - vright;
	Vector3D l_s = s_l*(-1);
	Vector3D l_r = vleft - vright;
	Vector3D r_s = s_r*(-1);
	Vector3D r_l = vright - vleft;

	//¨Œ¦µiT
	Vector3D ha =  SimpleCompute::GetTriangleVertexGradient(s_l,s_r);
	Vector3D hb =  SimpleCompute::GetTriangleVertexGradient(l_r,l_s);
	Vector3D hc =  SimpleCompute::GetTriangleVertexGradient(r_s,r_l);

	//gradient field
	Vector3D wx = hb*(l_s.m_x) + hc*(r_s.m_x);
	Vector3D wy = hb*(l_s.m_y) + hc*(r_s.m_y);
	Vector3D wz = hb*(l_s.m_z) + hc*(r_s.m_z);

	//S¡÷
	double area = SimpleCompute::GetTriangleArea(source,vleft,vright);

	//divergence
	Vector3D div = Vector3D(wx*ha*area,wy*ha*area,wz*ha*area);
	return   div;
}

void PoissonDeformation::TriangleLocalTransform(TriMesh::VertexHandle vh_s, TriMesh::VertexHandle vh_l,TriMesh::VertexHandle vh_r,
												Point3D& source,Point3D& left,Point3D& right)
{
	Matrix4d triangleTransMatrix = Matrix4d::Identity();
	Matrix4d interpMat= Matrix4d::Identity();

	int s = vh_s.idx(), l = vh_l.idx(), r = vh_r.idx();
	Point3D v      =  Point3D(m_mesh.point(vh_s)[0],m_mesh.point(vh_s)[1],m_mesh.point(vh_s)[2]);
	Point3D	vleft  =  Point3D(m_mesh.point(vh_l)[0],m_mesh.point(vh_l)[1],m_mesh.point(vh_l)[2]);
	Point3D vright =  Point3D(m_mesh.point(vh_r)[0],m_mesh.point(vh_r)[1],m_mesh.point(vh_r)[2]);
	Point3D center((v.m_x+vleft.m_x+vright.m_x)/3.0, (v.m_y+vleft.m_y+vright.m_y)/3.0, (v.m_z+vleft.m_z+vright.m_z)/3.0);

	//ensure local transform
	Matrix4d  t1 = SimpleCompute::Translate2Matrix(center.m_x,center.m_y,center.m_z);
	Matrix4d  t2 = SimpleCompute::Translate2Matrix(-center.m_x,-center.m_y,-center.m_z);

	//interpolation with bc`s geodesic distance 
	double factor_s = poissonCtrl.m_freeVertexWeight[s];
	double factor_l = poissonCtrl.m_freeVertexWeight[l];
	double factor_r = poissonCtrl.m_freeVertexWeight[r];
	double factor = (factor_s+factor_l+factor_r)/3.0;

    //quaternuon  interpolation
	CQuaternion quater;
	quater.Slerp(m_quater_hand,m_quater_fixed,factor);
	quater.Q2R(interpMat);

	//local transform
	triangleTransMatrix = t1*interpMat*t2;

	source = SimpleCompute::ComputeMatrixMultiPoint(triangleTransMatrix,v);
	left   = SimpleCompute::ComputeMatrixMultiPoint(triangleTransMatrix,vleft);
	right  = SimpleCompute::ComputeMatrixMultiPoint(triangleTransMatrix,vright);
}

void PoissonDeformation::ComputeDivergence()
{
	divMatri_x = VectorXd::Zero(m_mesh.n_vertices());
	divMatri_y = VectorXd::Zero(m_mesh.n_vertices());
	divMatri_z = VectorXd::Zero(m_mesh.n_vertices());
	vector<bool> isControlVertex = poissonCtrl.GetVertexProperty();
	vector<Point3D> PosOfHand = poissonCtrl.m_posOfHand;
	vector<int> handVindex = poissonCtrl.m_HandVindex;

	int vid = 0,l = 0,r = 0;
	TriMesh::VertexHandle vh0,vh1,vh2;
	for(TriMesh::VertexIter v_it = m_static_mesh.vertices_begin(); v_it != m_static_mesh.vertices_end(); v_it++)
	{
		vid = v_it.handle().idx();
		if(isControlVertex[vid])
		{
			divMatri_x[vid] = m_mesh.point(v_it)[0];
			divMatri_y[vid] = m_mesh.point(v_it)[1];
			divMatri_z[vid] = m_mesh.point(v_it)[2];
			continue;
		}

		for(TriMesh::VertexFaceIter vf_it = m_static_mesh.vf_begin(v_it); vf_it != m_static_mesh.vf_end(v_it); vf_it++)
		{	
			TriMesh::FaceVertexIter fv_it = m_static_mesh.fv_begin(vf_it);
			int tri_vid0 = fv_it.handle().idx(); fv_it++;
			int tri_vid1 = fv_it.handle().idx(); fv_it++;
			int tri_vid2 = fv_it.handle().idx(); 

			if(tri_vid0 == vid)		{ l = tri_vid1; r = tri_vid2;}
			else if(tri_vid1 == vid){ l = tri_vid0; r = tri_vid2;}
			else					{ l = tri_vid0; r = tri_vid1;}
			vh0 = m_static_mesh.vertex_handle(vid); 
			vh1 = m_static_mesh.vertex_handle(l); 
			vh2 = m_static_mesh.vertex_handle(r); 

			//triangle local transform 
			Point3D source_,right_,left_;
			TriangleLocalTransform(vh0,vh1,vh2,source_,left_,right_);

			//compute divergence
			Vector3D W = ComputeTriangleDiv(source_,left_,right_,l,r);
			divMatri_x[vid] += W.m_x;
			divMatri_y[vid] += W.m_y;
			divMatri_z[vid] += W.m_z;
		}
	}

	//vertex at hand 
	for(int j = 0; j < PosOfHand.size(); j++)
	{
		vid = handVindex[j];
		divMatri_x[vid] = PosOfHand[j].m_x;
		divMatri_y[vid] = PosOfHand[j].m_y;
		divMatri_z[vid] = PosOfHand[j].m_z;
	}
}

void PoissonDeformation::MyDeformation()
{
	ComputeDivergence();

	//solve x
	myLPLslover.SetRightHandB(divMatri_x);
	VectorXd x = myLPLslover.LplacianSolve();

	//solve y
	myLPLslover.SetRightHandB(divMatri_y);
	VectorXd y = myLPLslover.LplacianSolve();

	//solve z
	myLPLslover.SetRightHandB(divMatri_z);
	VectorXd z = myLPLslover.LplacianSolve();

	//update mesh
	TriMesh::VertexHandle vh; 

	for(int i = 0; i < m_mesh.n_vertices(); i++)
	{
		vh  = m_mesh.vertex_handle(i);

		m_mesh.point(vh)[0] = x[i];
		m_mesh.point(vh)[1] = y[i];
		m_mesh.point(vh)[2] = z[i];
	}

	poissonCtrl.m_mesh = &m_mesh;
	poissonCtrl.UpdateHandPos();
	m_handTransMat =Matrix4d::Identity();
	afxGetMainMesh()->UpdateDisList(&m_mesh);
}