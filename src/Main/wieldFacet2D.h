///
/// \page MainFacet2D Facet2D
///
/// Description
/// -----------
/// Given a precomputed energy surface, determine the facet configuration that minimizes the energy and print to a file.
///
/// Input file options
/// ------------------
///   - \b Facet2D: 
///     - \b *DataFile: Location of a file containing energy surface data
///     - \b MaxFacetOrder [3]: The highest facet order to allow
///     - \b D1 [0], \b D2 [1], \b D3 [1]: Affine scaling constants
///     - \b N1Guess, \b N2Guess, \b N3Guess: A guess of the minimizing facet pattern
///     - \b SearchRadius: If NnGuess specified, how far from each NnGuess to search
///     - \b OutFile: If specified, the location of the output data.
///
/// *=Required
///

#ifndef WIELD_MAIN_FACET2D_H
#define WIELD_MAIN_FACET2D_H

#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <csignal>
#include <cstdlib>
#include <pthread.h>

#include "Reader/Reader.h"

#include "Utils/wieldExceptions.h"
#include "Utils/wieldColor.h"
#include "Utils/wieldTypes.h"
#include "Utils/wieldRotations.h"
#include "Utils/wieldProgress.h"
#include "Utils/wieldEigen.h"
#include "Optimization/wieldConvexify2D.h"
#include "Integrator/wieldSurface.h"
#include "IO/wieldReaderMacros.h"


namespace Wield
{
namespace Main
{
void Facet2D(Reader::Reader &reader,
	     int numThreads = 1)
{
  WIELD_EXCEPTION_TRY;
  // srand(time(NULL));
  // for (int i=0;i<25;i++)
  //   ThetaInRange(360.*(double)rand()/(double)RAND_MAX,
  // 		 360.*(double)rand()/(double)RAND_MAX,
  // 		 360.*(double)rand()/(double)RAND_MAX);
  // exit(0);

  

  //
  // Open input deck
  // 

  std::string dataFile     = reader.Read<std::string>("Facet2D", "DataFile");
  int maxFacetOrder        = reader.Read<int>("Facet2D", "MaxFacetOrder", 3);
  bool symmetricY          = reader.Find("Facet2D.SymmetricY");
  double D1                = reader.Read<double>("Facet2D", "D1",  0.);
  double D2                = reader.Read<double>("Facet2D", "D2",  1.);
  double D3                = reader.Read<double>("Facet2D", "D3",  1.);
  std::ifstream in(dataFile.c_str());
  if (!in)
    WIELD_EXCEPTION_NEW("Could not open input file " << dataFile); 

  //
  // Read in the raw data from the datafile
  //

  std::vector<double> x,y,z,w;
  std::string line;
  while (getline(in,line))
    {
      std::istringstream iss(line);
      std::string token;
      for (int i=0; iss >> token; i++)
	{
	  if (i==0) x.push_back(atof(token.c_str()));
	  if (i==1) y.push_back(atof(token.c_str()));
	  if (i==2) z.push_back(atof(token.c_str()));
	  if (i==3) w.push_back(D2*(D1 + D3*atof(token.c_str())));
	}
    }
  if ( x.size() != y.size() ||
       y.size() != z.size() ||
       z.size() != w.size())
    WIELD_EXCEPTION_NEW("Error reading input file: different x,y,z,w sizes");

  if ( x.size() == 0)
    WIELD_EXCEPTION_NEW("Error: data file appears to be empty");
  
  //
  // Containers for global minimum values
  //

  double wMin = INFINITY;
  Eigen::Vector3d lambdaMin;
  Eigen::Vector3d n1Min, n2Min, n3Min;

  // 
  // Compute r, theta arrays and 
  // Store default minimum value (planar case)
  //

  std::vector<double> r(x.size()), theta(x.size());
  for (int i=0; i<x.size(); i++)
    {
      r[i] = sqrt(x[i]*x[i] + y[i]*y[i]);
      theta[i] = atan2(y[i],x[i])*180./pi; 
      while (theta[i] >= 0.) theta[i] -= 360.;
      while (theta[i] < 0.)  theta[i] += 360.;
      if (fabs(theta[i] - 360.) < 1E-8) theta[i] = 0.;
      if (r[i] < 1E-6) 
	{
	  wMin = w[i];
	  lambdaMin << 1,0,0;
	  n1Min << 0,0,1;
	  n2Min << 0,0,0;
	  n3Min << 0,0,0;
	}
    }

  //
  // Special case: does the user have "a good guess"?
  //

  double searchRadius;
  if (reader.Find("Facet2D","N1Guess"))
    {
      n1Min         = reader.Read<Eigen::Vector3d>("Facet2D","N1Guess");
      n2Min         = reader.Read<Eigen::Vector3d>("Facet2D","N2Guess");
      n3Min         = reader.Read<Eigen::Vector3d>("Facet2D","N3Guess");
      searchRadius  = reader.Read<double>("Facet2D", "SearchRadius");
      std::cout << n1Min.transpose() << std::endl;
      std::cout << n2Min.transpose() << std::endl;
      std::cout << n3Min.transpose() << std::endl;
      std::cout << searchRadius      << std::endl;
    }
  else
    {
      n1Min << 0,0,1;
      n2Min << 0,0,1;
      n3Min << 0,0,1;
      searchRadius  = 2.;
    }

  //
  // Create threading machinery
  //

  pthread_t threads[numThreads];
  int errorCode;
  Wield::Optimization::ConvexifyData2D<3> *args =
    (Wield::Optimization::ConvexifyData2D<3> *)malloc(numThreads * sizeof(Wield::Optimization::ConvexifyData2D<3>));
  for (int i=0; i<numThreads; i++)
    {
      // Store values
      args[i].index = i;
      args[i].numThreads = numThreads;
      args[i].maxFacetOrder = maxFacetOrder;
      args[i].x = &x;
      args[i].y = &y;
      args[i].z = &z;
      args[i].w = &w;
      args[i].r = &r;
      args[i].theta = &theta;
      args[i].wMin = wMin;
      args[i].lambdaMin = lambdaMin;
      args[i].n1Min = n1Min;
      args[i].n2Min = n2Min;
      args[i].n3Min = n3Min;
      args[i].coarsen = 1; 
      args[i].refining = 1; //boolean
      args[i].searchRadius = searchRadius;
      args[i].symmetricY = symmetricY;

      // Off to the races
      errorCode = pthread_create(&threads[i], NULL, Wield::Optimization::Convexify2D<3>, (void*)(&args[i]));
      if (errorCode)
	WIELD_EXCEPTION_NEW("Error starting thread #" << i << ": errorCode = " << errorCode);
    }  

  wMin = INFINITY;
  Eigen::Matrix3d nMin;
  for (int i=0; i<numThreads; i++)
    {
      errorCode = pthread_join(threads[i],NULL);
      if (errorCode)
	WIELD_EXCEPTION_NEW("Error joinng thread #" << i << ": errorCode = " << errorCode);
      if (args[i].wMin < wMin)
	{
	  wMin = args[i].wMin;
	  lambdaMin = args[i].lambdaMin;
	  nMin.col(0) = args[i].n1Min;
	  nMin.col(1) = args[i].n2Min;
	  nMin.col(2) = args[i].n3Min;
	}
    }
  if (reader.Find("Facet2D","OutFile"))
    {
      std::string outFile = reader.Read<std::string>("Facet2D", "OutFile");
      std::ofstream out;
      out.open(outFile.c_str());
      out << wMin << std::endl;
      out << lambdaMin.transpose() << std::endl;
      out << nMin << std::endl;
      out.close();
    }

  WIELD_EXCEPTION_CATCH_FINAL;
}
}
}


#endif
