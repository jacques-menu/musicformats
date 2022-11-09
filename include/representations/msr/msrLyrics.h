#ifndef ___msrLyrics___
#define ___msrLyrics___

#include "msrElements.h"
#include "msrMeasureElements.h"

#include "msrNotes.h"
#include "msrVoices.h"


namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations for classes mutual dependencies
class   msrStanza;
typedef SMARTP<msrStanza> S_msrStanza;

class   msrPart;
typedef SMARTP<msrPart> S_msrPart;

class   msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class   msrNote;
typedef SMARTP<msrNote> S_msrNote;

//______________________________________________________________________________
// data types

// we want to end the line in the LilyPond code at a break
enum class msrSyllableKind {
  kSyllableNone,
  kSyllableSingle,
  kSyllableBegin, kSyllableMiddle, kSyllableEnd,

  kSyllableOnRestNote,
  kSyllableSkipRestNote,
  kSyllableSkipNonRestNote,

  kSyllableMeasureEnd,
  kSyllableLineBreak, kSyllablePageBreak
};

string msrSyllableKindAsString (
  msrSyllableKind syllableKind);

ostream& operator << (ostream& os, const msrSyllableKind& elt);

enum class msrSyllableExtendKind {
  kSyllableExtendNone,
  kSyllableExtendEmpty,
  kSyllableExtendSingle,
  kSyllableExtendStart, kSyllableExtendContinue, kSyllableExtendStop
};

string msrSyllableExtendKindAsString (
  msrSyllableExtendKind syllableExtendKind);

ostream& operator << (ostream& os, const msrSyllableExtendKind& elt);

class EXP msrSyllable : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSyllable> create (
                            int                   inputLineNumber,
                            S_msrMeasure          upLinkToMeasure,
                            msrSyllableKind       syllableKind,
                            msrSyllableExtendKind syllableExtendKind,
                            const string&         syllableStanzaNumber,
                            const Rational&       syllableWholeNotes,
                            msrTupletFactor       syllableTupletFactor,
                            S_msrStanza           syllableUpLinkToStanza);

    SMARTP<msrSyllable> createSyllableNewbornClone (
                            S_msrPart containingPart); // JMI

    SMARTP<msrSyllable> createSyllableDeepClone (
                            S_msrPart containingPart);

    static SMARTP<msrSyllable> createWithNextMeasurePuristNumber ( // JMI superflous??? v0.9.66
                            int                   inputLineNumber,
                            S_msrMeasure          upLinkToMeasure,
                            msrSyllableKind       syllableKind,
                            msrSyllableExtendKind syllableExtendKind,
                            const string&         syllableStanzaNumber,
                            const Rational&       syllableWholeNotes,
                            msrTupletFactor       syllableTupletFactor,
                            S_msrStanza           syllableUpLinkToStanza,
                            int                   syllableNextMeasurePuristNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSyllable (
                            int                   inputLineNumber,
                            S_msrMeasure          upLinkToMeasure,
                            msrSyllableKind       syllableKind,
                            msrSyllableExtendKind syllableExtendKind,
                            const string&         syllableStanzaNumber,
                            const Rational&       syllableWholeNotes,
                            msrTupletFactor       syllableTupletFactor,
                            S_msrStanza           syllableUpLinkToStanza);

    virtual               ~msrSyllable ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    void                  setSyllableUpLinkToNote (S_msrNote note)
                              { fSyllableUpLinkToNote = note; }

    S_msrNote             getSyllableUpLinkToNote () const
                              { return fSyllableUpLinkToNote; }

    void                  setSyllableUpLinkToStanza (S_msrStanza stanza)
                              { fSyllableUpLinkToStanza = stanza; }

    S_msrStanza           getSyllableUpLinkToStanza () const
                              { return fSyllableUpLinkToStanza; }

//     // position in measure
//     void                  setMeasureElementMeasurePosition (
//                             const S_msrMeasure measure,
//                             const Rational&    measurePosition,
//                             const string&      context) override
//                               {
//                                 setSyllableMeasurePosition (
//                                   measure,
//                                   measurePosition,
//                                   context);
//                               }
//
//     void                  setSyllableMeasurePosition (
//                             const S_msrMeasure measure,
//                             const Rational&    measurePosition,
//                             const string&      context);

    // syllable kind
    msrSyllableKind       getSyllableKind () const
                              { return fSyllableKind; }

    // texts list
    const list<string>&   getSyllableTextsList ()
                              { return fSyllableTextsList; }

    // extend kind
    msrSyllableExtendKind getSyllableExtendKind () const
                              { return fSyllableExtendKind; }

    // stanza number
    string                getSyllableStanzaNumber () const
                              { return fSyllableStanzaNumber; }

    // syllable whole notes
    Rational              getSyllableWholeNotes () const
                              { return fSyllableWholeNotes; }

    // syllable tuplet factor
    msrTupletFactor       getSyllableTupletFactor () const
                              { return fSyllableTupletFactor; }

    // syllable next measure purist number
    void                  setSyllableNextMeasurePuristNumber (
                            int puristMeasureNumber);

    int                   getSyllableNextMeasurePuristNumber () const
                              { return fSyllableNextMeasurePuristNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendSyllableToNoteAndSetItsUpLinkToNote (
                            S_msrNote note);

    void                  appendLyricTextToSyllable (const string& text);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                syllableUpLinkToNoteAsString () const;

    string                syllableWholeNotesAsMsrString () const;

    string                syllableTextsListAsString () const;

    static void           writeTextsList (
                            const list<string>& textsList,
                            ostream&            os);

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrNote             fSyllableUpLinkToNote;
    S_msrStanza           fSyllableUpLinkToStanza; // for use in stanzas

    // syllable kind
    msrSyllableKind       fSyllableKind;

    // texts list
    list<string>          fSyllableTextsList;

    // extend kind
    msrSyllableExtendKind fSyllableExtendKind;

    // stanza number, may contain non-digits
    string                fSyllableStanzaNumber;

    // syllable whole notes
    Rational              fSyllableWholeNotes;

    // syllable tuplet factor
    msrTupletFactor       fSyllableTupletFactor;

    // syllable complementary measure number
    // for kSyllableMeasureEnd, kSyllableLineBreak and kSyllablePageBreak
    int                   fSyllableNextMeasurePuristNumber;
};
typedef SMARTP<msrSyllable> S_msrSyllable;
EXP ostream& operator << (ostream& os, const S_msrSyllable& elt);

//______________________________________________________________________________
class EXP msrStanza : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    static const string K_NO_STANZA_NUMBER;
    static const string K_NO_STANZA_NAME;

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStanza> create (
                            int           inputLineNumber,
                            const string& stanzaNumber,
                            S_msrVoice    stanzaUpLinkToVoice);

    SMARTP<msrStanza> createStanzaNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrStanza> createStanzaDeepClone (
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrStanza (
                            int           inputLineNumber,
                            const string& stanzaNumber,
                            S_msrVoice    stanzaUpLinkToVoice);

  public:

    virtual               ~msrStanza ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  initializeStanza ();

  public:

    // set and get
    // ------------------------------------------------------

    // number
    string                getStanzaNumber () const
                              { return fStanzaNumber; }

    // name
    string                getStanzaName () const
                              { return fStanzaName; }

    // contents
    const vector<S_msrSyllable>&
                          getSyllables () const
                              { return fSyllables; }

    void                  setStanzaTextPresent ()
                              { fStanzaTextPresent = true; }

    Bool                  getStanzaTextPresent () const
                              { return fStanzaTextPresent; }

    // upLinks
    S_msrVoice            getStanzaUpLinkToVoice () const
                              { return fStanzaUpLinkToVoice; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendSyllableToStanza (
                            S_msrSyllable syllable);

    S_msrSyllable         appendRestSyllableToStanza (
                            int             inputLineNumber,
                            const Rational& wholeNotes);

    S_msrSyllable         appendSkipSyllableToStanza (
                            int             inputLineNumber,
                            const Rational& wholeNotes);

    S_msrSyllable         appendMeasureEndSyllableToStanza (
                            int inputLineNumber);

    S_msrSyllable         appendMelismaSyllableToStanza (
                            int             inputLineNumber,
                            msrSyllableKind syllableKind,
                            const Rational& wholeNote);

/* JMI
    S_msrSyllable         appendTiedSyllableToStanza (
                            int      inputLineNumber,
                            const Rational& wholeNotes);

    S_msrSyllable         appendSlurSyllableToStanza (
                            int      inputLineNumber,
                            const Rational& wholeNotes);

    S_msrSyllable         appendSlurBeyondEndSyllableToStanza (
                            int      inputLineNumber,
                            const Rational& wholeNotes);

    S_msrSyllable         appendLigatureSyllableToStanza (
                            int      inputLineNumber,
                            const Rational& wholeNotes);

    S_msrSyllable         appendLigatureBeyondEndSyllableToStanza (
                            int      inputLineNumber,
                            const Rational& wholeNotes);

    S_msrSyllable         appendBarNumberCheckSyllableToStanza (
                            int           inputLineNumber,
                            const string& nextMeasureNumber);

    S_msrSyllable         appendBarcheckSyllableToStanza (
                            int           inputLineNumber,
                            const string& nextMeasureNumber);
*/

    S_msrSyllable         appendLineBreakSyllableToStanza (
                            int inputLineNumber,
                            int nextMeasurePuristNumber);

    S_msrSyllable         appendPageBreakSyllableToStanza (
                            int inputLineNumber,
                            int nextMeasurePuristNumber);

    void                  padUpToCurrentMeasureWholeNotesDurationInStanza ( // JMI
                            int             inputLineNumber,
                            const Rational& wholeNotes);

    void                  appendPaddingNoteToStanza (
                            int             inputLineNumber,
                            const Rational& forwardStepLength);

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

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fStanzaUpLinkToVoice;

    // number
    // The lyric number indicates multiple lines,
    // though a name can be used as well (as in Finale's verse/chorus/section specification)
    string                fStanzaNumber;

    // name
    string                fStanzaName;

    // contents
    vector<S_msrSyllable> fSyllables;

    Bool                  fStanzaTextPresent;

    // current measure whole notes
    Rational              fStanzaCurrentMeasureWholeNotesDuration;
};
typedef SMARTP<msrStanza> S_msrStanza;
EXP ostream& operator << (ostream& os, const S_msrStanza& elt);


}


#endif
