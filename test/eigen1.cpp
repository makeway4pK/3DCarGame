#include <iostream>
#include "../Eigen/Dense"
using namespace Eigen;
using namespace std;
int main()
{
	Matrix<double, 4, 4, RowMajor> m = MatrixXd::Random(4, 4);
	m = (m + MatrixXd::Constant(4, 4, 1.2)) * 50;
	cout << "m =" << endl
		 << m << endl;
	VectorXd v(4);
	v << 1, 2, 3, 4;
	cout << "m * v =" << endl;
	int i = 200;
	// while (i--)
	// 	cout << m * m;
	cout << Matrix<double, 200, 4, ColMajor>::Random() * m;
}