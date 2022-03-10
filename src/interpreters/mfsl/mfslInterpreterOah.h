/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfslInterpreterOah___
#define ___mfslInterpreterOah___

#include "msr.h"

#include "msrGenerationBasicTypes.h"

#include "brailleGenerationOah.h"
#include "brailleGeneration.h"


namespace MusicFormats
{

//______________________________________________________________________________
#define MSDR_STANDARD_INPUT_NAME ""
//#define MSDR_STANDARD_INPUT_NAME "-"

//______________________________________________________________________________
class EXP mfslInterpreterOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslInterpreterOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMfslInterpreterOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslInterpreterOahGroup ();

    virtual               ~mfslInterpreterOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    void                  setVerboseMode ()
                              { fVerboseMode = true; }

    Bool                  getVerboseMode () const
                              { return fVerboseMode; }

    void                  setLexicalAnalysisOnly ()
                              { fLexicalAnalysisOnly = true; }

    Bool                  getLexicalAnalysisOnly () const
                              { return fLexicalAnalysisOnly; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
// JMI    void                  initializemfslInterpretertracingOah ();
#endif

    void                  initializeMfslOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMfslInterpreterOahHelp ();

    void                  printMfslInterpreterOahValues (
                            int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    Bool                  fVerboseMode;

    Bool                  fLexicalAnalysisOnly;
};
typedef SMARTP<mfslInterpreterOahGroup> S_mfslInterpreterOahGroup;
EXP ostream& operator<< (ostream& os, const S_mfslInterpreterOahGroup& elt);

EXP extern S_mfslInterpreterOahGroup gGlobalMfslInterpreterOahGroup;

//______________________________________________________________________________
EXP S_mfslInterpreterOahGroup createGlobalMfslInterpreterOahGroup ();


}


#endif
