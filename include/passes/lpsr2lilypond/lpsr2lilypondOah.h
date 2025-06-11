/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

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
                            std::map <std::string, S_msrSemiTonesPitchAndOctave>&
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
                            std::map <std::string, S_msrSemiTonesPitchAndOctave>&
                                               stringToMsrSemiTonesPitchAndOctaveMapVariable);

    virtual               ~lilypondTransposePartNameAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringToMsrSemiTonesPitchAndOctaveMapVariable (
                            const std::string& partName,
                            const S_msrSemiTonesPitchAndOctave&
                                               semiTonesPitchAndOctave);

    const std::map <std::string, S_msrSemiTonesPitchAndOctave>&
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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::map <std::string, S_msrSemiTonesPitchAndOctave>&
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
                            std::map <std::string, S_msrSemiTonesPitchAndOctave>&
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
                            std::map <std::string, S_msrSemiTonesPitchAndOctave>&
                                               stringToMsrSemiTonesPitchAndOctaveMapVariable);

    virtual               ~lilypondTransposePartIDAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringToMsrSemiTonesPitchAndOctaveMapVariable (
                            const std::string& partName,
                            const S_msrSemiTonesPitchAndOctave&
                                               semiTonesPitchAndOctave);

    const std::map <std::string, S_msrSemiTonesPitchAndOctave>&
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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::map <std::string, S_msrSemiTonesPitchAndOctave>&
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
                            const std::string& variableName,
                            msrOctaveEntryKind octaveEntryKind);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrOctaveEntryVariable (
                            const std::string& variableName,
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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const;

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fVariableName;
    msrOctaveEntryKind    fOctaveEntryKind;
};
typedef SMARTP<msrOctaveEntryVariable> S_msrOctaveEntryVariable;
EXP std::ostream& operator << (std::ostream& os, const msrOctaveEntryVariable& elt);

//______________________________________________________________________________
class EXP lilypondAbsoluteOctaveEntryAtom : public oahValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondAbsoluteOctaveEntryAtom> create (
                            const std::string&      longName,
                            const std::string&      shortName,
                            const std::string&      description,
                            const std::string&      variableName,
                            msrOctaveEntryVariable& octaveEntryKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondAbsoluteOctaveEntryAtom (
                            const std::string&      longName,
                            const std::string&      shortName,
                            const std::string&      description,
                            const std::string&      variableName,
                            msrOctaveEntryVariable& octaveEntryKindVariable);

    virtual               ~lilypondAbsoluteOctaveEntryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os); // JMI 0.9.67 override ??? remove method ???
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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

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
class EXP lilypondFixedOctaveEntryAtom : public oahAtomStoringAValue
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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

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
class EXP lilypondDurationsGenerationKindAtom : public oahAtomImplicitlySettingAVariable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondDurationsGenerationKindAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& variableName,
                            lilypondDurationsGenerationKind&
                                               lilypondDurationsGenerationKindVariable,
                            lilypondDurationsGenerationKind
                                               lilypondDurationsGenerationKindValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondDurationsGenerationKindAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& variableName,
                            lilypondDurationsGenerationKind&
                                               lilypondDurationsGenerationKindVariable,
                            lilypondDurationsGenerationKind
                                               lilypondDurationsGenerationKindValue);

    virtual               ~lilypondDurationsGenerationKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    lilypondDurationsGenerationKind&
                          getLilypondDurationsGenerationKindVariable () const
                              { return fLilypondDurationsGenerationKindVariable; }
    lilypondDurationsGenerationKind
                          getLilypondDurationsGenerationKindValue () const
                              { return fLilypondDurationsGenerationKindValue; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override
                              { setImplicitVariable (os); }

    void                  setImplicitVariable (std::ostream& os) override;

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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lilypondDurationsGenerationKind&
                          fLilypondDurationsGenerationKindVariable;
    lilypondDurationsGenerationKind
                          fLilypondDurationsGenerationKindValue;
};
typedef SMARTP<lilypondDurationsGenerationKindAtom> S_lilypondDurationsGenerationKindAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lilypondDurationsGenerationKindAtom& elt);

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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

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
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::list <std::pair <std::string, std::string>>&
                                               lilypondChordsDisplayVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondChordsDisplayAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::list <std::pair <std::string, std::string>>&
                                               lilypondChordsDisplayVariable);

    virtual               ~lilypondChordsDisplayAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringsPairListVariable (
                            std::pair <std::string, std::string> value)
                              {
                                fStringsPairListVariable.push_back (value);
                              }

    const std::list <std::pair <std::string, std::string>>&
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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::list <std::pair <std::string, std::string>>&
                          fStringsPairListVariable;
};
typedef SMARTP<lilypondChordsDisplayAtom> S_lilypondChordsDisplayAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lilypondChordsDisplayAtom& elt);

//______________________________________________________________________________
class EXP lilypondLyricsNotesDurationsKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondLyricsNotesDurationsKindAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            lpsrLyricsNotesDurationsKind&
                                               lilypondLyricsNotesDurationsKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondLyricsNotesDurationsKindAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            lpsrLyricsNotesDurationsKind&
                                               lilypondLyricsNotesDurationsKindVariable);

    virtual               ~lilypondLyricsNotesDurationsKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLpsrLyricsNotesDurationsKindVariable (
                            lpsrLyricsNotesDurationsKind value)
                              {
                                fLpsrLyricsNotesDurationsKindVariable = value;
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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrLyricsNotesDurationsKind&
                          fLpsrLyricsNotesDurationsKindVariable;
};
typedef SMARTP<lilypondLyricsNotesDurationsKindAtom> S_lilypondLyricsNotesDurationsKindAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lilypondLyricsNotesDurationsKindAtom& elt);

//______________________________________________________________________________
class EXP lilypondDynamicsTextSpannersStyleKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondDynamicsTextSpannersStyleKindAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            lpsrDynamicsTextSpannersStyleKind&
                                               lilypondDynamicsTextSpannersStyleKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondDynamicsTextSpannersStyleKindAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

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

    void                  initializeLpsr2LilypondOahGroup ();

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

    std::string           getLilypondVersionMinimumValue () const
                              { return fLilypondVersionMinimumValue; }
    // score notation
    // --------------------------------------

    void                  setLilyJazz ()
                              { fLilyJazz = true; }
    Bool                  getLilyJazz () const
                              { return fLilyJazz; }

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
    const S_oahBooleanAtom&
                          getNoSetGlobalStaffSizeAtom () const
                              { return fNoSetGlobalStaffSizeAtom; }

    // languages
    // --------------------------------------

    Bool                  getUseLilypondDefaultLanguages () const
                              { return fUseLilypondDefaultLanguages; }
    const S_oahBooleanAtom&
                          getUseLilypondDefaultLanguagesAtom () const
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
    const S_oahBooleanAtom&
                          getNoBookBlockAtom () const
                              { return fNoBookBlockAtom; }

    // header block
    // --------------------------------------

    Bool                  getNoHeaderBlock () const
                              { return fNoHeaderBlock; }
    const S_oahBooleanAtom&
                          getNoHeaderBlockAtom () const
                              { return fNoHeaderBlockAtom; }

    // layout block
    // --------------------------------------

    Bool                  getNoLayoutScoreContext () const
                              { return fNoLayoutScoreContext; }
    const S_oahBooleanAtom&
                          getNoLayoutScoreContextAtom () const
                              { return fNoLayoutScoreContextAtom; }

    Bool                  getNoLayoutVoiceContext () const
                              { return fNoLayoutVoiceContext; }
    const S_oahBooleanAtom&
                          getNoLayoutVoiceContextAtom () const
                              { return fNoLayoutVoiceContextAtom; }

    Bool                  getNoTopLevelLayoutBlock () const
                              { return fNoTopLevelLayoutBlock; }

    // paper block
    // --------------------------------------

    Bool                  getNoPaperBlock () const
                              { return fNoPaperBlock; }
    const S_oahBooleanAtom&
                          getNoPaperBlockAtom () const
                              { return fNoPaperBlockAtom; }

    // parts
    // --------------------------------------

    const std::map <std::string, S_msrSemiTonesPitchAndOctave>&
                          getPartNamesTranspositionMap () const
                              { return fPartNamesTranspositionMap; }
    const std::map <std::string, S_msrSemiTonesPitchAndOctave>&
                          getPartMusicXMLIDsTranspositionMap () const
                              { return fPartMusicXMLIDsTranspositionMap; }


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


    void                  setAllNotesDurations ()
                              { fAllNotesDurations = true; }
    Bool                  getAllNotesDurations () const
                              { return fAllNotesDurations; }

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

    void                  setCompressMeasureRestsInLilypond ()
                              { fCompressMeasureRestsInLilypond = true; }
    Bool                  getCompressMeasureRestsInLilypond () const
                              { return fCompressMeasureRestsInLilypond; }

    void                  setMergeStaffCommonRests ()
                              { fMergeStaffCommonRests = true; }
    Bool                  getMergeStaffCommonRests () const
                              { return fMergeStaffCommonRests; }

    void                  setInputLineNumbers ()
                              { fInputStartLineNumbers = true; }
    Bool                  getInputLineNumbers () const
                              { return fInputStartLineNumbers; }

    const S_oahBooleanAtom&
                          getInputLineNumbersAtom () const
                              { return fInputStartLineNumbersAtom; }

    void                  setNotesComments ()
                              { fNotesComments = true; }
    Bool                  getNotesComments () const
                              { return fNotesComments; }

    void                  setOriginalMeasureNumbers ()
                              { fOriginalMeasureNumbers = true; }
    Bool                  getOriginalMeasureNumbers () const
                              { return fOriginalMeasureNumbers; }

    void                  setGeneratePositionInMeasures ()
                              { fGeneratePositionInMeasures = true; }
    Bool                  getGeneratePositionInMeasures () const
                              { return fGeneratePositionInMeasures; }

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

    const std::map <mfMeasureNumber, mfMeasureNumber>&
                          getResetMeasureElementMeasureNumberMap () const
                              { return fResetMeasureElementMeasureNumberMap; }
    const std::map <mfMeasureNumber, int>&
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

    const std::set <mfMeasureNumber>&
                          getShowMeasureNumbersAtMeasureSet () const
                              { return fShowMeasureNumbersAtMeasureSet; }

    void                  setBarNumbersSize (float value)
                              { fBarNumbersSize = value; }
    float                 getBarNumbersSize () const
                              { return fBarNumbersSize; }

    S_oahFloatAtom        getBarNumbersSizeAtom () const
                              { return fBarNumbersSizeAtom; }

    const std::set <mfMeasureNumber>&
                          getBoxAroundBarNumberSet () const
                              { return fBoxAroundBarNumberSet; }

    // bar number checks
    // --------------------------------------

    void                  setNoBarNumberChecks ()
                              { fNoBarNumberChecks = true; }
    Bool                  getNoBarNumberChecks () const
                              { return fNoBarNumberChecks; }

    // bar number comments
    // --------------------------------------

    void                  setNoBarCheckComments ()
                              { fNoBarCheckComments = true; }
    Bool                  getNoBarCheckComments () const
                              { return fNoBarCheckComments; }

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

    const std::set <mfMeasureNumber>&
                          getBreakLineAfterMeasureNumberSet () const
                              { return fBreakLineAfterMeasureNumberSet; }

    // page and line breaks
    // --------------------------------------

    void                  setIgnoreLpsrPageBreaks ()
                              { fIgnoreLpsrPageBreaks = true; }
    Bool                  getIgnoreLpsrPageBreaks () const
                              { return fIgnoreLpsrPageBreaks; }

    const std::set <mfMeasureNumber>&
                          getBreakPageAfterMeasureNumberSet () const
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

    // multiple measure rests
    // --------------------------------------

    void                  setMultipleMeasureRestsExpandLimit ()
                              { fMultipleMeasureRestsExpandLimit = true; }

    Bool                  getMultipleMeasureRestsExpandLimit () const
                              { return fMultipleMeasureRestsExpandLimit; }
    S_oahIntegerAtom      getMultipleMeasureRestsExpandLimitAtom () const
                              { return fMultipleMeasureRestsExpandLimitAtom; }

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

    mfRational              getDelayedOrnamentsRational () const
                              { return fDelayedOrnamentsRational; }


    // chords display
    // --------------------------------------

    const std::list <std::pair <std::string, std::string>>&
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

    // structure
    void                  setCommentLilypondStructureBasics ()
                              { fCommentLilypondStructureBasics = true; }
    Bool                  getCommentLilypondStructureBasics () const
                              { return fCommentLilypondStructureBasics; }

    void                  setCommentLilypondStructure ()
                              { fCommentLilypondStructure = true; }
    Bool                  getCommentLilypondStructure () const
                              { return fCommentLilypondStructure; }

    // parts, voices & measures
    void                  setCommentLilypondPartGroups ()
                              { fCommentLilypondPartGroups = true; }
    Bool                  getCommentLilypondPartGroups () const
                              { return fCommentLilypondPartGroups; }

    void                  setCommentLilypondVoices ()
                              { fCommentLilypondVoices = true; }
    Bool                  getCommentLilypondVoices () const
                              { return fCommentLilypondVoices; }

    void                  setCommentLilypondMeasures ()
                              { fCommentLilypondMeasures = true; }
    Bool                  getCommentLilypondMeasures () const
                              { return fCommentLilypondMeasures; }

    void                  setCommentLilypondMultipleMeasureRests ()
                              { fCommentLilypondMultipleMeasureRests = true; }
    Bool                  getCommentLilypondMultipleMeasureRests () const
                              { return fCommentLilypondMultipleMeasureRests; }

    void                  setCommentLilypondMeasureRepeats ()
                              { fCommentLilypondMeasureRepeats = true; }
    Bool                  getCommentLilypondMeasureRepeats () const
                              { return fCommentLilypondMeasureRepeats; }

    void                  setCommentLilypondBarLines ()
                              { fCommentLilypondBarLines = true; }
    Bool                  getCommentLilypondBarLines () const
                              { return fCommentLilypondBarLines; }

    // durations
    void                  setCommentLilypondDurations ()
                              { fCommentLilypondDurations = true; }
    Bool                  getCommentLilypondDurations () const
                              { return fCommentLilypondDurations; }

    // notes
    void                  setCommentLilypondNotes ()
                              { fCommentLilypondNotes = true; }
    Bool                  getCommentLilypondNotes () const
                              { return fCommentLilypondNotes; }

    void                  setCommentLilypondkipNotess ()
                              { fCommentLilypondSkipNotes = true; }
    Bool                  getCommentLilypondSkipNotes () const
                              { return fCommentLilypondSkipNotes; }

    void                  setCommentLilypondRestNotes ()
                              { fCommentLilypondRestNotes = true; }
    Bool                  getCommentLilypondRestNotes () const
                              { return fCommentLilypondRestNotes; }

    void                  setCommentLilypondBeams ()
                              { fCommentLilypondBeams = true; }
    Bool                  getCommentLilypondBeams () const
                              { return fCommentLilypondBeams; }

    void                  setCommentLilypondOrnaments ()
                              { fCommentLilypondOrnaments = true; }
    Bool                  getCommentLilypondOrnaments () const
                              { return fCommentLilypondOrnaments; }


    // grace notes
    void                  setCommentLilypondGraceNotes ()
                              { fCommentLilypondGraceNotes = true; }
    Bool                  getCommentLilypondGraceNotes () const
                              { return fCommentLilypondGraceNotes; }

    // chords
    void                  setCommentLilypondChords ()
                              { fCommentLilypondChords = true; }
    Bool                  getCommentLilypondChords () const
                              { return fCommentLilypondChords; }

    // tuplets
    void                  setCommentLilypondTuplets ()
                              { fCommentLilypondTuplets = true; }
    Bool                  getCommentLilypondTuplets () const
                              { return fCommentLilypondTuplets; }

    // ligatures
    void                  setCommentLilypondLigatures ()
                              { fCommentLilypondLigatures = true; }
    Bool                  getCommentLilypondLigatures () const
                              { return fCommentLilypondLigatures; }

    // spanners
    void                  setCommentLilypondSpanners ()
                              { fCommentLilypondSpanners = true; }
    Bool                  getCommentLilypondSpanners () const
                              { return fCommentLilypondSpanners; }

    // harmonies
    void                  setCommentLilypondHarmonies ()
                              { fCommentLilypondHarmonies = true; }
    Bool                  getCommentLilypondHarmonies () const
                              { return fCommentLilypondHarmonies; }

    // figured basses
    void                  setCommentLilypondFiguredBasses ()
                              { fCommentLilypondFiguredBasses = true; }
    Bool                  getCommentLilypondFiguredBasses () const
                              { return fCommentLilypondFiguredBasses; }

    // lyrics
    void                  setCommentLilypondLyrics ()
                              { fCommentLilypondLyrics = true; }
    Bool                  getCommentLilypondLyrics () const
                              { return fCommentLilypondLyrics; }


    void                  setGlobalEmptyVariable ()
                              { fGlobalEmptyVariable = true; }
    Bool                  getGlobalEmptyVariable () const
                              { return fGlobalEmptyVariable; }

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

    void                  setGenerateSlursDirections ()
                              { fGenerateSlursDirections = true; }
    Bool                  getGenerateSlursDirections () const
                              { return fGenerateSlursDirections; }

    void                  setWhiteNoteHeads ()
                              { fWhiteNoteHeads = true; }
    Bool                  getWhiteNoteHeads () const
                              { return fWhiteNoteHeads; }

    void                  setgenerateNoteBeamsAfterNote ()
                              { fgenerateNoteBeamsAfterNote = true; }
    Bool                  getgenerateNoteBeamsAfterNote () const
                              { return fgenerateNoteBeamsAfterNote; }

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

    void                  setLyricsNotesDurationsKind (
                            lpsrLyricsNotesDurationsKind value)
                              { fLyricsNotesDurationsKind = value; }
    lpsrLyricsNotesDurationsKind
                          getLyricsNotesDurationsKind () const
                              { return fLyricsNotesDurationsKind; }

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

    void                  initializeLpsr2LilypondLilypondVersionOptions ();

    void                  initializeLpsr2LilypondScoreNotationOptions ();

    void                  initializeLpsr2LilypondGlobalStaffSizeOptions ();

    void                  initializeLpsr2LilypondIdentificationOptions ();

    void                  initializeLpsr2LilypondVoicesOptions ();

    void                  initializeLpsr2LilypondPartsOptions ();

    void                  initializeLpsr2LilypondEngraversOptions ();

    void                  initializeLpsr2LilypondClefsKeysTimesOptions ();

    void                  initializeLpsr2LilypondNotesOptions ();

    void                  initializeLpsr2LilypondBarsOptions ();

    void                  initializeLpsr2LilypondPageAndLineBreaksOptions ();

    void                  initializeLpsr2LilypondStavesOptions ();

    void                  initializeLpsr2LilypondChordsOptions ();

    void                  initializeLpsr2LilypondTupletsOptions ();

    void                  initializeLpsr2LilypondRepeatOptions ();

    void                  initializeLpsr2LilypondOrnamentsOptions ();

    void                  initializeLpsr2LilypondChordsDisplayOptions ();

    void                  initializeLpsr2LilypondLyricsOptions ();

    void                  initializeLpsr2LilypondDynamicsOptions ();

    void                  initializeLpsr2LilypondFontsOptions ();

    void                  initializeLpsr2LilypondCodeGenerationOptions ();

    void                  initializeLpsr2LilypondMidiOptions ();

    void                  initializeLpsr2LilypondCombinedOptionsOptions ();

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

    void                  displayLpsr2LilypondOahValues (int fieldWidth);

    virtual void          displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const;

  private:

    // private fields
    // --------------------------------------

    // LilyPond version
    // --------------------------------------

    std::string           fLilypondVersion;
    S_oahStringAtom       fLilypondVersionAtom;

    std::string           fLilypondVersionMinimumValue;

    // score notation
    // --------------------------------------

    // lilypondScoreNotationKind     fScoreNotationKind; JMI ???
    Bool                  fLilyJazz;

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

    std::map <std::string, S_msrSemiTonesPitchAndOctave>
                          fPartNamesTranspositionMap;
    std::map <std::string, S_msrSemiTonesPitchAndOctave>
                          fPartMusicXMLIDsTranspositionMap;

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

    std::map <mfMeasureNumber, mfMeasureNumber>
                          fResetMeasureElementMeasureNumberMap;
    std::map <mfMeasureNumber, int>
                          fAddEmptyMeasuresAfterMeasureNumberMap;

    // bar numbers
    // --------------------------------------

    Bool                  fShowAllBarNumbers;

    std::set <mfMeasureNumber>
                          fShowMeasureNumbersAtMeasureSet;
    S_oahMeasureNumberSetAtom
                          fShowMeasureNumbersAtMeasureAtom;

    float                 fBarNumbersSize;
    S_oahFloatAtom        fBarNumbersSizeAtom;

    Bool                  fBoxAroundAllBarNumbers;
    std::set <mfMeasureNumber>
                          fBoxAroundBarNumberSet;

    // bar number checks
    // --------------------------------------

    Bool                  fNoBarNumberChecks;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fNoBarNumberChecksAtom;
    // this atom is used by the '-minimal' combined option

    // bar check comments
    // --------------------------------------

    S_oahBooleanAtom      fNoBarCheckCommentsAtom;

    Bool                  fNoBarCheckComments;

    // line breaks
    // --------------------------------------

    Bool                  fIgnoreLpsrLineBreaks;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fIgnoreLpsrLineBreaksAtom;

    Bool                  fBreakLinesAtIncompleteRightMeasures;

    int                   fSeparatorLineEveryNMeasures;

    std::set <mfMeasureNumber>
                          fBreakLineAfterMeasureNumberSet;

    // page and line breaks
    // --------------------------------------

    Bool                  fIgnoreLpsrPageBreaks;
    // this atom is used by the '-minimal' combined option
    S_oahBooleanAtom      fIgnoreLpsrPageBreaksAtom;

    std::set <mfMeasureNumber>
                          fBreakPageAfterMeasureNumberSet;


    // staves
    // --------------------------------------

    Bool                  fModernTab;
    Bool                  fTabFullNotation;

    Bool                  fKeepStaffSize;

    msrColorRGB           fLedgerLinesColorRGB; // #(rgb-color 1.0 0.9 0.5)
    S_oahColorRGBAtom     fLedgerLinesColorRGBAtom;


    // multiple measure rests
    // --------------------------------------

    int                   fMultipleMeasureRestsExpandLimit;
    S_oahIntegerAtom      fMultipleMeasureRestsExpandLimitAtom;

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


    Bool                  fAllNotesDurations;

    Bool                  fNoAutoBeaming;
    Bool                  fNoBeams;

    Bool                  fRomanStringNumbers;
    Bool                  fAvoidOpenStrings;

    lpsrAccidentalStyleKind
                          fAccidentalStyleKind;

    Bool                  fCompressMeasureRestsInLilypond;

    Bool                  fMergeStaffCommonRests;

    Bool                  fInputStartLineNumbers;
    S_oahBooleanAtom      fInputStartLineNumbersAtom;

    Bool                  fNotesComments;

    Bool                  fOriginalMeasureNumbers;

    Bool                  fGeneratePositionInMeasures;
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

    mfRational            fDelayedOrnamentsRational;


    // chords display
    // --------------------------------------

    std::list <std::pair <std::string, std::string>>
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

    // structure
    Bool                  fCommentLilypondStructureBasics;
    S_oahBooleanAtom      fCommentLilypondStructureBasicsAtom;

    Bool                  fCommentLilypondStructure;

    // parts, voices & measures
    Bool                  fCommentLilypondPartGroups;
    Bool                  fCommentLilypondVoices;
    Bool                  fCommentLilypondMeasures;

    Bool                  fCommentLilypondMultipleMeasureRests;
    Bool                  fCommentLilypondMeasureRepeats;

    Bool                  fCommentLilypondBarLines;

    // durations
    Bool                  fCommentLilypondDurations;

    // notes
    Bool                  fCommentLilypondNotes;
    Bool                  fCommentLilypondSkipNotes;
    Bool                  fCommentLilypondRestNotes;

    Bool                  fCommentLilypondBeams;
    Bool                  fCommentLilypondOrnaments;

    // grace notes
    Bool                  fCommentLilypondGraceNotes;

    // chords
    Bool                  fCommentLilypondChords;

    // tuplets
    Bool                  fCommentLilypondTuplets;

    // ligatures
    Bool                  fCommentLilypondLigatures;

    // spanners
    Bool                  fCommentLilypondSpanners;

    // harmonies
    Bool                  fCommentLilypondHarmonies;

    // figured basses
    Bool                  fCommentLilypondFiguredBasses;

    // lyrics
    Bool                  fCommentLilypondLyrics;


    Bool                  fGlobalEmptyVariable;

    Bool                  fDisplayMusic;

    Bool                  fNoLilypondCode;

    Bool                  fNoLilypondLyrics;

    Bool                  fLilypondRunDate;

    Bool                  fDraftMode;

    Bool                  fPointAndClickOff;

    Bool                  fGenerateStemsDirections;

    Bool                  fGenerateSlursDirections;

    Bool                  fWhiteNoteHeads;
    Bool                  fgenerateNoteBeamsAfterNote;

    Bool                  fGenerateCommentedOutVariables;
                            // this is to avoid having to add them by hand
                            // in the LilyPond output

//     Bool                  fGenerateMsrVisitingInformation; // JMI 0.9.61
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

    lpsrLyricsNotesDurationsKind
                          fLyricsNotesDurationsKind;

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


#endif // ___lpsr2lilypondOah___

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
                            std::set <std::string>&  S_lilypondAbsoluteOctaveEntryAtomtringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lilypondBreakPageAfterMeasureNumberAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::set <std::string>&  S_lilypondAbsoluteOctaveEntryAtomtringSetVariable);

    virtual               ~lilypondBreakPageAfterMeasureNumberAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::set <std::string>&    getStringSetVariable ()
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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::set <std::string>&          fStringSetVariable;
};
typedef SMARTP<lilypondBreakPageAfterMeasureNumberAtom> S_lilypondBreakPageAfterMeasureNumberAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lilypondBreakPageAfterMeasureNumberAtom& elt);
*/

