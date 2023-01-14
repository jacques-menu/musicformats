/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2lpsrOah___
#define ___msr2lpsrOah___


#include "oahBasicTypes.h"

#include "lpsrEnumTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msr2lpsrScoreOutputKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2lpsrScoreOutputKindAtom> create (
                            const std::string&   longName,
                            const std::string&   shortName,
                            const std::string&   description,
                            const std::string&   valueSpecification,
                            const std::string&   variableName,
                            lpsrScoreOutputKind& lpsrScoreOutputKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2lpsrScoreOutputKindAtom (
                            const std::string&   longName,
                            const std::string&   shortName,
                            const std::string&   description,
                            const std::string&   valueSpecification,
                            const std::string&   variableName,
                            lpsrScoreOutputKind& lpsrScoreOutputKindVariable);

    virtual               ~msr2lpsrScoreOutputKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLpsrScoreOutputKindVariable (
                            lpsrScoreOutputKind value)
                              { fLpsrScoreOutputKindVariable = value; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrScoreOutputKind&  fLpsrScoreOutputKindVariable;
};
typedef SMARTP<msr2lpsrScoreOutputKindAtom> S_msr2lpsrScoreOutputKindAtom;
EXP std::ostream& operator << (std::ostream& os, const S_msr2lpsrScoreOutputKindAtom& elt);

//______________________________________________________________________________
class EXP msr2lpsrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2lpsrOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2lpsrOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2lpsrOahGroup ();

    virtual               ~msr2lpsrOahGroup ();

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

    // score output kind
    lpsrScoreOutputKind   getScoreOutputKind () const
                              { return fScoreOutputKind; }

    // repeats
    Bool                  getCreateImplicitInitialRepeatBarLine () const
                              { return fCreateImplicitInitialRepeatBarLine; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef MF_TRACING_IS_ENABLED
    void                  initializMsr2lpsrTraceOptions ();
#endif

    void                  initializeScoreOutputOptions ();

    void                  initializeRepeatsOptions ();

    void                  initializeNotesOptions ();

    void                  initializeLyricsOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsr2lpsrOahHelp ();

    void                  printMsr2lpsrOahValues (int valueFieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // score output kind
    lpsrScoreOutputKind   fScoreOutputKind;

    // repeats
    Bool                  fCreateImplicitInitialRepeatBarLine;
};
typedef SMARTP<msr2lpsrOahGroup> S_msr2lpsrOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msr2lpsrOahGroup& elt);

EXP extern S_msr2lpsrOahGroup gGlobalMsr2lpsrOahGroup;

//______________________________________________________________________________
EXP S_msr2lpsrOahGroup createGlobalMsr2lpsrOahGroup ();


}


#endif
