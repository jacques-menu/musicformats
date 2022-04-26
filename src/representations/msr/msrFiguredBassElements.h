
#ifndef ___msrFiguredBassElements___
#define ___msrFiguredBassElements___

#include "msrElements.h"
#include "msrMeasureElements.h"
#include "msrNotes.h"
#include "msrParts.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrBassFigure : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrBassFigurePrefixKind {
      k_NoFigurePrefix,
      kDoubleFlatPrefix, kFlatPrefix, kFlatFlatPrefix,
      kNaturalPrefix,
      kSharpSharpPrefix, kSharpPrefix, kDoubleSharpPrefix
    };

    static string figurePrefixKindAsString (
      msrBassFigurePrefixKind figurePrefixKind);

    enum msrBassFigureSuffixKind {
      k_NoFigureSuffix,
      kDoubleFlatSuffix, kFlatSuffix, kFlatFlatSuffix,
      kNaturalSuffix,
      kSharpSharpSuffix, kSharpSuffix, kDoubleSharpSuffix,
      kSlashSuffix
    };

    static string figureSuffixKindAsString (
      msrBassFigureSuffixKind figureSuffixKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBassFigure> create (
      int                 inputLineNumber,
      S_msrPart           figurePartUpLink,
      msrBassFigurePrefixKind figurePrefixKind,
      int                 figureNumber,
      msrBassFigureSuffixKind figureSuffixKind);

    SMARTP<msrBassFigure> createFigureNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrBassFigure> createFigureDeepClone ( // JMI ???
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBassFigure (
      int                 inputLineNumber,
      S_msrPart           figurePartUpLink,
      msrBassFigurePrefixKind figurePrefixKind,
      int                 figureNumber,
      msrBassFigureSuffixKind figureSuffixKind);

    virtual               ~msrBassFigure ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrPart             getFigurePartUpLink () const
                              { return fFigurePartUpLink; }

    msrBassFigurePrefixKind
                          getFigurePrefixKind () const
                              { return fFigurePrefixKind; }

    int                   getFigureNumber () const
                              { return fFigureNumber; }

    msrBassFigureSuffixKind
                          getFigureSuffixKind () const
                              { return fFigureSuffixKind; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                figurePrefixKindAsString () const;
    string                figureSuffixKindAsShortString () const;

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrPart             fFigurePartUpLink;

    msrBassFigurePrefixKind
                          fFigurePrefixKind;
    int                   fFigureNumber;
    msrBassFigureSuffixKind
                          fFigureSuffixKind;
};
typedef SMARTP<msrBassFigure> S_msrBassFigure;
EXP ostream& operator<< (ostream& os, const S_msrBassFigure& elt);

//______________________________________________________________________________
class EXP msrFiguredBassElement : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrFiguredBassElementParenthesesKind {
      kFiguredBassElementParenthesesYes, kFiguredBassElementParenthesesNo
    };

    static string figuredBassElementParenthesesKindAsString (
      msrFiguredBassElementParenthesesKind
        figuredBassElementParenthesesKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFiguredBassElement> create (
                            int inputLineNumber); // JMI ???

    static SMARTP<msrFiguredBassElement> create (
                            int             inputLineNumber,
                            const rational& figuredBassElementSoundingWholeNotes,
                            const rational& figuredBassElementDisplayWholeNotes,
                            msrFiguredBassElementParenthesesKind
                                            figuredBassElementParenthesesKind,
                            msrTupletFactor figuredBassElementTupletFactor);

    SMARTP<msrFiguredBassElement> createFiguredBassElementNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrFiguredBassElement> createFiguredBassElementDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFiguredBassElement (
                            int             inputLineNumber,
                            const rational& figuredBassElementSoundingWholeNotes,
                            const rational& figuredBassElementDisplayWholeNotes,
                            msrFiguredBassElementParenthesesKind
                                            figuredBassElementParenthesesKind,
                            msrTupletFactor figuredBassElementTupletFactor);

    virtual               ~msrFiguredBassElement ();

  public:

    // set and get
    // ------------------------------------------------------
    void                  setFiguredBassNoteUpLink (
                            S_msrNote note)
                              { fFiguredBassElementNoteUpLink = note; }

    S_msrNote             getFiguredBassNoteUpLink () const
                             { return fFiguredBassElementNoteUpLink; }

    void                  setFiguredBassVoiceUpLink (
                            S_msrVoice voice)
                              { fFiguredBassElementVoiceUpLink = voice; }

    S_msrVoice            getFiguredBassVoiceUpLink () const
                             { return fFiguredBassElementVoiceUpLink; }

/* JMI
    S_msrPart             getFiguredBassPartUpLink () const
                              { return fFiguredBassElementPartUpLink; }
*/

    void                  setFiguredBassDisplayWholeNotes (
                            const rational& wholeNotes)
                              {
                                fFiguredBassElementDisplayWholeNotes =
                                  wholeNotes;
                              }

    rational              getFiguredBassDisplayWholeNotes () const
                              { return fFiguredBassElementDisplayWholeNotes; }

    msrFiguredBassElementParenthesesKind
                          getFiguredBassParenthesesKind () const
                              { return fFiguredBassElementParenthesesKind; }

    const list<S_msrBassFigure>&
                          getFiguredBassFiguresList () const
                              { return fFiguredBassElementFiguresList;  }

    void                  setFiguredBassTupletFactor (
                            msrTupletFactor tupletFactor)
                              { fFiguredBassElementTupletFactor = tupletFactor; }

    msrTupletFactor       getFiguredBassTupletFactor () const
                              { return fFiguredBassElementTupletFactor; }

    void                  setFiguredBassPositionInMeasure (
                            const rational& positionInMeasure);

  public:

    // public services
    // ------------------------------------------------------

    void                  appendFigureToFiguredBass (
                            S_msrBassFigure bassFigure);

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

    // upLinks
    S_msrNote             fFiguredBassElementNoteUpLink;
    S_msrVoice            fFiguredBassElementVoiceUpLink; // for use in figured bass voices JMI

    rational              fFiguredBassElementDisplayWholeNotes;

    msrFiguredBassElementParenthesesKind
                          fFiguredBassElementParenthesesKind;

    list<S_msrBassFigure> fFiguredBassElementFiguresList;

    msrTupletFactor       fFiguredBassElementTupletFactor;
};
typedef SMARTP<msrFiguredBassElement> S_msrFiguredBassElement;
EXP ostream& operator<< (ostream& os, const S_msrFiguredBassElement& elt);


}


#endif
