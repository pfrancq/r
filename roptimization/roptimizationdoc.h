/*

	R Project Library

	ROptimizationDoc.h

	R Optimization Documentation - Header.

	Copyright 2009-2010 by Pascal Francq (pascal@francq.info).

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
#ifndef ROPTIMIZATIONDOCH
#define ROPTIMIZATIONDOCH


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * \mainpage R Optimization Library
 *
 * \section roptimization_intro_sec Introduction
 *
 * The R Optimization library provides several optimization-oriented tools.
 *
 * \section main_classes_sec Main classes
 * The main types of classes are:
 * - A complete implementation of the PROMETHEE multi-criteria decision method.
 * - A basic Genetic Algorithm (GA).
 * - A complete implementation of the Grouping Genetic Algorithms (GGA).
 * - A complete implementation of the Similarity-based Clustering Genetic Algorithm (SCGA).
 * - A complete implementation of the Hierarchical Genetic Algorithms (HGA).
 * - A complete implementation of the 2D Placement Genetic Algorithm (GA2D).
 *
 * \section roptimization_promethee PROMETHEE
 * PROMETHEE is multi-criteria decision method which allows to classify of
 * solutions depending of different criteria, each criterion having a given
 * weight. One of the advantages of the PROMETHEE method is that it allows to
 * compare criteria with different scales.
 *
 * The RPromKernel class provides a kernel that implements the PROMETHEE
 * method. It manages a set of solutions that instances of the RPromSol class.
 * The different criteria are represented by classes inheriting from
 * RPromCriterion. Actually, the only existing type of criteria is implemented
 * in the RPromLinearCriterion class. Once the solutions and the criteria
 * defined, the methods RPromKernel::Assign allows to associate a value for a
 * given pair of solution and criteria.
 *
 * \section roptimization_ga Genetic Algorithms (GA)
 * A set of classes provides a representation for a generic algorithm. These
 * classes are implemented as templates since the genetic algorithms are a
 * generic approach to solve optimization problem.
 *
 * To implement a particular genetic algorithm, at least, three classes must be
 * created. A first one inheriting from RChromo and representing the chromosome
 * (i.e. the coding used by the particular GA). A second one inheriting from
 * RInst. The third class must inherit from RThreadData and represent a set of
 * data used by the GA and thread-dependent. In particular, when implementing a
 * given operator (such as the crossover), it is sometimes necessary to used
 * specific structures of data. By using the mechanism of RThreadData, these
 * structures will be duplicated of the GA uses multiple threads (which is
 * currently not implemented).
 *
 * \section roptimization_2dga 2D-Placement Genetic Algorithms (GA2D)
 * A set of classes provides a implementation of a generic 2D-placement genetic
 * algorithm. Its aim is to place a set of objects on an area in order to
 * minimize the total area occupied and the total distances of the
 * corresponding connections.
 *
 * Each object is represented as an instance of the RObj2D class. Its defines
 * several connectors (RObj2DConnector), each one having multiple pins
 * (RObj2DPin). Moreover, each object may have several configurations
 * (RObj2DConfig), each one defining the possible pins (RObj2DConfigPin) of
 * each connector (RObj2DConfigConnector). Finally, a RGeoInfo class represent
 * a particular configuration (RObj2DConfig) of an object (RObj2D) placed at a
 * given position.
 *
 * A connection is represented by an instance of the RConnection class. It is
 * represented by a container of RObj2DConnector. The RGeoInfoConnection class
 * represents the instance of that connection. It is a container of RGeoInfoPin
 * representing association of a pin and a geometric information.
 *
 * Each chromosome (RChromo2D) implements a particular layout (RLayout) of the
 * objects. All heuristics inherit from RPlacementHeuristic which place a list
 * of unplaced object on a given layout.
 */


}  //-------- End of namespace R -----------------------------------------------

//------------------------------------------------------------------------------
#endif
