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

#include "lpsrEnumTypes.h"

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
                            const std::string&   shortName,
                            const std::string&   longName,
                            const std::string&   description,
                            const std::string&   valueSpecification,
                            const std::string&   variableName,
                            lpsrScoreOutputKind& lilypondScoreOutputKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondScoreOutputKindAtom (
                            const std::string&   shortName,
                            const std::string&   longName,
                            const std::string&   description,
                            const std::string&   valueSpecification,
                            const std::string&   variableName,
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
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrScoreOutputKind&  fLpsrScoreOutputKindVariable;
};
typedef SMARTP<lilypondScoreOutputKindAtom> S_lilypondScoreOutputKindAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lilypondScoreOutputKindAtom& elt);

//______________________________________________________________________________
class EXP lilypondTransposePartNameAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondTransposePartNameAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::map<std::string, S_msrSemiTonesPitchAndOctave>&
                                          stringToMsrSemiTonesPitchAndOctaveMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondTransposePartNameAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::map<std::string, S_msrSemiTonesPitchAndOctave>&
                                          stringToMsrSemiTonesPitchAndOctaveMapVariable);

    virtual               ~lilypondTransposePartNameAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringToMsrSemiTonesPitchAndOctaveMapVariable (
                            const std::string& partName,
                            const S_msrSemiTonesPitchAndOctave&
                                          semiTonesPitchAndOctave)
                              {
                                fStringToMsrSemiTonesPitchAndOctaveMapVariable [
                                  partName
                                ] =
                                  semiTonesPitchAndOctave;
                              }

    const std::map<std::string, S_msrSemiTonesPitchAndOctave>&
                          getStringToMsrSemiTonesPitchAndOctaveMapVariable () const
                              {
                                return
                                  fStringToMsrSemiTonesPitchAndOctaveMapVariable;
                              }

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
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::map<std::string, S_msrSemiTonesPitchAndOctave>&
                          fStringToMsrSemiTonesPitchAndOctaveMapVariable;
};
typedef SMARTP<lilypondTransposePartNameAtom> S_lilypondTransposePartNameAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lilypondTransposePartNameAtom& elt);

//______________________________________________________________________________
class EXP lilypondTransposePartIDAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondTransposePartIDAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::map<std::string, S_msrSemiTonesPitchAndOctave>&
                                          stringToMsrSemiTonesPitchAndOctaveMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondTransposePartIDAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::map<std::string, S_msrSemiTonesPitchAndOctave>&
                                          stringToMsrSemiTonesPitchAndOctaveMapVariable);

    virtual               ~lilypondTransposePartIDAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringToMsrSemiTonesPitchAndOctaveMapVariable (
                            const std::string& partName,
                            const S_msrSemiTonesPitchAndOctave&
                                          semiTonesPitchAndOctave)
                              {
                                fStringToMsrSemiTonesPitchAndOctaveMapVariable [
                                  partName
                                ] =
                                  semiTonesPitchAndOctave;
                              }

    const std::map<std::string, S_msrSemiTonesPitchAndOctave>&
                          getStringToMsrSemiTonesPitchAndOctaveMapVariable () const
                              {
                                return
                                  fStringToMsrSemiTonesPitchAndOctaveMapVariable;
                              }

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
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::map<std::string, S_msrSemiTonesPitchAndOctave>&
                          fStringToMsrSemiTonesPitchAndOctaveMapVariable;
};
typedef SMARTP<lilypondTransposePartIDAtom> S_lilypondTransposePartIDAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lilypondTransposePartIDAtom& elt);

//______________________________________________________________________________
class EXP msrOctaveEntryVariable : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrOctaveEntryVariable> create (
                            const std::string&           variableName,
                            msrOctaveEntryKind octaveEntryKind);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrOctaveEntryVariable (
                            const std::string&           variableName,
                            msrOctaveEntryKind octaveEntryKind);

    virtual               ~msrOctaveEntryVariable ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getVariableName () const
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

    void                  print (std::ostream& os) const;

    void                  printAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fVariableName;
    msrOctaveEntryKind    fOctaveEntryKind;
};
typedef SMARTP<msrOctaveEntryVariable> S_msrOctaveEntryVariable;
EXP std::ostream& operator << (std::ostream& os, const msrOctaveEntryVariable& elt);

//______________________________________________________________________________
class EXP lilypondAbsoluteOctaveEntryAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondAbsoluteOctaveEntryAtom> create (
                            const std::string&           longName,
                            const std::string&           shortName,
                            const std::string&           description,
                            const std::string&           variableName,
                            msrOctaveEntryVariable& octaveEntryKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondAbsoluteOctaveEntryAtom (
                            const std::string&           longName,
                            const std::string&           shortName,
                            const std::string&           description,
                            const std::string&           variableName,
                            msrOctaveEntryVariable& octaveEntryKindVariable);

    virtual               ~lilypondAbsoluteOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (std::ostream& os) override;

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os); // JMI CAFE override ??? remove method ???
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

    void                  print (std::ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrOctaveEntryVariable&
                          fOctaveEntryKindVariable;
};
typedef SMARTP<lilypondAbsoluteOctaveEntryAtom> S_lilypondAbsoluteOctaveEntryAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lilypondAbsoluteOctaveEntryAtom& elt);

//______________________________________________________________________________
class EXP lilypondRelativeOctaveEntryAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondRelativeOctaveEntryAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            msrOctaveEntryVariable&
                                               octaveEntryKindVariable,
                            S_msrSemiTonesPitchAndOctave&
                                               lilypondRelativeOctaveEntryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondRelativeOctaveEntryAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            msrOctaveEntryVariable&
                                               octaveEntryKindVariable,
                            S_msrSemiTonesPitchAndOctave&
                                               lilypondRelativeOctaveEntryVariable);

    virtual               ~lilypondRelativeOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setRelativeOctaveEntryVariableValue (
                            const S_msrSemiTonesPitchAndOctave& value)
                              {
                                fSemiTonesPitchAndOctaveVariable = value;
                              }

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
EXP std::ostream& operator << (std::ostream& os, const S_lilypondRelativeOctaveEntryAtom& elt);

//______________________________________________________________________________
class EXP lilypondFixedOctaveEntryAtom : public oahAtomExpectingAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondFixedOctaveEntryAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            msrOctaveEntryVariable&
                                               octaveEntryKindVariable,
                            S_msrSemiTonesPitchAndOctave&
                                               lilypondFixedOctaveEntryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondFixedOctaveEntryAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            msrOctaveEntryVariable&
                                               octaveEntryKindVariable,
                            S_msrSemiTonesPitchAndOctave&
                                               lilypondFixedOctaveEntryVariable);

    virtual               ~lilypondFixedOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setFixedOctaveEntryVariableValue (
                            const S_msrSemiTonesPitchAndOctave& value)
                              { fSemiTonesPitchAndOctaveVariable = value; }

    S_msrSemiTonesPitchAndOctave&
                          getFixedOctaveEntryVariableValue () const
                              { return fSemiTonesPitchAndOctaveVariable; }
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
EXP std::ostream& operator << (std::ostream& os, const S_lilypondFixedOctaveEntryAtom& elt);

//______________________________________________________________________________
class EXP lilypondAccidentalStyleKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondAccidentalStyleKindAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            lpsrAccidentalStyleKind&
                                          lilypondAccidentalStyleKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondAccidentalStyleKindAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
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
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrAccidentalStyleKind&
                          fLpsrAccidentalStyleKindVariable;
};
typedef SMARTP<lilypondAccidentalStyleKindAtom> S_lilypondAccidentalStyleKindAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lilypondAccidentalStyleKindAtom& elt);

//______________________________________________________________________________
class EXP lilypondChordsDisplayAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondChordsDisplayAtom> create (
                            const std::string&         shortName,
                            const std::string&         longName,
                            const std::string&         description,
                            const std::string&         valueSpecification,
                            const std::string&         variableName,
                            std::list<std::pair<std::string, std::string> >&
                                                  lilypondChordsDisplayVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondChordsDisplayAtom (
                            const std::string&         shortName,
                            const std::string&         longName,
                            const std::string&         description,
                            const std::string&         valueSpecification,
                            const std::string&         variableName,
                            std::list<std::pair<std::string, std::string> >&
                                                  lilypondChordsDisplayVariable);

    virtual               ~lilypondChordsDisplayAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringsPairListVariable (
                            std::pair<std::string, std::string> value)
                              {
                                fStringsPairListVariable.push_back (value);
                              }

    const std::list<std::pair<std::string, std::string> >&
                          getStringsPairListVariable () const
                              { return fStringsPairListVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::list<std::pair<std::string, std::string> >&
                          fStringsPairListVariable;
};
typedef SMARTP<lilypondChordsDisplayAtom> S_lilypondChordsDisplayAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lilypondChordsDisplayAtom& elt);

//______________________________________________________________________________
class EXP lilypondLyricsDurationsKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondLyricsDurationsKindAtom> create (
                            const std::string&          longName,
                            const std::string&          shortName,
                            const std::string&          description,
                            const std::string&          valueSpecification,
                            const std::string&          variableName,
                            lpsrLyricsDurationsKind&
                                              lilypondLyricsDurationsKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondLyricsDurationsKindAtom (
                            const std::string&          longName,
                            const std::string&          shortName,
                            const std::string&          description,
                            const std::string&          valueSpecification,
                            const std::string&          variableName,
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
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrLyricsDurationsKind&
                          fLpsrLyricsDurationsKindVariable;
};
typedef SMARTP<lilypondLyricsDurationsKindAtom> S_lilypondLyricsDurationsKindAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lilypondLyricsDurationsKindAtom& elt);

//______________________________________________________________________________
class EXP lilypondDynamicsTextSpannersStyleKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondDynamicsTextSpannersStyleKindAtom> create (
                            const std::string&          longName,
                            const std::string&          shortName,
                            const std::string&          description,
                            const std::string&          valueSpecification,
                            const std::string&          variableName,
                            lpsrDynamicsTextSpannersStyleKind&
                                              lilypondDynamicsTextSpannersStyleKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondDynamicsTextSpannersStyleKindAtom (
                            const std::string&          longName,
                            const std::string&          shortName,
                            const std::string&          description,
                            const std::string&          valueSpecification,
                            const std::string&          variableName,
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
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrDynamicsTextSpannersStyleKind&
                          fLpsrdDynamicsTextSpannersStyleKindVariable;
};
typedef SMARTP<lilypondDynamicsTextSpannersStyleKindAtom>
  S_lilypondDynamicsTextSpannersStyleKindAtom;
EXP std::ostream& operator <<
  (std::ostream& os, const S_lilypondDynamicsTextSpannersStyleKindAtom& elt);

//______________________________________________________________________________
enum class lilypondScoreNotationKind {
  kNotationWestern,
  kNotationJianpu,
  kNotationABC // not used yet
};

extern std::string lilypondScoreNotationKindAsString (
  lilypondScoreNotationKind notationKind);

std::ostream& operator << (std::ostream& os, const lilypondScoreNotationKind& elt);

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
//                             const std::string& outputKind);

    // LilyPond version
    // --------------------------------------

    void                  setLilypondVersion (std::string version)
                              { fLilypondVersion = version; }
    std::string           getLilypondVersion () const
                              { return fLilypondVersion; }
    S_oahStringAtom       getLilypondVersionAtom () const
                              { return fLilypondVersionAtom; }

    std::string           getLilypondVersionDefaultValue () const
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
    const S_oahBooleanAtom&      getNoSetGlobalStaffSizeAtom () const
                              { return fNoSetGlobalStaffSizeAtom; }

    // languages
    // --------------------------------------

    Bool                  getUseLilypondDefaultLanguages () const
                              { return fUseLilypondDefaultLanguages; }
    const S_oahBooleanAtom&      getUseLilypondDefaultLanguagesAtom () const
                              { return fUseLilypondDefaultLanguagesAtom; }

    // identification
    // --------------------------------------

    // MusicXML informations
    std::string           getRights () const
                              { return fRights; }
    std::string           getComposer () const
                              { return fComposer; }
    std::string           getArranger () const
                              { return fArranger; }

    S_oahStringAtom       getPoetAtom () const
                              { return fPoetAtom; }
    std::string           getPoet () const
                              { return fPoet; }
    std::string           getLyricist () const
                              { return fLyricist; }
    std::string           getSoftware () const
                              { return fSoftware; }

    // Lilypond informations
    std::string           getDedication () const
                              { return fDedication; }

    std::string           getPiece () const
                              { return fPiece; }
    std::string           getOpus () const
                              { return fOpus; }
    Bool                  getOpusInTitle () const
                              { return fOpusInTitle; }

    std::string           getTitle () const
                              { return fTitle; }
    std::string           getSubTitle () const
                              { return fSubTitle; }
    std::string           getSubSubTitle () const
                              { return fSubSubTitle; }

    std::string           getHeaderInstrument () const
                              { return fHeaderInstrument; }

    std::string           getHeaderMeter () const
                              { return fHeaderMeter; }

    std::string           getTagline () const
                              { return fTagline; }

    std::string           getCopyright () const
                              { return fCopyright; }

    // book block
    // --------------------------------------

    Bool                  getNoBookBlock () const
                              { return fNoBookBlock; }
    const S_oahBooleanAtom&      getNoBookBlockAtom () const
                              { return fNoBookBlockAtom; }

    // header block
    // --------------------------------------

    Bool                  getNoHeaderBlock () const
                              { return fNoHeaderBlock; }
    const S_oahBooleanAtom&      getNoHeaderBlockAtom () const
                              { return fNoHeaderBlockAtom; }

    // layout block
    // --------------------------------------

    Bool                  getNoLayoutScoreContext () const
                              { return fNoLayoutScoreContext; }
    const S_oahBooleanAtom&      getNoLayoutScoreContextAtom () const
                              { return fNoLayoutScoreContextAtom; }

    Bool                  getNoLayoutVoiceContext () const
                              { return fNoLayoutVoiceContext; }
    const S_oahBooleanAtom&      getNoLayoutVoiceContextAtom () const
                              { return fNoLayoutVoiceContextAtom; }

    Bool                  getNoTopLevelLayoutBlock () const
                              { return fNoTopLevelLayoutBlock; }

    // paper block
    // --------------------------------------

    Bool                  getNoPaperBlock () const
                              { return fNoPaperBlock; }
    const S_oahBooleanAtom&      getNoPaperBlockAtom () const
                              { return fNoPaperBlockAtom; }

    // parts
    // --------------------------------------

    const std::map<std::string, S_msrSemiTonesPitchAndOctave>&
                          getPartNamesTranspositionMap () const
                              { return fPartNamesTranspositionMap; }
    const std::map<std::string, S_msrSemiTonesPitchAndOctave>&
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

    void                  setCompressFullBarRestsInLilypond ()
                              { fCompressFullBarRestsInLilypond = true; }
    Bool                  getCompressFullBarRestsInLilypond () const
                              { return fCompressFullBarRestsInLilypond; }

    void                  setMergeStaffCommonRests ()
                              { fMergeStaffCommonRests = true; }
    Bool                  getMergeStaffCommonRests () const
                              { return fMergeStaffCommonRests; }

    void                  setInputLineNumbers ()
                              { fInputLineNumbers = true; }
    Bool                  getInputLineNumbers () const
                              { return fInputLineNumbers; }

    const S_oahBooleanAtom&      getInputLineNumbersAtom () const
                              { return fInputLineNumbersAtom; }

    void                  setNotesComments ()
                              { fNotesComments = true; }
    Bool                  getNotesComments () const
                              { return fNotesComments; }

    void                  setOriginalMeasureNumbers ()
                              { fOriginalMeasureNumbers = true; }
    Bool                  getOriginalMeasureNumbers () const
                              { return fOriginalMeasureNumbers; }

    void                  setGenerateMeasurePositions ()
                              { fGenerateMeasurePositions = true; }
    Bool                  getGenerateMeasurePositions () const
                              { return fGenerateMeasurePositions; }

    void                  setGenerateMeasureMoments ()
                              { fGenerateMeasureMoments = true; }
    Bool                  getGenerateMeasureMoments () const
                              { return fGenerateMeasureMoments; }

    const msrColorRGB&    getNonPrintNotesHeadColorRGB () const
                              { return fNonPrintNotesHeadColorRGB; }
    S_oahColorRGBAtom     getNonPrintNotesHeadColorRGBAtom () const
                              { return fNonPrintNotesHeadColorRGBAtom; }

    // measure numbers
    // --------------------------------------

    const std::map<std::string, int>&
                          getResetMeasureElementMeasureNumberMap () const
                              { return fResetMeasureElementMeasureNumberMap; }
    const std::map<std::string, int>&
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

    const std::set<std::string>&    getShowNumbersAtMeasureSet () const
                              { return fShowNumbersAtMeasureSet; }

    void                  setBarNumbersSize (float value)
                              { fBarNumbersSize = value; }
    float                 getBarNumbersSize () const
                              { return fBarNumbersSize; }

    S_oahFloatAtom        getBarNumbersSizeAtom () const
                              { return fBarNumbersSizeAtom; }

    const std::set<int>&       getBoxAroundBarNumberSet () const
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

    const std::set<std::string>&    getBreakLineAfterMeasureNumberSet () const
                              { return fBreakLineAfterMeasureNumberSet; }

    // page and line breaks
    // --------------------------------------

    void                  setIgnoreLpsrPageBreaks ()
                              { fIgnoreLpsrPageBreaks = true; }
    Bool                  getIgnoreLpsrPageBreaks () const
                              { return fIgnoreLpsrPageBreaks; }

    const std::set<std::string>&    getBreakPageAfterMeasureNumberSet () const
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

    const msrColorRGB&    getLedgerLinesColorRGB () const
                              { return fLedgerLinesColorRGB; }
    S_oahColorRGBAtom     getLedgerLinesColorRGBAtom () const
                              { return fLedgerLinesColorRGBAtom; }

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

    Rational              getDelayedOrnamentsFraction () const
                              { return fDelayedOrnamentsFraction; }


    // chords display
    // --------------------------------------

    const std::list<std::pair<std::string, std::string> >&
                          getChordsDisplayList () const
                              { return fChordsDisplayList; }

    void                  setJazzChordsDisplay ()
                              { fJazzChordsDisplay = true; }

    Bool                  getJazzChordsDisplay () const
                              { return fJazzChordsDisplay; }

    std::string           getJazzChordsDisplayLilypondCode () const
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

    void                  initializePageAndLineBreaksOptions ();

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
                            std::ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private fields
    // --------------------------------------

    // LilyPond version
    // --------------------------------------

    std::string           fLilypondVersion;
    S_oahStringAtom       fLilypondVersionAtom;

    std::string           fLilypondVersionDefaultValue;

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
    std::string           fRights;
    std::string           fComposer;
    std::string           fArranger;

    S_oahStringAtom       fPoetAtom;
    std::string           fPoet;
    std::string           fLyricist;
    std::string           fSoftware;

    // LilyPond informations
    std::string           fDedication;

    std::string           fPiece;
    std::string           fOpus;
    Bool                  fOpusInTitle;

    std::string           fTitle;
    std::string           fSubTitle;
    std::string           fSubSubTitle;

    std::string           fHeaderInstrument;

    std::string           fHeaderMeter;

    std::string           fTagline;

    std::string           fCopyright;

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

    std::map<std::string, S_msrSemiTonesPitchAndOctave>
                          fPartNamesTranspositionMap;
    std::map<std::string, S_msrSemiTonesPitchAndOctave>
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

    std::map<std::string, int>      fResetMeasureElementMeasureNumberMap;
    std::map<std::string, int>      fAddEmptyMeasuresAfterMeasureNumberMap;

    // bar numbers
    // --------------------------------------

    Bool                  fShowAllBarNumbers;

    std::set<std::string> fShowNumbersAtMeasureSet;
    S_oahStringSetElementAtom
                          fShowNumbersAtMeasureAtom;

    float                 fBarNumbersSize;
    S_oahFloatAtom        fBarNumbersSizeAtom;

    Bool                  fBoxAroundAllBarNumbers;
    std::set<int>         fBoxAroundBarNumberSet;

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

    std::set<std::string> fBreakLineAfterMeasureNumberSet;

    // page and line breaks
    // --------------------------------------

    Bool                  fIgnoreLpsrPageBreaks;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fIgnoreLpsrPageBreaksAtom;

    std::set<std::string> fBreakPageAfterMeasureNumberSet;


    // staves
    // --------------------------------------

    Bool                  fModernTab;
    Bool                  fTabFullNotation;

    Bool                  fKeepStaffSize;

    msrColorRGB           fLedgerLinesColorRGB; // #(rgb-color 1.0 0.9 0.5)
    S_oahColorRGBAtom     fLedgerLinesColorRGBAtom;


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

    Bool                  fCompressFullBarRestsInLilypond;

    Bool                  fMergeStaffCommonRests;

    Bool                  fInputLineNumbers;
    S_oahBooleanAtom      fInputLineNumbersAtom;

    Bool                  fNotesComments;

    Bool                  fOriginalMeasureNumbers;

    Bool                  fGenerateMeasurePositions;
    Bool                  fGenerateMeasureMoments;

    // for MusicXML's <note print-object="no">
    msrColorRGB           fNonPrintNotesHeadColorRGB; // #(rgb-color 1.0 0.9 0.5)
    S_oahColorRGBAtom     fNonPrintNotesHeadColorRGBAtom;

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

    Rational              fDelayedOrnamentsFraction;


    // chords display
    // --------------------------------------

    std::list<std::pair<std::string, std::string> >
                          fChordsDisplayList;

    Bool                  fJazzChordsDisplay;
    std::string           fJazzChordsDisplayLilypondCode;


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
EXP std::ostream& operator << (std::ostream& os, const S_lpsr2lilypondOahGroup& elt);

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
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::set<std::string>&  S_lilypondAbsoluteOctaveEntryAtomtringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondBreakPageAfterMeasureNumberAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::set<std::string>&  S_lilypondAbsoluteOctaveEntryAtomtringSetVariable);

    virtual               ~lilypondBreakPageAfterMeasureNumberAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::set<std::string>&    getStringSetVariable ()
                              { return fStringSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::set<std::string>&          fStringSetVariable;
};
typedef SMARTP<lilypondBreakPageAfterMeasureNumberAtom> S_lilypondBreakPageAfterMeasureNumberAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lilypondBreakPageAfterMeasureNumberAtom& elt);
*/

