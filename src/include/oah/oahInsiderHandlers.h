/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahInsiderHandlers___
#define ___oahInsiderHandlers___

#include "msrBasicTypes.h"

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP oahInsiderHandler : public oahHandler
/*
  An insider OAH handler proposes a full set of options,
  displayed in the group-subgroup-atom hierarchy,
  some of which are selected by so-called 'regular' handlers,
  which display them in a simpler group-atom hierarchy.

  The idea that options in insider handlers are grouped
  by internal epresentations and passes used,
  when they are grouped by user topics
  such as tuplets or grace notes in regular handlers.

  The variables containing the values of the options chosen by the user
  are actually held by the insider handler.
*/
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahInsiderHandler> create (
                            const string&           serviceName,
                            const string&           handlerHeader,
                            const string&           handlerDescription,
                            const string&           handlerUsage);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahInsiderHandler (
                            const string&           serviceName,
                            const string&           handlerHeader,
                            const string&           handlerDescription,
                            const string&           handlerUsage);

    virtual               ~oahInsiderHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  private:

    // private initialization
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  protected:

    // protected services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

  private:

    // private fields
    // ------------------------------------------------------

};
typedef SMARTP<oahInsiderHandler> S_oahInsiderHandler;
EXP ostream& operator<< (ostream& os, const S_oahInsiderHandler& elt);


}


#endif
