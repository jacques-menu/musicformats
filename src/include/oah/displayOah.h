/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___displayOah___
#define ___displayOah___

#include <set>

#include "exports.h"

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP displayOahGroup : public oahGroup
{
  public:

    static SMARTP<displayOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeDisplayOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          displayOahGroup ();

    virtual               ~displayOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // display
    void                  setDisplayOptionsAndArguments ()
                              { fDisplayOptionsAndArguments = true; }
    Bool                  getDisplayOptionsAndArguments () const
                              { return fDisplayOptionsAndArguments; }

    void                  setDisplayEarlyOptionsValues ()
                              { fDisplayEarlyOptionsValues = true; }
    Bool                  getDisplayEarlyOptionsValues () const
                              { return fDisplayEarlyOptionsValues; }

    void                  setDisplayOptionsValues ()
                              { fDisplayOptionsValues = true; }
    Bool                  getDisplayOptionsValues () const
                              { return fDisplayOptionsValues; }

    void                  setDisplayOptionsValuesAll ()
                              { fDisplayOptionsValuesAll = true; }
    Bool                  getDisplayOptionsValuesAll () const
                              { return fDisplayOptionsValuesAll; }

    void                  setDisplayOahHandler ()
                              { fDisplayOahHandler = true; }
    Bool                  getDisplayOahHandler () const
                              { return fDisplayOahHandler; }

    void                  setDisplayOahHandlerSummary ()
                              { fDisplayOahHandlerSummary = true; }
    Bool                  getDisplayOahHandlerSummary () const
                              { return fDisplayOahHandlerSummary; }

    void                  setDisplayOahHandlerEssentials ()
                              { fDisplayOahHandlerEssentials = true; }
    Bool                  getDisplayOahHandlerEssentials () const
                              { return fDisplayOahHandlerEssentials; }

    // CPU usage
    // --------------------------------------

    Bool                  getDisplayCPUusage () const
                              { return fDisplayCPUusage;  }

    S_oahBooleanAtom      getDisplayCPUusageAtom () const
                              { return fDisplayCPUusageAtom;  }

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

    void                  initializeDisplayOahOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printDisplayOahHelp ();

    void                  printDisplayOptionsValues (int valueFieldWidth);

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private work fields
    // --------------------------------------

  private:

    // private fields
    // --------------------------------------

    // display
    Bool                  fDisplayOptionsAndArguments;
    Bool                  fDisplayEarlyOptionsValues;

    Bool                  fDisplayOptionsValues;
    Bool                  fDisplayOptionsValuesAll;

    Bool                  fDisplayOahHandler;
    Bool                  fDisplayOahHandlerSummary;
    Bool                  fDisplayOahHandlerEssentials;

    // CPU usage
    // --------------------------------------

    Bool                  fDisplayCPUusage;
    S_oahBooleanAtom      fDisplayCPUusageAtom;
};
typedef SMARTP<displayOahGroup> S_displayOahGroup;
EXP ostream& operator<< (ostream& os, const S_displayOahGroup& elt);

EXP extern S_displayOahGroup gGlobalDisplayOahGroup;

//______________________________________________________________________________
EXP S_displayOahGroup createGlobalDisplayOahGroup ();


}


#endif
