/*

	Rainbow Library Project

  RGASignals.h

  Signals for Genetic Algorithms - Header

  (C) 1998-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


	As a special exception to the GNU General Public License, permission is
	granted for additional uses of the text contained in its release
	of the Rainbow Library.

	The exception is that, if you link the Rainbow with other files
	to produce an executable, this does not by itself cause the
	resulting executable to be covered by the GNU General Public License.
	Your use of that executable is in no way restricted on account of
	linking the Rainbow library code into it.

	This exception does not however invalidate any other reasons why
	the executable file might be covered by the GNU General Public License.

	This exception applies only to the code released under the
	name Rainbow.  If you copy code from other releases into a copy of
	RAinbow, as the General Public License permits, the exception does
	not apply to the code that you add in this way.  To avoid misleading
	anyone as to the status of such modified files, you must delete
	this exception notice from them.

	If you write modifications of your own for Rainbow, it is your choice
	whether to permit this exception to apply to your modifications.
	If you do not wish that, delete this exception notice.

*/



//---------------------------------------------------------------------------
#ifndef RRASignalsH
#define RRASignalsH



//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/** This class represent a basic reveicer for GA signals. By default, it does
	* nothing.
	* @author Pascal Francq
	* @short Basic signal receiver.
	*/
template<class cInst,class cChromo,class cFit>
	class RGASignalsReceiver
{
public:
	/** This class represent a basic reveicer for GA signals. By default, it does
		* @author Pascal Francq
		* @short Best Signal.
		*/
	class BestSig
	{
	public:
		cChromo *Best;	

		BestSig(cChromo *best) : Best(best) {}
	};

  /**	This class represent a signal to gives the user-interface the hand.
  	* @author Pascal Francq
  	* @short Interact Signal.
  	*/
  class InteractSig
  {
  public:
  	/** Construct this signal.*/
  	InteractSig(void) {}
  };

  /**	This class represent a signal to indicate that the GA is running.
  	* @author Pascal Francq
  	* @short Run Signal.
  	*/
  class RunSig
  {
  public:

  	/** Construct this signal.*/
  	RunSig(void) {}
  };

  /**	This class represent a signal to indicate that the GA is stopped.
  	* @author Pascal Francq
  	* @short Stop Signal.
  	*/
  class StopSig
  {
  public:
  	/** Indicate if the GA stops after an external break.*/
  	bool bExternBreak;						

  	/** Construct this signal.*/
  	StopSig(bool externbreak) : bExternBreak(externbreak) {}
  };

	/** This class represent a signal to indicate that a generation is
		*	finished.
		* @author Pascal Francq
		* @short Generation Signal.
		*/
	class GenSig
	{
	public:
		unsigned int Gen;
		unsigned int BestGen;
		cChromo **Pop;
		cChromo *Best;
		
		GenSig(unsigned int gen,unsigned int bestgen,cChromo **pop,cChromo *best)
			: Gen(gen), BestGen(bestgen), Pop(pop), Best(best) {}	
	};

	/** Construct the receiver.*/
	RGASignalsReceiver(void) {}

	/** Respond to a interact signal.*/
	virtual void receiveInteractSig(InteractSig*) {}

	/** Respond to a run signal.*/
	virtual void receiveRunSig(RunSig*) {}

	/** Respond to a stop signal.*/
	virtual void receiveStopSig(StopSig*) {}

	/** Respond to a stop signal.*/
	virtual void receiveBestSig(BestSig*) {}

	/** Respond to a gen signal.*/
	virtual void receiveGenSig(GenSig*) {}

	/** Compare function. */
	int Compare(RGASignalsReceiver*) { return(1); }
};


}//------- End of namespace RGA ---------------------------------------------
#endif
