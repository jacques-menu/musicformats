/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrLyrics___
#define ___msrLyrics___

#include <list>
#include <vector>

#include "msrElements.h"
#include "msrMeasureElementLambda.h"

#include "msrTupletFactors.h"
#include "msrVoices.h"


namespace MusicFormats
{

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

std::string msrSyllableKindAsString (
  msrSyllableKind syllableKind);

std::ostream& operator << (std::ostream& os, const msrSyllableKind& elt);

enum class msrSyllableExtendKind {
  kSyllableExtendNone,
  kSyllableExtendEmpty,
  kSyllableExtendSingle,
  kSyllableExtendStart, kSyllableExtendContinue, kSyllableExtendStop
};

std::string msrSyllableExtendKindAsString (
  msrSyllableExtendKind syllableExtendKind);

std::ostream& operator << (std::ostream& os, const msrSyllableExtendKind& elt);

//______________________________________________________________________________
class EXP msrSyllable : public msrMeasureElementLambda
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSyllable> create (
                            int                   inputLineNumber,
                            S_msrMeasure&         upLinkToMeasure,
                            msrSyllableKind       syllableKind,
                            msrSyllableExtendKind syllableExtendKind,
                            const std::string&    syllableStanzaNumber,
                            const Rational&       syllableWholeNotes,
                            msrTupletFactor       syllableTupletFactor,
                            S_msrStanza           syllableUpLinkToStanza);

    static SMARTP<msrSyllable> create (
                            int                   inputLineNumber,
                            msrSyllableKind       syllableKind,
                            msrSyllableExtendKind syllableExtendKind,
                            const std::string&    syllableStanzaNumber,
                            const Rational&       syllableWholeNotes,
                            msrTupletFactor       syllableTupletFactor,
                            S_msrStanza           syllableUpLinkToStanza);

    SMARTP<msrSyllable> createSyllableNewbornClone (
                            S_msrPart containingPart); // JMI

    SMARTP<msrSyllable> createSyllableDeepClone (
                            S_msrPart containingPart);

    static SMARTP<msrSyllable> createWithNextMeasurePuristNumber ( // JMI superflous??? v0.9.66
                            int                   inputLineNumber,
                            S_msrMeasure&         upLinkToMeasure,
                            msrSyllableKind       syllableKind,
                            msrSyllableExtendKind syllableExtendKind,
                            const std::string&    syllableStanzaNumber,
                            const Rational&       syllableWholeNotes,
                            msrTupletFactor       syllableTupletFactor,
                            S_msrStanza           syllableUpLinkToStanza,
                            int                   syllableNextMeasurePuristNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSyllable (
                            int                   inputLineNumber,
                            S_msrMeasure&         upLinkToMeasure,
                            msrSyllableKind       syllableKind,
                            msrSyllableExtendKind syllableExtendKind,
                            const std::string&    syllableStanzaNumber,
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
//                             const S_msrMeasure& measure,
//                             const Rational&    measurePosition,
//                             const std::string& context) override
//                               {
//                                 setSyllableMeasurePosition (
//                                   measure,
//                                   measurePosition,
//                                   context);
//                               }
//
//     void                  setSyllableMeasurePosition (
//                             const S_msrMeasure& measure,
//                             const Rational&    measurePosition,
//                             const std::string& context);

    // syllable kind
    msrSyllableKind       getSyllableKind () const
                              { return fSyllableKind; }

    // texts std::list
    const std::list<std::string>&   getSyllableTextsList ()
                              { return fSyllableTextsList; }

    // extend kind
    msrSyllableExtendKind getSyllableExtendKind () const
                              { return fSyllableExtendKind; }

    // stanza number
    std::string           getSyllableStanzaNumber () const
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

    void                  appendLyricTextToSyllable (const std::string& text);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           syllableUpLinkToNoteAsString () const;

    std::string           syllableWholeNotesAsMsrString () const;

    std::string           syllableTextsListAsString () const;

    static void           writeTextsList (
                            const std::list<std::string>& textsList,
                            std::ostream&                 os);

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrNote             fSyllableUpLinkToNote;
    S_msrStanza           fSyllableUpLinkToStanza; // for use in stanzas

    // syllable kind
    msrSyllableKind       fSyllableKind;

    // texts std::list
    std::list<std::string>          fSyllableTextsList;

    // extend kind
    msrSyllableExtendKind fSyllableExtendKind;

    // stanza number, may contain non-digits
    std::string           fSyllableStanzaNumber;

    // syllable whole notes
    Rational              fSyllableWholeNotes;

    // syllable tuplet factor
    msrTupletFactor       fSyllableTupletFactor;

    // syllable complementary measure number
    // for kSyllableMeasureEnd, kSyllableLineBreak and kSyllablePageBreak
    int                   fSyllableNextMeasurePuristNumber;
};
typedef SMARTP<msrSyllable> S_msrSyllable;
EXP std::ostream& operator << (std::ostream& os, const S_msrSyllable& elt);

//______________________________________________________________________________
class EXP msrStanza : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    static const std::string K_NO_STANZA_NUMBER;
    static const std::string K_NO_STANZA_NAME;

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStanza> create (
                            int           inputLineNumber,
                            const std::string& stanzaNumber,
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
                            const std::string& stanzaNumber,
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
    std::string           getStanzaNumber () const
                              { return fStanzaNumber; }

    // name
    std::string           getStanzaName () const
                              { return fStanzaName; }

    // contents
    const std::vector<S_msrSyllable>&
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
                            const std::string& nextMeasureNumber);

    S_msrSyllable         appendBarcheckSyllableToStanza (
                            int           inputLineNumber,
                            const std::string& nextMeasureNumber);
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

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fStanzaUpLinkToVoice;

    // number
    // The lyric number indicates multiple lines,
    // though a name can be used as well (as in Finale's verse/chorus/section specification)
    std::string           fStanzaNumber;

    // name
    std::string           fStanzaName;

    // contents
    std::vector<S_msrSyllable> fSyllables;

    Bool                  fStanzaTextPresent;

    // current measure whole notes
    Rational              fStanzaCurrentMeasureWholeNotesDuration;
};
typedef SMARTP<msrStanza> S_msrStanza;
EXP std::ostream& operator << (std::ostream& os, const S_msrStanza& elt);


}


#endif
