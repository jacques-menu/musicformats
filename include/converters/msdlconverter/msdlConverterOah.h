/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlConverterOah___
#define ___msdlConverterOah___

#include "msrGenerationBasicTypes.h"

#include "brailleGenerationOah.h"
#include "brailleGeneration.h"


namespace MusicFormats
{

//______________________________________________________________________________
#define MSDR_STANDARD_INPUT_NAME ""
//#define MSDR_STANDARD_INPUT_NAME "-"

//______________________________________________________________________________
class EXP msdlConverterOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlConverterOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdlConverterOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlConverterOahGroup ();

    virtual               ~msdlConverterOahGroup ();

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

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
// JMI    void                  initializeMsdlConvertertracingOah ();
#endif

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsdlConverterOahHelp ();

    void                  printMsdlConverterOahValues (
                            int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlConverterOahGroup> S_msdlConverterOahGroup;
EXP ostream& operator << (ostream& os, const S_msdlConverterOahGroup& elt);

EXP extern S_msdlConverterOahGroup gGlobalMsdlConverterOahGroup;

//______________________________________________________________________________
EXP S_msdlConverterOahGroup createGlobalMsdlConverterOahGroup ();


}


#endif
