/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
                            const string&     serviceName,
                            const string&     handlerHeader,
                            S_ischemeInterpreterInsiderHandler
                                              insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeInterpreterRegularHandler (
                            const string&     serviceName,
                            const string&     handlerHeader,
                            S_ischemeInterpreterInsiderHandler
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

    void                  print (ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    // regular handler groups creation

    void                  createRegularHandlerGroups () override;

    // the groups themselves

    void                  createInformationsRegularGroup ();

    void                  createIschemeRegularUserGroup ();
    void                  createIschemeRegularMaintainanceGroup ();

    void                  createOahRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<ischemeInterpreterRegularHandler> S_ischemeInterpreterRegularHandler;
EXP ostream& operator<< (ostream& os, const S_ischemeInterpreterRegularHandler& elt);


}


#endif
