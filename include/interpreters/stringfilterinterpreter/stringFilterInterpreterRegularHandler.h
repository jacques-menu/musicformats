/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___stringFilterInterpreterRegularHandler___
#define ___stringFilterInterpreterRegularHandler___

#include "oahRegularHandlers.h"

#include "stringFilterInterpreterInsiderHandler.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP stringFilterInterpreterRegularHandler : public oahRegularHandler
/*
  An stringFilter regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2gmnInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringFilterInterpreterRegularHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            S_stringFilterInterpreterInsiderHandler
                                               insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          stringFilterInterpreterRegularHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            S_stringFilterInterpreterInsiderHandler
                                               insiderOahHandler);

    virtual               ~stringFilterInterpreterRegularHandler ();

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

    void                  createstringFilterRegularUserGroup ();
    void                  createstringFilterRegularMaintainanceGroup ();

    void                  createOahRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<stringFilterInterpreterRegularHandler> S_stringFilterInterpreterRegularHandler;
EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterInterpreterRegularHandler& elt);


}


#endif // ___stringFilterInterpreterRegularHandler___
