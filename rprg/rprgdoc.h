/*

	R Project Library

	RPrgDoc.h

	R Prg Documentation - Header.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef RPRGDOCH
#define RPRGDOCH


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* \mainpage R PRG Library
*
* \section rprg_intro_sec Introduction
*
* The R PRG library provides a very simple scripting language. It can be used
* to automate some tasks.
*
* The example below is from the GALILEI platform and shows how a script can
* help to make tests:
* @code
* sub Cluster(dev):
*   GALILEIApp.SetTest(dev)
*   GALILEIApp.SetMeasureParam("Documents Similarities","Multi-space","DeviationRate",dev)
*   Session.GroupDocs()
*   Simulator.CompareIdeal("Topics Evaluation")
*
* sub TestConfig(name,minprof,maxprof,nbdocs):
*   GALILEIApp.SetOutput(name)
*   Session.SetRand("1")
*   Simulator.SetSimulationParam("NbProfMin",minprof)
*   Simulator.SetSimulationParam("NbProfMax",maxprof)
*   Simulator.SetSimulationParam("NbOK",nbdocs)
*   Simulator.SetSimulationParam("NbKO",nbdocs)
*   Simulator.StartSimulation()
*   GALILEIApp.PrintOutput("Sets","Recall","Precision","Adjusted Rand Index")
*   for devs from -2 to 12 step 0.05:
*     Cluster(devs)
*   GALILEIApp.PrintOutput()
*   GALILEIApp.PrintOutput()
*
* Session=new GSession(true,true,true,true)
* Simulator=new GSimulator()
* TestConfig("/home/pfrancq/test.txt",0,0,0)
* @endcode
*
* \section rprg_main_classes_sec Main Classes
* The R::RInterpreter provides an interpreter that can execute a scripting
* program.
*
* You can add whatever classes that you need by inheriting from R::RPrgClass and
* R::RPrgFunc.
*/


}  //-------- End of namespace R -----------------------------------------------

//------------------------------------------------------------------------------
#endif
