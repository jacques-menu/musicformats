/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfFindInterpreterRegularHandler___
#define ___mfFindInterpreterRegularHandler___

#include "oahRegularHandlers.h"

#include "mfFindInterpreterInsiderHandler.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP mfFindInterpreterRegularHandler : public oahRegularHandler
/*
  An mfFind regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2gmnInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfFindInterpreterRegularHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            S_mfFindInterpreterInsiderHandler
                                               insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFindInterpreterRegularHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            S_mfFindInterpreterInsiderHandler
                                               insiderOahHandler);

    virtual               ~mfFindInterpreterRegularHandler ();

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

    void                  createmfFindRegularUserGroup ();
    void                  createmfFindRegularMaintainanceGroup ();

    void                  createOahRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<mfFindInterpreterRegularHandler> S_mfFindInterpreterRegularHandler;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFindInterpreterRegularHandler& elt);


}


#endif // ___mfFindInterpreterRegularHandler___
