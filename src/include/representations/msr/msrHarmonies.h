#ifndef ___msrHarmonies___
#define ___msrHarmonies___

#include "msrBasicTypes.h"
#include "msrElements.h"
#include "msrMeasureElements.h"
#include "msrNotes.h"
#include "msrTablatures.h"
#include "msrVoices.h"

namespace MusicFormats
{

//______________________________________________________________________________
// pre-declaration
class msrHarmony;
typedef SMARTP<msrHarmony> S_msrHarmony;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrNote;
typedef SMARTP<msrNote> S_msrNote;

//______________________________________________________________________________
class EXP msrHarmonyDegree : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrHarmonyDegreeTypeKind {
      kHarmonyDegreeTypeAdd,
      kHarmonyDegreeTypeAlter,
      kHarmonyDegreeTypeSubstract
    };

    static string harmonyDegreeTypeKindAsString (
      msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);

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
EXP ostream& operator<< (ostream& os, const S_msrHarmonyDegree& elt);

//______________________________________________________________________________
class EXP msrHarmony : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmony> createWithoutVoiceUplink (
                            int                      inputLineNumber,
                            // no harmoniesUpLinkToVoice yet
                            msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
                            msrHarmonyKind           harmonyKind,
                            const string&            harmonyKindText,
                            int                      harmonyInversion,
                            msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
                            const rational&          harmonySoundingWholeNotes,
                            const rational&          harmonyDisplayWholeNotes,
                            int                      harmoniesStaffNumber,
                            msrTupletFactor          harmonyTupletFactor,
                            const rational&          harmonyWholeNotesOffset);

    SMARTP<msrHarmony> createHarmonyNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrHarmony> createHarmonyDeepClone ( // JMI ???
                            S_msrVoice containingVoice);

    // applications API
    // ------------------------------------------------------

    static SMARTP<msrHarmony> createWithVoiceUplink (
                            int                      inputLineNumber,
                            S_msrVoice               harmoniesUpLinkToVoice,
                            msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
                            msrHarmonyKind           harmonyKind,
                            const string&            harmonyKindText,
                            int                      harmonyInversion,
                            msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
                            const rational&          harmonySoundingWholeNotes,
                            const rational&          harmonyDisplayWholeNotes,
                            int                      harmoniesStaffNumber,
                            msrTupletFactor          harmonyTupletFactor,
                            const rational&          harmonyWholeNotesOffset);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHarmony (
                            int                      inputLineNumber,
                            S_msrVoice               harmoniesUpLinkToVoice,
                            msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
                            msrHarmonyKind           harmonyKind,
                            const string&            harmonyKindText,
                            int                      harmonyInversion,
                            msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
                            const rational&          harmonySoundingWholeNotes,
                            const rational&          harmonyDisplayWholeNotes,
                            int                      harmoniesStaffNumber,
                            msrTupletFactor          harmonyTupletFactor,
                            const rational&          harmonyWholeNotesOffset);

    virtual               ~msrHarmony ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
    void                  setUpLinkToHarmonyToNote (S_msrNote note);

    S_msrNote             getUpLinkToHarmonyToNote () const
                             { return fUpLinkToHarmonyToNote; }

    void                  setHarmoniesUpLinkToVoice (
                            S_msrVoice voice)
                              { fHarmoniesUpLinkToVoice = voice; }

    S_msrVoice            getHarmoniesUpLinkToVoice () const
                             { return fHarmoniesUpLinkToVoice; }

    // position in measure
    void                  setMeasureElementPositionInMeasure (
                            const rational& positionInMeasure,
                            const string&   context) override;

    // whole notes
    void                  setHarmonyDisplayWholeNotes (
                            const rational& wholeNotes)
                              { fHarmonyDisplayWholeNotes = wholeNotes; }

    rational              getHarmonyDisplayWholeNotes () const
                              { return fHarmonyDisplayWholeNotes; }

    // offset
    void                  setHarmonyWholeNotesOffset (rational offset)
                              { fHarmonyWholeNotesOffset = offset; }

    rational              getHarmonyWholeNotesOffset () const
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

    msrTupletFactor       getHarmonyTupletFactor () const
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
                            const rational& wholeNotesDelta);

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
    S_msrNote             fUpLinkToHarmonyToNote;
    S_msrVoice            fHarmoniesUpLinkToVoice; // for use in harmonies voices JMI

    // whole notes
    rational              fHarmonyDisplayWholeNotes;

    // offset
    rational              fHarmonyWholeNotesOffset;

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
EXP ostream& operator<< (ostream& os, const S_msrHarmony& elt);


}


#endif
