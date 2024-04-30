/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___stringMatcherInterpreterRegularHandler___
#define ___stringMatcherInterpreterRegularHandler___

#include "oahRegularHandlers.h"

#include "stringMatcherInterpreterInsiderHandler.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP stringMatcherInterpreterRegularHandler : public oahRegularHandler
/*
  An stringMatcher regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2gmnInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringMatcherInterpreterRegularHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            S_stringMatcherInterpreterInsiderHandler
                                               insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          stringMatcherInterpreterRegularHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            S_stringMatcherInterpreterInsiderHandler
                                               insiderOahHandler);

    virtual               ~stringMatcherInterpreterRegularHandler ();

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

    void                  createstringMatcherRegularUserGroup ();
    void                  createstringMatcherRegularMaintainanceGroup ();

    void                  createOahRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<stringMatcherInterpreterRegularHandler> S_stringMatcherInterpreterRegularHandler;
EXP std::ostream& operator<< (std::ostream& os, const S_stringMatcherInterpreterRegularHandler& elt);


}


#endif // ___stringMatcherInterpreterRegularHandler___
