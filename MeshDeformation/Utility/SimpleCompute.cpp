#include "StdAfx.h"
#include "SimpleCompute.h"


SimpleCompute::SimpleCompute(void)
{
}


SimpleCompute::~SimpleCompute(void)
{
}

Point3D SimpleCompute::ComputeMatrixMultiPoint(Matrix4d Mt, Point3D point)
{
	/*
	|m1  m2  m3  m4 |   |x|
	|m5  m6  m7  m8 | * |y|
	|m9  m10 m11 m12|	|z|
	|m13 m14 m15 m16|	|1|
	*/

	Point3D tar;
	tar.m_x = Mt(0,0)*point.m_x + Mt(0,1)*point.m_y + Mt(0,2)*point.m_z + Mt(0,3);
	tar.m_y = Mt(1,0)*point.m_x + Mt(1,1)*point.m_y + Mt(1,2)*point.m_z + Mt(1,3);
	tar.m_z = Mt(2,0)*point.m_x + Mt(2,1)*point.m_y + Mt(2,2)*point.m_z + Mt(2,3);

	return tar;
}

Point3D SimpleCompute::ComputeMatrixMultiPoint(Matrix3d Mt, Point3D point)
{
	/*
	|m1  m2  m3 |   |x|
	|m4  m5  m6 | * |y|
	|m7  m8  m9 |	|z|
	*/

	Point3D tar;
	tar.m_x = Mt(0,0)*point.m_x + Mt(0,1)*point.m_y + Mt(0,2)*point.m_z;
	tar.m_y = Mt(1,0)*point.m_x + Mt(1,1)*point.m_y + Mt(1,2)*point.m_z;
	tar.m_z = Mt(2,0)*point.m_x + Mt(2,1)*point.m_y + Mt(2,2)*point.m_z;

	return tar; 
}

Vector3D SimpleCompute::ComputeMatrixMutiVector(Matrix3d Mt, Vector3D vec)
{
	Vector3D tar;
	tar.m_x = Mt(0,0)*vec.m_x + Mt(0,1)*vec.m_y + Mt(0,2)*vec.m_z;
	tar.m_y = Mt(1,0)*vec.m_x + Mt(1,1)*vec.m_y + Mt(1,2)*vec.m_z;
	tar.m_z = Mt(2,0)*vec.m_x + Mt(2,1)*vec.m_y + Mt(2,2)*vec.m_z;

	return tar;
}

double SimpleCompute::GetTriangleArea(const Point3D& v1,  const Point3D& v2, const Point3D& v3)
{
	Vector3D a = v2 - v1;
	Vector3D b = v3 - v1;
	Vector3D c = v2 - v3;

	double aa = a.mf_getLength();
	double bb = b.mf_getLength();
	double cc = c.mf_getLength();
	double p = (aa+bb+cc)/2.0;
	double area = sqrt(p*(p-aa)*(p-bb)*(p-cc));

	return area;
}

Vector3D SimpleCompute::GetTriangleVertexGradient( Vector3D a,  Vector3D b)
{
	Vector3D high, c;
	double len2 = 0;
	a = a*(-1);
	b = b*(-1);
	c = a - b;

	double dotres = a * c;

	double lenc2 = c.m_x*c.m_x + c.m_y*c.m_y + c.m_z*c.m_z;
	double ratio = dotres / (lenc2);

	high = (b - a)*ratio + a;
	high = high*(-1);

	len2 = high.m_x*high.m_x + high.m_y*high.m_y + high.m_z*high.m_z;
	high.m_x /= len2;
	high.m_y /= len2;
	high.m_z /= len2;

	return high;
}

Matrix4d SimpleCompute::Rotation2Matrix(double angle_x, double angle_y, double angle_z, bool x, bool y, bool z)
{
	Matrix4d rotation_x;
	Matrix4d rotation_y;
	Matrix4d rotation_z;
	Matrix4d result;

	rotation_y = Rotation2Matrix(angle_x,0,1,0);
	rotation_x = Rotation2Matrix(angle_y,1,0,0);

	result = rotation_y*rotation_x;
	return result;
}

Matrix4d SimpleCompute::Rotation2Matrix(double angle,int x,int y,int z)
{
	Matrix4d result = Matrix4d::Identity();

	double hudu = angle/180.0 * PI;
	double cosa = cos(hudu);
	double sina = sin(hudu);

	if(1 == x)
	{
		result(1,1) = cosa;
		result(1,2) = sina;
		result(2,1) = -sina;
		result(2,2) = cosa;
	}
	else if(1 == y)
	{
		result(0,0) = cosa;
		result(0,2) = -sina;
		result(2,0) = sina;
		result(2,2) = cosa;
	}
	else if(1 == z)
	{
		result(0,0) = cosa;
		result(0,1) = sina;
		result(1,0) = -sina;
		result(1,1) = cosa;
	}
	return result;
}

Matrix4d SimpleCompute::Translate2Matrix(double x,double y,double z)
{
	Matrix4d result = Matrix4d::Identity();

	result(0,3) = x;
	result(1,3) = y;
	result(2,3) = z;

	return result;
}

Matrix4d SimpleCompute::Scale2Matrix(double x,double y,double z)
{
	Matrix4d result = Matrix4d::Identity();

	result(0,0) = x;
	result(1,1) = y;
	result(2,2) = z;

	return result;
}

 Matrix4d SimpleCompute::MatrixMultNum(const Matrix4d mat, double factor)
 {
	 Matrix4d mt = mat*factor;
	 return mt;
 }

 Vector3D SimpleCompute::getNormalOfTri(const Point3D p1,const Point3D p2,const Point3D p3)
 {
	 Vector3D v1(p2.m_x-p1.m_x, p2.m_y-p1.m_y, p2.m_z-p1.m_z);
	 Vector3D v2(p3.m_x-p1.m_x, p3.m_y-p1.m_y, p3.m_z-p1.m_z);

	 Vector3D result = (v1^v2);
	 result.mf_normalize();

	 return result;
 }

 double SimpleCompute::ComputeMeshVolume(TriMesh& desMesh)
 {
	 double  minZ  = INFINITY;
	 for(TriMesh::VertexIter vi = desMesh.vertices_begin(); vi != desMesh.vertices_end(); vi++)
	 {	
		 if(desMesh.point(vi)[2] < minZ)
			 minZ = desMesh.point(vi)[2];
	 }
	 minZ -= 1;

	
	 Vector3D upVector(0,0,1);
	 double meshVolume = 0;

	 desMesh.request_face_normals();
	 desMesh.request_vertex_normals();
	 desMesh.update_normals();

	 for(TriMesh::FaceIter f_it = desMesh.faces_begin(); f_it != desMesh.faces_end(); f_it++)
	 {
		TriMesh::Normal fn = desMesh.normal(f_it);
		Vector3D f_normal = Vector3D(fn[0],fn[1],fn[2]);
		double dianji = f_normal * upVector;
		int signx = dianji >= 0 ? 1 : -1; 
		double prismHeight = 0;

		int i = 0;
		Point3D triVertex[3];
		for(TriMesh::FaceVertexIter fv_it = desMesh.fv_begin(f_it); fv_it != desMesh.fv_end(f_it); fv_it++)
		{
			TriMesh::Point point = desMesh.point(fv_it);
			triVertex[i++] = Point3D(point[0],point[1],point[2]);
			prismHeight += (point[2] - minZ);
		}
		prismHeight /= 3.0;
		

		Vector2D edge_xy_1(triVertex[1].m_x - triVertex[0].m_x, triVertex[1].m_y - triVertex[0].m_y);
		Vector2D edge_xy_2(triVertex[2].m_x - triVertex[1].m_x, triVertex[2].m_y - triVertex[1].m_y);
		double area_xy = edge_xy_1^edge_xy_2*signx;
		double prismVolume = 1/3.0 * area_xy * prismHeight;
		meshVolume += prismVolume;
	 }

	return meshVolume;
 }

 void SimpleCompute::EvaluateVNormalByArea(TriMesh& desMesh,vector<Vector3D>& vNormal)
 {
	 desMesh.request_face_normals();
	 desMesh.request_vertex_normals();
	 desMesh.update_normals();

	 vNormal.resize(desMesh.n_vertices());

	 for (TriMesh::VertexIter vi = desMesh.vertices_begin(); vi != desMesh.vertices_end(); vi++)
	 {
		 double sumArea = 0;
		 vector<double> triAreas;
		 vector<Vector3D> triFaceNorm;

		 
		 for(TriMesh::VertexFaceIter vf_i = desMesh.vf_begin(vi); vf_i != desMesh.vf_end(vi); vf_i++)
		 {
			 int i = 0;
			 Point3D triPoint[3];
			 triFaceNorm.push_back(Vector3D(desMesh.normal(vf_i)[0],desMesh.normal(vf_i)[1],desMesh.normal(vf_i)[2]));

			 for(TriMesh::FaceVertexIter fv_i = desMesh.fv_begin(vf_i); fv_i != desMesh.fv_end(vf_i); fv_i++)
			 {
				 TriMesh::Point point =  desMesh.point(fv_i);
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
		 vNormal[vi.handle().idx()] = avgNorm;
		 

		// Vector3D vNorm(desMesh.normal(vi)[0],desMesh.normal(vi)[1],desMesh.normal(vi)[2]);
		// vNormal[vi.handle().idx()] = vNorm;
	 }
 }