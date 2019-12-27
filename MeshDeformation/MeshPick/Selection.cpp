#include "stdafx.h"
#include "Selection.h"
#include "glut.h"

Selection::Selection(void)
{
	this->vec_selected_pts_index.clear();
	this->pts = NULL;
}


Selection::~Selection(void)
{

}

void Selection::highlight_selected_pts()
{
	int i = 0;

	//保存上一个opengl状态
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixf(proj);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(model_view);

	glPushAttrib(GL_POINT_BIT);
	glPointSize(5);
	glColor4f(0, 1, 0, 1);
	glDisable(GL_LIGHTING);
	glBegin(GL_POINTS);
	for(i = 0; i != vec_selected_pts_index.size(); i++){
		glVertex3fv(pts[ vec_selected_pts_index[i] ]);
	}
	glEnable(GL_LIGHTING);

	glEnd(); 
	glPopAttrib();
	glPopMatrix();
	glPopMatrix();
}

void Selection::get_selected_pts_index( vector<int> &v )
{
	// assign 赋值最快 
	v.assign(vec_selected_pts_index.begin(), vec_selected_pts_index.end());
}

void Selection::set_config( M3DVector3f *_pts, int _nr, M3DVector2f _left_bottom, M3DVector2f _right_top, M3DMatrix44f _model_view, M3DMatrix44f _proj, int _viewport[] )
{
	pts = _pts;
	nr  = _nr;

	int x = pts[2][0];

	m3dCopyVector2(left_bottom, _left_bottom);
	m3dCopyVector2(right_top, _right_top);

	m3dCopyMatrix44(model_view, _model_view);
	m3dCopyMatrix44(proj, _proj);
	memcpy(viewport, _viewport, sizeof(int) * 4);

	/*** 计算被选中的index ***/

	cal_selected_index();
}

void Selection::cal_selected_index()
{
	vec_selected_pts_index.clear();

	int i = 0;
	for(i = 0; i != nr; i++){

		if( drop_in_area(pts[i]) ){
			vec_selected_pts_index.push_back(i);
		} 

	}
}

bool Selection::drop_in_area( M3DVector3f x )
{
	M3DVector2f win_coord;

	m3dProjectXY(win_coord, model_view, proj, viewport, x);

	if( (win_coord[0] < left_bottom[0] && win_coord[0] <right_top[0]) || (win_coord[0] > left_bottom[0] && win_coord[0] > right_top[0] ))
		return false;

	if( (win_coord[1] < left_bottom[1] && win_coord[1] <right_top[1]) || (win_coord[1] > left_bottom[1] && win_coord[1] > right_top[1] ))
		return false;

	return true;
}
