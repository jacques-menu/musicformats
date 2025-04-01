/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mnx2mnxsrInterpreterRegularHandler___
#define ___mnx2mnxsrInterpreterRegularHandler___

#include "oahRegularHandlers.h"

#include "mnx2mnxsrInterpreterInsiderHandler.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP mnx2mnxsrInterpreterRegularHandler : public oahRegularHandler
/*
  An mnx2mnxsr regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2gmnInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mnx2mnxsrInterpreterRegularHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            S_mnx2mnxsrInterpreterInsiderHandler
                                               insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mnx2mnxsrInterpreterRegularHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            S_mnx2mnxsrInterpreterInsiderHandler
                                               insiderOahHandler);

    virtual               ~mnx2mnxsrInterpreterRegularHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  checkOptionsAndArguments () const override;

    // quiet mode
    void                  enforceHandlerQuietness () override
                              {
                                fInsiderHandler->
                                  checkHandlerOptionsConsistency ();
                              }

    // consistency check
    void                  checkHandlerOptionsConsistency () override
                              {
                                fInsiderHandler->
                                  checkHandlerOptionsConsistency ();
                              }

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

  private:

    // private services
    // ------------------------------------------------------

    // regular handler groups creation

    void                  createRegularHandlerGroups () override;

    // the groups themselves

    void                  createInformationsRegularGroup ();

    void                  createMnx2mnxsrRegularUserGroup ();
    void                  createMnx2mnxsrRegularMaintainanceGroup ();

    void                  createOahRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<mnx2mnxsrInterpreterRegularHandler> S_mnx2mnxsrInterpreterRegularHandler;
EXP std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrInterpreterRegularHandler& elt);


}


#endif // ___mnx2mnxsrInterpreterRegularHandler___
