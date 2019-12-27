#ifndef CQUATERNION_H
#define CQUATERNION_H

#include "baseHeadFile.h"

class CQuaternion
{
public:
	double w;
	double x;
	double y;
	double z;
	
public:
	CQuaternion();
	//~CQuaternion();

	CQuaternion(double a,double b,double c,double d);
	CQuaternion  operator +(const CQuaternion &);
	CQuaternion  operator -(const CQuaternion &);
	CQuaternion  operator *(const double &);
	CQuaternion  operator /(const double &);
	CQuaternion  operator *(const CQuaternion &);
	void R2Q(const double &,const double &,const double & ,const double &);
	void Q2R(Matrix4d& q_r);
	int Sign(double x){return (x > 0)?1:-1;};
	void GetInverse();
	double GetNorm();
	void Normalize();
	double DotProduct(const CQuaternion&  q);
	void RotationMatrix2Qua(const Matrix4d& mat);
	CQuaternion& Slerp(CQuaternion& qua1, CQuaternion& qua2, double t);
	
};
#endif 