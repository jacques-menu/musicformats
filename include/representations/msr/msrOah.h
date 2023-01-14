/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrOah___
#define ___msrOah___

#include "msrPitchesNames.h"
#include "oahAtomsCollection.h"


namespace MusicFormats
{

//_______________________________________________________________________________
// constants
EXP extern const std::string K_MSR_GENERATION_API_KIND_SHORT_NAME;
EXP extern const std::string K_MSR_GENERATION_API_KIND_LONG_NAME;

//______________________________________________________________________________
class EXP msrPitchesLanguageAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrPitchesLanguageAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                               quarterTonesPitchesLanguageKind);
  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPitchesLanguageAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
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

    msrQuarterTonesPitchesLanguageKind&
                          fMsrQuarterTonesPitchesLanguageKindVariable;
};
typedef SMARTP<msrPitchesLanguageAtom> S_msrPitchesLanguageAtom;
EXP std::ostream& operator << (std::ostream& os, const S_msrPitchesLanguageAtom& elt);

//______________________________________________________________________________
class EXP msrRenamePartAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrRenamePartAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::map<std::string, std::string>&
                                               stringToStringMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRenamePartAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::map<std::string, std::string>&
                                               stringToStringMapVariable);

    virtual               ~msrRenamePartAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::map<std::string, std::string>&
                          getStringToStringMapVariable () const
                              { return fStringToStringMapVariable; }

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

    std::map<std::string, std::string>&  fStringToStringMapVariable;
};
typedef SMARTP<msrRenamePartAtom> S_msrRenamePartAtom;
EXP std::ostream& operator << (std::ostream& os, const S_msrRenamePartAtom& elt);

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

    Bool                  getUseFilenameAsWorkCreditTypeTitle () const
                              { return fUseFilenameAsWorkCreditTypeTitle; }

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

    void                  setDisplayFirstMsr ()
                              { fDisplayFirstMsr = true; }
    Bool                  getDisplayFirstMsr () const
                              { return fDisplayFirstMsr; }

    void                  setDisplayFirstMsrFull ()
                              { fDisplayFirstMsrFull = true; }
    Bool                  getDisplayFirstMsrFull () const
                              { return fDisplayFirstMsrFull; }

    void                  setDisplayFirstMsrSlices ()
                              { fDisplayFirstMsrSlices = true; }
    Bool                  getDisplayFirstMsrSlices () const
                              { return fDisplayFirstMsrSlices; }

    void                  setDisplaySecondMsr ()
                              { fDisplaySecondMsr = true; }
    Bool                  getDisplaySecondMsr () const
                              { return fDisplaySecondMsr; }

    void                  setDisplaySecondMsrFull ()
                              { fDisplaySecondMsrFull = true; }
    Bool                  getDisplaySecondMsrFull () const
                              { return fDisplaySecondMsrFull; }

    Bool                  getDisplayMsr () const // JMI ???
                              {
                                return
                                  fDisplayFirstMsr || fDisplaySecondMsrFull;
                              }

    void                  setDisplaySecondMsrSlices ()
                              { fDisplaySecondMsrSlices = true; }
    Bool                  getDisplaySecondMsrSlices () const
                              { return fDisplaySecondMsrSlices; }

    Bool                  getDisplayMsrFull () const // JMI ???
                              {
                                return
                                  fDisplayFirstMsr || fDisplaySecondMsrFull;
                              }

    void                  setDisplayFirstMsrNames ()
                              { fDisplayFirstMsrNames = true; }
    Bool                  getDisplayFirstMsrNames () const
                              { return fDisplayFirstMsrNames; }

    void                  setDisplaySecondMsrNames ()
                              { fDisplaySecondMsrNames = true; }
    Bool                  getDisplaySecondMsrNames () const
                              { return fDisplaySecondMsrNames; }

    void                  setDisplayFirstMsrSummary ()
                              { fDisplayFirstMsrSummary = true; }
    Bool                  getDisplayFirstMsrSummary () const
                              { return fDisplayFirstMsrSummary; }

    void                  setDisplaySecondMsrSummary ()
                              { fDisplaySecondMsrSummary = true; }
    Bool                  getDisplaySecondMsrSummary () const
                              { return fDisplaySecondMsrSummary; }


    // languages
    msrQuarterTonesPitchesLanguageKind
                          getMsrQuarterTonesPitchesLanguageKind () const
                              { return fMsrQuarterTonesPitchesLanguageKind; }

    // parts
    const std::map<std::string, std::string>&
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
                            const std::string& language);

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

#ifdef MF_TRACING_IS_ENABLED
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
    Bool                  fUseFilenameAsWorkCreditTypeTitle;

    Bool                  fUseLyricistsAsPoets; // lyricist is unknown to LilyPond

    // durations
    Bool                  fTraceMsrDurations;

    // display
    Bool                  fDisplayPartGroups;

    Bool                  fDisplayMsrSkeleton;

    Bool                  fDisplayFirstMsr;
    Bool                  fDisplayFirstMsrFull;

    Bool                  fDisplaySecondMsr;
    Bool                  fDisplaySecondMsrFull;

    Bool                  fDisplayFirstMsrSlices;
    Bool                  fDisplaySecondMsrSlices;

    Bool                  fDisplayFirstMsrNames;
    Bool                  fDisplaySecondMsrNames;

    Bool                  fDisplayFirstMsrSummary;
    Bool                  fDisplaySecondMsrSummary;

    // languages
    msrQuarterTonesPitchesLanguageKind
                          fMsrQuarterTonesPitchesLanguageKind;

    // parts
    std::map<std::string, std::string>
                          fMsrPartsRenamingMap;

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
EXP std::ostream& operator << (std::ostream& os, const S_msrOahGroup& elt);

EXP extern S_msrOahGroup gGlobalMsrOahGroup;

//______________________________________________________________________________
EXP S_msrOahGroup createGlobalMsrOahGroup ();


}


#endif
