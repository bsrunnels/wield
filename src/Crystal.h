#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include "Eigen/Core"
#include "Eigen/Geometry"
#define PI 3.14159265369

using namespace std;
using namespace Eigen;

class Crystal
{
  Crystal();
  Crystal(double C[4][4][4]);
  Crystal(int order,
	  vector<pair<Vector3d,double> > atoms);

  int order;
  double alpha[3];
  double ***C;
};


// int Generate(int order,
// 	     vector<double> alpha,
//           vector<pair<vector<double>, double> > atoms)
int Generate();
double CosExpInt(int n, double alpha, double epsilon, double x0);