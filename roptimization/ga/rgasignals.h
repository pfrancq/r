/*

	R Project Library

	RGASignals.h

	Signals for Genetic Algorithms - Header

	Copyright 1998-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef RRASignals_H
#define RRASignals_H


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* \ingroup GA
* This class represent a basic reveicer for GA signals. By default, it does
* nothing.
* @author Pascal Francq
* @short Basic signal receiver.
*/
template<class cInst,class cChromo,class cFit>
	class RGASignalsReceiver
{
public:

	/**
	* \ingroup GA
	* This class represent a basic reveicer for GA signals. By default, it does
	* @author Pascal Francq
	* @short Best Signal.
	*/
	class BestSig
	{
	public:
		cChromo *Best;

		BestSig(cChromo *best) : Best(best) {}
	};

	/**
	* \ingroup GA
	* This class represent a signal to gives the user-interface the hand.
	* @author Pascal Francq
	* @short Interact Signal.
	*/
	class InteractSig
	{
	public:
		/**
		* Construct this signal.
		*/
		InteractSig(void) {}
	};

	/**
	* \ingroup GA
	* This class represent a signal to indicate that the GA is running.
	* @author Pascal Francq
	* @short Run Signal.
	*/
	class RunSig
	{
	public:
		/**
		* Construct this signal.
		*/
		RunSig(void) {}
	};

	/**
	* \ingroup GA
	* This class represent a signal to indicate that the GA is stopped.
	* @author Pascal Francq
	* @short Stop Signal.
	*/
	class StopSig
	{
	public:
		/**
		* Indicate if the GA stops after an external break.
		*/
		bool bExternBreak;

		/**
		* Construct this signal.
		*/
		StopSig(bool externbreak) : bExternBreak(externbreak) {}
	};

	/**
	* \ingroup GA
	* This class represent a signal to indicate that a generation is
	* finished.
	* @author Pascal Francq
	* @short Generation Signal.
	*/
	class GenSig
	{
	public:
		unsigned int Gen;
		unsigned int BestGen;
		cChromo** Pop;
		cChromo* Best;

		GenSig(unsigned int gen,unsigned int bestgen,cChromo **pop,cChromo *best)
			: Gen(gen), BestGen(bestgen), Pop(pop), Best(best) {}
	};

	/**
	* Construct the receiver.
	*/
	RGASignalsReceiver(void) {}

	/**
	* Respond to a interact signal.
	*/
	virtual void receiveInteractSig(InteractSig*) {}

	/**
	* Respond to a run signal.
	*/
	virtual void receiveRunSig(RunSig*) {}

	/**
	* Respond to a stop signal.
	*/
	virtual void receiveStopSig(StopSig*) {}

	/**
	* Respond to a stop signal.
	*/
	virtual void receiveBestSig(BestSig*) {}

	/**
	* Respond to a gen signal.
	*/
	virtual void receiveGenSig(GenSig*) {}

	/**
	* Compare function.
	*/
	int Compare(const RGASignalsReceiver* s) const { return(this!=s); }
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
