/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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
#include "msrMeasures.h"
#include "msrTupletFactors.h"
#include "msrVoices.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrSyllableElementKind {
  kSyllableElementText,
  kSyllableElementElision
};

std::string msrSyllableElementKindAsString (
  msrSyllableElementKind syllableElementKind);

std::ostream& operator << (std::ostream& os, const msrSyllableElementKind& elt);

enum class msrSyllableKind {
  kSyllableNone,
  kSyllableSingle,
  kSyllableBegin, kSyllableMiddle, kSyllableEnd,

  kSyllableOnRestNote,
  kSyllableSkipOnRestNote,
  kSyllableSkipOnNonRestNote,

  kSyllableMeasureEnd,
  kSyllableLineBreak, kSyllablePageBreak
};

std::string msrSyllableKindAsString (
  msrSyllableKind syllableKind);

std::ostream& operator << (std::ostream& os, const msrSyllableKind& elt);

enum class msrSyllableExtendKind {
  kSyllableExtend_NONE,
  kSyllableExtendTypeLess,
  kSyllableExtendTypeStart, kSyllableExtendTypeContinue, kSyllableExtendTypeStop
};

std::string msrSyllableExtendKindAsString (
  msrSyllableExtendKind syllableExtendKind);

std::ostream& operator << (std::ostream& os, const msrSyllableExtendKind& elt);

//______________________________________________________________________________
class EXP msrSyllableElement : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSyllableElement> create (
                            const mfInputLineNumber& inputStartLineNumber,
                            msrSyllableElementKind   syllableElementKind,
                            const std::string&       syllableElementContents);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSyllableElement (
                            const mfInputLineNumber& inputStartLineNumber,
                            msrSyllableElementKind   syllableElementKind,
                            const std::string&       syllableElementContents);

    virtual               ~msrSyllableElement ();

  public:

    // set and get
    // ------------------------------------------------------

    mfInputLineNumber     getInputLineNumber () const
                              { return fInputLineNumber; }

    msrSyllableElementKind
                          getSyllableElementKind () const
                              { return fSyllableElementKind; }

    const std::string&    getSyllableElementContents () const
                              { return fSyllableElementContents; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

//     void                  acceptIn  (basevisitor* v) override;
//     void                  acceptOut (basevisitor* v) override;
//
//     void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;
    std::string           asShortString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    mfInputLineNumber     fInputLineNumber;

    // texts list
    msrSyllableElementKind
                          fSyllableElementKind;
    std::string           fSyllableElementContents;
};
typedef SMARTP<msrSyllableElement> S_msrSyllableElement;
EXP std::ostream& operator << (std::ostream& os, const S_msrSyllableElement& elt);

//______________________________________________________________________________
std::string syllableElementsListAsString (
  const std::list <msrSyllableElement>& syllableElementsList);

std::string syllableElementsListAsShortString (
  const std::list <msrSyllableElement>& syllableElementsList);

//______________________________________________________________________________
class EXP msrSyllable : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSyllable> create (
                            const mfInputLineNumber& inputLineNumber,
                            msrSyllableKind        syllableKind,
                            msrSyllableExtendKind  syllableExtendKind,
                            const mfStanzaNumber&  syllableStanzaNumber,
                            const mfWholeNotes&    syllableWholeNotes,
                            const msrTupletFactor& syllableTupletFactor);

    static SMARTP<msrSyllable> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure&    upLinkToMeasure,
                            msrSyllableKind        syllableKind,
                            msrSyllableExtendKind  syllableExtendKind,
                            const mfStanzaNumber&  syllableStanzaNumber,
                            const mfWholeNotes&    syllableWholeNotes,
                            const msrTupletFactor& syllableTupletFactor,
                            const S_msrStanza&     syllableUpLinkToStanza);

    static SMARTP<msrSyllable> create (
                            const mfInputLineNumber& inputLineNumber,
                            msrSyllableKind        syllableKind,
                            msrSyllableExtendKind  syllableExtendKind,
                            const mfStanzaNumber&  syllableStanzaNumber,
                            const mfWholeNotes&    syllableWholeNotes,
                            const msrTupletFactor& syllableTupletFactor,
                            const S_msrStanza&     syllableUpLinkToStanza);

    SMARTP<msrSyllable> createSyllableNewbornClone (
                            const S_msrPart& containingPart); // JMI 0.9.67

    SMARTP<msrSyllable> createSyllableDeepClone (
                            const S_msrPart& containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSyllable (
                            const mfInputLineNumber& inputLineNumber,
                            msrSyllableKind        syllableKind,
                            msrSyllableExtendKind  syllableExtendKind,
                            const mfStanzaNumber&  syllableStanzaNumber,
                            const mfWholeNotes&    syllableWholeNotes,
                            const msrTupletFactor& syllableTupletFactor);

                          msrSyllable (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure&    upLinkToMeasure,
                            msrSyllableKind        syllableKind,
                            msrSyllableExtendKind  syllableExtendKind,
                            const mfStanzaNumber&  syllableStanzaNumber,
                            const mfWholeNotes&    syllableWholeNotes,
                            const msrTupletFactor& syllableTupletFactor,
                            const S_msrStanza&     syllableUpLinkToStanza);

    virtual               ~msrSyllable ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    void                  setSyllableUpLinkToNote (const S_msrNote& note);

    const S_msrNote&      getSyllableUpLinkToNote () const
                              { return fSyllableUpLinkToNote; }

    void                  setSyllableUpLinkToStanza (const S_msrStanza& stanza)
                              { fSyllableUpLinkToStanza = stanza; }

    const S_msrStanza&    getSyllableUpLinkToStanza () const
                              { return fSyllableUpLinkToStanza; }

    void                  setSyllableUpLinkToMeasure (const S_msrMeasure& measure)
                              { fSyllableUpLinkToMeasure = measure; }

    const S_msrMeasure&   getSyllableUpLinkToMeasure () const
                              { return fSyllableUpLinkToMeasure; }

    // measure numbers
//     void                  setSyllableMeasureNumber (
//                             const std::string& measureNumber);

    void                  setSyllableMeasurePuristNumber (int measureNumber)
                              { fSyllableMeasurePuristNumber = measureNumber; }

    int                   getSyllableMeasurePuristNumber () const
                              { return fSyllableMeasurePuristNumber; }

    // syllable kind
    void                  setSyllableKind (msrSyllableKind syllableKind)
                              { fSyllableKind = syllableKind; }

    msrSyllableKind       getSyllableKind () const
                              { return fSyllableKind; }

    // extend kind
    void                  setSyllableExtendKind (
                            msrSyllableExtendKind syllableExtendKind)
                              { fSyllableExtendKind = syllableExtendKind; }

    msrSyllableExtendKind getSyllableExtendKind () const
                              { return fSyllableExtendKind; }

    // stanza number
    void                  setSyllableStanzaNumber (
                            const mfStanzaNumber& stanzaNumber)
                              { fSyllableStanzaNumber = stanzaNumber; }

    const mfStanzaNumber& getSyllableStanzaNumber () const
                              { return fSyllableStanzaNumber; }

    // syllable whole notes
    void                  setSyllableWholeNotes (const mfWholeNotes& wholeNotes)
                              { fSyllableWholeNotes = wholeNotes; }

    const mfWholeNotes&   getSyllableWholeNotes () const
                              { return fSyllableWholeNotes; }

    // syllable tuplet factor
    void                  setSyllableTupletFactor (
                            const msrTupletFactor& tupletFactor)
                              { fSyllableTupletFactor = tupletFactor; }

    const msrTupletFactor&
                          getSyllableTupletFactor () const
                              { return fSyllableTupletFactor; }

    // syllable elements list
    const std::list <msrSyllableElement>&
                          getSyllableElementsList ()
                              { return fSyllableElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendSyllableElementToSyllable (
                            const msrSyllableElement& syllableElement);

    int                   fetchSyllableMeasurePuristNumber () const;

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
    std::string           syllableUpLinkToNoteAsShortString () const;

    std::string           syllableWholeNotesPitchAndOctaveAsString () const;

    std::string           asString () const override;
    std::string           asShortString () const override;

    void                  print (std::ostream& os) const override;
//     void                  printFull (
//                             std::ostream& os,
//                             int           theFieldWidth) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrNote             fSyllableUpLinkToNote;
                            // basic attachment

    S_msrStanza           fSyllableUpLinkToStanza;
                            // a stanza is a sequence of syllables

    S_msrMeasure          fSyllableUpLinkToMeasure;
                            // to access next measure's purist number

    int                   fSyllableMeasurePuristNumber;

    // syllable kind
    msrSyllableKind       fSyllableKind;

    // extend kind
    msrSyllableExtendKind fSyllableExtendKind;

    // stanza number, may contain non-digits
    mfStanzaNumber        fSyllableStanzaNumber;

    // syllable whole notes
    mfWholeNotes          fSyllableWholeNotes;

    // syllable tuplet factor
    msrTupletFactor       fSyllableTupletFactor;

    // elements
    std::list <msrSyllableElement>
                          fSyllableElementsList;
};

using S_msrSyllable = SMARTP<msrSyllable>;

EXP std::ostream& operator << (std::ostream& os, const S_msrSyllable& elt);

//______________________________________________________________________________
class EXP msrStanza : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStanza> create (
                            const mfInputLineNumber& inputLineNumber,
                            const mfStanzaNumber& stanzaNumber,
                            const S_msrVoice&  stanzaUpLinkToVoice);

    SMARTP<msrStanza> createStanzaNewbornClone (
                            const S_msrVoice& containingVoice);

    SMARTP<msrStanza> createStanzaDeepClone (
                            const S_msrVoice& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrStanza (
                            const mfInputLineNumber& inputLineNumber,
                            const mfStanzaNumber& stanzaNumber,
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
    mfStanzaNumber        getStanzaNumber () const
                              { return fStanzaNumber; }

    // path-like name
    std::string           getStanzaPathLikeName () const
                              { return fStanzaPathLikeName; }
    // names
    std::string           getStanzaName () const
                              { return fStanzaName; }

    // contents
    const std::vector <S_msrSyllable>&
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
                            const S_msrSyllable&       syllable,
                            const S_msrMeasure&        upLinkToMeasure,
                            const mfPositionInMeasure& partCurrentDrawingPositionInMeasure);

    void                  appendSyllableToStanzaClone (
                            const S_msrSyllable& syllable,
                            const S_msrMeasure&  upLinkToMeasure);

//     S_msrSyllable         appendRestSyllableToStanza (
//                             const mfInputLineNumber& inputLineNumber,
//                             const mfWholeNotes& wholeNotes);
//
//     S_msrSyllable         appendSkipSyllableToStanza (
//                             const mfInputLineNumber& inputLineNumber,
//                             const mfWholeNotes& wholeNotes);

    void                  appendMeasureEndSyllableToStanza (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure&        upLinkToMeasure,
                            const mfPositionInMeasure& partCurrentDrawingPositionInMeasure);

//     S_msrSyllable         appendMelismaSyllableToStanza (
//                             const mfInputLineNumber& inputLineNumber,
//                             msrSyllableKind syllableKind,
//                             const mfWholeNotes& wholeNote);

/* JMI
    S_msrSyllable         appendTiedSyllableToStanza (
                            const mfInputLineNumber& inputLineNumber,
                            const mfWholeNotes& wholeNotes);

    S_msrSyllable         appendSlurSyllableToStanza (
                            const mfInputLineNumber& inputLineNumber,
                            const mfWholeNotes& wholeNotes);

    S_msrSyllable         appendSlurBeyondEndSyllableToStanza (
                            const mfInputLineNumber& inputLineNumber,
                            const mfWholeNotes& wholeNotes);

    S_msrSyllable         appendLigatureSyllableToStanza (
                            const mfInputLineNumber& inputLineNumber,
                            const mfWholeNotes& wholeNotes);

    S_msrSyllable         appendLigatureBeyondEndSyllableToStanza (
                            const mfInputLineNumber& inputLineNumber,
                            const mfWholeNotes& wholeNotes);

    S_msrSyllable         appendBarNumberCheckSyllableToStanza (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& nextMeasureNumber);

    S_msrSyllable         appendBarcheckSyllableToStanza (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& nextMeasureNumber);
*/

    S_msrSyllable         appendLineBreakSyllableToStanza (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    S_msrSyllable         appendPageBreakSyllableToStanza (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

//     void                  padUpToMeasureCurrentPositionInMeasureInStanza ( // JMI 0.9.68
//                             const mfInputLineNumber& inputLineNumber,
//                             const mfWholeNotes& wholeNotes);

    void                  appendPaddingNoteToStanza (
                            const mfInputLineNumber& inputLineNumber,
                            const mfWholeNotes& forwardStepLength);

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
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fStanzaUpLinkToVoice;

    // number
    // The lyric number indicates multiple lines,
    // though a name can be used as well (as in Finale's verse/chorus/section specification)
    mfStanzaNumber        fStanzaNumber;

   // path-like name
    std::string           fStanzaPathLikeName;

   // names
    std::string           fStanzaName;

    // contents
    std::vector <S_msrSyllable>
                          fSyllables;

    Bool                  fStanzaTextPresent;

    // position in measure
    mfPositionInMeasure   fStanzaMeasureCurrentPositionInMeasure;
};

using S_msrStanza = SMARTP<msrStanza>;

EXP std::ostream& operator << (std::ostream& os, const S_msrStanza& elt);


}


#endif // ___msrLyrics___
