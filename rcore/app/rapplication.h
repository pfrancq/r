/*

	R Project Library

	RApplication.h

	Generic Application - Header.

	Copyright 2006-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 Universit√© Libre de Bruxelles (ULB).

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

*/



//-----------------------------------------------------------------------------
#ifndef RApplication_H
#define RApplication_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rcontainer.h>
#include <rconfig.h>
#include <rparam.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RApplication class provides a representation for a generic application.
* It defines a configuration file (stored in "~/.r/config/app") that can
* eventually have a template (stored in "/usr/r/config/app"). It manages also
* arguments on the form: "--argwithoutvalue" or "--argwithvalue value".
*
* In practice, the important methods are CreateConfig(void) to create some
* configuration parameters, Init(void) to initialize the application (for
* example to allocate some structures) and Run(void) that contains the code
* to execute. Here is an example:
* @code
* class MyApplication : public RApplication
* {
* public:
*   MyApplication(int argc, char** argv)
*   	: RApplication("Test",argc,argv)
*   {
*   }
*   virtual void CreateConfig(void)
*   {
*   	Config.InsertParam(new RParamValue("Message","Hello world !"));
*   }
*   virtual void Run(void)
*   {
*   	cout<<Config.FindParam<RParamValue>("Message")->Get()<<endl;
*   }
* };
*
* //------------------------------------------------------------------------------
* int main(int argc, char *argv[])
* {
*	MyApplication App(argc,argv);
*	App.Execute();
*	return(EXIT_SUCCESS);
* }
* @endcode
* @short Generic Application.
*/
class RApplication
{
protected:

	/**
	* Name of the application.
	*/
	RString Name;

	/**
	* Name of executable file.
	*/
	RString File;

	/**
	* Arguments of the application.
	*/
	RContainer<RString,true,false> Args;

	/**
	* Parameters of the application.
	*/
	RContainer<RParamValue,true,false> Params;

	/**
	 * Local directory of the configuration.
	 */
	RString LocalConfigDir;

	/**
	* Configuration
	*/
	RConfig Config;

private:

	/**
	* Application initialized?
	*/
	bool HasInitApp;

public:

	/**
	 * Construct an application. If both parameters 'localconfig' and
	 * 'globalconfig' are null (or not specified), the default directories are
	 * chosen.
	 * @param name           Name of the application.
	 * @param argc           Number of arguments of the program (received from
	 *                       main).
	 * @param argv           Arguments of the program (received from main).
	 * @param localconfig    Local configuration.
	 * @param globalconfig   Local configuration.
	 */
	RApplication(const RString& name,int argc, char** argv,const RString& localconfig=RString::Null,const RString& globalconfig=RString::Null);

	/**
	 * Construct an application. If both parameters 'localconfig' and
	 * 'globalconfig' are null (or not specified), the default directories are
	 * chosen.
	 *
	 * This constructor doesn't analyzed the parameter passed to the program
	 * throught the main function. The method RApplication::ParseArguments should
	 * be manually called if parameters must be parsed.
	 * @param name           Name of the application.
	 * @param localconfig    Local configuration.
	 * @param globalconfig   Local configuration.
	 */
	RApplication(const RString& name,const RString& localconfig=RString::Null,const RString& globalconfig=RString::Null);

	/**
	 * Parse the arguments.
	 * @param argc           Number of arguments of the program (received from
	 *                       main).
	 * @param argv           Arguments of the program (received from main).
	 */
	void ParseArguments(int argc, char** argv);

	/**
	* Get the name of the application.
	*/
	RString GetName(void) const;

	/**
	* Get the name of the file representing the executable.
	*/
	RString GetApplicationFile(void) const;

	/**
	* Look if the application was initialized (call 'Init()' method).
	*/
	bool IsInit(void) const {return(HasInitApp);}

	/**
	 * @return the directory where the local configuration is stored.
	 */
	RString GetLocalConfigDir(void) const {return(LocalConfigDir);}

	/**
	* Get a pointer over the configuration.
	*/
	RConfig* GetConfig(void) {return(&Config);}

	/**
	 * @return a cursor on the arguments.
	 */
	RCursor<RString> GetArgs(void) const;

	/**
	 * Get the value of a parameter passed as argument to program. Suppose the
	 * following program launch:
	 * @code
	 * ./testr --o ~/test.txt --d
	 * @endcode
	 * These parameters can be treated with the code:
	 * @code
	 * void MyApplication::TestParams(void)
	 * {
	 *    RString val;
	 *    if(GetParamValue("o",val))
	 *    {
	 *       // Create the file with name is given by the variable 'val'.
	 *    }
	 *    if(GetParamValue("d",val))
	 *    {
	 *       // The value of 'val' is RString::Null.
	 *    }
	 *    if(GetParamValue("z",val))
	 *    {
	 *       // Nothing happens since '--z' was not passed to the program.
	 *    }
	 * }
	 * @endcode
	 * @param name           Name of the parameter.
	 * @param value          The value of the parameter. It can be RString::Null
	 *                       if no value is associated.
	 * @return true if the parameter was found.
	 */
	bool GetParamValue(const RString& name,RString& value) const;

	/**
	 * @return a cursor on the parameters.
	 */
	RCursor<RParamValue> GetParams(void) const;

protected:

	/**
	* Create the configuration structure. New parameters can be added by
	* defining a new method.
	*/
	virtual void CreateConfig(void);

	/**
	* Initialize the application. By default, it create the configuration
	* structure and load the configuration file.
	*/
	virtual void Init(void);

private:

	/**
	* Run the application.
	*/
	virtual void Run(void);

public:

	/**
	 * Execute the application. In practice, it initializes the application if
	 * necessary, and then calls the Run() method.
	 */
	void Execute(void);

	/**
	* Destructor of the application. By default, the configuration is saved.
	*/
	virtual ~RApplication(void);
};


//-----------------------------------------------------------------------------
/**
* Pointer to the current application. Only one application can be run at once.
*/
extern RApplication* App;


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
