/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsr2lilypondOah___
#define ___lpsr2lilypondOah___

#include "lpsrBasicTypes.h"

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP lilypondScoreOutputKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondScoreOutputKindAtom> create (
                            const string&        shortName,
                            const string&        longName,
                            const string&        description,
                            const string&        valueSpecification,
                            const string&        variableName,
                            lpsrScoreOutputKind& lilypondScoreOutputKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondScoreOutputKindAtom (
                            const string&        shortName,
                            const string&        longName,
                            const string&        description,
                            const string&        valueSpecification,
                            const string&        variableName,
                            lpsrScoreOutputKind& lilypondScoreOutputKindVariable);

    virtual               ~lilypondScoreOutputKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setScoreOutputKindVariableValue (
                            lpsrScoreOutputKind value)
                              {  fLpsrScoreOutputKindVariable = value; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrScoreOutputKind&  fLpsrScoreOutputKindVariable;
};
typedef SMARTP<lilypondScoreOutputKindAtom> S_lilypondScoreOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondScoreOutputKindAtom& elt);

//______________________________________________________________________________
class EXP lilypondTransposePartNameAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondTransposePartNameAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            map<string, S_msrSemiTonesPitchAndOctave>&
                                          stringToMsrSemiTonesPitchAndOctaveMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondTransposePartNameAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            map<string, S_msrSemiTonesPitchAndOctave>&
                                          stringToMsrSemiTonesPitchAndOctaveMapVariable);

    virtual               ~lilypondTransposePartNameAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringToMsrSemiTonesPitchAndOctaveMapVariable (
                            const string& partName,
                            S_msrSemiTonesPitchAndOctave
                                          semiTonesPitchAndOctave)
                              {
                                fStringToMsrSemiTonesPitchAndOctaveMapVariable [
                                  partName
                                ] =
                                  semiTonesPitchAndOctave;
                              }

    const map<string, S_msrSemiTonesPitchAndOctave>&
                          getStringToMsrSemiTonesPitchAndOctaveMapVariable () const
                              {
                                return
                                  fStringToMsrSemiTonesPitchAndOctaveMapVariable;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, S_msrSemiTonesPitchAndOctave>&
                          fStringToMsrSemiTonesPitchAndOctaveMapVariable;
};
typedef SMARTP<lilypondTransposePartNameAtom> S_lilypondTransposePartNameAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondTransposePartNameAtom& elt);

//______________________________________________________________________________
class EXP lilypondTransposePartIDAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondTransposePartIDAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            map<string, S_msrSemiTonesPitchAndOctave>&
                                          stringToMsrSemiTonesPitchAndOctaveMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondTransposePartIDAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            map<string, S_msrSemiTonesPitchAndOctave>&
                                          stringToMsrSemiTonesPitchAndOctaveMapVariable);

    virtual               ~lilypondTransposePartIDAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringToMsrSemiTonesPitchAndOctaveMapVariable (
                            const string& partName,
                            S_msrSemiTonesPitchAndOctave
                                          semiTonesPitchAndOctave)
                              {
                                fStringToMsrSemiTonesPitchAndOctaveMapVariable [
                                  partName
                                ] =
                                  semiTonesPitchAndOctave;
                              }

    const map<string, S_msrSemiTonesPitchAndOctave>&
                          getStringToMsrSemiTonesPitchAndOctaveMapVariable () const
                              {
                                return
                                  fStringToMsrSemiTonesPitchAndOctaveMapVariable;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, S_msrSemiTonesPitchAndOctave>&
                          fStringToMsrSemiTonesPitchAndOctaveMapVariable;
};
typedef SMARTP<lilypondTransposePartIDAtom> S_lilypondTransposePartIDAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondTransposePartIDAtom& elt);

//______________________________________________________________________________
class EXP msrOctaveEntryVariable : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrOctaveEntryVariable> create (
                            const string&      variableName,
                            msrOctaveEntryKind octaveEntryKind);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrOctaveEntryVariable (
                            const string&      variableName,
                            msrOctaveEntryKind octaveEntryKind);

    virtual               ~msrOctaveEntryVariable ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getVariableName () const
                              { return fVariableName; }

    void                  setOctaveEntryKind (msrOctaveEntryKind value)
                              { fOctaveEntryKind = value; }

    msrOctaveEntryKind    getOctaveEntryKind () const
                              { return fOctaveEntryKind; }

  public:

    // public services
    // ------------------------------------------------------

    void                  setRelativeOctaveEntryVariableValue (
                            msrOctaveEntryKind value);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private fields
    // ------------------------------------------------------

    string                fVariableName;
    msrOctaveEntryKind    fOctaveEntryKind;
};
typedef SMARTP<msrOctaveEntryVariable> S_msrOctaveEntryVariable;
EXP ostream& operator<< (ostream& os, const msrOctaveEntryVariable& elt);

//______________________________________________________________________________
class EXP lilypondAbsoluteOctaveEntryAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondAbsoluteOctaveEntryAtom> create (
                            const string&           longName,
                            const string&           shortName,
                            const string&           description,
                            const string&           variableName,
                            msrOctaveEntryVariable& octaveEntryKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondAbsoluteOctaveEntryAtom (
                            const string&           longName,
                            const string&           shortName,
                            const string&           description,
                            const string&           variableName,
                            msrOctaveEntryVariable& octaveEntryKindVariable);

    virtual               ~lilypondAbsoluteOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (ostream& os) override;

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os); // JMI CAFE override ??? remove method ???
                            // returns an error

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrOctaveEntryVariable&
                          fOctaveEntryKindVariable;
};
typedef SMARTP<lilypondAbsoluteOctaveEntryAtom> S_lilypondAbsoluteOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondAbsoluteOctaveEntryAtom& elt);

//______________________________________________________________________________
class EXP lilypondRelativeOctaveEntryAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondRelativeOctaveEntryAtom> create (
                            const string&   shortName,
                            const string&   longName,
                            const string&   description,
                            const string&   valueSpecification,
                            const string&   variableName,
                            msrOctaveEntryVariable&
                                            octaveEntryKindVariable,
                            S_msrSemiTonesPitchAndOctave&
                                            lilypondRelativeOctaveEntryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondRelativeOctaveEntryAtom (
                            const string&   shortName,
                            const string&   longName,
                            const string&   description,
                            const string&   valueSpecification,
                            const string&   variableName,
                            msrOctaveEntryVariable&
                                            octaveEntryKindVariable,
                            S_msrSemiTonesPitchAndOctave&
                                            lilypondRelativeOctaveEntryVariable);

    virtual               ~lilypondRelativeOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setRelativeOctaveEntryVariableValue (
                            S_msrSemiTonesPitchAndOctave value)
                              {
                                fSemiTonesPitchAndOctaveVariable = value;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrOctaveEntryVariable&
                          fOctaveEntryKindVariable;

    S_msrSemiTonesPitchAndOctave&
                          fSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lilypondRelativeOctaveEntryAtom> S_lilypondRelativeOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondRelativeOctaveEntryAtom& elt);

//______________________________________________________________________________
class EXP lilypondFixedOctaveEntryAtom : public oahAtomExpectingAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondFixedOctaveEntryAtom> create (
                            const string&       longName,
                            const string&       shortName,
                            const string&       description,
                            const string&       valueSpecification,
                            const string&       variableName,
                            msrOctaveEntryVariable&
                                                octaveEntryKindVariable,
                            S_msrSemiTonesPitchAndOctave&
                                                lilypondFixedOctaveEntryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondFixedOctaveEntryAtom (
                            const string&       longName,
                            const string&       shortName,
                            const string&       description,
                            const string&       valueSpecification,
                            const string&       variableName,
                            msrOctaveEntryVariable&
                                                octaveEntryKindVariable,
                            S_msrSemiTonesPitchAndOctave&
                                                lilypondFixedOctaveEntryVariable);

    virtual               ~lilypondFixedOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setFixedOctaveEntryVariableValue (
                            S_msrSemiTonesPitchAndOctave value)
                              { fSemiTonesPitchAndOctaveVariable = value; }

    S_msrSemiTonesPitchAndOctave&
                          getFixedOctaveEntryVariableValue () const
                              { return fSemiTonesPitchAndOctaveVariable; }
  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrOctaveEntryVariable&
                          fOctaveEntryKindVariable;

    S_msrSemiTonesPitchAndOctave&
                          fSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lilypondFixedOctaveEntryAtom> S_lilypondFixedOctaveEntryAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondFixedOctaveEntryAtom& elt);

//______________________________________________________________________________
class EXP lilypondAccidentalStyleKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondAccidentalStyleKindAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            lpsrAccidentalStyleKind&
                                          lilypondAccidentalStyleKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondAccidentalStyleKindAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            lpsrAccidentalStyleKind&
                                          lilypondAccidentalStyleKindVariable);

    virtual               ~lilypondAccidentalStyleKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLpsrAccidentalStyleKindVariable (
                            lpsrAccidentalStyleKind value)
                              {
                                fLpsrAccidentalStyleKindVariable = value;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrAccidentalStyleKind&
                          fLpsrAccidentalStyleKindVariable;
};
typedef SMARTP<lilypondAccidentalStyleKindAtom> S_lilypondAccidentalStyleKindAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondAccidentalStyleKindAtom& elt);

//______________________________________________________________________________
class EXP lilypondChordsDisplayAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondChordsDisplayAtom> create (
                            const string&         shortName,
                            const string&         longName,
                            const string&         description,
                            const string&         valueSpecification,
                            const string&         variableName,
                            list<pair<string, string> >&
                                                  lilypondChordsDisplayVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondChordsDisplayAtom (
                            const string&         shortName,
                            const string&         longName,
                            const string&         description,
                            const string&         valueSpecification,
                            const string&         variableName,
                            list<pair<string, string> >&
                                                  lilypondChordsDisplayVariable);

    virtual               ~lilypondChordsDisplayAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringsPairListVariable (
                            pair<string, string> value)
                              {
                                fStringsPairListVariable.push_back (value);
                              }

    const list<pair<string, string> >&
                          getStringsPairListVariable () const
                              { return fStringsPairListVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    list<pair<string, string> >&
                          fStringsPairListVariable;
};
typedef SMARTP<lilypondChordsDisplayAtom> S_lilypondChordsDisplayAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondChordsDisplayAtom& elt);

//______________________________________________________________________________
class EXP lilypondLyricsDurationsKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondLyricsDurationsKindAtom> create (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            lpsrLyricsDurationsKind&
                                              lilypondLyricsDurationsKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondLyricsDurationsKindAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            lpsrLyricsDurationsKind&
                                              lilypondLyricsDurationsKindVariable);

    virtual               ~lilypondLyricsDurationsKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLpsrLyricsDurationsKindVariable (
                            lpsrLyricsDurationsKind value)
                              {
                                fLpsrLyricsDurationsKindVariable = value;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrLyricsDurationsKind&
                          fLpsrLyricsDurationsKindVariable;
};
typedef SMARTP<lilypondLyricsDurationsKindAtom> S_lilypondLyricsDurationsKindAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondLyricsDurationsKindAtom& elt);

//______________________________________________________________________________
class EXP lilypondDynamicsTextSpannersStyleKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondDynamicsTextSpannersStyleKindAtom> create (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            lpsrDynamicsTextSpannersStyleKind&
                                              lilypondDynamicsTextSpannersStyleKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondDynamicsTextSpannersStyleKindAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            lpsrDynamicsTextSpannersStyleKind&
                                              lilypondDynamicsTextSpannersStyleKindVariable);

    virtual               ~lilypondDynamicsTextSpannersStyleKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLpsrdDynamicsTextSpannersStyleKindVariable (
                            lpsrDynamicsTextSpannersStyleKind value)
                              {
                                fLpsrdDynamicsTextSpannersStyleKindVariable = value;
                                fSetByAnOption = true;
                              }

    lpsrDynamicsTextSpannersStyleKind
                            getLpsrdDynamicsTextSpannersStyleKindVariable () const
                              {
                                return fLpsrdDynamicsTextSpannersStyleKindVariable;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrDynamicsTextSpannersStyleKind&
                          fLpsrdDynamicsTextSpannersStyleKindVariable;
};
typedef SMARTP<lilypondDynamicsTextSpannersStyleKindAtom>
  S_lilypondDynamicsTextSpannersStyleKindAtom;
EXP ostream& operator<<
  (ostream& os, const S_lilypondDynamicsTextSpannersStyleKindAtom& elt);

//______________________________________________________________________________
enum class lilypondScoreNotationKind {
  kNotationWestern,
  kNotationJianpu,
  kNotationABC // not used yet
};

extern string lilypondScoreNotationKindAsString (
  lilypondScoreNotationKind notationKind);

ostream& operator<< (ostream& os, lilypondScoreNotationKind& elt);

class EXP lpsr2lilypondOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsr2lilypondOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLilypondGenerationOahGroup ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsr2lilypondOahGroup ();

    virtual               ~lpsr2lilypondOahGroup ();

    // set and get
    // ------------------------------------------------------

 // JMI   Bool                  setAccidentalStyleKind (
  //                          lpsrAccidentalStyleKind accidentalStyleKind);

//  JMI   Bool                  setScoreOutputKind (
//                             const string& outputKind);

    // LilyPond version
    // --------------------------------------

    void                  setLilypondVersion (string version)
                              { fLilypondVersion = version; }
    string                getLilypondVersion () const
                              { return fLilypondVersion; }
    S_oahStringAtom       getLilypondVersionAtom () const
                              { return fLilypondVersionAtom; }

    string                getLilypondVersionDefaultValue () const
                              { return fLilypondVersionDefaultValue; }
    // score notation
    // --------------------------------------

    void                  setJianpu ()
                              { fJianpu = true; }
    Bool                  getJianpu () const
                              { return fJianpu; }

    void                  setLyLuaTexOutput ()
                              { fLyLuaTexOutput = true; }
    Bool                  getLyLuaTexOutput () const
                              { return fLyLuaTexOutput; }

    // global staff size
    // --------------------------------------

    float                 getGlobalStaffSize () const
                              { return fGlobalStaffSize; }
    S_oahFloatAtom        getGlobalStaffSizeAtom () const
                              { return fGlobalStaffSizeAtom; }

    float                 getStaffGlobalSizeDefaultValue () const
                              { return fStaffGlobalSizeDefaultValue; }

    Bool                  getNoSetGlobalStaffSize () const
                              { return fNoSetGlobalStaffSize; }
    S_oahBooleanAtom      getNoSetGlobalStaffSizeAtom () const
                              { return fNoSetGlobalStaffSizeAtom; }

    // languages
    // --------------------------------------

    Bool                  getUseLilypondDefaultLanguages () const
                              { return fUseLilypondDefaultLanguages; }
    S_oahBooleanAtom      getUseLilypondDefaultLanguagesAtom () const
                              { return fUseLilypondDefaultLanguagesAtom; }

    // identification
    // --------------------------------------

    // MusicXML informations
    string                getRights () const
                              { return fRights; }
    string                getComposer () const
                              { return fComposer; }
    string                getArranger () const
                              { return fArranger; }

    S_oahStringAtom       getPoetAtom () const
                              { return fPoetAtom; }
    string                getPoet () const
                              { return fPoet; }
    string                getLyricist () const
                              { return fLyricist; }
    string                getSoftware () const
                              { return fSoftware; }

    // Lilypond informations
    string                getDedication () const
                              { return fDedication; }

    string                getPiece () const
                              { return fPiece; }
    string                getOpus () const
                              { return fOpus; }
    Bool                  getOpusInTitle () const
                              { return fOpusInTitle; }

    string                getTitle () const
                              { return fTitle; }
    string                getSubTitle () const
                              { return fSubTitle; }
    string                getSubSubTitle () const
                              { return fSubSubTitle; }

    string                getHeaderInstrument () const
                              { return fHeaderInstrument; }

    string                getHeaderMeter () const
                              { return fHeaderMeter; }

    string                getTagline () const
                              { return fTagline; }

    string                getCopyright () const
                              { return fCopyright; }

    // book block
    // --------------------------------------

    Bool                  getNoBookBlock () const
                              { return fNoBookBlock; }
    S_oahBooleanAtom      getNoBookBlockAtom () const
                              { return fNoBookBlockAtom; }

    // header block
    // --------------------------------------

    Bool                  getNoHeaderBlock () const
                              { return fNoHeaderBlock; }
    S_oahBooleanAtom      getNoHeaderBlockAtom () const
                              { return fNoHeaderBlockAtom; }

    // layout block
    // --------------------------------------

    Bool                  getNoLayoutScoreContext () const
                              { return fNoLayoutScoreContext; }
    S_oahBooleanAtom      getNoLayoutScoreContextAtom () const
                              { return fNoLayoutScoreContextAtom; }

    Bool                  getNoLayoutVoiceContext () const
                              { return fNoLayoutVoiceContext; }
    S_oahBooleanAtom      getNoLayoutVoiceContextAtom () const
                              { return fNoLayoutVoiceContextAtom; }

    Bool                  getNoTopLevelLayoutBlock () const
                              { return fNoTopLevelLayoutBlock; }

    // paper block
    // --------------------------------------

    Bool                  getNoPaperBlock () const
                              { return fNoPaperBlock; }
    S_oahBooleanAtom      getNoPaperBlockAtom () const
                              { return fNoPaperBlockAtom; }

    // parts
    // --------------------------------------

    const map<string, S_msrSemiTonesPitchAndOctave>&
                          getPartNamesTranspositionMap () const
                              { return fPartNamesTranspositionMap; }
    const map<string, S_msrSemiTonesPitchAndOctave>&
                          getPartIDsTranspositionMap () const
                              { return fPartIDsTranspositionMap; }


    // voices
    // --------------------------------------

    void                  setAutoVoices ()
                              { fAutoVoices = true; }
    Bool                  getAutoVoices () const
                              { return fAutoVoices; }

    // names
    // --------------------------------------


    // engravers
    // --------------------------------------

    Bool                  getAmbitusEngraver () const
                              { return fAmbitusEngraver; }
    Bool                  getCustosEngraver () const
                              { return fCustosEngraver; }

    // clefs
    // --------------------------------------

    Bool                  getNoInitialTrebleClef () const
                              { return fNoInitialTrebleClef; }

    Bool                  getCommentClefChanges () const
                              { return fCommentClefChanges; }

    // keys
    // --------------------------------------

    Bool                  getNoInitialCMajorKey () const
                              { return fNoInitialCMajorKey; }

    // time signatures
    // --------------------------------------

    Bool                  getNoInitialCommonTime () const
                              { return fNoInitialCommonTimeSignature; }

    Bool                  getNumericalTime () const
                              { return fNumericalTimeSignature; }

    // notes
    // --------------------------------------

    const msrOctaveEntryVariable&
                          getOctaveEntryVariable () const
                              { return fOctaveEntryVariable; }

    // this is relevant only for relative octave entry kinds
    S_msrSemiTonesPitchAndOctave
                          getRelativeOctaveEntrySemiTonesPitchAndOctave () const
                              { return fRelativeOctaveEntrySemiTonesPitchAndOctave; }

    // this is relevant only for fixed octave entry kinds
    S_msrSemiTonesPitchAndOctave
                          getFixedOctaveEntrySemiTonesPitchAndOctave () const
                              { return fFixedOctaveEntrySemiTonesPitchAndOctave; }

    S_msrSemiTonesPitchAndOctave
                          getSemiTonesPitchAndOctaveDefaultValue () const
                              { return fSemiTonesPitchAndOctaveDefaultValue; }


    void                  setAllDurations ()
                              { fAllDurations = true; }
    Bool                  getAllDurations () const
                              { return fAllDurations; }

    void                  setNoAutoBeaming ()
                              { fNoAutoBeaming = true; }
    Bool                  getNoAutoBeaming () const
                              { return fNoAutoBeaming; }

    void                  setNoBeams ()
                              { fNoBeams = true; }
    Bool                  getNoBeams () const
                              { return fNoBeams; }

    void                  setRomanStringNumbers ()
                              { fRomanStringNumbers = true; }
    Bool                  getRomanStringNumbers () const
                              { return fRomanStringNumbers; }

    void                  setAvoidOpenStrings ()
                              { fAvoidOpenStrings = true; }
    Bool                  getAvoidOpenStrings () const
                              { return fAvoidOpenStrings; }

    lpsrAccidentalStyleKind
                          getAccidentalStyleKind () const
                              { return fAccidentalStyleKind; }

    void                  setCompressEmptyMeasuresInLilypond ()
                              { fCompressEmptyMeasuresInLilypond = true; }
    Bool                  getCompressEmptyMeasuresInLilypond () const
                              { return fCompressEmptyMeasuresInLilypond; }

    void                  setMergeStaffCommonRests ()
                              { fMergeStaffCommonRests = true; }
    Bool                  getMergeStaffCommonRests () const
                              { return fMergeStaffCommonRests; }

    void                  setInputLineNumbers ()
                              { fInputLineNumbers = true; }
    Bool                  getInputLineNumbers () const
                              { return fInputLineNumbers; }

    S_oahBooleanAtom      getInputLineNumbersAtom () const
                              { return fInputLineNumbersAtom; }

    void                  setNotesComments ()
                              { fNotesComments = true; }
    Bool                  getNotesComments () const
                              { return fNotesComments; }

    void                  setOriginalMeasureNumbers ()
                              { fOriginalMeasureNumbers = true; }
    Bool                  getOriginalMeasureNumbers () const
                              { return fOriginalMeasureNumbers; }

    void                  setGeneratePositionsInMeasures ()
                              { fGeneratePositionsInMeasures = true; }
    Bool                  getGeneratePositionsInMeasures () const
                              { return fGeneratePositionsInMeasures; }

    void                  setGenerateMomentsInMeasures ()
                              { fGenerateMomentsInMeasures = true; }
    Bool                  getGenerateMomentsInMeasures () const
                              { return fGenerateMomentsInMeasures; }

    const msrRGBColor&    getNonPrintNotesHeadRGBColor () const
                              { return fNonPrintNotesHeadRGBColor; }
    S_oahRGBColorAtom     getNonPrintNotesHeadRGBColorAtom () const
                              { return fNonPrintNotesHeadRGBColorAtom; }

    // measure numbers
    // --------------------------------------

    const map<string, int>&
                          getResetMeasureElementMeasureNumberMap () const
                              { return fResetMeasureElementMeasureNumberMap; }
    const map<string, int>&
                          getAddEmptyMeasuresAfterMeasureNumberMap () const
                              { return fAddEmptyMeasuresAfterMeasureNumberMap; }

    // bar numbers
    // --------------------------------------

    void                  setShowAllBarNumbers ()
                              { fShowAllBarNumbers = true; }
    Bool                  getShowAllBarNumbers () const
                              { return fShowAllBarNumbers; }

    Bool                  getBoxAroundAllBarNumbers () const
                              { return fBoxAroundAllBarNumbers; }

    const set<string>&    getShowNumbersAtMeasureSet () const
                              { return fShowNumbersAtMeasureSet; }

    void                  setBarNumbersSize (float value)
                              { fBarNumbersSize = value; }
    float                 getBarNumbersSize () const
                              { return fBarNumbersSize; }

    S_oahFloatAtom        getBarNumbersSizeAtom () const
                              { return fBarNumbersSizeAtom; }

    const set<int>&       getBoxAroundBarNumberSet () const
                              { return fBoxAroundBarNumberSet; }

    // bar number checks
    // --------------------------------------

    void                  setNoBarNumberChecks ()
                              { fNoBarNumberChecks = true; }
    Bool                  getNoBarNumberChecks () const
                              { return fNoBarNumberChecks; }

    // line breaks
    // --------------------------------------

    void                  setIgnoreLpsrLineBreaks ()
                              { fIgnoreLpsrLineBreaks = true; }
    Bool                  getIgnoreLpsrLineBreaks () const
                              { return fIgnoreLpsrLineBreaks; }

    void                  setBreakLinesAtIncompleteRightMeasures ()
                              { fBreakLinesAtIncompleteRightMeasures = true; }
    Bool                  getBreakLinesAtIncompleteRightMeasures () const
                              { return fBreakLinesAtIncompleteRightMeasures; }

    void                  setSeparatorLineEveryNMeasures (int value)
                              { fSeparatorLineEveryNMeasures = value; }
    int                   getSeparatorLineEveryNMeasures () const
                              { return fSeparatorLineEveryNMeasures; }

    const set<string>&    getBreakLineAfterMeasureNumberSet () const
                              { return fBreakLineAfterMeasureNumberSet; }

    // page breaks
    // --------------------------------------

    void                  setIgnoreLpsrPageBreaks ()
                              { fIgnoreLpsrPageBreaks = true; }
    Bool                  getIgnoreLpsrPageBreaks () const
                              { return fIgnoreLpsrPageBreaks; }

    const set<string>&    getBreakPageAfterMeasureNumberSet () const
                              { return fBreakPageAfterMeasureNumberSet; }


    // staves
    // --------------------------------------

    void                  setModernTab ()
                              { fModernTab = true; }
    Bool                  getModernTab () const
                              { return fModernTab; }

    void                  setTabFullNotation ()
                              { fTabFullNotation = true; }
    Bool                  getTabFullNotation () const
                              { return fTabFullNotation; }

    void                  setKeepStaffSize ()
                              { fKeepStaffSize = true; }
    Bool                  getKeepStaffSize () const
                              { return fKeepStaffSize; }

    const msrRGBColor&    getLedgerLinesRGBColor () const
                              { return fLedgerLinesRGBColor; }
    S_oahRGBColorAtom     getLedgerLinesRGBColorAtom () const
                              { return fLedgerLinesRGBColorAtom; }

    // multiple full-bar rests
    // --------------------------------------

    void                  setMultipleFullBarRestsExpandLimit ()
                              { fMultipleFullBarRestsExpandLimit = true; }

    Bool                  getMultipleFullBarRestsExpandLimit () const
                              { return fMultipleFullBarRestsExpandLimit; }
    S_oahIntegerAtom      getMultipleFullBarRestsExpandLimitAtom () const
                              { return fMultipleFullBarRestsExpandLimitAtom; }

    // chords
    // --------------------------------------

    void                  setConnectArpeggios ()
                              { fConnectArpeggios = true; }
    Bool                  getConnectArpeggios () const
                              { return fConnectArpeggios; }


    // tuplets
    // --------------------------------------

    void                  setIndentTuplets ()
                              { fIndentTuplets = true; }
    Bool                  getIndentTuplets () const
                              { return fIndentTuplets; }


    // repeats
    // --------------------------------------

    void                  setKeepRepeatBarLines ()
                              { fKeepRepeatBarLines = true; }
    Bool                  getKeepRepeatBarLines () const
                              { return fKeepRepeatBarLines; }

    void                  setRepeatBrackets ()
                              { fRepeatBrackets = true; }
    Bool                  getRepeatBrackets () const
                              { return fRepeatBrackets; }

    void                  setIgnoreRepeatNumbers ()
                              { fIgnoreRepeatNumbers = true; }
    Bool                  getIgnoreRepeatNumbers () const
                              { return fIgnoreRepeatNumbers; }


    // ornaments
    // --------------------------------------

    rational              getDelayedOrnamentsFraction () const
                              { return fDelayedOrnamentsFraction; }


    // chords display
    // --------------------------------------

    const list<pair<string, string> >&
                          getChordsDisplayList () const
                              { return fChordsDisplayList; }

    void                  setJazzChordsDisplay ()
                              { fJazzChordsDisplay = true; }

    Bool                  getJazzChordsDisplay () const
                              { return fJazzChordsDisplay; }

    string                getJazzChordsDisplayLilypondCode () const
                              { return fJazzChordsDisplayLilypondCode; }


    // fonts
    // --------------------------------------

    void                  setJazzFonts ()
                              { fJazzFonts = true; }
    Bool                  getJazzFonts () const
                              { return fJazzFonts; }


    // code generation
    // --------------------------------------

    void                  setXml2lyInfos ()
                              { fXml2lyInfos = true; }
    Bool                  getXml2lyInfos () const
                              { return fXml2lyInfos; }

    void                  setLilypondComments ()
                              { fLilypondComments = true; }
    Bool                  getLilypondComments () const
                              { return fLilypondComments; }

    void                  setGlobal ()
                              { fGlobal = true; }
    Bool                  getGlobal () const
                              { return fGlobal; }

    void                  setDisplayMusic ()
                              { fDisplayMusic = true; }
    Bool                  getDisplayMusic () const
                              { return fDisplayMusic; }

    void                  setNoLilypondCode ()
                              { fNoLilypondCode = true; }
    Bool                  getNoLilypondCode () const
                              { return fNoLilypondCode; }

    void                  setNoLilypondLyrics ()
                              { fNoLilypondLyrics = true; }
    Bool                  getNoLilypondLyrics () const
                              { return fNoLilypondLyrics; }

    void                  setLilypondRunDate ()
                              { fLilypondRunDate = true; }
    Bool                  getLilypondRunDate () const
                              { return fLilypondRunDate; }

    void                  setDraftMode ()
                              { fDraftMode = true; }
    Bool                  getDraftMode () const
                              { return fDraftMode; }

    void                  setPointAndClickOff ()
                              { fPointAndClickOff = true; }
    Bool                  getPointAndClickOff () const
                              { return fPointAndClickOff; }

    void                  setGenerateStemsDirections ()
                              { fGenerateStemsDirections = true; }
    Bool                  getGenerateStemsDirections () const
                              { return fGenerateStemsDirections; }

    void                  setWhiteNoteHeads ()
                              { fWhiteNoteHeads = true; }
    Bool                  getWhiteNoteHeads () const
                              { return fWhiteNoteHeads; }

    void                  setGenerateCommentedOutVariables ()
                              { fGenerateCommentedOutVariables = true; }
    Bool                  getGenerateCommentedOutVariables () const
                              { return fGenerateCommentedOutVariables; }

//     void                  setGenerateMsrVisitingInformation ()
//                               { fGenerateMsrVisitingInformation = true; }
//     Bool                  getGenerateMsrVisitingInformation () const
//                               { return fGenerateMsrVisitingInformation; }

    void                  setGenerateLpsrVisitingInformation ()
                              { fGenerateLpsrVisitingInformation = true; }
    Bool                  getGenerateLpsrVisitingInformation () const
                              { return fGenerateLpsrVisitingInformation; }


    // dynamics
    // --------------------------------------

    void                  setAddLilypondStanzasNumbers (
                            lpsrDynamicsTextSpannersStyleKind value)
                              { fDynamicsTextSpannersStyleKind = value; }
    lpsrDynamicsTextSpannersStyleKind
                          getDynamicsTextSpannersStyleKind () const
                              { return fDynamicsTextSpannersStyleKind; }

    S_lilypondDynamicsTextSpannersStyleKindAtom
                          getDynamicsTextSpannersStyleKindAtom () const
                              { return fDynamicsTextSpannersStyleKindAtom; }

    // lyrics
    // --------------------------------------

    void                  setAddLilypondStanzasNumbers ()
                              { fAddLilypondStanzasNumbers = true; }
    Bool                  getAddLilypondStanzasNumbers () const
                              { return fAddLilypondStanzasNumbers; }

    void                  setLyricsDurationsKind (
                            lpsrLyricsDurationsKind value)
                              { fLyricsDurationsKind = value; }
    lpsrLyricsDurationsKind
                          getLyricsDurationsKind () const
                              { return fLyricsDurationsKind; }

    // midi
    // --------------------------------------

    void                  setMidiTempo (msrMidiTempo value)
                              { fMidiTempo = value; }
    const msrMidiTempo&   getMidiTempo () const
                              { return fMidiTempo; }

    S_oahMidiTempoAtom    getMidiTempoAtom () const
                              { return fMidiTempoAtom;}


    void                  setNoMidi ()
                              { fNoMidi = true; }
    Bool                  getNoMidi () const
                              { return fNoMidi; }

/* JMI ???
    Bool                  getCreateScoreBlockMidiBlock () const
                              { return fetCreateScoreBlockMidiBlock; }
*/

  public:

    // public services
    // ------------------------------------------------------

    // octave entry

    msrOctaveEntryKind    fetchOctaveEntryVariableValue () const
                              {
                                return fOctaveEntryVariable.getOctaveEntryKind ();
                              }

//     void                  setOctaveEntryKindToAbsolute () // JMI ???
//                               {
//                                 fOctaveEntryKind =
//                                   msrOctaveEntryKind::kOctaveEntryAbsolute;
//                               }

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeLilypondVersionOptions ();

    void                  initializeScoreNotationOptions ();

    void                  initializeGlobalStaffSizeOptions ();

    void                  initializeIdentificationOptions ();

    void                  initializeVoicesOptions ();

    void                  initializePartsOptions ();

    void                  initializeEngraversOptions ();

    void                  initializeClefsKeysTimesOptions ();

    void                  initializeNotesOptions ();

    void                  initializeBarsOptions ();

    void                  initializeLineBreaksOptions ();

    void                  initializePageBreaksOptions ();

    void                  initializeStavesOptions ();

    void                  initializeChordsOptions ();

    void                  initializeTupletsOptions ();

    void                  initializeRepeatOptions ();

    void                  initializeOrnamentsOptions ();

    void                  initializeChordsDisplayOptions ();

    void                  initializeLyricsOptions ();

    void                  initializeDynamicsOptions ();

    void                  initializeFontsOptions ();

    void                  initializeCodeGenerationOptions ();

    void                  initializeMidiOptions ();

    void                  initializeCombinedOptionsOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printLilypondGenerationOahHelp ();

    void                  printLilypondGenerationOahValues (int fieldWidth);

    virtual void          printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private fields
    // --------------------------------------

    // LilyPond version
    // --------------------------------------

    string                fLilypondVersion;
    S_oahStringAtom       fLilypondVersionAtom;

    string                fLilypondVersionDefaultValue;

    // score notation
    // --------------------------------------

    // lilypondScoreNotationKind     fScoreNotationKind; JMI ???
    Bool                  fJianpu;

    // lyluatex output
    // --------------------------------------

    // to embed LilyPond code in LyLuaTex code
    Bool                  fLyLuaTexOutput;

    // global staff size
    // --------------------------------------

    float                 fGlobalStaffSize;
    S_oahFloatAtom        fGlobalStaffSizeAtom;

    float                 fStaffGlobalSizeDefaultValue;

    Bool                  fNoSetGlobalStaffSize;
    S_oahBooleanAtom      fNoSetGlobalStaffSizeAtom;

    // languages
    // --------------------------------------

    Bool                  fUseLilypondDefaultLanguages;
    S_oahBooleanAtom      fUseLilypondDefaultLanguagesAtom;

    // identification
    // --------------------------------------

    // MusicXML informations
    string                fRights;
    string                fComposer;
    string                fArranger;

    S_oahStringAtom       fPoetAtom;
    string                fPoet;
    string                fLyricist;
    string                fSoftware;

    // LilyPond informations
    string                fDedication;

    string                fPiece;
    string                fOpus;
    Bool                  fOpusInTitle;

    string                fTitle;
    string                fSubTitle;
    string                fSubSubTitle;

    string                fHeaderInstrument;

    string                fHeaderMeter;

    string                fTagline;

    string                fCopyright;

    // book block
    // --------------------------------------

    Bool                  fNoBookBlock;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fNoBookBlockAtom;

    // header block
    // --------------------------------------

    Bool                  fNoHeaderBlock;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fNoHeaderBlockAtom;

    // layout block
    // --------------------------------------

    Bool                  fNoLayoutScoreContext;
    S_oahBooleanAtom      fNoLayoutScoreContextAtom;

    Bool                  fNoLayoutVoiceContext;
    S_oahBooleanAtom      fNoLayoutVoiceContextAtom;

    Bool                  fNoTopLevelLayoutBlock;

    // paper block
    // --------------------------------------

    Bool                  fNoPaperBlock;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fNoPaperBlockAtom;

    // parts
    // --------------------------------------

    map<string, S_msrSemiTonesPitchAndOctave>
                          fPartNamesTranspositionMap;
    map<string, S_msrSemiTonesPitchAndOctave>
                          fPartIDsTranspositionMap;

    // voices
    // --------------------------------------

    Bool                  fAutoVoices;

    // names
    // --------------------------------------

    // engravers
    // --------------------------------------

    Bool                  fAmbitusEngraver;
    Bool                  fCustosEngraver;

    // clefs
    // --------------------------------------

    Bool                  fNoInitialTrebleClef;

    Bool                  fCommentClefChanges;

    // keys
    // --------------------------------------

    Bool                  fNoInitialCMajorKey;

    // time signatures
    // --------------------------------------

    Bool                  fNoInitialCommonTimeSignature;

    Bool                  fNumericalTimeSignature;

    // measure numbers
    // --------------------------------------

    map<string, int>      fResetMeasureElementMeasureNumberMap;
    map<string, int>      fAddEmptyMeasuresAfterMeasureNumberMap;

    // bar numbers
    // --------------------------------------

    Bool                  fShowAllBarNumbers;

    set<string>           fShowNumbersAtMeasureSet;
    S_oahStringSetElementAtom
                          fShowNumbersAtMeasureAtom;

    float                 fBarNumbersSize;
    S_oahFloatAtom        fBarNumbersSizeAtom;

    Bool                  fBoxAroundAllBarNumbers;
    set<int>              fBoxAroundBarNumberSet;

    // bar number checks
    // --------------------------------------

    Bool                  fNoBarNumberChecks;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fNoBarNumberChecksAtom;

    // line breaks
    // --------------------------------------

    Bool                  fIgnoreLpsrLineBreaks;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fIgnoreLpsrLineBreaksAtom;

    Bool                  fBreakLinesAtIncompleteRightMeasures;

    int                   fSeparatorLineEveryNMeasures;

    set<string>           fBreakLineAfterMeasureNumberSet;

    // page breaks
    // --------------------------------------

    Bool                  fIgnoreLpsrPageBreaks;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fIgnoreLpsrPageBreaksAtom;

    set<string>           fBreakPageAfterMeasureNumberSet;


    // staves
    // --------------------------------------

    Bool                  fModernTab;
    Bool                  fTabFullNotation;

    Bool                  fKeepStaffSize;

    msrRGBColor           fLedgerLinesRGBColor; // #(rgb-color 1.0 0.9 0.5)
    S_oahRGBColorAtom     fLedgerLinesRGBColorAtom;


    // multiple full-bar rests
    // --------------------------------------

    int                   fMultipleFullBarRestsExpandLimit;
    S_oahIntegerAtom      fMultipleFullBarRestsExpandLimitAtom;

    // notes
    // --------------------------------------

    msrOctaveEntryVariable
                          fOctaveEntryVariable;

    // this is relevant only for relative octave entry kind
    S_msrSemiTonesPitchAndOctave
                          fRelativeOctaveEntrySemiTonesPitchAndOctave;

    // this is relevant only for fixed octave entry kind
    S_msrSemiTonesPitchAndOctave
                          fFixedOctaveEntrySemiTonesPitchAndOctave;

    S_msrSemiTonesPitchAndOctave
                          fSemiTonesPitchAndOctaveDefaultValue;


    Bool                  fAllDurations;

    Bool                  fNoAutoBeaming;
    Bool                  fNoBeams;

    Bool                  fRomanStringNumbers;
    Bool                  fAvoidOpenStrings;

    lpsrAccidentalStyleKind
                          fAccidentalStyleKind;

    Bool                  fCompressEmptyMeasuresInLilypond;

    Bool                  fMergeStaffCommonRests;

    Bool                  fInputLineNumbers;
    S_oahBooleanAtom      fInputLineNumbersAtom;

    Bool                  fNotesComments;

    Bool                  fOriginalMeasureNumbers;

    Bool                  fGeneratePositionsInMeasures;
    Bool                  fGenerateMomentsInMeasures;

    // for MusicXML's <note print-object="no">
    msrRGBColor           fNonPrintNotesHeadRGBColor; // #(rgb-color 1.0 0.9 0.5)
    S_oahRGBColorAtom     fNonPrintNotesHeadRGBColorAtom;

    // chords
    // --------------------------------------

    Bool                  fConnectArpeggios;


    // tuplets
    // --------------------------------------

    Bool                  fIndentTuplets;


    // repeats
    // --------------------------------------

    Bool                  fKeepRepeatBarLines;  // otherwise let LilyPond do the job
    Bool                  fRepeatBrackets;
    Bool                  fIgnoreRepeatNumbers; // and let LilyPond do the job


    // ornaments
    // --------------------------------------

    rational              fDelayedOrnamentsFraction;


    // chords display
    // --------------------------------------

    list<pair<string, string> >
                          fChordsDisplayList;

    Bool                  fJazzChordsDisplay;
    string                fJazzChordsDisplayLilypondCode;


    // harmonies
    // --------------------------------------


    // fonts
    // --------------------------------------

    Bool                  fJazzFonts;


    // code generation
    // --------------------------------------

    Bool                  fXml2lyInfos;

    Bool                  fLilypondComments;

    Bool                  fGlobal;

    Bool                  fDisplayMusic;

    Bool                  fNoLilypondCode;

    Bool                  fNoLilypondLyrics;

    Bool                  fLilypondRunDate;

    Bool                  fDraftMode;

    Bool                  fPointAndClickOff;

    Bool                  fGenerateStemsDirections;

    Bool                  fWhiteNoteHeads;

    Bool                  fGenerateCommentedOutVariables;
                            // this is to avoid having to add them by hand
                            // in the LilyPond output

//     Bool                  fGenerateMsrVisitingInformation; // JMI v0.9.61
    Bool                  fGenerateLpsrVisitingInformation;

    // dynamics
    // --------------------------------------

    lpsrDynamicsTextSpannersStyleKind
                          fDynamicsTextSpannersStyleKind;
    S_lilypondDynamicsTextSpannersStyleKindAtom
                          fDynamicsTextSpannersStyleKindAtom;

    // lyrics
    // --------------------------------------

    Bool                  fAddLilypondStanzasNumbers;

    lpsrLyricsDurationsKind
                          fLyricsDurationsKind;

    // midi
    // --------------------------------------

    msrMidiTempo          fMidiTempo;
    S_oahMidiTempoAtom    fMidiTempoAtom;

    Bool                  fNoMidi;
};
typedef SMARTP<lpsr2lilypondOahGroup> S_lpsr2lilypondOahGroup;
EXP ostream& operator<< (ostream& os, const S_lpsr2lilypondOahGroup& elt);

EXP extern S_lpsr2lilypondOahGroup gGlobalLpsr2lilypondOahGroup;

// JMI typedef void (lpsr2lilypondOahGroup::*lpsr2lilypondOahMethodPtr)(); //MyTypedef is a type!

/*
  It's worth noting that, from C++11 up, one could write this expression
  as a more legible using statement:
    using lpsr2lilypondOahMethodPtr = void (lpsr2lilypondOahGroup::*)();
*/

//______________________________________________________________________________
EXP S_lpsr2lilypondOahGroup createGlobalLpsr2lilypondOahGroup ();
  // JMI ???
}


#endif

/* JMI
//______________________________________________________________________________
class EXP lilypondBreakPageAfterMeasureNumberAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondBreakPageAfterMeasureNumberAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            set<string>&  S_lilypondAbsoluteOctaveEntryAtomtringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondBreakPageAfterMeasureNumberAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            set<string>&  S_lilypondAbsoluteOctaveEntryAtomtringSetVariable);

    virtual               ~lilypondBreakPageAfterMeasureNumberAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const set<string>&    getStringSetVariable ()
                              { return fStringSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    set<string>&          fStringSetVariable;
};
typedef SMARTP<lilypondBreakPageAfterMeasureNumberAtom> S_lilypondBreakPageAfterMeasureNumberAtom;
EXP ostream& operator<< (ostream& os, const S_lilypondBreakPageAfterMeasureNumberAtom& elt);
*/

