/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrLyrics___
#define ___msrLyrics___

#include <list>
#include <vector>

#include "msrElements.h"
#include "msrMeasureElements.h"

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

enum class msrSyllableExtendTypeKind {
  kSyllableExtendType_NONE,
  kSyllableExtendTypeEmpty,
  kSyllableExtendTypeStart, kSyllableExtendTypeContinue, kSyllableExtendTypeStop
};

std::string msrSyllableExtendTypeKindAsString (
  msrSyllableExtendTypeKind syllableExtendKind);

std::ostream& operator << (std::ostream& os, const msrSyllableExtendTypeKind& elt);

//______________________________________________________________________________
class EXP msrSyllable : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSyllable> create (
                            int                       inputLineNumber,
                            const S_msrMeasure&       upLinkToMeasure,
                            msrSyllableKind           syllableKind,
                            msrSyllableExtendTypeKind syllableExtendKind,
                            const std::string&        syllableStanzaNumber,
                            const msrWholeNotes&      syllableWholeNotes,
                            const msrTupletFactor&    syllableTupletFactor,
                            const S_msrStanza&        syllableUpLinkToStanza);

    static SMARTP<msrSyllable> create (
                            int                       inputLineNumber,
                            msrSyllableKind           syllableKind,
                            msrSyllableExtendTypeKind syllableExtendKind,
                            const std::string&        syllableStanzaNumber,
                            const msrWholeNotes&      syllableWholeNotes,
                            const msrTupletFactor&    syllableTupletFactor,
                            const S_msrStanza&        syllableUpLinkToStanza);

    SMARTP<msrSyllable> createSyllableNewbornClone (
                            const S_msrPart& containingPart); // JMI

    SMARTP<msrSyllable> createSyllableDeepClone (
                            const S_msrPart& containingPart);

//     static SMARTP<msrSyllable> createWithNextMeasurePuristNumber ( // JMI superflous v0.9.68
//                             int                       inputLineNumber,
//                             const S_msrMeasure&       upLinkToMeasure,
//                             msrSyllableKind           syllableKind,
//                             msrSyllableExtendTypeKind syllableExtendKind,
//                             const std::string&        syllableStanzaNumber,
//                             const msrWholeNotes&      syllableWholeNotes,
//                             const msrTupletFactor&    syllableTupletFactor,
//                             const S_msrStanza&        syllableUpLinkToStanza,
//                             int                       syllableNextMeasurePuristNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSyllable (
                            int                       inputLineNumber,
                            const S_msrMeasure&       upLinkToMeasure,
                            msrSyllableKind           syllableKind,
                            msrSyllableExtendTypeKind syllableExtendKind,
                            const std::string&        syllableStanzaNumber,
                            const msrWholeNotes&      syllableWholeNotes,
                            const msrTupletFactor&    syllableTupletFactor,
                            const S_msrStanza&        syllableUpLinkToStanza);

    virtual               ~msrSyllable ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to measure
    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override
                              { setSyllableUpLinkToMeasure (measure); }

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override
                            { upLinkToMeasure = getSyllableUpLinkToMeasure (); }

    void                  setSyllableUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getSyllableUpLinkToMeasure () const
                            { return fSyllableUpLinkToMeasure; }

    // upLinks
    void                  setSyllableUpLinkToNote (const S_msrNote& note)
                              { fSyllableUpLinkToNote = note; }

    S_msrNote             getSyllableUpLinkToNote () const
                              { return fSyllableUpLinkToNote; }

    void                  setSyllableUpLinkToStanza (const S_msrStanza& stanza)
                              { fSyllableUpLinkToStanza = stanza; }

    S_msrStanza           getSyllableUpLinkToStanza () const
                              { return fSyllableUpLinkToStanza; }

    // syllable kind
    msrSyllableKind       getSyllableKind () const
                              { return fSyllableKind; }

    // texts list
    const std::list<std::string>&
                          getSyllableTextsList ()
                              { return fSyllableTextsList; }

    // extend kind
    msrSyllableExtendTypeKind getSyllableExtendKind () const
                              { return fSyllableExtendKind; }

    // stanza number
    std::string           getSyllableStanzaNumber () const
                              { return fSyllableStanzaNumber; }

    // syllable whole notes
    msrWholeNotes         getSyllableWholeNotes () const
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
                            const S_msrNote& note);

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

    S_msrMeasure          fSyllableUpLinkToMeasure;

    // upLinks
    S_msrNote             fSyllableUpLinkToNote;
    S_msrStanza           fSyllableUpLinkToStanza; // for use in stanzas

    // syllable kind
    msrSyllableKind       fSyllableKind;

    // texts list
    std::list<std::string>
                          fSyllableTextsList;

    // extend kind
    msrSyllableExtendTypeKind fSyllableExtendKind;

    // stanza number, may contain non-digits
    std::string           fSyllableStanzaNumber;

    // syllable whole notes
    msrWholeNotes         fSyllableWholeNotes;

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

    static const std::string K_STANZA_NUMBER_UNKNOWN_;
    static const std::string K_STANZA_NAME_UNKNOWN_;

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStanza> create (
                            int                inputLineNumber,
                            const std::string& stanzaNumber,
                            const S_msrVoice&  stanzaUpLinkToVoice);

    SMARTP<msrStanza> createStanzaNewbornClone (
                            const S_msrVoice& containingVoice);

    SMARTP<msrStanza> createStanzaDeepClone (
                            const S_msrVoice& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrStanza (
                            int                inputLineNumber,
                            const std::string& stanzaNumber,
                            const S_msrVoice&  stanzaUpLinkToVoice);

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

    // names
    std::string           getStanzaName () const
                              { return fStanzaName; }

    std::string           getStanzaAlphabeticName () const
                              { return fStanzaAlphabeticName; }

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
                            const S_msrSyllable& syllable);

//     S_msrSyllable         appendRestSyllableToStanza (
//                             int             inputLineNumber,
//                             const msrWholeNotes& wholeNotes);
//
//     S_msrSyllable         appendSkipSyllableToStanza (
//                             int             inputLineNumber,
//                             const msrWholeNotes& wholeNotes);

    S_msrSyllable         appendMeasureEndSyllableToStanza (
                            int inputLineNumber);

//     S_msrSyllable         appendMelismaSyllableToStanza (
//                             int             inputLineNumber,
//                             msrSyllableKind syllableKind,
//                             const msrWholeNotes& wholeNote);

/* JMI
    S_msrSyllable         appendTiedSyllableToStanza (
                            int      inputLineNumber,
                            const msrWholeNotes& wholeNotes);

    S_msrSyllable         appendSlurSyllableToStanza (
                            int      inputLineNumber,
                            const msrWholeNotes& wholeNotes);

    S_msrSyllable         appendSlurBeyondEndSyllableToStanza (
                            int      inputLineNumber,
                            const msrWholeNotes& wholeNotes);

    S_msrSyllable         appendLigatureSyllableToStanza (
                            int      inputLineNumber,
                            const msrWholeNotes& wholeNotes);

    S_msrSyllable         appendLigatureBeyondEndSyllableToStanza (
                            int      inputLineNumber,
                            const msrWholeNotes& wholeNotes);

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

//     void                  padUpToMeasureCurrentWholeNotesDurationInStanza ( // JMI v0.9.68
//                             int                  inputLineNumber,
//                             const msrWholeNotes& wholeNotes);

    void                  appendPaddingNoteToStanza (
                            int                  inputLineNumber,
                            const msrWholeNotes& forwardStepLength);

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

    // names
    std::string           fStanzaName;
    std::string           fStanzaAlphabeticName;

    // contents
    std::vector<S_msrSyllable>
                          fSyllables;

    Bool                  fStanzaTextPresent;

    // measure whole notes
    msrWholeNotes         fStanzaMeasureCurrentWholeNotesDuration;
};
typedef SMARTP<msrStanza> S_msrStanza;
EXP std::ostream& operator << (std::ostream& os, const S_msrStanza& elt);


}


#endif // ___msrLyrics___
