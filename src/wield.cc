/// 
/// \mainpage WIELD: (W)eak approximation of (I)nterface (E)nergy for bicrysta(L) boun(D)aries 
/// This is a small research program to compute the energy of a bicrystal interface. 
///
/// Usage
/// =====
/// 
/// Running the program:
/// 
///     ./main [input file] [-D<StructName>.<VarName>=<VarValue> ... ] [-p --dynamic-plot] [-n --num-threads <num>] [-h --help]
/// 
/// Runtime variables
/// -----------------
/// 
/// All runtime variables can be specified in one of two ways:
/// - In the input file
/// 
///       $VarName varvalue
///       $StructName {
///           $VarName varvalue
///       }
/// 
/// - In the command line
///
///       ./main -DVarName=varvalue -DStructName.VarName=varvalue
///
/// Command line options
/// --------------------
///
/// Some routines support additional command line options:
/// - -p --dynamic-plot: for some supported functions, a VTK window will be displayed that shows the results of the computation real-time.
/// - -n --num-threads <num>: for some supported functions, use pthread parallelism
///
/// Methods
/// -------
/// 
/// This program is a collection of other methods that can be run seperately or simultaneously.
/// Use the following links to find documentation and example input files for each method
/// 
/// - \ref Wield::Main::CSL Compute the thermalized coincident site lattice \f$\Sigma\f$ value of a bicrystal
/// - \ref Wield::Main::Energy1D Compute the thermalized grain boundary energy for an interface
/// - \ref Wield::Main::MainInterface1D Compute the interface energy as a function of interface orientation, parameterized by one angle
/// - \ref MainFacet2D Use the relaxation algorithm to compute the facet pattern and relaxed energy, given an energy surface
///
/// \file main.cpp
/// \brief This is the short description for main.cpp
///
/// This is the long description for main.cpp
///


#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <csignal>

#include "tclap/CmdLine.h"
#include "TCLAP/IgnoreArg.h"
#include "Reader/Reader.h"

#include "Main/wieldEnergy1D.h"
#include "Main/wieldEnergy2D.h"
#include "Main/wieldCSL.h"
#include "Main/wieldEnergyOffset.h"
// #include "Main/wieldBlenderVoxelData.h"
// #include "Main/wieldVisualizeOR.h"
// #include "Main/wieldEnergySurfaceSphere.h"
// #include "Main/wieldEnergyInterface1D.h"
// #include "Main/wieldEnergyOR1D.h"
#include "Main/wieldFacet2D.h"
#include "Utils/wieldExceptions.h"
#include "Utils/wieldProgress.h"

///
/// \fn signalHandler
/// \brief In case of kill signal, clean up the terminal.
///
void signalHandler(int signum)
{
  /// Hello world
  std::cout << std::endl;
  std::cout << WIELD_COLOR_RESET << "Program terminated" << std::endl;
  exit(signum);
}

int main(int argc,     ///< Number of arguments
	 char* argv[]) ///< Argument values
{
  /// This function parses command line options and opens the file reader.

  WIELD_EXCEPTION_TRY;

  signal(SIGINT, signalHandler);

  //
  // COMMAND LINE PARSING
  // 
  TCLAP::CmdLine cmd("(W)eak approximation of (I)nterface (E)nergy for bicrysta(L) boun(D)aries");
  TCLAP::SwitchArg switchDynamicPlot("p", "dynamic-plot", "Show real-time VTK plot of energy", cmd, false);
  TCLAP::SwitchArg switchVerbose("v", "verbose", "Print extraneous information", cmd, false);
  TCLAP::ValueArg<int> valueNumThreads("n", "num-threads", "Number of pthreads to use",false,1,"", cmd);
  TCLAP::IgnoreArg testIgnoreArg("D","User defined variables","",cmd);
  TCLAP::UnlabeledValueArg<std::string> argFileName("name", "Path to input file", false, "", "inputfile", cmd);
  cmd.parse(argc, argv);
  bool dynamicPlot; dynamicPlot = switchDynamicPlot.getValue();
  bool verbose; verbose = switchVerbose.getValue();
  int numThreads = valueNumThreads.getValue();
  std::string fileName = argFileName.getValue();

  //
  // INPUT FILE PARSING
  // 

  // Create reader
  Reader::Reader *rabbit;
  if (fileName == "")
    rabbit = new Reader::Reader(argc,argv,"$","#","...");
  else 
    rabbit = new Reader::Reader(fileName, argc, argv, "$", "#", "...");
  
  if (rabbit->Find("Energy1D"))
    Wield::Main::Energy1D(*rabbit,numThreads);
  if (rabbit->Find("Energy2D"))
    Wield::Main::Energy2D(*rabbit,numThreads);
  if (rabbit->Find("CSL"))
    Wield::Main::CSL(*rabbit,numThreads,verbose);
  if (rabbit->Find("EnergyOffset"))
	  Wield::Main::EnergyOffset(*rabbit);
  // if (rabbit->Find("BlenderVoxelData"))
  //   Wield::Main::BlenderVoxelData(*rabbit);
  // if (rabbit->Find("VisualizeOR"))
  //   Wield::Main::VisualizeOR(*rabbit, dynamicPlot);
  // if (rabbit->Find("EnergyOR1D"))
  //   Wield::Main::EnergyOR1D(*rabbit, dynamicPlot);
  // if (rabbit->Find("EnergyInterface1D"))
  //   Wield::Main::EnergyInterface1D(*rabbit, dynamicPlot);
  // if (rabbit->Find("EnergySurfaceSphere"))
  //   Wield::Main::EnergySurfaceSphere(*rabbit, dynamicPlot, numThreads); 
  if (rabbit->Find("Facet2D"))
    Wield::Main::Facet2D(*rabbit,numThreads); 

  // WARN ABOUT UNUSED VARIABLES
  rabbit->PrintUnusedVariableWarnings();

  WIELD_EXCEPTION_CATCH_FINAL;
}


