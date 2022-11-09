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

#include <vector>

#include "msrElements.h"
#include "msrMeasureElements.h"

#include "msrIntervals.h"
#include "msrNotes.h"
#include "msrOctaves.h"
#include "msrTablatures.h"
#include "msrTupletFactors.h"
#include "msrVoices.h"

namespace MusicFormats
{

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

string msrHarmonyKindAsString (
  msrHarmonyKind harmonyKind);

string msrHarmonyKindAsShortString (
  msrHarmonyKind harmonyKind);

ostream& operator << (ostream& os, const msrHarmonyKind& elt);

string msrHarmonyKindShortName (
  msrHarmonyKind harmonyKind);

EXP msrHarmonyKind msrHarmonyKindFromString (
  const string& theString);

string existingHarmonyKinds (size_t namesListMaxLength);
string existingHarmonyKindsNames (size_t namesListMaxLength);

// constant
EXP extern const int K_HARMONY_NO_INVERSION;

EXP extern map<string, msrHarmonyKind>
  gGlobalHarmonyKindsMap;

EXP extern list<string>
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
// PRE-declarations for classes mutual dependencies
class   msrHarmony;
typedef SMARTP<msrHarmony> S_msrHarmony;

class   msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class   msrNote;
typedef SMARTP<msrNote> S_msrNote;

//______________________________________________________________________________
// data types

enum class msrHarmonyDegreeTypeKind {
  kHarmonyDegreeTypeAdd,
  kHarmonyDegreeTypeAlter,
  kHarmonyDegreeTypeSubstract
};

string msrHarmonyDegreeTypeKindAsString (
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);

ostream& operator << (ostream& os, const msrHarmonyDegreeTypeKind& elt);

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

    string                harmonyDegreeKindAsString () const;
    string                harmonyDegreeKindAsShortString () const;

    string                asString () const override;

    void                  print (ostream& os) const override;

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
EXP ostream& operator << (ostream& os, const S_msrHarmonyDegree& elt);

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
                            const string&            harmonyKindText,
                            int                      harmonyInversion,
                            msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
                            const Rational&          harmonySoundingWholeNotes,
                            const Rational&          harmonyDisplayWholeNotes,
                            int                      harmoniesStaffNumber,
                            msrTupletFactor          harmonyTupletFactor,
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
                            const string&            harmonyKindText,
                            int                      harmonyInversion,
                            msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
                            const Rational&          harmonySoundingWholeNotes,
                            const Rational&          harmonyDisplayWholeNotes,
                            int                      harmoniesStaffNumber,
                            msrTupletFactor          harmonyTupletFactor,
                            const Rational&          harmonyWholeNotesOffset);

    virtual               ~msrHarmony ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
    void                  setHarmonyUpLinkToNote (S_msrNote note);

    S_msrNote             getHarmonyUpLinkToNote () const
                             { return fHarmonyUpLinkToNote; }

    void                  setHarmoniesUpLinkToVoice (
                            S_msrVoice voice)
                              { fHarmoniesUpLinkToVoice = voice; }

    S_msrVoice            getHarmoniesUpLinkToVoice () const
                             { return fHarmoniesUpLinkToVoice; }

//     // position in measure
//     void                  setMeasureElementMeasurePosition (
//                             const S_msrMeasure measure,
//                             const Rational&    measurePosition,
//                             const string&      context) override
//                               {
//                                 setHarmonyMeasurePosition (
//                                   measure,
//                                   measurePosition,
//                                   context);
//                               }
//
//     void                  setHarmonyMeasurePosition (
//                             const S_msrMeasure measure,
//                             const Rational&    measurePosition,
//                             const string&      context);

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

    string                getHarmonyKindText () const
                              { return fHarmonyKindText; }

    // degrees
    const list<S_msrHarmonyDegree>&
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
                            msrTupletFactor tupletFactor);

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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // uplinks
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
    string                fHarmonyKindText;

    // inversion
    int                   fHarmonyInversion;

    // bass
    msrQuarterTonesPitchKind
                          fHarmonyBassQuarterTonesPitchKind;

    // degrees
    list<S_msrHarmonyDegree>
                          fHarmonyDegreesList;

    // staff
    int                   fHarmoniesStaffNumber;

    // tuplet factor
    msrTupletFactor       fHarmonyTupletFactor;

    // frame
    S_msrFrame            fHarmonyFrame;
};
typedef SMARTP<msrHarmony> S_msrHarmony;
EXP ostream& operator << (ostream& os, const S_msrHarmony& elt);

// harmonies intervals
//______________________________________________________________________________
class   msrHarmonyInterval;
typedef SMARTP<msrHarmonyInterval> S_msrHarmonyInterval;

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

    string                harmonyIntervalAsString () const;
    string                harmonyIntervalAsShortString () const;

    /*
      we handle intervals up to the thirteenth in harmonies,
      but operations on intervals are easier to compute
      in a normal form limited to an octave, hence:
    */
    void                  normalizeInterval ();
    void                  deNormalizeInterval ();

    S_msrHarmonyInterval    intervalDifference (
                            S_msrHarmonyInterval otherHarmonyInterval);
    S_msrHarmonyInterval    intervalSum (
                            S_msrHarmonyInterval otherHarmonyInterval);

    string                asString () const;
    string                asShortString () const;

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

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrIntervalKind       fHarmonyIntervalIntervalKind;

    int                   fHarmonyIntervalRelativeOctave;
};
typedef SMARTP<msrHarmonyInterval> S_msrHarmonyInterval;
EXP ostream& operator << (ostream& os, const S_msrHarmonyInterval& elt);

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

    const vector <S_msrHarmonyInterval>&
                          getHarmonyStructureIntervals () const
                              { return fHarmonyStructureIntervals; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendHarmonyIntervalToHarmonyStructure (
                            S_msrHarmonyInterval harmonyInterval);

    void                  populateHarmonyStructure ();

    string                harmonyStructureAsString () const;
    string                harmonyStructureAsShortString () const;

    S_msrHarmonyInterval    bassHarmonyIntervalForHarmonyInversion (
                            int inputLineNumber,
                            int inversionNumber);

    S_msrHarmonyStructure   invertHarmonyStructure (int inversion);

    static void           printAllHarmoniesStructures (ostream& os);

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

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrHarmonyKind        fHarmonyStructureHarmonyKind;
    vector<S_msrHarmonyInterval>
                          fHarmonyStructureIntervals;
};
typedef SMARTP<msrHarmonyStructure> S_msrHarmonyStructure;
EXP ostream& operator << (ostream& os, const S_msrHarmonyStructure& elt);

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

    const vector<S_msrSemiTonesPitchAndOctave>&
                          getHarmonyElementsVector () const
                              { return fHarmonyElementsVector; }

  public:

    // public services
    // ------------------------------------------------------

    string                harmonyContentsAsString () const;
    string                harmonyContentsAsShortString () const;

    msrSemiTonesPitchKind bassSemiTonesPitchKindForHarmonyInversion (
                            int inputLineNumber,
                            int inversionNumber);

    static void           printAllHarmoniesContents (
                            ostream&              os,
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

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrSemiTonesPitchKind fHarmonyContentsRootNote;
    msrHarmonyKind        fHarmonyContentsHarmonyKind;

    vector<S_msrSemiTonesPitchAndOctave>
                          fHarmonyElementsVector;
};
typedef SMARTP<msrHarmonyContents> S_msrHarmonyContents;
EXP ostream& operator << (ostream& os, const S_msrHarmonyContents& elt);

// harmonies details and analysis
//______________________________________________________________________________
void printHarmonyDetails (
  ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind,
  msrHarmonyKind        harmonyKind);

void printHarmonyAnalysis (
  ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind,
  msrHarmonyKind        harmonyKind,
  int                   inversion);

// global variables
//______________________________________________________________________________
extern map<msrHarmonyKind, S_msrHarmonyStructure>
  gGlobalHarmonyStructuresMap;

void initializeHarmonyStructuresMap ();

void printHarmonyStructuresMap ();


}


#endif
