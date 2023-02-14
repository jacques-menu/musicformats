/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___ischemeInterpreterRegularHandler___
#define ___ischemeInterpreterRegularHandler___

#include "oahRegularHandlers.h"

#include "ischemeInterpreterInsiderHandler.h"

namespace MusicFormats
{

//_______________________________________________________________________________
class EXP ischemeInterpreterRegularHandler : public oahRegularHandler
/*
  An ischeme regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2gmnInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<ischemeInterpreterRegularHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            const S_ischemeInterpreterInsiderHandler&
                                               insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeInterpreterRegularHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            const S_ischemeInterpreterInsiderHandler&
                                               insiderOahHandler);

    virtual               ~ischemeInterpreterRegularHandler ();

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

    void                  createDisplayRegularGroup ();

    void                  createIschemeRegularUserGroup ();
    void                  createIschemeRegularMaintainanceGroup ();

    void                  createOahRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<ischemeInterpreterRegularHandler> S_ischemeInterpreterRegularHandler;
EXP std::ostream& operator << (std::ostream& os, const S_ischemeInterpreterRegularHandler& elt);


}


#endif // ___ischemeInterpreterRegularHandler___
