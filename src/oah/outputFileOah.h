/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___outputFileOahGroup___
#define ___outputFileOahGroup___

#include <set>

#include "exports.h"

#include "oahBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP outputFileOahGroup : public oahGroup
{
  public:

    static SMARTP<outputFileOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeOutputFileOah ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    outputFileOahGroup ();

    virtual               ~outputFileOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // output file
    // --------------------------------------

    void                  setAutoOutputFileName ()
                              { fAutoOutputFileName = true; }
    Bool                  getAutoOutputFileName () const
                              { return fAutoOutputFileName; }

    const S_oahBooleanAtom&
                          getAutoOutputFileNameAtom () const
                              { return fAutoOutputFileNameAtom; }

    void                  setOutputFileName (std::string value)
                              { fOutputFileName = value; }
    std::string           getOutputFileName () const
                              { return fOutputFileName; }
    S_oahStringAtom       getOutputFileNameStringAtom () const
                              { return fOutputFileNameStringAtom; }

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

    void                  initializeOutputFileNameOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printOutputFileOahHelp ();

    void                  printOutputFileOahValues (int fieldWidth);

  private:

    // output file
    // these private variables are the ones accessible through the atoms
    // we store the atoms for the needs of fetchOutputFileNameFromTheOptions()
    // --------------------------------------

    S_oahBooleanAtom      fAutoOutputFileNameAtom;
    Bool                  fAutoOutputFileName;

    std::string           fOutputFileName;
    S_oahStringAtom       fOutputFileNameStringAtom;
};
typedef SMARTP<outputFileOahGroup> S_outputFileOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_outputFileOahGroup& elt);

EXP extern S_outputFileOahGroup gGlobalOutputFileOahGroup;

//______________________________________________________________________________
EXP S_outputFileOahGroup createGlobalOutputFileOahGroup ();


}


#endif
