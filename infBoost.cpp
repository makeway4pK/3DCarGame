#include <fstream>
#include <graphics.h>
#include <time.h>
#include "Eigen/Dense"
using namespace std;
using namespace Eigen;
#define DType float  // data type for matrices
#define C_r 40		 // coins' radius
#define C_t 8		 // coins' thickness
#define O_r 40		 // obstacles' radius
#define Car_w 100	// Car width
#define Car_l 250	// Car length
#define Car_yOff 500 // Car offset from road mid
#define camspeed 3   // cam rotation speed
#define R_l 2400	 // Road's length
#define R_tw 200	 // Road's third width
#define R_sp 1		 // Road speed, boost=booster*R_sp
#define C_int 350	// coin spawn interval
#define O_int 600	// obstacle spawn interval

class Camera
{
  public:
	DType d, mag, zAng, pitchAng;
	int xShift, yShift;
	Matrix<DType, 4, 4> basis;
	Matrix<DType, 4, 4> project;

	Camera()
	{
		d = -getmaxx() / 2;
		mag = 0.3;
		basis << 1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;

		xShift = getmaxx() / 2;
		yShift = getmaxy() / 2;
		zAng = pitchAng = 0; //in rad
		rePos();			 // initialises projection matrix
	}

	Matrix<DType, 4, 2, RowMajor> view(const Matrix<DType, 4, 4, RowMajor> &obj)
	{
		Matrix<DType, 4, 4, RowMajor> v;
		v.noalias() = obj * project;
		return v.block<4, 2>(0, 0);
	}
	Matrix<DType, 3, 2, RowMajor> view2d(const Matrix<DType, 3, 4, RowMajor> &obj)
	{
		Matrix<DType, 3, 4, RowMajor> v;
		v.noalias() = obj * project;
		return v.block<3, 2>(0, 0);
	}
	Matrix<DType, 1, 2, RowMajor> view0d(const Matrix<DType, 1, 4, RowMajor> &obj)
	{
		Matrix<DType, 1, 4, RowMajor> v;
		v.noalias() = obj * project;
		return v.block<1, 2>(0, 0);
	}

	void rePos()
	{ //to recalc inverse of camstate matrix
		project.noalias() = basis.adjoint() * mag;
		project.row(3) << xShift, yShift, 0, 1;
	}
	void reOrient()
	{

		Matrix<DType, 4, 4> ro;
		// first rotate around World's Z-axis
		DType c = cos(zAng), s = sin(zAng);
		ro << c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;
		project.noalias() = basis * ro;

		// then rotate Cam's X-axis
		c = cos(pitchAng), s = sin(pitchAng);
		ro << 1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1;
		project.noalias() = project * ro * mag;
		project.row(3) << xShift, yShift, 0, 1;
	}

	void dView(DType e)
	{
		d += e;
	}
	void moveX(DType x)
	{
		basis.row(3) += x * basis.row(0);
	}
	void moveY(DType y)
	{
		basis.row(3) += y * basis.row(0);
	}
	void moveZ(DType z)
	{
		basis.row(3) += z * basis.row(0);
	}
	void pitch(DType t)
	{ // Rotates around camXaxis
		pitchAng += t / 57.295779513082320876798154814105;
		if (pitchAng > 1.5707963267948966192313216916398)
			pitchAng = 1.5707963267948966192313216916398;
		else if (pitchAng < 0)
			pitchAng = 0;
	}
	void yaw(DType t)
	{ //rotates around worldZaxis
		zAng += t / 57.295779513082320876798154814105;
		if (zAng < -1.5707963267948966192313216916398)
			zAng = -1.5707963267948966192313216916398;
		else if (zAng > 0)
			zAng = 0;
	}
	// void roY(DType t)
	// {// Rotates around worldYaxis
	// 	Matrix<DType, 4, 4> rotY;
	// 	t *= 3.1428 / 180;
	// 	DType c = cos(t);
	// 	t = sin(t);
	// 	rotY << c, 0, t, 0,
	// 		0, 1, 0, 0,
	// 		-t, 0, c, 0,
	// 		0, 0, 0, 1;
	// 	basis.noalias() = basis * rotY;
	// }
};

class Car
{
	int priCol;

  public:
	Matrix<DType, 4, 4> xyzp;
	Car(int c = COLOR(240, 121, 24))
	{
		priCol = c;
		xyzp = Matrix<DType, 4, 4>::Zero();
		xyzp.diagonal() << Car_w, Car_l, -Car_l / 5, 1;
		xyzp.row(3) << 0, Car_yOff, 0, 1;
	}
	void draw(Camera &cam)
	{
		Matrix<DType, 4, 2> flat = cam.view(xyzp);
		int d[20];

		// slants
		// setcolor(COLOR(240, 121, 24));
		setcolor(COLOR(128, 128, 128));
		//bonnet
		setfillstyle(1, priCol);
		d[0] = flat(3, 0) + 0.5 * flat(0, 0) + 0.25 * flat(2, 0),
		d[2] = flat(3, 0) + 0.5 * flat(0, 0) + 0.2 * flat(1, 0) + 0.5 * flat(2, 0),
		d[4] = flat(3, 0) - 0.5 * flat(0, 0) + 0.2 * flat(1, 0) + 0.5 * flat(2, 0),
		d[6] = flat(3, 0) - 0.5 * flat(0, 0) + 0.25 * flat(2, 0);
		d[1] = flat(3, 1) + 0.5 * flat(0, 1) + 0.25 * flat(2, 1),
		d[3] = flat(3, 1) + 0.5 * flat(0, 1) + 0.2 * flat(1, 1) + 0.5 * flat(2, 1),
		d[5] = flat(3, 1) - 0.5 * flat(0, 1) + 0.2 * flat(1, 1) + 0.5 * flat(2, 1),
		d[7] = flat(3, 1) - 0.5 * flat(0, 1) + 0.25 * flat(2, 1);
		fillpoly(4, d);

		//headlamps
		setfillstyle(1, YELLOW);
		d[0] = flat(3, 0) + 0.3 * flat(0, 0) + 0.25 * flat(2, 0),
		d[2] = flat(3, 0) + 0.5 * flat(0, 0) + 0.25 * flat(2, 0),
		d[4] = flat(3, 0) + 0.5 * flat(0, 0) + 0.1 * flat(1, 0) + 0.375 * flat(2, 0),
		d[6] = flat(3, 0) + 0.3 * flat(0, 0) + 0.1 * flat(1, 0) + 0.375 * flat(2, 0);
		d[1] = flat(3, 1) + 0.3 * flat(0, 1) + 0.25 * flat(2, 1),
		d[3] = flat(3, 1) + 0.5 * flat(0, 1) + 0.25 * flat(2, 1),
		d[5] = flat(3, 1) + 0.5 * flat(0, 1) + 0.1 * flat(1, 1) + 0.375 * flat(2, 1),
		d[7] = flat(3, 1) + 0.3 * flat(0, 1) + 0.1 * flat(1, 1) + 0.375 * flat(2, 1);
		fillpoly(4, d);
		d[0] = flat(3, 0) - 0.3 * flat(0, 0) + 0.25 * flat(2, 0),
		d[2] = flat(3, 0) - 0.5 * flat(0, 0) + 0.25 * flat(2, 0),
		d[4] = flat(3, 0) - 0.5 * flat(0, 0) + 0.1 * flat(1, 0) + 0.375 * flat(2, 0),
		d[6] = flat(3, 0) - 0.3 * flat(0, 0) + 0.1 * flat(1, 0) + 0.375 * flat(2, 0);
		d[1] = flat(3, 1) - 0.3 * flat(0, 1) + 0.25 * flat(2, 1),
		d[3] = flat(3, 1) - 0.5 * flat(0, 1) + 0.25 * flat(2, 1),
		d[5] = flat(3, 1) - 0.5 * flat(0, 1) + 0.1 * flat(1, 1) + 0.375 * flat(2, 1),
		d[7] = flat(3, 1) - 0.3 * flat(0, 1) + 0.1 * flat(1, 1) + 0.375 * flat(2, 1);
		fillpoly(4, d);

		// windows
		// setcolor(COLOR(100, 100, 100));
		setfillstyle(1, 0);
		//windshield
		d[0] = flat(3, 0) + 0.5 * flat(0, 0) + 0.4 * flat(1, 0) + 0.5 * flat(2, 0),
		d[2] = flat(3, 0) + 0.5 * flat(0, 0) + 0.6 * flat(1, 0) + flat(2, 0),
		d[4] = flat(3, 0) - 0.5 * flat(0, 0) + 0.6 * flat(1, 0) + flat(2, 0),
		d[6] = flat(3, 0) - 0.5 * flat(0, 0) + 0.4 * flat(1, 0) + 0.5 * flat(2, 0);
		d[1] = flat(3, 1) + 0.5 * flat(0, 1) + 0.4 * flat(1, 1) + 0.5 * flat(2, 1),
		d[3] = flat(3, 1) + 0.5 * flat(0, 1) + 0.6 * flat(1, 1) + flat(2, 1),
		d[5] = flat(3, 1) - 0.5 * flat(0, 1) + 0.6 * flat(1, 1) + flat(2, 1),
		d[7] = flat(3, 1) - 0.5 * flat(0, 1) + 0.4 * flat(1, 1) + 0.5 * flat(2, 1);
		fillpoly(4, d);

		//bonnet
		setfillstyle(1, priCol);
		d[0] = flat(3, 0) + 0.5 * flat(0, 0) + 0.2 * flat(1, 0) + 0.5 * flat(2, 0),
		d[2] = flat(3, 0) + 0.5 * flat(0, 0) + 0.4 * flat(1, 0) + 0.5 * flat(2, 0),
		d[4] = flat(3, 0) - 0.5 * flat(0, 0) + 0.4 * flat(1, 0) + 0.5 * flat(2, 0),
		d[6] = flat(3, 0) - 0.5 * flat(0, 0) + 0.2 * flat(1, 0) + 0.5 * flat(2, 0);
		d[1] = flat(3, 1) + 0.5 * flat(0, 1) + 0.2 * flat(1, 1) + 0.5 * flat(2, 1),
		d[3] = flat(3, 1) + 0.5 * flat(0, 1) + 0.4 * flat(1, 1) + 0.5 * flat(2, 1),
		d[5] = flat(3, 1) - 0.5 * flat(0, 1) + 0.4 * flat(1, 1) + 0.5 * flat(2, 1),
		d[7] = flat(3, 1) - 0.5 * flat(0, 1) + 0.2 * flat(1, 1) + 0.5 * flat(2, 1);
		fillpoly(4, d);

		//backWindow
		setfillstyle(1, 0);
		d[0] = flat(3, 0) + 0.5 * flat(0, 0) + flat(1, 0) + 0.5 * flat(2, 0),
		d[2] = flat(3, 0) + 0.5 * flat(0, 0) + 0.8 * flat(1, 0) + flat(2, 0),
		d[4] = flat(3, 0) - 0.5 * flat(0, 0) + 0.8 * flat(1, 0) + flat(2, 0),
		d[6] = flat(3, 0) - 0.5 * flat(0, 0) + flat(1, 0) + 0.5 * flat(2, 0);
		d[1] = flat(3, 1) + 0.5 * flat(0, 1) + flat(1, 1) + 0.5 * flat(2, 1),
		d[3] = flat(3, 1) + 0.5 * flat(0, 1) + 0.8 * flat(1, 1) + flat(2, 1),
		d[5] = flat(3, 1) - 0.5 * flat(0, 1) + 0.8 * flat(1, 1) + flat(2, 1),
		d[7] = flat(3, 1) - 0.5 * flat(0, 1) + flat(1, 1) + 0.5 * flat(2, 1);
		fillpoly(4, d);

		//flats
		//top
		setfillstyle(1, priCol);
		d[0] = flat(3, 0) + 0.5 * flat(0, 0) + 0.6 * flat(1, 0) + flat(2, 0),
		d[2] = flat(3, 0) + 0.5 * flat(0, 0) + 0.8 * flat(1, 0) + flat(2, 0),
		d[4] = flat(3, 0) - 0.5 * flat(0, 0) + 0.8 * flat(1, 0) + flat(2, 0),
		d[6] = flat(3, 0) - 0.5 * flat(0, 0) + 0.6 * flat(1, 0) + flat(2, 0);
		d[1] = flat(3, 1) + 0.5 * flat(0, 1) + 0.6 * flat(1, 1) + flat(2, 1),
		d[3] = flat(3, 1) + 0.5 * flat(0, 1) + 0.8 * flat(1, 1) + flat(2, 1),
		d[5] = flat(3, 1) - 0.5 * flat(0, 1) + 0.8 * flat(1, 1) + flat(2, 1),
		d[7] = flat(3, 1) - 0.5 * flat(0, 1) + 0.6 * flat(1, 1) + flat(2, 1);
		fillpoly(4, d);

		//back
		d[0] = flat(3, 0) + 0.5 * flat(0, 0) + flat(1, 0),
		d[2] = flat(3, 0) + 0.5 * flat(0, 0) + flat(1, 0) + 0.5 * flat(2, 0),
		d[4] = flat(3, 0) - 0.5 * flat(0, 0) + flat(1, 0) + 0.5 * flat(2, 0),
		d[6] = flat(3, 0) - 0.5 * flat(0, 0) + flat(1, 0);
		d[1] = flat(3, 1) + 0.5 * flat(0, 1) + flat(1, 1),
		d[3] = flat(3, 1) + 0.5 * flat(0, 1) + flat(1, 1) + 0.5 * flat(2, 1),
		d[5] = flat(3, 1) - 0.5 * flat(0, 1) + flat(1, 1) + 0.5 * flat(2, 1),
		d[7] = flat(3, 1) - 0.5 * flat(0, 1) + flat(1, 1);
		fillpoly(4, d);
		//tails
		setfillstyle(1, RED);
		d[0] = flat(3, 0) + 0.3 * flat(0, 0) + flat(1, 0) + 0.375 * flat(2, 0),
		d[2] = flat(3, 0) + 0.5 * flat(0, 0) + flat(1, 0) + 0.375 * flat(2, 0),
		d[4] = flat(3, 0) + 0.5 * flat(0, 0) + flat(1, 0) + 0.5 * flat(2, 0),
		d[6] = flat(3, 0) + 0.3 * flat(0, 0) + flat(1, 0) + 0.5 * flat(2, 0);
		d[1] = flat(3, 1) + 0.3 * flat(0, 1) + flat(1, 1) + 0.375 * flat(2, 1),
		d[3] = flat(3, 1) + 0.5 * flat(0, 1) + flat(1, 1) + 0.375 * flat(2, 1),
		d[5] = flat(3, 1) + 0.5 * flat(0, 1) + flat(1, 1) + 0.5 * flat(2, 1),
		d[7] = flat(3, 1) + 0.3 * flat(0, 1) + flat(1, 1) + 0.5 * flat(2, 1);
		fillpoly(4, d);
		d[0] = flat(3, 0) - 0.3 * flat(0, 0) + flat(1, 0) + 0.375 * flat(2, 0),
		d[2] = flat(3, 0) - 0.5 * flat(0, 0) + flat(1, 0) + 0.375 * flat(2, 0),
		d[4] = flat(3, 0) - 0.5 * flat(0, 0) + flat(1, 0) + 0.5 * flat(2, 0),
		d[6] = flat(3, 0) - 0.3 * flat(0, 0) + flat(1, 0) + 0.5 * flat(2, 0);
		d[1] = flat(3, 1) - 0.3 * flat(0, 1) + flat(1, 1) + 0.375 * flat(2, 1),
		d[3] = flat(3, 1) - 0.5 * flat(0, 1) + flat(1, 1) + 0.375 * flat(2, 1),
		d[5] = flat(3, 1) - 0.5 * flat(0, 1) + flat(1, 1) + 0.5 * flat(2, 1),
		d[7] = flat(3, 1) - 0.3 * flat(0, 1) + flat(1, 1) + 0.5 * flat(2, 1);
		fillpoly(4, d);

		//rightSide
		setfillstyle(1, priCol);
		d[0] = flat(3, 0) + 0.5 * flat(0, 0),
		d[2] = flat(3, 0) + 0.5 * flat(0, 0) + 0.25 * flat(2, 0),
		d[4] = flat(3, 0) + 0.5 * flat(0, 0) + 0.2 * flat(1, 0) + 0.5 * flat(2, 0),
		d[6] = flat(3, 0) + 0.5 * flat(0, 0) + 0.4 * flat(1, 0) + 0.5 * flat(2, 0),
		d[8] = flat(3, 0) + 0.5 * flat(0, 0) + 0.6 * flat(1, 0) + flat(2, 0),
		d[10] = flat(3, 0) + 0.5 * flat(0, 0) + 0.8 * flat(1, 0) + flat(2, 0),
		d[12] = flat(3, 0) + 0.5 * flat(0, 0) + flat(1, 0) + 0.5 * flat(2, 0),
		d[14] = flat(3, 0) + 0.5 * flat(0, 0) + flat(1, 0);
		d[1] = flat(3, 1) + 0.5 * flat(0, 1),
		d[3] = flat(3, 1) + 0.5 * flat(0, 1) + 0.25 * flat(2, 1),
		d[5] = flat(3, 1) + 0.5 * flat(0, 1) + 0.2 * flat(1, 1) + 0.5 * flat(2, 1),
		d[7] = flat(3, 1) + 0.5 * flat(0, 1) + 0.4 * flat(1, 1) + 0.5 * flat(2, 1),
		d[9] = flat(3, 1) + 0.5 * flat(0, 1) + 0.6 * flat(1, 1) + flat(2, 1),
		d[11] = flat(3, 1) + 0.5 * flat(0, 1) + 0.8 * flat(1, 1) + flat(2, 1),
		d[13] = flat(3, 1) + 0.5 * flat(0, 1) + flat(1, 1) + 0.5 * flat(2, 1),
		d[15] = flat(3, 1) + 0.5 * flat(0, 1) + flat(1, 1);
		fillpoly(8, d);

		//rightWindows
		setfillstyle(1, 0);
		//front
		d[0] = flat(3, 0) + 0.5 * flat(0, 0) + 0.4 * flat(1, 0) + 0.5 * flat(2, 0),
		d[2] = flat(3, 0) + 0.5 * flat(0, 0) + 0.6 * flat(1, 0) + flat(2, 0),
		d[4] = flat(3, 0) + 0.5 * flat(0, 0) + 0.6 * flat(1, 0) + 0.5 * flat(2, 0);
		d[1] = flat(3, 1) + 0.5 * flat(0, 1) + 0.4 * flat(1, 1) + 0.5 * flat(2, 1),
		d[3] = flat(3, 1) + 0.5 * flat(0, 1) + 0.6 * flat(1, 1) + flat(2, 1),
		d[5] = flat(3, 1) + 0.5 * flat(0, 1) + 0.6 * flat(1, 1) + 0.5 * flat(2, 1);
		fillpoly(3, d);
		//back
		d[0] = flat(3, 0) + 0.5 * flat(0, 0) + 0.8 * flat(1, 0) + 0.5 * flat(2, 0),
		d[2] = flat(3, 0) + 0.5 * flat(0, 0) + 0.8 * flat(1, 0) + flat(2, 0),
		d[4] = flat(3, 0) + 0.5 * flat(0, 0) + flat(1, 0) + 0.5 * flat(2, 0);
		d[1] = flat(3, 1) + 0.5 * flat(0, 1) + 0.8 * flat(1, 1) + 0.5 * flat(2, 1),
		d[3] = flat(3, 1) + 0.5 * flat(0, 1) + 0.8 * flat(1, 1) + flat(2, 1),
		d[5] = flat(3, 1) + 0.5 * flat(0, 1) + flat(1, 1) + 0.5 * flat(2, 1);
		fillpoly(3, d);
		//mid
		d[0] = flat(3, 0) + 0.5 * flat(0, 0) + 0.62 * flat(1, 0) + 0.5 * flat(2, 0),
		d[2] = flat(3, 0) + 0.5 * flat(0, 0) + 0.78 * flat(1, 0) + 0.5 * flat(2, 0),
		d[4] = flat(3, 0) + 0.5 * flat(0, 0) + 0.78 * flat(1, 0) + flat(2, 0),
		d[6] = flat(3, 0) + 0.5 * flat(0, 0) + 0.62 * flat(1, 0) + flat(2, 0);
		d[1] = flat(3, 1) + 0.5 * flat(0, 1) + 0.62 * flat(1, 1) + 0.5 * flat(2, 1),
		d[3] = flat(3, 1) + 0.5 * flat(0, 1) + 0.78 * flat(1, 1) + 0.5 * flat(2, 1),
		d[5] = flat(3, 1) + 0.5 * flat(0, 1) + 0.78 * flat(1, 1) + flat(2, 1),
		d[7] = flat(3, 1) + 0.5 * flat(0, 1) + 0.62 * flat(1, 1) + flat(2, 1);
		fillpoly(4, d);
	}
};

class Road
{
  public:
	Matrix<DType, 3, 4> xyp;

	Road(DType x, DType y)
	{
		xyp = Matrix<DType, 3, 4>::Zero();
		xyp.row(2) << x, y, 0, 1;
		xyp.diagonal() << R_tw * 3, R_l, 0;
	}
	virtual void draw(Camera &cam)
	{
		Matrix<DType, 3, 2> flat = cam.view2d(xyp);
		int d[8], color = getcolor();
		// blue road background
		setcolor(BLUE);
		setfillstyle(1, BLUE);
		d[0] = flat(2, 0),
		d[1] = flat(2, 1),
		d[2] = flat(2, 0) + flat(1, 0),
		d[3] = flat(2, 1) + flat(1, 1),
		d[4] = flat(2, 0) + flat(1, 0) + flat(0, 0),
		d[5] = flat(2, 1) + flat(1, 1) + flat(0, 1),
		d[6] = flat(2, 0) + flat(0, 0),
		d[7] = flat(2, 1) + flat(0, 1);
		fillpoly(4, d);

		// two dividers Yellow
		DType p = 0.1 * flat(0, 0), q = 0.1 * flat(0, 1);
		setcolor(YELLOW);
		setfillstyle(1, YELLOW);
		d[0] = flat(2, 0) - p,
		d[2] = flat(2, 0) - p + flat(1, 0),
		d[4] = flat(2, 0) + flat(1, 0),
		d[6] = flat(2, 0);
		d[1] = flat(2, 1) - q,
		d[3] = flat(2, 1) - q + flat(1, 1),
		d[5] = flat(2, 1) + flat(1, 1),
		d[7] = flat(2, 1);
		fillpoly(4, d);

		DType r = flat(0, 0) + flat(2, 0), s = flat(0, 1) + flat(2, 1);
		d[0] = r,
		d[2] = r + flat(1, 0),
		d[4] = r + flat(1, 0) + p,
		d[6] = r + p;
		d[1] = s,
		d[3] = s + flat(1, 1),
		d[5] = s + flat(1, 1) + q,
		d[7] = s + q;
		fillpoly(4, d);

		DType t = flat(1, 0) * 0.05, u = flat(1, 1) * 0.05;
		// black blocks on dividers
		setcolor(0);
		setfillstyle(1, 0);
		for (int i = 0; i < 20; i += 2)
		{
			d[0] = flat(2, 0) - p + i * t,
			d[2] = flat(2, 0) - p + (i + 1) * t,
			d[4] = flat(2, 0) + (i + 1) * t,
			d[6] = flat(2, 0) + i * t;
			d[1] = flat(2, 1) - q + i * u,
			d[3] = flat(2, 1) - q + (i + 1) * u,
			d[5] = flat(2, 1) + (i + 1) * u,
			d[7] = flat(2, 1) + i * u;
			fillpoly(4, d);
		}

		for (int i = 0; i < 20; i += 2)
		{
			d[0] = r + i * t,
			d[1] = s + i * u,
			d[2] = r + (i + 1) * t,
			d[3] = s + (i + 1) * u,
			d[4] = r + (i + 1) * t + p,
			d[5] = s + (i + 1) * u + q,
			d[6] = r + i * t + p,
			d[7] = s + i * u + q;
			fillpoly(4, d);
		}

		//lanes seperators
		setcolor(15);
		setfillstyle(1, 15);
		p /= 2;
		q /= 2;
		r = flat(2, 0) + flat(0, 0) / 3 - p / 2, s = flat(2, 1) + flat(0, 1) / 3 - q / 2;
		for (int i = 0; i < 20; i += 2)
		{
			d[0] = r + i * t,
			d[2] = r + (i + 1) * t,
			d[4] = r + (i + 1) * t + p,
			d[6] = r + i * t + p;
			d[1] = s + i * u,
			d[3] = s + (i + 1) * u,
			d[5] = s + (i + 1) * u + q,
			d[7] = s + i * u + q;
			fillpoly(4, d);
		}
		r += flat(0, 0) / 3, s += flat(0, 1) / 3;
		for (int i = 0; i < 20; i += 2)
		{
			d[0] = r + i * t,
			d[2] = r + (i + 1) * t,
			d[4] = r + i * t + t + p,
			d[6] = r + i * t + p;
			d[1] = s + i * u,
			d[3] = s + (i + 1) * u,
			d[5] = s + i * u + u + q,
			d[7] = s + i * u + q;
			fillpoly(4, d);
		}

		setcolor(color);
	}
};

class Coins
{

  public:
	Matrix<DType, 1, 4> p;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	static float angle; // and angle equal for all coins

	//Coins() {  }

	Coins(int k)
	{
		p << k, -R_l, -C_r, 1;
	}

	Coins(int i, int j)
	{
		p << i, j - C_r, -C_r, 1;
	}

	void draw()
	{
		int temp = getcolor();
		if (angle > M_PI)
			angle = fmod(angle, M_PI); // simple solution
		int r = 30, t = 5;
		setcolor(COLOR(240, 121, 24));
		setlinestyle(0, 0, 5);
		setfillstyle(1, COLOR(240, 121, 24));
		fillellipse(p(0, 0) + t * cos(angle), p(0, 1), r * sin(angle), r);
		bar(p(0, 0) - t * cos(angle), p(0, 1) - r, p(0, 0) + t * cos(angle), p(0, 1) + r);
		setfillstyle(1, YELLOW);
		fillellipse(p(0, 0) - t * cos(angle), p(0, 1), r * sin(angle), r);
		setcolor(temp);
		setlinestyle(0, 0, 1);
	}
	void draw(Camera &cam)
	{
		Matrix<DType, 1, 2> flat = cam.view0d(p);
		DType s = C_r * cam.mag, u = C_t * cam.mag;
		int temp = getcolor();
		if (angle > M_PI)
			angle = fmod(angle, M_PI); // simple solution
		setcolor(COLOR(240, 121, 24));
		setlinestyle(0, 0, 5);
		setfillstyle(1, COLOR(240, 121, 24));
		fillellipse(flat(0, 0) + u * cos(angle), flat(0, 1), s * sin(angle), s);
		bar(flat(0, 0) - u * cos(angle), flat(0, 1) - s, flat(0, 0) + u * cos(angle), flat(0, 1) + s);
		setfillstyle(1, YELLOW);
		fillellipse(flat(0, 0) - u * cos(angle), flat(0, 1), s * sin(angle), s);
		setcolor(temp);
		setlinestyle(0, 0, 1);
	}
};
float Coins::angle = 0;

class Obstacle
{

  public:
	Matrix<DType, 1, 4, RowMajor> p;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	Obstacle(DType lane)
	{
		// x = lane - 25; //the ref/centre of bomb is about 25units offset from lanes
		// drop = -1212.5;
		// drop = y;
		p << lane, -R_l, -O_r, 1;
	}
	void draw(Camera &cam)
	{
		Matrix<DType, 1, 2> flat = cam.view0d(p);
		DType t = O_r * cam.mag;
		setcolor(BLACK);
		setfillstyle(1, 0);
		bar(flat(0, 0) - 0.25 * t, flat(0, 1) - 1.15 * t, flat(0, 0) + 0.25 * t, flat(0, 1));

		setfillstyle(SOLID_FILL, COLOR(50, 50, 50));
		arc(flat(0, 0) + 0.45 * t, flat(0, 1) - 1.1 * t, 90, 180, 0.45 * t);
		arc(flat(0, 0) + 0.45 * t, flat(0, 1) - 2 * t, 270, 0, 0.45 * t);
		fillellipse(flat(0, 0), flat(0, 1), t, t);

		putpixel(flat(0, 0) + 0.9 * t + rand() % 5, flat(0, 1) - 2.05 * t + rand() % 5, YELLOW);
		putpixel(flat(0, 0) + 0.9 * t + rand() % 5, flat(0, 1) - 2.05 * t + rand() % 5, YELLOW);
		putpixel(flat(0, 0) + 0.9 * t - rand() % 5, flat(0, 1) - 2.05 * t - rand() % 5, YELLOW);
		putpixel(flat(0, 0) + 0.9 * t - rand() % 5, flat(0, 1) - 2.05 * t + rand() % 5, YELLOW);
		putpixel(flat(0, 0) + 0.9 * t + rand() % 5, flat(0, 1) - 2.05 * t - rand() % 5, YELLOW);
	}
};

class LIST_obstacles
{
  public:
	typedef struct node_o
	{
		struct node_o *next;
		bool life = 1;
		Obstacle *o;
	} node_o;

	node_o *cn, *hn;

	LIST_obstacles()
	{
		hn = NULL;
	}
	~LIST_obstacles()
	{
		if (!hn)
			return;
		do
		{
			cn = hn;
			hn = hn->next;
			delete cn;
		} while (hn);
	}

	void add_drop(Obstacle *cbomb)
	{
		cn = new node_o;
		cn->o = cbomb;
		cn->next = hn;
		hn = cn;
		return;
	}

	void delet()
	{
		bool flag = 0;
		node_o *temp;
		cn = hn;
		if (!hn)
			return;

		while (hn->life == 0)
		{
			hn = hn->next;
			delete cn;
			if (!hn) //prevents segFault
				return;
			cn = hn;
		}

		temp = cn;
		cn = cn->next;

		while (cn != NULL)
		{
			if (cn->life == 0)
			{
				temp->next = cn->next;
				flag = 1;
				delete cn;
				return;
			}
			if (flag)
			{
				cn = temp->next;
			}
			temp = cn;
			cn = cn->next;
		}
	}
};

class LIST_coins
{
  public:
	typedef struct node
	{
		Coins *c;
		struct node *next;
		bool life = 1;
	} node;

	node *hn, *cn;

	LIST_coins()
	{
		hn = NULL;
	}
	~LIST_coins()
	{
		if (!hn)
			return;
		do
		{
			cn = hn;
			hn = hn->next;
			delete cn;
		} while (hn);
	}

	void add(Coins *ncoin)
	{
		cn = new node;
		cn->next = hn;
		cn->c = ncoin;
		hn = cn;
		return;
	}
	void delet()
	{
		bool flag = 0;
		node *temp;
		cn = hn;
		if (!hn) //prevents segFault
			return;

		while (hn->life == 0)
		{
			hn = hn->next;
			delete cn;
			if (!hn) //prevents segFault
				return;
			cn = hn;
		}

		temp = cn;
		cn = cn->next;

		while (cn != NULL)
		{
			if (cn->life == 0)
			{
				temp->next = cn->next;
				flag = 1;
				delete cn;
				return;
			}
			if (flag)
			{
				cn = temp->next;
			}
			temp = cn;
			cn = cn->next;
		}
	}

	int count_coins()
	{
		int count = 0;
		cn = hn;
		while (cn != NULL)
		{
			cn = cn->next;
			count++;
		}
		return count;
	}
};

typedef struct node
{
	string name;
	int score;
	node *next;
	node(string n, int s) : name(n), score(s), next(NULL) {}
} node;

class LeaderBoard
{
	node *hn, *ln;
	string name;
	int score;

  public:
	LeaderBoard() : hn(NULL), ln(NULL), name(""), score(0) {}
	void load()
	{
		int count = 9;
		ifstream file;
		file.open("LeaderBoard.txt");
		file >> name >> score;
		node *nn = new node(name, score);
		node *cn;
		hn = nn; // Initialize value of hn
		cn = hn;
		while (count--)
		{
			file >> name >> score;
			node *nn = new node(name, score);
			cn->next = nn;
			cn = cn->next;
		}
		ln = cn;
		file.close();
	}
	void display()
	{

		setfillstyle(1, WHITE);
		bar(getmaxwidth() / 3 - 50, 0, 2 * getmaxwidth() / 3, getmaxheight() / 10 + 300);
		readimagefile("trophy.jpg", getmaxwidth() / 3 - 30, getmaxheight() / 10, getmaxwidth() / 3, getmaxheight() / 10 + 30);

		setcolor(BLACK);
		rectangle(getmaxwidth() / 3 - 40, 10, 2 * getmaxwidth() / 3 - 10, getmaxheight() / 10 + 300 - 10);

		setbkcolor(WHITE);
		setcolor(BLACK);
		settextstyle(BOLD_FONT, HORIZ_DIR, 5);

		bgiout << "Top 10\n";
		outstreamxy(getmaxwidth() / 3, 0);
		settextstyle(BOLD_FONT, HORIZ_DIR, 3);
		int t = 0;
		node *cn = hn;
		while (cn)
		{
			bgiout << cn->name << "               " << cn->score;
			outstreamxy(getmaxwidth() / 3, getmaxheight() / 10 + t);
			t += 30;
			cn = cn->next;
		}
		bgiout << "Press 'x' to go back";
		outstreamxy(0.40 * getmaxwidth(), 0.55 * getmaxheight());
	}
	void newEntry(string name, int score)
	{
		load();
		node *cn = hn;
		bool flag = 0;
		if (hn->score < score)
		{
			node *nn = new node(name, score);
			nn->next = hn;
			hn = nn;
			flag = 1;
		}
		else
		{
			while (cn->next)
			{
				if (cn->next->score < score)
				{
					node *nn = new node(name, score);
					nn->next = cn->next;
					cn->next = nn;
					flag = 1;
					break;
				}
				cn = cn->next;
			}
		}
		if (flag)
		{
			while (cn->next != ln)
			{
				cn = cn->next;
			}
			delete cn->next;
			ln = cn;
			cn->next = NULL;
		}
		store();
	}
	void store()
	{
		ofstream file;
		file.open("LeaderBoard.txt");
		node *cn = hn;
		while (cn)
		{
			file << cn->name << " " << cn->score << endl;
			cn = cn->next;
		}
		file.close();
	}
};

void game()
{
	LeaderBoard lb;
	int p, points = 0, mflag = 1, oc = 0, pob, gameover = 0;
	int poc;
	int mll, mml, mrl;
	char c, ki;
	bool mleft = 0, mright = 0;
	int speed = 1;
	int random, cc = 0;

	initwindow(getmaxwidth(), getmaxheight(), "New Game");

	Camera cam;
	// cam.yaw(-45);
	// cam.pitch(60);	//isometric view
	cam.mag = 0.45;
	cam.rePos();
	cam.reOrient();

	Car car;
	Road road(-R_tw * 1.5, -R_l), road1(-R_tw * 1.5, 0);

	LIST_coins l;
	LIST_obstacles lob;
	srand(time(0));

	settextstyle(10, 0, 4);

	do
	{ // game loop
		// fast speed(x5)
		cc += speed; //
		oc += speed; //
		road.xyp(2, 1) += speed;
		road1.xyp(2, 1) += speed;
		if (road.xyp(2, 1) > -R_l)
		{
			road.xyp(2, 1) = -R_l * 1.1;
			road1.xyp(2, 1) = -R_l * 0.1;
		}

		if (kbhit())
		{ // input control
			switch (ki = getch())
			{
			case 'p':
			case 'P':
				bgiout << "PAUSED";
				outstreamxy(getmaxx() / 2, getmaxy() / 2);
				while (c = getch())
					if (c == 'p' || c == 'P')
						break;
				break;
			case 'Q':
			case 'q':
				cam.mag *= 0.9;
				cam.reOrient();
				break;
			case 'E':
			case 'e':
				cam.mag += 0.1;
				cam.reOrient();
				break;
			case 'A':
			case 'a':
				cam.yaw(camspeed);
				cam.reOrient();
				break;
			case 'D':
			case 'd':
				cam.yaw(-camspeed);
				cam.reOrient();
				break;
			case 'S':
			case 's':
				cam.pitch(camspeed);
				cam.reOrient();
				break;
			case 'W':
			case 'w':
				cam.pitch(-camspeed);
				cam.reOrient();
				break;
			case 0:
				switch (getch())
				{
				case KEY_LEFT:

					if (car.xyzp(3, 0) == 0 || car.xyzp(3, 0) == R_tw)
					{
						if (mflag == 1)
						{
							mleft = 1;
							p = car.xyzp(3, 0);
						}
						if (car.xyzp(3, 0) == 0)
							mll = 1;
						else
							mml = 1;
					}
					break;
				case KEY_RIGHT:
					if (car.xyzp(3, 0) == 0 || car.xyzp(3, 0) == -R_tw)
					{
						if (mflag == 1)
						{
							mright = 1;
							p = car.xyzp(3, 0);
						}
						if (car.xyzp(3, 0) == 0)
							mrl = 1;
						else
							mml = 1;
					}
					break;
				case KEY_UP:
					speed += 5 * R_sp;
					if (speed < 2 * R_sp)
						speed = R_sp;
					if (cc % 5 != 0 || oc % 5 != 0)
					{
						cc += (5 - cc % 5);
						oc += (5 - oc % 5);
					}
					break;
				case KEY_DOWN:
					speed -= 5 * R_sp;
					if (speed < 1)
						speed = 1;
					if (cc % 5 != 0 || oc % 5 != 0)
					{
						cc += (5 - cc % 5);
						oc += (5 - oc % 5);
					}
					break;
				}
				break;

			case 'Z':
			case 'z':
				c = 'x';
			}
		}
		if (cc > C_int)
		{			// spawn coins
			cc = 0; //resetting cc to zero, no modulo limitations now

			poc = rand();
			poc %= 3;

			switch (poc)
			{
			case 0:
				l.add(new Coins(-R_tw));
				break;
			case 1:
				l.add(new Coins(0));
				break;
			case 2:
				l.add(new Coins(R_tw));
				break;
			}
		}
		if (oc > O_int)
		{			// spawn obstacles
			oc = 0; //resetting oc to zero, no modulo limitations now
			switch (int(car.xyzp(3, 0)))
			{
			case -R_tw:
				pob = rand();
				pob %= 5;
				switch (pob)
				{
				case 0:
					lob.add_drop(new Obstacle(-R_tw));
					break;
				case 1:
					lob.add_drop(new Obstacle(0));
					break;
				case 2:
					lob.add_drop(new Obstacle(R_tw));
					break;
				case 3:
					lob.add_drop(new Obstacle(0));
					lob.add_drop(new Obstacle(R_tw));
					break;
				case 4:
					lob.add_drop(new Obstacle(-R_tw));
					lob.add_drop(new Obstacle(R_tw));
					break;
				}
				break;

			case 0:
				pob = rand();
				pob %= 6;
				switch (pob)
				{
				case 0:
					lob.add_drop(new Obstacle(-R_tw));
					break;
				case 1:
					lob.add_drop(new Obstacle(0));
					break;
				case 2:
					lob.add_drop(new Obstacle(R_tw));
					break;
				case 3:
					lob.add_drop(new Obstacle(0));
					lob.add_drop(new Obstacle(R_tw));
					break;
				case 4:
					lob.add_drop(new Obstacle(-R_tw));
					lob.add_drop(new Obstacle(R_tw));
					break;
				case 5:
					lob.add_drop(new Obstacle(-R_tw));
					lob.add_drop(new Obstacle(0));
					break;
				}

				break;

			case R_tw:
				pob = rand();
				pob %= 5;
				switch (pob)
				{
				case 0:
					lob.add_drop(new Obstacle(-R_tw));
					break;
				case 1:
					lob.add_drop(new Obstacle(0));
					break;
				case 2:
					lob.add_drop(new Obstacle(R_tw));
					break;
				case 3:
					lob.add_drop(new Obstacle(0));
					lob.add_drop(new Obstacle(-R_tw));
					break;
				case 4:
					lob.add_drop(new Obstacle(-R_tw));
					lob.add_drop(new Obstacle(R_tw));
					break;
				}

				break;

			default:
			{
				if (mll)
				{
					pob = rand();
					pob %= 5;
					switch (pob)
					{
					case 0:
						lob.add_drop(new Obstacle(-R_tw));
						break;
					case 1:
						lob.add_drop(new Obstacle(0));
						break;
					case 2:
						lob.add_drop(new Obstacle(R_tw));
						break;
					case 3:
						lob.add_drop(new Obstacle(0));
						lob.add_drop(new Obstacle(R_tw));
						break;
					case 4:
						lob.add_drop(new Obstacle(-R_tw));
						lob.add_drop(new Obstacle(R_tw));
						break;
					}
				}
				else if (mrl)
				{
					pob = rand();
					pob %= 5;
					switch (pob)
					{
					case 0:
						lob.add_drop(new Obstacle(-R_tw));
						break;
					case 1:
						lob.add_drop(new Obstacle(0));
						break;
					case 2:
						lob.add_drop(new Obstacle(R_tw));
						break;
					case 3:
						lob.add_drop(new Obstacle(0));
						lob.add_drop(new Obstacle(-R_tw));
						break;
					case 4:
						lob.add_drop(new Obstacle(-R_tw));
						lob.add_drop(new Obstacle(R_tw));
						break;
					}
				}
				else
				{
					pob = rand();
					pob %= 6;
					switch (pob)
					{
					case 0:
						lob.add_drop(new Obstacle(-R_tw));
						break;
					case 1:
						lob.add_drop(new Obstacle(0));
						break;
					case 2:
						lob.add_drop(new Obstacle(R_tw));
						break;
					case 3:
						lob.add_drop(new Obstacle(0));

						lob.add_drop(new Obstacle(R_tw));
						break;
					case 4:
						lob.add_drop(new Obstacle(-R_tw));

						lob.add_drop(new Obstacle(R_tw));
						break;
					case 5:
						lob.add_drop(new Obstacle(-R_tw));

						lob.add_drop(new Obstacle(0));
						break;
					}
				}
			}
			}
		}
		if (mleft)
		{ // move left
			if (car.xyzp(3, 0) > p - R_tw)
				car.xyzp(3, 0) -= 4;
			else
				mleft = 0;

			if (car.xyzp(3, 0) == p - R_tw)
				mflag = 1;
			else
				mflag = 0;
		}
		if (mright)
		{ //move right
			if (car.xyzp(3, 0) < p + R_tw)
				car.xyzp(3, 0) += 4;
			else
				mright = 0;

			if (car.xyzp(3, 0) == p + R_tw)
				mflag = 1;
			else
				mflag = 0;
		}

		l.cn = l.hn;
		if (l.hn != NULL)
		{ //draw all coins
			while (l.cn != NULL)
			{
				if (l.cn->life == 1)
				{
					if (l.cn->c->p(0, 1) + C_r > car.xyzp(3, 1) && l.cn->c->p(0, 1) < car.xyzp(3, 1) + Car_l + C_r && l.cn->c->p(0, 0) + C_r + Car_w / 2 > car.xyzp(3, 0) && l.cn->c->p(0, 0) < car.xyzp(3, 0) + C_r + Car_w / 2)
					{
						l.cn->life = 0;
						points += 50;
					}
					else if (l.cn->c->p(0, 1) > R_l)
						l.cn->life = 0;

					l.cn->c->draw(cam);
					l.cn->c->angle += 0.8 * 0.01745329251994329576923690768489;
					l.cn->c->p(0, 1) += speed;
				}
				l.cn = l.cn->next;
			}
		}

		lob.cn = lob.hn;
		if (lob.hn != NULL)
		{ // draw all obstacles
			while (lob.cn != NULL)
			{
				if (lob.cn->life == 1)
				{
					if (lob.cn->o->p(0, 1) + O_r > car.xyzp(3, 1) && lob.cn->o->p(0, 1) < car.xyzp(3, 1) + Car_l + O_r && lob.cn->o->p(0, 0) + O_r + Car_w / 2 > car.xyzp(3, 0) && lob.cn->o->p(0, 0) < car.xyzp(3, 0) + O_r + Car_w / 2)
					{
						gameover = 1;
						c = 'x';
						break;
					}
					else if (lob.cn->o->p(0, 1) > R_l)
						lob.cn->life = 0;

					lob.cn->o->draw(cam);
					lob.cn->o->p(0, 1) += speed;
				}
				lob.cn = lob.cn->next;
			}
		}

		l.delet();
		lob.delet();
		bgiout << "POINTS:    " << points << "\n\nUse W,A,S,D\nto rotate camera!\nUp arrow => accelerate\nDown arrow => slow down\n\nz => quit";
		setcolor(LIGHTGREEN);
		outstreamxy(1000, 500);
		// delay(1);
		car.draw(cam);
		swapbuffers();
		cleardevice();
		road.draw(cam);
		road1.draw(cam);

	} while (c != 'x');

	cleardevice();

	setcolor(0);
	setbkcolor(15);
	settextstyle(BOLD_FONT, HORIZ_DIR, 3);
	readimagefile("boom.jpg", 0, 0, getwindowwidth(), getwindowheight());
	bgiout << "\nGAMEOVER!!\nPoints:" << points << "\n";
	outstreamxy(getmaxwidth() / 2 - 150, getwindowheight() / 2 - 100);
	bgiout << "Press Enter to continue";
	outstreamxy(getmaxwidth() / 3, getwindowheight() / 2);
	swapbuffers();
	string name;

	while (getch() != '\r')
		;
	readimagefile("boom.jpg", 0, 0, getwindowwidth(), getwindowheight());
	swapbuffers();
	readimagefile("boom.jpg", 0, 0, getwindowwidth(), getwindowheight());
	char ch = ' ';
	setviewport(0.347 * getmaxx(), 0.357 * getmaxy(), 0.627 * getmaxx(), 0.642 * getmaxy(), 0);
	for (;;)
	{ // ask player name
		bgiout << "\nThank you for Playing!!\nPoints:" << points << "\n";
		outstreamxy(0.023 * getmaxx(), 0.054 * getmaxy());
		bgiout << "Enter your name:" << name;
		outstreamxy(0.023 * getmaxx(), 0.163 * getmaxy());
		swapbuffers();
		clearviewport();
		ch = getch();
		if (ch == '\r')
			break;
		else if (ch == 8)
		{
			if (name.size())
				name.pop_back();
		}
		else
		{
			name += ch;
		}
	}

	lb.newEntry(name, points);

	closegraph();
}
void leaderboard()
{

	initwindow(getmaxwidth(), getmaxheight(), "Leader Board");

	LeaderBoard l;
	l.load();
	l.display();
back:

	if (getch() == 'x')
	{
		closegraph();
		return;
	}
	else
	{
		goto back;
	}
}
int main()
{
start:
	initwindow(getmaxwidth(), getmaxheight(), "MENU");

	int coin_X = 0.01 * getmaxy() + C_r, coin_Y = 0.04 * getmaxy() + 2 * C_r;
	int coin_pos = 1;
	Coins c(coin_X, coin_Y);
	short a = 1;
	setbkcolor(WHITE);
	setcolor(BLUE);
	bar(0, 0, getwindowwidth(), getwindowheight());

	//readimagefile("mskb.jpg", 0, 0, getwindowwidth(), getwindowheight());
	readimagefile("mskb.jpg", 0, 0, getwindowwidth(), getwindowheight() * 0.35);
	readimagefile("coins.jpg", 0.50 * getwindowwidth(), 0.37 * getwindowheight(), getwindowwidth(), 0.60 * getwindowheight());
	readimagefile("car.jpg", 0, 0.40 * getwindowheight(), getwindowwidth() * 0.50, getwindowheight() * 0.90);
	readimagefile("bomb.jpg", 0.50 * getwindowwidth(), 0.60 * getwindowheight(), getwindowwidth() * 0.65, getwindowheight() * 0.80);
	settextstyle(1, HORIZ_DIR, 7);
	bgiout << "MS-KB";
	outstreamxy(0.42 * getwindowwidth() + 1, 0.13 * getwindowheight() + 1);
	swapbuffers();
	bar(0, 0, getwindowwidth(), getwindowheight());
	//    readimagefile("mskb.jpg", 0, 0, getwindowwidth(), getwindowheight());
	readimagefile("mskb.jpg", 0, 0, getwindowwidth(), getwindowheight() * 0.35);
	readimagefile("coins.jpg", 0.50 * getwindowwidth(), 0.37 * getwindowheight(), getwindowwidth(), 0.60 * getwindowheight());
	readimagefile("car.jpg", 0, 0.40 * getwindowheight(), getwindowwidth() * 0.50, getwindowheight() * 0.90);
	readimagefile("bomb.jpg", 0.50 * getwindowwidth() + 2, 0.60 * getwindowheight() + 2, getwindowwidth() * 0.65 + 2, getwindowheight() * 0.80 + 2);
	settextstyle(1, HORIZ_DIR, 7);
	bgiout << "MS-KB";
	outstreamxy(0.42 * getwindowwidth(), 0.13 * getwindowheight());
	setviewport(0.68 * getwindowwidth(), 0.60 * getwindowheight(), 0.98 * getwindowwidth(), 0.95 * getwindowheight(), 1);
	while (1)
	{
		setbkcolor(WHITE);
		setfillstyle(1, WHITE);
		bar(0, 0, 100, 100);

		settextstyle(1, HORIZ_DIR, 5);
		bgiout << "New game";
		outstreamxy(0.05 * getmaxx(), 0.05 * getmaxy());

		settextstyle(1, HORIZ_DIR, 5);
		bgiout << "Leaderboard";
		outstreamxy(0.05 * getmaxx(), 0.15 * getmaxy());

		settextstyle(1, HORIZ_DIR, 5);
		bgiout << "Exit";
		outstreamxy(0.05 * getmaxx(), 0.25 * getmaxy());
		if (kbhit())
		{
			switch (getch())
			{
			case '\r':
				switch (coin_pos)
				{
				case 1:
					game();
					goto start;
					break;
				case 2:
					leaderboard();
					goto start;
					break;
				case 3:
					return 0;
				}
				break;
			case KEY_UP:
				if (coin_pos != 1)
				{
					coin_pos--;
					c.p(0, 1) -= 0.10 * getmaxy();
				}
				break;
			case KEY_DOWN:
				if (coin_pos != 3)
				{
					coin_pos++;
					c.p(0, 1) += 0.10 * getmaxy();
				}
				break;
			default:
				break;
			}
		}
		c.draw();
		Coins::angle += 10 * 0.01745329251994329576923690768489;
		delay(15);
		swapbuffers();
		clearviewport();
	}
}