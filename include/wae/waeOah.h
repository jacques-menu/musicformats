/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___waeOahGroup___
#define ___waeOahGroup___

#include <set>

#include "exports.h"

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP waeOahGroup : public oahGroup
{
  public:

    static SMARTP<waeOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeWaeOah ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          waeOahGroup ();

    virtual               ~waeOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // warning and error handling
    // --------------------------------------

    Bool                  getDontShowErrors () const
                              { return fDontShowErrors;  }

    Bool                  getDontQuitOnErrors () const
                              { return fDontQuitOnErrors;  }

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeWaeWarningAndErrorsOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printWaeOahHelp ();

    void                  printWaeOahValues (int fieldWidth);

  private:

    // warning and error handling
    // --------------------------------------

    Bool                  fQuiet;

    Bool                  fDontShowErrors;
    Bool                  fDontQuitOnErrors;
};
typedef SMARTP<waeOahGroup> S_waeOahGroup;
EXP ostream& operator << (ostream& os, const S_waeOahGroup& elt);

EXP extern S_waeOahGroup gGlobalWaeOahGroup;

//______________________________________________________________________________
EXP S_waeOahGroup createGlobalWaeOahGroup ();


}


#endif
