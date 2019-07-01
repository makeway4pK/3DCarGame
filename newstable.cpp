#include <iostream>
#include <graphics.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <fstream>
#include "Eigen/Dense"
#define DType float
using namespace std;
using namespace Eigen;

typedef std::chrono::high_resolution_clock Clock;

inline void vecLine(const Matrix<DType, 1, 2> &a, const Matrix<DType, 1, 2> &b)
{
	line(a(0, 0), a(0, 1), b(0, 0), b(0, 1));
}

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
		bgiout << v << "\n\n";

		// //x' = x*d/z
		// v.col(0) = d * v.col(0).cwiseQuotient(v.col(2));
		// //y' = y*d/z
		// v.col(1) = d * v.col(1).cwiseQuotient(v.col(2));

		bgiout << endl
			   << obj << "\n\n"
			   << project << "\n\n"
			   << v;
		return v.block<4, 2>(0, 0);
	}
	Matrix<DType, 3, 2, RowMajor> view2d(const Matrix<DType, 3, 4, RowMajor> &obj)
	{
		Matrix<DType, 3, 4, RowMajor> v;
		v.noalias() = obj * project;
		bgiout << v << "\n\n";

		// //x' = x*d/z
		// v.col(0) = d * v.col(0).cwiseQuotient(v.col(2));
		// //y' = y*d/z
		// v.col(1) = d * v.col(1).cwiseQuotient(v.col(2));

		bgiout << endl
			   << obj << "\n\n"
			   << project << "\n\n"
			   << v;
		return v.block<3, 2>(0, 0);
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
	}
	void yaw(DType t)
	{ //rotates around worldZaxis
		zAng += t / 57.295779513082320876798154814105;
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

class Cuboid
{
  public:
	Matrix<DType, 4, 4> xyzp;
	Cuboid(DType x = 0, DType y = 0, DType z = 1, DType l = 1000, DType b = 1000, DType h = 1000)
	{
		xyzp = Matrix<DType, 4, 4>::Zero();
		xyzp.row(3) << x, y, z, 1;

		xyzp.diagonal() << l, b, h, 1;
	}

	virtual void draw(Camera &cam)
	{
		Matrix<DType, 4, 2> flat = cam.view(xyzp);
		// bgiout << endl
		// 	   << flat;
		int t = getcolor();
		setcolor(RED);
		vecLine(flat.row(3), flat.row(3) + flat.row(0));
		setcolor(GREEN);
		vecLine(flat.row(3), flat.row(3) + flat.row(1));
		setcolor(BLUE);
		vecLine(flat.row(3), flat.row(3) + flat.row(2));
		setcolor(t);
		vecLine(flat.row(3) + flat.row(0), flat.row(3) + flat.row(0) + flat.row(1));
		vecLine(flat.row(3) + flat.row(0), flat.row(3) + flat.row(0) + flat.row(2));
		vecLine(flat.row(3) + flat.row(1), flat.row(3) + flat.row(1) + flat.row(2));
		vecLine(flat.row(3) + flat.row(1), flat.row(3) + flat.row(1) + flat.row(0));
		vecLine(flat.row(3) + flat.row(2), flat.row(3) + flat.row(2) + flat.row(0));
		vecLine(flat.row(3) + flat.row(2), flat.row(3) + flat.row(2) + flat.row(1));
		vecLine(flat.row(3) + flat.row(0) + flat.row(1), flat.row(3) + flat.row(0) + flat.row(1) + flat.row(2));
		vecLine(flat.row(3) + flat.row(0) + flat.row(2), flat.row(3) + flat.row(0) + flat.row(1) + flat.row(2));
		vecLine(flat.row(3) + flat.row(1) + flat.row(2), flat.row(3) + flat.row(0) + flat.row(1) + flat.row(2));
	}
};

class Car : public Cuboid
{
	int priCol;

  public:
	Car(int c = COLOR(240, 121, 24))
	{
		priCol = c;
		xyzp.diagonal() << 100, 250, -50, 1;
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

	Road(DType l = 500, DType b = 2000)
	{
		xyp = Matrix<DType, 3, 4>::Zero();
		xyp.row(2) << -l / 2, -b / 2, 0, 1;
		xyp.diagonal() << l, b, 0;
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
		d[0] = flat(2, 0),
		d[1] = flat(2, 1),
		d[2] = flat(2, 0) + flat(1, 0),
		d[3] = flat(2, 1) + flat(1, 1),
		d[4] = flat(2, 0) + flat(1, 0) + p,
		d[5] = flat(2, 1) + flat(1, 1) + q,
		d[6] = flat(2, 0) + p,
		d[7] = flat(2, 1) + q;
		fillpoly(4, d);

		DType r = p * 9 + flat(2, 0), s = q * 9 + flat(2, 1);
		// setcolor(YELLOW);
		// setfillstyle(1, YELLOW);
		d[0] = r,
		d[1] = s,
		d[2] = r + flat(1, 0),
		d[3] = s + flat(1, 1),
		d[4] = r + flat(1, 0) + p,
		d[5] = s + flat(1, 1) + q,
		d[6] = r + p,
		d[7] = s + q;
		fillpoly(4, d);

		DType t = flat(1, 0) * 0.05, u = flat(1, 1) * 0.05;
		// black blocks on dividers
		setcolor(0);
		setfillstyle(1, 0);
		for (int i = 0; i < 20; i += 2)
		{
			d[0] = flat(2, 0) + i * t,
			d[1] = flat(2, 1) + i * u,
			d[2] = flat(2, 0) + i * t + t,
			d[3] = flat(2, 1) + i * u + u,
			d[4] = flat(2, 0) + i * t + t + p,
			d[5] = flat(2, 1) + i * u + u + q,
			d[6] = flat(2, 0) + i * t + p,
			d[7] = flat(2, 1) + i * u + q;
			fillpoly(4, d);
		}

		for (int i = 0; i < 20; i += 2)
		{
			d[0] = r + i * t,
			d[1] = s + i * u,
			d[2] = r + i * t + t,
			d[3] = s + i * u + u,
			d[4] = r + i * t + t + p,
			d[5] = s + i * u + u + q,
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
			d[1] = s + i * u,
			d[2] = r + (i + 1) * t,
			d[3] = s + (i + 1) * u,
			d[4] = r + i * t + t + p,
			d[5] = s + i * u + u + q,
			d[6] = r + i * t + p,
			d[7] = s + i * u + q;
			fillpoly(4, d);
		}
		r += flat(0, 0) / 3, s += flat(0, 1) / 3;
		for (int i = 0; i < 20; i += 2)
		{
			d[0] = r + i * t,
			d[1] = s + i * u,
			d[2] = r + (i + 1) * t,
			d[3] = s + (i + 1) * u,
			d[4] = r + i * t + t + p,
			d[5] = s + i * u + u + q,
			d[6] = r + i * t + p,
			d[7] = s + i * u + q;
			fillpoly(4, d);
		}

		setcolor(color);
	}
};

class vehicle
{
  public:
	int xc, yc;
	vehicle()
	{
		xc = 750;
		yc = 550;
	}
	void draw()
	{
		line(xc - 20, yc, xc + 20, yc);
		line(xc + 25, yc + 5, xc + 25, yc + 35);
		line(xc + 25, yc + 35, xc - 25, yc + 35);
		line(xc - 25, yc + 35, xc - 25, yc + 5);

		arc(xc - 20, yc + 5, 90, 180, 5);
		arc(xc + 20, yc + 5, 0, 90, 5);

		line(xc + 25, yc + 35, xc + 25, yc + 50);
		line(xc + 25, yc + 50, xc - 25, yc + 50);
		line(xc - 25, yc + 50, xc - 25, yc + 35);

		line(xc - 25, yc + 50, xc - 25, yc + 95);
		line(xc - 25, yc + 95, xc + 25, yc + 95);
		line(xc + 25, yc + 95, xc + 25, yc + 50);

		line(xc - 25, yc + 110, xc + 25, yc + 110); // rear glass 'line'

		line(xc - 25, yc + 95, xc - 25, yc + 120);
		line(xc - 20, yc + 125, xc + 20, yc + 125);
		line(xc + 25, yc + 120, xc + 25, yc + 95);

		arc(xc - 20, yc + 120, 180, 270, 5);
		arc(xc + 20, yc + 120, 270, 360, 5);

		line(xc - 15, yc, xc - 15, yc - 2); //left headlamp
		line(xc - 15, yc - 2, xc - 5, yc - 2);
		line(xc - 5, yc - 2, xc - 5, yc);

		line(xc + 15, yc, xc + 15, yc - 2); //right headlamp
		line(xc + 15, yc - 2, xc + 5, yc - 2);
		line(xc + 5, yc - 2, xc + 5, yc);

		setfillstyle(1, 4); // car colouring
		floodfill(xc, yc + 10, 15);
		floodfill(xc, yc + 70, 15);
		floodfill(xc, yc + 120, 15);

		setfillstyle(1, BLACK);
		floodfill(xc, yc + 45, 15);
		floodfill(xc, yc + 105, 15);
	}
};

class road
{
  public:
	int xrl, yrl, yll, xll;
	float xld, yld, xrd, yrd;

	int xr, yr, xl, yl;
	float xldi, yldi, xrdi, yrdi;
	int wd = 10;

	road()
	{
		xr = xrl = 800; //lanes
		yr = yrl = 0;

		xl = xll = 700;
		yl = yll = 0;

		xldi = xld = 590; //dividers
		yldi = yld = 0;
		xrdi = xrd = 910;
		yrdi = yrd = 0;
	}

	void dr()
	{
		//setcolor(15);
		setlinestyle(0, 0, 5);
		line(580, 0, 580, 833);
		line(920, 0, 920, 833);

		setlinestyle(0, 0, 1);
		line(600, 0, 600, 833);
		line(900, 0, 900, 833);

		setlinestyle(0, 0, 0);
		line(600, 0, 900, 0);
		line(580, 833, 920, 833); //try changing here if any floodfill problem...

		setfillstyle(1, 1);
		floodfill(700, 700, 15);
		//setfillstyle(1,15);
	}

	void dividers()
	{
		if (yldi > 50 && yrdi > 50)
		{
			setfillstyle(1, 0);
			bar(580, 0, xldi + wd, yldi - 50);

			setfillstyle(1, 14);
			bar(xldi - wd, yldi - 50, xldi + wd, yldi);

			setfillstyle(1, 0);
			bar(900, 0, xrdi + wd, yrdi - 50);

			setfillstyle(1, 14);
			bar(xrdi - wd, yrdi - 50, xrdi + wd, yrdi);
		}
		else if (yldi > 0 && yrdi > 0)
		{
			setfillstyle(1, 14);
			bar(580, 0, xldi + wd, yldi);

			bar(900, 0, xrdi + wd, yrdi);
		}

		setfillstyle(1, 0);
		bar(xldi - wd, yldi, xldi + wd, yldi + 50);
		bar(xrdi - wd, yrdi, xrdi + wd, yrdi + 50);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 50, xldi + wd, yldi + 100);
		bar(xrdi - wd, yrdi + 50, xrdi + wd, yrdi + 100);

		setfillstyle(1, 0);
		bar(xldi - wd, yldi + 100, xldi + wd, yldi + 150);
		bar(xrdi - wd, yrdi + 100, xrdi + wd, yrdi + 150);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 150, xldi + wd, yldi + 200);
		bar(xrdi - wd, yrdi + 150, xrdi + wd, yrdi + 200);

		setfillstyle(1, 0);
		bar(xldi - wd, yldi + 200, xldi + wd, yldi + 250);
		bar(xrdi - wd, yrdi + 200, xrdi + wd, yrdi + 250);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 250, xldi + wd, yldi + 300);
		bar(xrdi - wd, yrdi + 250, xrdi + wd, yrdi + 300);

		setfillstyle(1, 0);
		bar(xldi - wd, yldi + 300, xldi + wd, yldi + 350);
		bar(xrdi - wd, yrdi + 300, xrdi + wd, yrdi + 350);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 350, xldi + wd, yldi + 400);
		bar(xrdi - wd, yrdi + 350, xrdi + wd, yrdi + 400);

		setfillstyle(1, 0);
		bar(xldi - wd, yldi + 400, xldi + wd, yldi + 450);
		bar(xrdi - wd, yrdi + 400, xrdi + wd, yrdi + 450);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 450, xldi + wd, yldi + 500);
		bar(xrdi - wd, yrdi + 450, xrdi + wd, yrdi + 500);

		setfillstyle(1, 0);
		bar(xldi - wd, yldi + 500, xldi + wd, yldi + 550);
		bar(xrdi - wd, yrdi + 500, xrdi + wd, yrdi + 550);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 550, xldi + wd, yldi + 600);
		bar(xrdi - wd, yrdi + 550, xrdi + wd, yrdi + 600);

		setfillstyle(1, 0);
		bar(xldi - wd, yldi + 600, xldi + wd, yldi + 650);
		bar(xrdi - wd, yrdi + 600, xrdi + wd, yrdi + 650);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 650, xldi + wd, yldi + 700);
		bar(xrdi - wd, yrdi + 650, xrdi + wd, yrdi + 700);

		if (yldi < 34 || yrdi < 34)
		{
			setfillstyle(1, 0);
			bar(xldi - wd, yldi + 700, xldi + wd, yldi + 750);
			bar(xrdi - wd, yrdi + 700, xrdi + wd, yrdi + 750);

			setfillstyle(1, 14);
			bar(xldi - wd, yldi + 750, xldi + wd, yldi + 800);
			bar(xrdi - wd, yrdi + 750, xrdi + wd, yrdi + 800);

			setfillstyle(1, 0);
			bar(xldi - wd, yldi + 800, 600, 833);
			bar(xrdi - wd, yrdi + 800, 920, 833);
		}
		else if (yldi > 33 || yrdi > 33)
		{
			setfillstyle(1, 14);
			bar(xldi - wd, yldi + 750, 600, 833);
			bar(xrdi - wd, yrdi + 750, 920, 833);
		}
		/*else if(yrdi > 83||yldi > 83)
			{
				setfillstyle(1,0);
				bar(xldi-wd,yldi+800,600,833);
				bar(xrdi-wd,yrdi+800,920,833);
			}*/

		setfillstyle(1, 15);
	}

	void lanes()
	{

		int w = 5;

		// setfillstyle(1, 15);

		if (yl > 25 && yr > 25) //extra half bar at the begining
			bar(695, 0, xl + w, yl - 25);

		bar(xl - w, yl, xl + w, yl + 50);

		bar(xl - w, yl + 75, xl + w, yl + 125);

		bar(xl - w, yl + 150, xl + w, yl + 200);

		bar(xl - w, yl + 225, xl + w, yl + 275);

		bar(xl - w, yl + 300, xl + w, yl + 350);

		bar(xl - w, yl + 375, xl + w, yl + 425);

		bar(xl - w, yl + 450, xl + w, yl + 500);

		bar(xl - w, yl + 525, xl + w, yl + 575);

		bar(xl - w, yl + 600, xl + w, yl + 650);

		bar(xl - w, yl + 675, xl + w, yl + 725);

		if (yl < 33 && yr < 33)
		{
			bar(xl - w, yl + 750, xl + w, yl + 800); //2nd last lane
		}
		else
		{
			bar(xl - w, yl + 750, 705, 833);
		}

		if (yl > 25 && yr > 25)
			bar(795, 0, xr + w, yr - 25);

		bar(xr - w, yr, xr + w, yr + 50);

		bar(xr - w, yr + 75, xr + w, yr + 125);

		bar(xr - w, yr + 150, xr + w, yr + 200);

		bar(xr - w, yr + 225, xr + w, yr + 275);

		bar(xr - w, yr + 300, xr + w, yr + 350);

		bar(xr - w, yr + 375, xr + w, yr + 425);

		bar(xr - w, yr + 450, xr + w, yr + 500);

		bar(xr - w, yr + 525, xr + w, yr + 575);

		bar(xr - w, yr + 600, xr + w, yr + 650);

		bar(xr - w, yr + 675, xr + w, yr + 725);

		if (yl < 33 && yr < 33)
		{
			bar(xr - w, yr + 750, xr + w, yr + 800);
		}
		else
		{
			bar(xr - w, yr + 750, 805, 833);
		}

		if (yl < 50 && yr < 50) //half lanes at the last, i.e until xr,yr,l,yl reach 75
		{
			bar(xl - w, yl + 825, xl + w, 833);
			bar(xr - w, yr + 825, xr + w, 833);
		}
		//delay(1000);
	}
};

class game : public road, public vehicle //,public obstacles
{
  public:
	int z = 0;
	char ki;
	void gameon()
	{
		float t = 0.02, d = 10;
		while (1)
		{
			if (kbhit()) //this infinite while is for changing lanes
			{
				switch (ki = getch())
				{
				case KEY_LEFT:
					z = moveleft();
					break;
				case KEY_RIGHT:
					z = moveright();
					break;
				}
				/*if(!z)
						cout<<"\nGAME OVER!!!";*/
			}
			if (yr < 75 || yl < 75)
			{
				draw();
				dr();
				lanes();
				if (yrdi < 100 || yldi < 100) //we can also just write:if(yrdi==100/99):yrdi=yrd in case we dont need to increase speed
				{
					dividers();
					yrdi++;
					yldi++;
				}
				else
				{
					yrdi = yrd;
					yldi = yld;
					dividers();
				}
				yr++;
				yl++;
				//  delay(10);
				swapbuffers();
				cleardevice();
			}
			else
			{
				yr = yrl;
				yl = yll;
			}
		}
	}
	int moveleft()
	{
		int r = xc - 100;
		while (xc > r)
		{
			if (yr < 75 || yl < 75)
			{
				draw();
				dr();
				lanes();
				if (yrdi < 100 || yldi < 100) //we can also just write:if(yrdi==100/99):yrdi=yrd in case we dont need to increase speed
				{
					dividers();
					yrdi++;
					yldi++;
				}
				else
				{
					yrdi = yrd;
					yldi = yld;
					dividers();
				}
				xc -= 20; //if you want to change this, then change while loop condition also to "xc>=r-100"
				yr++;
				yl++;
				//  delay(5);
				swapbuffers();
				cleardevice();
			}
			else
			{
				yr = yrl;
				yl = yll;
			}
		}
		if (xc <= r)
			return 1;
		return 0;
	}
	int moveright()
	{
		int r = xc;
		while (xc != r + 100)
		{
			if (yr < 75 || yl < 75)
			{
				draw();
				dr();
				lanes();
				if (yrdi < 100 || yldi < 100) //we can also just write:if(yrdi==100/99):yrdi=yrd in case we dont need to increase speed
				{
					dividers();
					yrdi++;
					yldi++;
				}
				else
				{
					yrdi = yrd;
					yldi = yld;
					dividers();
				}
				xc++; //if you want to change this, then change while loop condition also to "xc>=r-100"
				yr++;
				yl++;
				//  delay(5);
				swapbuffers();
				cleardevice();
			}
			else
			{
				yr = yrl;
				yl = yll;
			}
		}
		if (xc == r + 100)
			return 1;
		return 0;
	}
};

class Coins
{
  public:
	float x, y; // coords

	static float angle;		 // and angle equal for all coins
	static int r, t, fstyle; // radius, thickness

	int i;

	//Coins() {  }

	Coins(int k /*, int j*/) : x(k) /*, y(j - r)*/
	{
		i = 834 + 2 * r;
		y = 0;
	}

	Coins(int i, int j) : x(i), y(j - r) {}

	void draw()
	{
		int temp = getcolor();
		if (angle > M_PI)
			angle = fmod(angle, M_PI); // simple solution
		setcolor(COLOR(240, 121, 24));
		setlinestyle(0, 0, 2);
		setfillstyle(fstyle, COLOR(240, 121, 24));
		fillellipse(x + t * cos(angle), y, r * sin(angle), r);
		bar(x - t * cos(angle), y - r, x + t * cos(angle), y + r);
		setfillstyle(fstyle, YELLOW);
		fillellipse(x - t * cos(angle), y, r * sin(angle), r);
		setcolor(temp);
		setlinestyle(0, 0, 1);
	}
};
float Coins::angle = 0;
int Coins::r = 30, Coins::t = 5, Coins::fstyle = 1;

class Obstacle
{

  public:
	int drop, x;
	Obstacle(int lane)
	{
		x = lane - 25; //the ref/centre of bomb is about 25units offset from lanes
		drop = -12.5;
	}
	void draw() //add parameter for lane // set default value for drop (y coordinate)
	{
		//int x = lane;		//200 + (lane - 1) * 20;				// Update x coordinate
		setfillstyle(SOLID_FILL, BLACK);
		fillellipse(25 + x, 25 + drop, 25, 25);
		// floodfill(25 + x, 25 + drop, WHITE);
		setfillstyle(SOLID_FILL, RED);
		putpixel(32 + x, 12 + drop, WHITE);
		putpixel(36 + x, 18 + drop, WHITE);
		putpixel(20 + x, 25 + drop, WHITE);
		line(10 + x, 5 + drop, 25 + x, 20 + drop);
		line(25 + x, 20 + drop, 39 + x, 5 + drop);
		line(44 + x, 10 + drop, 30 + x, 25 + drop);
		line(30 + x, 25 + drop, 44 + x, 39 + drop);
		line(39 + x, 44 + drop, 25 + x, 30 + drop);
		line(25 + x, 30 + drop, 10 + x, 44 + drop);
		line(5 + x, 39 + drop, 20 + x, 25 + drop);
		line(20 + x, 25 + drop, 5 + x, 10 + drop);
		floodfill(25 + x, 25 + drop, WHITE);
		arc(27 + x, -5 + drop, 90, 180, 5);
		arc(27 + x, -10 + drop, 270, 360, 5);

		arc(27 + x, -15 + drop, 270, 360, 5);
		line(32 + x, -10 + drop, 32 + x, -15 + drop);
		line(22 + x, -10 + drop, 27 + x, -5 + drop);
		line(22 + x, -5 + drop, 22 + x, 0 + drop);
		line(27 + x, -5 + drop, 27 + x, 0 + drop);
		setfillstyle(SOLID_FILL, YELLOW);
		floodfill(25 + x, -8 + drop, WHITE);
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

	void add(Coins *ncoin)
	{
		node *nn = new node;
		nn->next = NULL;
		nn->c = ncoin;
		if (hn == NULL)
		{
			hn = nn;
			return;
		}

		cn = hn;
		while (cn->next != NULL)
			cn = cn->next;
		cn->next = nn;
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

	void add_drop(Obstacle *cbomb)
	{
		node_o *nn = new node_o;
		nn->next = NULL;
		nn->o = cbomb;
		if (hn == NULL)
		{
			hn = nn;
			return;
		}
		cn = hn;
		while (cn->next != NULL)
			cn = cn->next;
		cn->next = nn;
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
		setfillstyle(1, YELLOW);

		bar(getmaxwidth() / 3 - 50, 0, 2 * getmaxwidth() / 3, getmaxheight() / 10 + 300);
		setcolor(BLACK);
		rectangle(getmaxwidth() / 3 - 40, 10, 2 * getmaxwidth() / 3 - 10, getmaxheight() / 10 + 300 - 10);

		setbkcolor(YELLOW);
		setcolor(BLACK);
		settextstyle(TRIPLEX_SCR_FONT, HORIZ_DIR, 3);

		bgiout << "NAME               SCORE";
		outstreamxy(getmaxwidth() / 3, 0);
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
void leaderboard()
{

	initwindow(getmaxwidth(), getmaxheight(), "Leader Board");

	LeaderBoard l;
	l.load();
	l.display();

	getch();
	closegraph();
	return;
}

void game()
{
	LeaderBoard lb;
	int p, points = 0, mflag = 1, oc = 0, pob, gameover = 0;
	int poc;
	int mll, mml, mrl;
	char c, ki;
	bool mleft = 0, mright = 0;

	initwindow(getmaxwidth(), getmaxheight());

	int random, cc = 0;

	Coins *nc;
	Camera cam;
	cam.yaw(-45);
	cam.pitch(60);
	cam.rePos();
	Road r;
	Car car;

	vehicle v;
	road ro;
	LIST_coins l;
	LIST_obstacles lob;
	Obstacle *bomb;
	srand(time(0));

	settextstyle(10, 0, 4);

	do
	{
		cc++;
		oc++;
		if (kbhit())
		{
			switch (ki = getch())
			{
			case KEY_LEFT:

				if (v.xc == 750 || v.xc == 850)
				{
					if (mflag == 1)
					{
						mleft = 1;
						p = v.xc;
					}
					if (v.xc == 750)
						mll = 1;
					else
						mml = 1;
				}
				break;
			case KEY_RIGHT:
				if (v.xc == 750 || v.xc == 650)
				{
					if (mflag == 1)
					{
						mright = 1;
						p = v.xc;
					}
					if (v.xc == 750)
						mrl = 1;
					else
						mml = 1;
				}
				break;

				/*case 'F':
						++Coins::fstyle %= 12;
						break;*/

			case 'X':
			case 'x':
				c = 'x';
			}
		}
		if (cc % 150 == 0)
		{
			poc = rand();
			poc %= 3;
			switch (poc)
			{
			case 0:
				nc = new Coins(650 /*,getmaxy() / 2*/);

				break;
			case 1:
				nc = new Coins(750 /*,getmaxy() / 2*/);

				break;
			case 2:
				nc = new Coins(850 /*,getmaxy() / 2*/);

				break;
			}
			l.add(nc);
		}
		if (oc % 250 == 0)
		{
			switch (v.xc)
			{
			case 650:
				pob = rand();
				pob %= 5;
				switch (pob)
				{
				case 0:
					bomb = new Obstacle(650);
					break;
				case 1:
					bomb = new Obstacle(750);
					break;
				case 2:
					bomb = new Obstacle(850);
					break;
				case 3:
					bomb = new Obstacle(750);
					lob.add_drop(bomb);
					bomb = new Obstacle(850);
					break;
				case 4:
					bomb = new Obstacle(650);
					lob.add_drop(bomb);
					bomb = new Obstacle(850);
					break;
				}
				lob.add_drop(bomb);
				break;

			case 750:
				pob = rand();
				pob %= 6;
				switch (pob)
				{
				case 0:
					bomb = new Obstacle(650);
					break;
				case 1:
					bomb = new Obstacle(750);
					break;
				case 2:
					bomb = new Obstacle(850);
					break;
				case 3:
					bomb = new Obstacle(750);
					lob.add_drop(bomb);
					bomb = new Obstacle(850);
					break;
				case 4:
					bomb = new Obstacle(650);
					lob.add_drop(bomb);
					bomb = new Obstacle(850);
					break;
				case 5:
					bomb = new Obstacle(650);
					lob.add_drop(bomb);
					bomb = new Obstacle(750);
					break;
				}
				lob.add_drop(bomb);
				break;

			case 850:
				pob = rand();
				pob %= 5;
				switch (pob)
				{
				case 0:
					bomb = new Obstacle(650);
					break;
				case 1:
					bomb = new Obstacle(750);
					break;
				case 2:
					bomb = new Obstacle(850);
					break;
				case 3:
					bomb = new Obstacle(750);
					lob.add_drop(bomb);
					bomb = new Obstacle(650);
					break;
				case 4:
					bomb = new Obstacle(650);
					lob.add_drop(bomb);
					bomb = new Obstacle(850);
					break;
				}
				lob.add_drop(bomb);
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
						bomb = new Obstacle(650);
						break;
					case 1:
						bomb = new Obstacle(750);
						break;
					case 2:
						bomb = new Obstacle(850);
						break;
					case 3:
						bomb = new Obstacle(750);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					case 4:
						bomb = new Obstacle(650);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					}
					lob.add_drop(bomb);
				}
				else if (mrl)
				{
					pob = rand();
					pob %= 5;
					switch (pob)
					{
					case 0:
						bomb = new Obstacle(650);
						break;
					case 1:
						bomb = new Obstacle(750);
						break;
					case 2:
						bomb = new Obstacle(850);
						break;
					case 3:
						bomb = new Obstacle(750);
						lob.add_drop(bomb);
						bomb = new Obstacle(650);
						break;
					case 4:
						bomb = new Obstacle(650);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					}
					lob.add_drop(bomb);
				}
				else
				{
					pob = rand();
					pob %= 6;
					switch (pob)
					{
					case 0:
						bomb = new Obstacle(650);
						break;
					case 1:
						bomb = new Obstacle(750);
						break;
					case 2:
						bomb = new Obstacle(850);
						break;
					case 3:
						bomb = new Obstacle(750);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					case 4:
						bomb = new Obstacle(650);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					case 5:
						bomb = new Obstacle(650);
						lob.add_drop(bomb);
						bomb = new Obstacle(750);
						break;
					}
					lob.add_drop(bomb);
				}
			}
			}
		}

		if (ro.yr < 75 || ro.yl < 75)
		{
			if (ro.yrdi < 100 || ro.yldi < 100) //we can also just write:if(yrdi==100/99):yrdi=yrd in case we dont need to increase speed
			{
				ro.dividers();
				ro.yrdi++;
				ro.yldi++;
			}
			else
			{
				ro.yrdi = ro.yrd;
				ro.yldi = ro.yld;
				ro.dividers();
			}
			ro.yr++;
			ro.yl++;
		}
		else
		{
			ro.yr = ro.yrl;
			ro.yl = ro.yll;
			ro.lanes();
			v.draw();
		}
		if (mleft)
		{
			if (v.xc > p - 100)
				v.xc -= 5;
			else
				mleft = 0;

			if (v.xc == p - 100)
				mflag = 1;
			else
				mflag = 0;
		}
		if (mright)
		{
			if (v.xc < p + 100)
				v.xc += 5;
			else
				mright = 0;

			if (v.xc == p + 100)
				mflag = 1;
			else
				mflag = 0;
		}

		l.cn = l.hn;
		if (l.hn != NULL)
		{
			while (l.cn != NULL)
			{
				if (l.cn->life == 1)
				{
					if ((l.cn->c->y + 30 > v.yc && l.cn->c->y + 30 < v.yc + 125 && l.cn->c->x > v.xc - 25 && l.cn->c->x < v.xc + 25) || (l.cn->c->y - 30 > v.yc && l.cn->c->y - 30 < v.yc + 125 && l.cn->c->x > v.xc - 25 && l.cn->c->x < v.xc + 25))
					{
						l.cn->life = 0;
						points += 50;
					}
					else if (l.cn->c->y > getmaxy() + Coins::r)
						l.cn->life = 0;
					l.cn->c->draw();
					l.cn->c->angle += 1.5 * 0.01745329251994329576923690768489;
					l.cn->c->y++;
				}
				l.cn = l.cn->next;
			}
		}

		lob.cn = lob.hn;
		if (lob.hn != NULL)
		{
			while (lob.cn != NULL)
			{
				if (lob.cn->life == 1)
				{
					// if ((lob.cn->o->drop + 25 > v.yc && lob.cn->o->drop + 25 < v.yc + 125 && lob.cn->o->x + 25 > v.xc - 25 && lob.cn->o->x + 25 < v.xc + 25) || (lob.cn->o->drop - 25 > v.yc && lob.cn->o->drop - 25 < v.yc + 125 && lob.cn->o->x + 25 > v.xc - 25 && lob.cn->o->x + 25 < v.xc + 25))
					if ((v.yc < lob.cn->o->drop + 50 && v.yc > lob.cn->o->drop - 125 && v.xc < lob.cn->o->x + 75 && v.xc > lob.cn->o->x - 25))
					{
						// lob.cn->life = 0;
						gameover = 1;
						c = 'x';
						break;
					}
					else if (lob.cn->o->drop == 834)
						lob.cn->life = 0;

					lob.cn->o->draw();
					lob.cn->o->drop++;
				}
				lob.cn = lob.cn->next;
			}
		}
		l.delet();
		lob.delet();
		bgiout << "\nPOINTS\n\t" << points;
		outstreamxy(1250, 400);
		delay(1);
		swapbuffers();
		cleardevice();

		ro.dr();
		ro.dividers();
		ro.lanes();
		v.draw();

	} while (c != 'x');

	cleardevice();

	setcolor(YELLOW);
	settextstyle(BOLD_FONT, HORIZ_DIR, 3);
	bgiout << "\nEnter your name(only 5 characters):";
	outstreamxy(getmaxwidth() / 3, 0);
	swapbuffers();
	string name;

	cleardevice();
	for (int q = 0; q < 5; q++)
	{
		bgiout << "\nEnter your name(only 5 characters):";
		outstreamxy(getmaxwidth() / 3, 0);
		name += getch();
		bgiout << name;
		outstreamxy(getmaxwidth() / 3, 100);
		swapbuffers();
	}

	lb.newEntry(name, points);

	closegraph();
}
int main()
{
start:
	initwindow(getmaxwidth(), getmaxheight(), "MENU");

	int coin_X = 0.01 * getmaxy() + (Coins::r), coin_Y = 0.04 * getmaxy() + 2 * (Coins::r);
	int coin_pos = 1;
	Coins c(coin_X, coin_Y);

	setbkcolor(LIGHTCYAN);
	setcolor(BLUE);
	readimagefile("a.jpg", 0, 0, getwindowwidth(), getwindowheight());
	settextstyle(1, HORIZ_DIR, 10);
	outtextxy(getmaxwidth() / 3, getmaxheight() / 10, "MKBS");
	swapbuffers();
	readimagefile("a.jpg", 0, 0, getwindowwidth(), getwindowheight());
	settextstyle(1, HORIZ_DIR, 10);
	outtextxy(getmaxwidth() / 3, getmaxheight() / 10, "MKBS");

	setviewport(0.68 * getmaxwidth(), 0.60 * getmaxheight(), 0.98 * getmaxwidth(), 0.95 * getmaxheight(), 1);

	while (1)
	{

		setfillstyle(1, LIGHTCYAN);
		bar(0, 0, 100, 100);

		settextstyle(1, HORIZ_DIR, 5);
		outtextxy(0.05 * getmaxx(), 0.05 * getmaxy(), "New game");

		settextstyle(1, HORIZ_DIR, 5);
		outtextxy(0.05 * getmaxx(), 0.15 * getmaxy(), "Leaderboard");

		settextstyle(1, HORIZ_DIR, 5);
		outtextxy(0.05 * getmaxx(), 0.25 * getmaxy(), "Exit");
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
					closegraph();
					return 0;
				}
				break;
			case KEY_UP:
				if (coin_pos != 1)
				{
					coin_pos--;
					c.y -= 0.10 * getmaxy();
				}
				break;
			case KEY_DOWN:
				if (coin_pos != 3)
				{
					coin_pos++;
					c.y += 0.10 * getmaxy();
				}
				break;
			default:
				break;
			}
		}
		c.draw();
		Coins::angle += 6 * 0.01745329251994329576923690768489;
		delay(15);

		swapbuffers();
		clearviewport();
	}
}
