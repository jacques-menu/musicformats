/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahRegularHandlers___
#define ___oahRegularHandlers___

#include "oahBasicTypes.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP oahRegularHandler : public oahHandler
/*
  A regular OAH handler relies on the existence of so-called 'insider' handler,
  that contains all the options values gathered from the user,
  grouped according to the internal representations and passes used.

  The variables containing the values of the options chosen by the user
  are actually held by the insider handler.
*/
{
/* this class is purely vir.htual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahRegularHandler> create (
                            std::string       serviceName,
                            const std::string&     handlerHeader,
                            S_oahHandler      insiderOahHandler);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahRegularHandler (
                            std::string       serviceName,
                            const std::string&     handlerHeader,
                            S_oahHandler      insiderOahHandler);

    virtual               ~oahRegularHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    void                  initializeOahRegularHandler ();

    void                  initializeHandlerMultiComponent () override
                            {
                              fHandlerMultiComponent =
                                fInsiderHandler->
                                  getHandlerMultiComponent ();
                            }

  public:

    // public services
    // ------------------------------------------------------

    std::string           handlerServiceAboutInformation () const override
                              {
                                return
                                  fInsiderHandler->
                                    handlerServiceAboutInformation ();
                              }

    void                  appendGroupToRegulalHandler (
                            S_oahGroup group);
    void                  prependGroupToRegularHandler (
                            S_oahGroup group);

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  protected:

    // protected services
    // ------------------------------------------------------

    // regular handler groups creation

    virtual void          createRegularHandlerGroups () = 0;

    void                  registerAtomInRegularSubgroup (
                            const std::string& atomName,
                            S_oahSubGroup subGroup);

  protected:

    // protected fields
    // ------------------------------------------------------

    S_oahHandler          fInsiderHandler;
};
typedef SMARTP<oahRegularHandler> S_oahRegularHandler;
EXP std::ostream& operator << (std::ostream& os, const S_oahRegularHandler& elt);


}


#endif
