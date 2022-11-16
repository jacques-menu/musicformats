/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrHarmonies___
#define ___msrHarmonies___

#include <map>
#include <vector>

#include "msrElements.h"
#include "msrMeasureElements.h"

#include "msrTypesForwardDeclarations.h"

#include "msrMeasures.h"

#include "msrDurations.h"
#include "msrIntervals.h"
// #include "msrNotes.h"
#include "msrTupletFactors.h"
#include "msrVoices.h"


namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations for mutual class dependencies

class   msrHarmony;
typedef SMARTP<msrHarmony> S_msrHarmony;

// harmonies
//______________________________________________________________________________
enum class msrHarmonyKind {
  kHarmony_NO_,

  // MusicXML harmonies

  kHarmonyMajor, kHarmonyMinor,
  kHarmonyAugmented, kHarmonyDiminished,

  kHarmonyDominant,
  kHarmonyMajorSeventh, kHarmonyMinorSeventh,
  kHarmonyDiminishedSeventh, kHarmonyAugmentedSeventh,
  kHarmonyHalfDiminished,
  kHarmonyMinorMajorSeventh,

  kHarmonyMajorSixth, kHarmonyMinorSixth,

  kHarmonyDominantNinth,
  kHarmonyMajorNinth, kHarmonyMinorNinth,

  kHarmonyDominantEleventh, kHarmonyMajorEleventh, kHarmonyMinorEleventh,

  kHarmonyDominantThirteenth, kHarmonyMajorThirteenth, kHarmonyMinorThirteenth,

  kHarmonySuspendedSecond, kHarmonySuspendedFourth,

  kHarmonyNeapolitan, kHarmonyItalian, kHarmonyFrench, kHarmonyGerman,

  kHarmonyPedal, kHarmonyPower, kHarmonyTristan,

  // jazz-specific harmonies

  kHarmonyMinorMajorNinth,                       // -maj9, minmaj9

  kHarmonyDominantSuspendedFourth,               // 7sus4, domsus4
  kHarmonyDominantAugmentedFifth,                // 7#5, domaug5
  kHarmonyDominantMinorNinth,                    // 7b9, dommin9
  kHarmonyDominantAugmentedNinthDiminishedFifth, // 7#9b5, domaug9dim5
  kIntervkDominantAugmentedNinthAugmentedFifthHarmonyalAugmentedThirteenth,  // 7#9#5, domaug9aug5
  kHarmonyDominantAugmentedEleventh,             // 7#11, domaug11

  kHarmonyMajorSeventhAugmentedEleventh,         // maj7#11, maj7aug11

  // other

  kHarmonyOther,

  kHarmonyNone,

  // aliases
  // ------------------------------------

  AllFirst = kHarmony_NO_,
  AllLast  = kHarmonyNone,

  TrueHarmoniesFirst = kHarmonyMajor,
  TrueHarmoniesLast  = kHarmonyMajorSeventhAugmentedEleventh
};

std::string msrHarmonyKindAsString (
  msrHarmonyKind harmonyKind);

std::string msrHarmonyKindAsShortString (
  msrHarmonyKind harmonyKind);

std::ostream& operator << (std::ostream& os, const msrHarmonyKind& elt);

std::string msrHarmonyKindShortName (
  msrHarmonyKind harmonyKind);

EXP msrHarmonyKind msrHarmonyKindFromString (
  const std::string& theString);

std::string existingHarmonyKinds (size_t namesListMaxLength);
std::string existingHarmonyKindsNames (size_t namesListMaxLength);

// constant
EXP extern const int K_HARMONY_NO_INVERSION;

EXP extern std::map<std::string, msrHarmonyKind>
  gGlobalHarmonyKindsMap;

EXP extern std::list<std::string>
  gHarmonyKindsNamesList;

void initializeHarmonyKinds ();

//________________________________________________________________________
template< typename T >
class   EnumTrueHarmonies
{
  public:

    class   Iterator
    {
      public:

        Iterator (int value)
          : fIterationIndex (value)
            {}

        T operator* (void) const
            { return (T) fIterationIndex; }

        void operator++ (void)
            {  ++fIterationIndex; }

        Bool operator != (Iterator rhs)
            {  return fIterationIndex != rhs.fIterationIndex; }

      private:

        int fIterationIndex;
     };
};

template< typename T >
typename EnumTrueHarmonies<T>::Iterator begin (EnumTrueHarmonies<T>)
{
  return typename EnumTrueHarmonies<T>::Iterator ((int)T::TrueHarmoniesFirst);
}

template< typename T >
typename EnumTrueHarmonies<T>::Iterator end (EnumTrueHarmonies<T>)
{
  return typename EnumTrueHarmonies<T>::Iterator (((int)T::TrueHarmoniesLast) + 1);
}

//______________________________________________________________________________
// data types

enum class msrHarmonyDegreeTypeKind {
  kHarmonyDegreeTypeAdd,
  kHarmonyDegreeTypeAlter,
  kHarmonyDegreeTypeSubstract
};

std::string msrHarmonyDegreeTypeKindAsString (
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);

std::ostream& operator << (std::ostream& os, const msrHarmonyDegreeTypeKind& elt);

//______________________________________________________________________________
class EXP msrHarmonyDegree : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmonyDegree> create (
                            int                      inputLineNumber,
                            int                      harmonyDegreeValue,
                            msrAlterationKind        harmonyDegreeAlterationKind,
                            msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHarmonyDegree (
                            int                      inputLineNumber,
                            int                      harmonyDegreeValue,
                            msrAlterationKind        harmonyDegreeAlterationKind,
                            msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);

    virtual               ~msrHarmonyDegree ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setHarmonyDegreeUpLinkToHarmony (
                            S_msrHarmony UpLinkToHarmony);

    S_msrHarmony          getHarmonyDegreeUpLinkToHarmony () const
                              { return fHarmonyDegreeUpLinkToHarmony; }

    int                   getHarmonyDegreeValue () const
                              { return fHarmonyDegreeValue; }

    msrAlterationKind     getHarmonyDegreeAlterationKind () const
                              { return fHarmonyDegreeAlterationKind; }

    msrHarmonyDegreeTypeKind
                          getHarmonyDegreeTypeKind () const
                              { return fHarmonyDegreeTypeKind; }

  public:

    // public services
    // ------------------------------------------------------

// JMI ???    int                   harmonyDegreeAsSemitones () const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           harmonyDegreeKindAsString () const;
    std::string           harmonyDegreeKindAsShortString () const;

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrHarmony          fHarmonyDegreeUpLinkToHarmony;

    int                   fHarmonyDegreeValue;
    msrAlterationKind     fHarmonyDegreeAlterationKind;
    msrHarmonyDegreeTypeKind
                          fHarmonyDegreeTypeKind;
};
typedef SMARTP<msrHarmonyDegree> S_msrHarmonyDegree;
EXP std::ostream& operator << (std::ostream& os, const S_msrHarmonyDegree& elt);

//______________________________________________________________________________
class EXP msrHarmony : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmony> create (
                            int                      inputLineNumber,
                            S_msrMeasure             upLinkToMeasure,
                            msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
                            msrHarmonyKind           harmonyKind,
                            const std::string&       harmonyKindText,
                            int                      harmonyInversion,
                            msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
                            const Rational&          harmonySoundingWholeNotes,
                            const Rational&          harmonyDisplayWholeNotes,
                            int                      harmoniesStaffNumber,
                            const msrTupletFactor&   harmonyTupletFactor,
                            const Rational&          harmonyWholeNotesOffset);

    SMARTP<msrHarmony> createHarmonyNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrHarmony> createHarmonyDeepClone ( // JMI ???
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHarmony (
                            int                      inputLineNumber,
                            S_msrMeasure             upLinkToMeasure,
                            msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
                            msrHarmonyKind           harmonyKind,
                            const std::string&       harmonyKindText,
                            int                      harmonyInversion,
                            msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
                            const Rational&          harmonySoundingWholeNotes,
                            const Rational&          harmonyDisplayWholeNotes,
                            int                      harmoniesStaffNumber,
                            const msrTupletFactor&   harmonyTupletFactor,
                            const Rational&          harmonyWholeNotesOffset);

    virtual               ~msrHarmony ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to measure
    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override
                              { setHarmonyUpLinkToMeasure (measure); }

    S_msrMeasure          getMeasureElementUpLinkToMeasure () const override
                            { return getHarmonyUpLinkToMeasure (); }

    void                  setHarmonyUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getHarmonyUpLinkToMeasure () const
                            { return fHarmonyUpLinkToMeasure; }

    // uplinks
    void                  setHarmonyUpLinkToNote (S_msrNote note);

    S_msrNote             getHarmonyUpLinkToNote () const
                             { return fHarmonyUpLinkToNote; }

    void                  setHarmoniesUpLinkToVoice (
                            S_msrVoice voice)
                              { fHarmoniesUpLinkToVoice = voice; }

    S_msrVoice            getHarmoniesUpLinkToVoice () const
                             { return fHarmoniesUpLinkToVoice; }

    // whole notes
    void                  setHarmonyDisplayWholeNotes (
                            const Rational& wholeNotes)
                              { fHarmonyDisplayWholeNotes = wholeNotes; }

    Rational              getHarmonyDisplayWholeNotes () const
                              { return fHarmonyDisplayWholeNotes; }

    // offset
    void                  setHarmonyWholeNotesOffset (Rational offset)
                              { fHarmonyWholeNotesOffset = offset; }

    Rational              getHarmonyWholeNotesOffset () const
                              { return fHarmonyWholeNotesOffset; }

    // root
    msrQuarterTonesPitchKind
                          getHarmonyRootQuarterTonesPitchKind () const
                              { return fHarmonyRootQuarterTonesPitchKind; }

    // kind
    msrHarmonyKind        getHarmonyKind () const
                              { return fHarmonyKind; }

    std::string           getHarmonyKindText () const
                              { return fHarmonyKindText; }

    // degrees
    const std::list<S_msrHarmonyDegree>&
                          getHarmonyDegreesList () const
                              { return fHarmonyDegreesList; }

    // inversion
    int                   getHarmonyInversion () const
                              { return fHarmonyInversion; }

    // bass
    msrQuarterTonesPitchKind
                          getHarmonyBassQuarterTonesPitchKind () const
                              { return fHarmonyBassQuarterTonesPitchKind; }

    // staff
    void                  setHarmoniesStaffNumber (
                            int staffNumber)
                              { fHarmoniesStaffNumber = staffNumber; }

    int                   getHarmoniesStaffNumber () const
                             { return fHarmoniesStaffNumber; }

    // tuplet factor
    void                  setHarmonyTupletFactor (
                            const msrTupletFactor& tupletFactor);

    const msrTupletFactor&
                          getHarmonyTupletFactor () const
                              { return fHarmonyTupletFactor; }

    // frame
    void                  setHarmonyFrame (
                            S_msrFrame frame);

    const S_msrFrame&     getHarmonyFrame () const
                              { return fHarmonyFrame; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendHarmonyDegreeToHarmony (
                            S_msrHarmonyDegree harmonyDegree)
                              {
                                fHarmonyDegreesList.push_back (
                                  harmonyDegree);
                              }

    void                  incrementHarmonySoundingWholeNotesDuration (
                            int             inputLineNumber,
                            const Rational& wholeNotesDelta);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // uplinks
    S_msrMeasure          fHarmonyUpLinkToMeasure;

    S_msrNote             fHarmonyUpLinkToNote;
    S_msrVoice            fHarmoniesUpLinkToVoice; // for use in harmonies voices JMI

    // whole notes
    Rational              fHarmonyDisplayWholeNotes;

    // offset
    Rational              fHarmonyWholeNotesOffset;

    // root
    msrQuarterTonesPitchKind
                          fHarmonyRootQuarterTonesPitchKind;

    // kind
    msrHarmonyKind        fHarmonyKind;
    std::string           fHarmonyKindText;

    // inversion
    int                   fHarmonyInversion;

    // bass
    msrQuarterTonesPitchKind
                          fHarmonyBassQuarterTonesPitchKind;

    // degrees
    std::list<S_msrHarmonyDegree>
                          fHarmonyDegreesList;

    // staff
    int                   fHarmoniesStaffNumber;

    // tuplet factor
    msrTupletFactor       fHarmonyTupletFactor;

    // frame
    S_msrFrame            fHarmonyFrame;
};
typedef SMARTP<msrHarmony> S_msrHarmony;
EXP std::ostream& operator << (std::ostream& os, const S_msrHarmony& elt);

// harmonies intervals
//______________________________________________________________________________
// PRE-declaration

class   msrHarmonyInterval;
typedef SMARTP<msrHarmonyInterval> S_msrHarmonyInterval;

//______________________________________________________________________________
class EXP msrHarmonyInterval : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmonyInterval> create (
                            msrIntervalKind harmonyIntervalIntervalKind,
                            int             harmonyIntervalRelativeOctave = 0);
                                              // 0: up to the thirteenth,
                                              // no relative octave is needed

    SMARTP<msrHarmonyInterval> createHarmonyIntervalNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHarmonyInterval (
                            msrIntervalKind harmonyIntervalIntervalKind,
                            int             harmonyIntervalRelativeOctave);

    virtual               ~msrHarmonyInterval ();

  public:

    // set and get
    // ------------------------------------------------------

    msrIntervalKind       getHarmonyIntervalIntervalKind () const
                              { return fHarmonyIntervalIntervalKind; }

    void                  incrementHarmonyIntervalRelativeOctave ()
                              { ++fHarmonyIntervalRelativeOctave; }

    void                  decrementHarmonyIntervalRelativeOctave ()
                              { --fHarmonyIntervalRelativeOctave; }

    int                   getHarmonyIntervalRelativeOctave () const
                              { return fHarmonyIntervalRelativeOctave; }

  public:

    // public services
    // ------------------------------------------------------

    int                   harmonyIntervalAsSemitones () const
                              {
                                return
                                  msrIntervalKindAsSemiTones (
                                    fHarmonyIntervalIntervalKind);
                              }

    std::string           harmonyIntervalAsString () const;
    std::string           harmonyIntervalAsShortString () const;

    /*
      we handle intervals up to the thirteenth in harmonies,
      but operations on intervals are easier to compute
      in a normal form limited to an octave, hence:
    */
    void                  normalizeInterval ();
    void                  deNormalizeInterval ();

    S_msrHarmonyInterval  intervalDifference (
                            S_msrHarmonyInterval otherHarmonyInterval);
    S_msrHarmonyInterval  intervalSum (
                            S_msrHarmonyInterval otherHarmonyInterval);

    std::string           asString () const;
    std::string           asShortString () const;

  public:

    // visitors
    // ------------------------------------------------------

/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrIntervalKind       fHarmonyIntervalIntervalKind;

    int                   fHarmonyIntervalRelativeOctave;
};
typedef SMARTP<msrHarmonyInterval> S_msrHarmonyInterval;
EXP std::ostream& operator << (std::ostream& os, const S_msrHarmonyInterval& elt);

// harmonies structure
//______________________________________________________________________________
class   msrHarmonyStructure;
typedef SMARTP<msrHarmonyStructure> S_msrHarmonyStructure;

class EXP msrHarmonyStructure : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmonyStructure> createBare (
                            msrHarmonyKind harmonyStructureHarmonyKind);

    static SMARTP<msrHarmonyStructure> create (
                            msrHarmonyKind harmonyStructureHarmonyKind);

    SMARTP<msrHarmonyStructure> createHarmonyStructureNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHarmonyStructure (
                            msrHarmonyKind harmonyStructureHarmonyKind);

    virtual               ~msrHarmonyStructure ();

  private:

    // private initialization
    // ------------------------------------------------------

  public:

    // set and get
    // ------------------------------------------------------

    msrHarmonyKind        getHarmonyStructureHarmonyKind () const
                              { return fHarmonyStructureHarmonyKind; }

    const std::vector <S_msrHarmonyInterval>&
                          getHarmonyStructureIntervals () const
                              { return fHarmonyStructureIntervals; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendHarmonyIntervalToHarmonyStructure (
                            S_msrHarmonyInterval harmonyInterval);

    void                  populateHarmonyStructure ();

    std::string           harmonyStructureAsString () const;
    std::string           harmonyStructureAsShortString () const;

    S_msrHarmonyInterval    bassHarmonyIntervalForHarmonyInversion (
                            int inputLineNumber,
                            int inversionNumber);

    S_msrHarmonyStructure   invertHarmonyStructure (int inversion);

    static void           printAllHarmoniesStructures (std::ostream& os);

  public:

    // visitors
    // ------------------------------------------------------
/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // ------------------------------------------------------

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrHarmonyKind        fHarmonyStructureHarmonyKind;
    std::vector<S_msrHarmonyInterval>
                          fHarmonyStructureIntervals;
};
typedef SMARTP<msrHarmonyStructure> S_msrHarmonyStructure;
EXP std::ostream& operator << (std::ostream& os, const S_msrHarmonyStructure& elt);

// harmonies contents
//______________________________________________________________________________
class EXP msrHarmonyContents : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmonyContents> create (
                       // JMI     int                   inputLineNumber,
                            msrSemiTonesPitchKind harmonyContentsRootNote,
                            msrHarmonyKind        harmonyContentsHarmonyKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHarmonyContents (
                       // JMI     int                   inputLineNumber,
                            msrSemiTonesPitchKind harmonyContentsRootNote,
                            msrHarmonyKind        harmonyContentsHarmonyKind);

    virtual               ~msrHarmonyContents ();

  public:

    // set and get
    // ------------------------------------------------------

    msrSemiTonesPitchKind
                          getHarmonyContentsRootNote () const
                              { return fHarmonyContentsRootNote; }

    msrHarmonyKind        getHarmonyContentsHarmonyKind () const
                              { return fHarmonyContentsHarmonyKind; }

    const std::vector<S_msrSemiTonesPitchAndOctave>&
                          getHarmonyElementsVector () const
                              { return fHarmonyElementsVector; }

  public:

    // public services
    // ------------------------------------------------------

    std::string           harmonyContentsAsString () const;
    std::string           harmonyContentsAsShortString () const;

    msrSemiTonesPitchKind bassSemiTonesPitchKindForHarmonyInversion (
                            int inputLineNumber,
                            int inversionNumber);

    static void           printAllHarmoniesContents (
                            std::ostream&              os,
                            msrSemiTonesPitchKind semiTonesPitchKind);

  public:

    // visitors
    // ------------------------------------------------------
/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // ------------------------------------------------------

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrSemiTonesPitchKind fHarmonyContentsRootNote;
    msrHarmonyKind        fHarmonyContentsHarmonyKind;

    std::vector<S_msrSemiTonesPitchAndOctave>
                          fHarmonyElementsVector;
};
typedef SMARTP<msrHarmonyContents> S_msrHarmonyContents;
EXP std::ostream& operator << (std::ostream& os, const S_msrHarmonyContents& elt);

// harmonies details and analysis
//______________________________________________________________________________
void printHarmonyDetails (
  std::ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind,
  msrHarmonyKind        harmonyKind);

void printHarmonyAnalysis (
  std::ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind,
  msrHarmonyKind        harmonyKind,
  int                   inversion);

// global variables
//______________________________________________________________________________
extern std::map<msrHarmonyKind, S_msrHarmonyStructure>
  gGlobalHarmonyStructuresMap;

void initializeHarmonyStructuresMap ();

void printHarmonyStructuresMap ();


}


#endif
