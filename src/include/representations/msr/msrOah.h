/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrOah___
#define ___msrOah___

#include "msrPitchesNames.h"
#include "msrBasicTypes.h"

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//_______________________________________________________________________________
// constants
EXP extern const string K_MSR_GENERATION_API_KIND_SHORT_NAME;
EXP extern const string K_MSR_GENERATION_API_KIND_LONG_NAME;

//______________________________________________________________________________
class EXP msrPitchesLanguageAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrPitchesLanguageAtom> create (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                              quarterTonesPitchesLanguageKind);
  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPitchesLanguageAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                              quarterTonesPitchesLanguageKind);

    virtual               ~msrPitchesLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMsrQuarterTonesPitchesLanguageKindVariable (
                            msrQuarterTonesPitchesLanguageKind value)
                              {
                                fMsrQuarterTonesPitchesLanguageKindVariable = value;
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

    msrQuarterTonesPitchesLanguageKind&
                          fMsrQuarterTonesPitchesLanguageKindVariable;
};
typedef SMARTP<msrPitchesLanguageAtom> S_msrPitchesLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_msrPitchesLanguageAtom& elt);

//______________________________________________________________________________
class EXP msrRenamePartAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrRenamePartAtom> create (
                            const string&         longName,
                            const string&         shortName,
                            const string&         description,
                            const string&         valueSpecification,
                            const string&         variableName,
                            map<string, string>&  stringToStringMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRenamePartAtom (
                            const string&         longName,
                            const string&         shortName,
                            const string&         description,
                            const string&         valueSpecification,
                            const string&         variableName,
                            map<string, string>&  stringToStringMapVariable);

    virtual               ~msrRenamePartAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, string>&
                          getStringToStringMapVariable () const
                              { return fStringToStringMapVariable; }

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

    map<string, string>&  fStringToStringMapVariable;
};
typedef SMARTP<msrRenamePartAtom> S_msrRenamePartAtom;
EXP ostream& operator<< (ostream& os, const S_msrRenamePartAtom& elt);

//______________________________________________________________________________
class EXP msrOahGroup : public oahGroup
{
  public:

    static SMARTP<msrOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsrOahGroup ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrOahGroup ();

    virtual               ~msrOahGroup ();

    // set and get
    // ------------------------------------------------------

    // trace
    void                  setTraceMsr ()
                              { fTraceMsr = true; }
    Bool                  getTraceMsr () const
                              { return fTraceMsr; }

    void                  setTraceMsrVisitors ()
                              { fTraceMsrVisitors = true; }
    Bool                  getTraceMsrVisitors () const
                              { return fTraceMsrVisitors; }

    // header

    Bool                  getUseFilenameAsWorkTitle () const
                              { return fUseFilenameAsWorkTitle; }

    Bool                  getUseLyricistsAsPoets () const
                              { return fUseLyricistsAsPoets; }

    // durations
    void                  setTraceMsrDurations ()
                              { fTraceMsrDurations = true; }
    Bool                  getTraceMsrDurations () const
                              { return fTraceMsrDurations; }

    // display
    void                  setDisplayPartGroups ()
                              { fDisplayPartGroups = true; }
    Bool                  getDisplayPartGroups () const
                              { return fDisplayPartGroups; }

    void                  setDisplayMsrSkeleton ()
                              { fDisplayMsrSkeleton = true; }
    Bool                  getDisplayMsrSkeleton () const
                              { return fDisplayMsrSkeleton; }

    void                  setDisplayFirstMsrShort ()
                              { fDisplayFirstMsrShort = true; }
    Bool                  getDisplayFirstMsrShort () const
                              { return fDisplayFirstMsrShort; }

    void                  setDisplayFirstMsrFull ()
                              { fDisplayFirstMsrFull = true; }
    Bool                  getDisplayFirstMsrFull () const
                              { return fDisplayFirstMsrFull; }

    void                  setDisplayFirstMsrSlices ()
                              { fDisplayFirstMsrShortSlices = true; }
    Bool                  getDisplayFirstMsrSlices () const
                              { return fDisplayFirstMsrShortSlices; }

    void                  setDisplaySecondMsrShort ()
                              { fDisplaySecondMsrShort = true; }
    Bool                  getDisplaySecondMsrShort () const
                              { return fDisplaySecondMsrShort; }

    void                  setDisplaySecondMsrFull ()
                              { fDisplaySecondMsrFull = true; }
    Bool                  getDisplaySecondMsrFull () const
                              { return fDisplaySecondMsrFull; }

    Bool                  getDisplayMsr () const // JMI ???
                              {
                                return
                                  fDisplayFirstMsrShort || fDisplaySecondMsrShort;
                              }

    void                  setDisplaySecondMsrSlices ()
                              { fDisplaySecondMsrShortSlices = true; }
    Bool                  getDisplaySecondMsrSlices () const
                              { return fDisplaySecondMsrShortSlices; }

    Bool                  getDisplayMsrFull () const // JMI ???
                              {
                                return
                                  fDisplayFirstMsrFull || fDisplaySecondMsrFull;
                              }

    void                  setDisplayFirstMsrNames ()
                              { fDisplayFirstMsrShortNames = true; }
    Bool                  getDisplayFirstMsrNames () const
                              { return fDisplayFirstMsrShortNames; }

    void                  setDisplaySecondMsrNames ()
                              { fDisplaySecondMsrShortNames = true; }
    Bool                  getDisplaySecondMsrNames () const
                              { return fDisplaySecondMsrShortNames; }

    void                  setDisplayFirstMsrSummary ()
                              { fDisplayFirstMsrShortSummary = true; }
    Bool                  getDisplayFirstMsrSummary () const
                              { return fDisplayFirstMsrShortSummary; }

    void                  setDisplaySecondMsrSummary ()
                              { fDisplaySecondMsrShortSummary = true; }
    Bool                  getDisplaySecondMsrSummary () const
                              { return fDisplaySecondMsrShortSummary; }


    // languages
    msrQuarterTonesPitchesLanguageKind
                          getMsrQuarterTonesPitchesLanguageKind () const
                              { return fMsrQuarterTonesPitchesLanguageKind; }

    // parts
    const map<string, string>&
                          getMsrPartsRenamingMap () const
                              { return fMsrPartsRenamingMap; }

    // staves
    void                  setCreateSingleLineStavesAsRythmic ()
                              { fCreateSingleLineStavesAsRythmic = true; }
    Bool                  getCreateSingleLineStavesAsRythmic () const
                              { return fCreateSingleLineStavesAsRythmic; }


    // voices
    void                  setCreateVoicesStaffRelativeNumbers ()
                              { fCreateVoicesStaffRelativeNumbers = true; }
    Bool                  getCreateVoicesStaffRelativeNumbers () const
                              { return fCreateVoicesStaffRelativeNumbers; }


    // harmonies
    void                  setShowHarmoniesVoices ()
                              { fShowHarmoniesVoices = true; }
    Bool                  getShowHarmoniesVoices () const
                              { return fShowHarmoniesVoices; }


    // figured bass
    void                  setShowFiguredBassVoices ()
                              { fShowFiguredBassVoices = true; }
    Bool                  getShowFiguredBassVoices () const
                              { return fShowFiguredBassVoices; }

    Bool                  setMsrQuarterTonesPitchesLanguage (
                            const string& language);

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMsrtracingOah ();
#endif

    void                  initializeMsrDisplayOptions ();

    void                  initializeMsrLanguagesOptions ();

    void                  initializeMsrHeaderOptions ();

    void                  initializeMsrPartsOptions ();

    void                  initializeMsrStavesOptions ();

    void                  initializeMsrVoicesOptions ();

    void                  initializeMsrHarmoniesOptions ();

    void                  initializeMsrFiguredBassOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsrOahHelp ();

    void                  printMsrOahValues (int valueFieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // trace
    Bool                  fTraceMsr;

    Bool                  fTraceMsrVisitors;

    // header
    Bool                  fUseFilenameAsWorkTitle;

    Bool                  fUseLyricistsAsPoets; // lyricist is unknown to LilyPond

    // durations
    Bool                  fTraceMsrDurations;

    // display
    Bool                  fDisplayPartGroups;

    Bool                  fDisplayMsrSkeleton;

    Bool                  fDisplayFirstMsrShort;
    Bool                  fDisplayFirstMsrFull;

    Bool                  fDisplaySecondMsrShort;
    Bool                  fDisplaySecondMsrFull;

    Bool                  fDisplayFirstMsrShortSlices;
    Bool                  fDisplaySecondMsrShortSlices;

    Bool                  fDisplayFirstMsrShortNames;
    Bool                  fDisplaySecondMsrShortNames;

    Bool                  fDisplayFirstMsrShortSummary;
    Bool                  fDisplaySecondMsrShortSummary;

    // languages
    msrQuarterTonesPitchesLanguageKind
                          fMsrQuarterTonesPitchesLanguageKind;

    // parts
    map<string, string>   fMsrPartsRenamingMap;

    // staves
    Bool                  fCreateSingleLineStavesAsRythmic;

    // voices
    Bool                  fCreateVoicesStaffRelativeNumbers;

    // harmonies
    Bool                  fShowHarmoniesVoices;

    // figured bass
    Bool                  fShowFiguredBassVoices;
};
typedef SMARTP<msrOahGroup> S_msrOahGroup;
EXP ostream& operator<< (ostream& os, const S_msrOahGroup& elt);

EXP extern S_msrOahGroup gGlobalMsrOahGroup;

//______________________________________________________________________________
EXP S_msrOahGroup createGlobalMsrOahGroup ();


}


#endif