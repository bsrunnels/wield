#ifndef WIELD_MAIN_GAMMAINTERFACE1D_H
#define WIELD_MAIN_GAMMAINTERFACE1D_H

#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <csignal>


#include "tclap/CmdLine.h"
#include "Reader.h"

#include "Utils/wieldExceptions.h"
#include "Utils/wieldColor.h"
#include "Utils/wieldTypes.h"
#include "Utils/wieldRotations.h"
#include "Utils/VTK/wieldVTK.h"
#include "Utils/wieldProgress.h"
#include "Utils/wieldEigen.h"
#include "Series/wieldCosSeries.h"
#include "Optimization/wieldConvexify1D.h"
#include "SurfaceIntegrate.h"
#include "IO/wieldReaderMacros.h"


using namespace std;

namespace Wield
{
namespace Main
{
void GammaInterface1D(Reader::Reader &reader,bool dynamicPlot)
{
  WIELD_EXCEPTION_TRY;

  // READ STANDARD VARIABLES
  WIELD_IO_READ_CRYSTAL(1);
  WIELD_IO_READ_CRYSTAL(2);
  WIELD_IO_READ_CRYSTAL_ORIENTATION("GammaInterface1D",1);
  WIELD_IO_READ_CRYSTAL_ORIENTATION("GammaInterface1D",2);
  WIELD_IO_READ_PARAMETERS("GammaInterface1D");

  // Specify the range of the FREE VARIABLES:
  double phiMin       = reader.Read<double>("GammaInterface1D","PhiMin",0.);
  double dPhi         = reader.Read<double>("GammaInterface1D","DPhi",0.);
  double phiMax       = reader.Read<double>("GammaInterface1D","PhiMax",0.);
  // Specify the rotation of the INTERFACE
  double phiRotX      = reader.Read<double>("GammaInterface1D","PhiRotX",0.);
  double phiRotY      = reader.Read<double>("GammaInterface1D","PhiRotY",0.);
  double phiRotZ      = reader.Read<double>("GammaInterface1D","PhiRotZ",0.);
  // MISC
  double tolerance    = reader.Read<double>("GammaInterface1D","Tolerance",0.);
  string distribution = reader.Read<string>("GammaInterface1D","Distribution", "cauchy");
  bool fullConvexify  = reader.Find("GammaInterface1D", "FullConvexify");
  // OUTPUT FILE STREAM
  bool printData      = reader.Find("GammaInterface1D","OutFile");
  ofstream out;
  if (printData)
    {
      string outFile      = reader.Read<string>("GammaInterface1D","OutFile"); 
      out.open(outFile.c_str());
      //open(out,outFile.c_str());
    }

  // ROTATE ORIENTATION RELATIONSHIP AND COMPUTE GRAIN BOUNDARY ENERGY
  Wield::Utils::VTK::PlotLine *plotWindow;
  if (dynamicPlot) plotWindow = new Wield::Utils::VTK::PlotLine();
  vector<double> X,Y;
  for (double phi = phiMin; phi <= phiMax; phi += dPhi)
    {
      WIELD_PROGRESS("Computing energy curve", phi-phiMin, phiMax-phiMin, dPhi);

      Matrix3d N = 
	createMatrixFromXAngle(phi*phiRotX) *
	createMatrixFromYAngle(phi*phiRotY) *
	createMatrixFromZAngle(phi*phiRotZ);
      
      //if (fabs(N(2,2)) < 1E-8) continue;
	  
      double W = (a - b*SurfaceIntegrate(crystal1, omega1*N, crystal2, omega2*N, stdDev, tolerance, distribution));
      //if (areaNormalization) W /= N(2,2);
      X.push_back(phi);
      Y.push_back(W);
      if (dynamicPlot) 
	if (X.size() > 1) 
	  {
	    plotWindow->clear(); 
	    plotWindow->SetData(X,Y);
	  }
    }

  cout << endl;
  
  vector<double> Yc = Wield::Optimization::Convexify1DAngles(X,Y,fullConvexify);
  if (printData)
    {
      for (int i=0; i<X.size(); i++)
	out << X[i] << " " << Y[i] << " " << Yc[i] << " " << endl;
      out.close();
    }
	
  if (dynamicPlot) 
    {
      plotWindow->clear(); 
      plotWindow->SetData(X,Y);
      plotWindow->SetData(X,Yc, true);
    }

  WIELD_EXCEPTION_CATCH_FINAL;
}
}
}

#endif
