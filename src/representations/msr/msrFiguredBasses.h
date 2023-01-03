/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrFiguredBasses___
#define ___msrFiguredBasses___

#include "msrElements.h"
#include "msrMeasureElements.h"

#include "msrMeasures.h"

#include "msrTypesForwardDeclarations.h"

#include "msrTupletFactors.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types
enum class msrBassFigurePrefixKind {
  kBassFigurePrefix_UNKNOWN,
  kBassFigurePrefixDoubleFlat, kBassFigurePrefixFlat, kBassFigurePrefixFlatFlat,
  kBassFigurePrefixNatural,
  kBassFigurePrefixSharpSharp, kBassFigurePrefixSharp, kBassFigurePrefixDoubleSharp
};

std::string msrBassFigurePrefixKindAsString (
  msrBassFigurePrefixKind figurePrefixKind);

std::ostream& operator << (std::ostream& os,const msrBassFigurePrefixKind& elt);

enum class msrBassFigureSuffixKind {
  kBassFigureSuffix_UNKNOWN,
  kBassFigureSuffixDoubleFlat, kBassFigureSuffixFlat, kBassFigureSuffixFlatFlat,
  kBassFigureSuffixNatural,
  kBassFigureSuffixSharpSharp, kBassFigureSuffixSharp, kBassFigureSuffixDoubleSharp,
  kBassFigureSuffixSlash
};

std::string msrBassFigureSuffixKindAsString (
  msrBassFigureSuffixKind figureSuffixKind);

std::ostream& operator << (std::ostream& os,const msrBassFigureSuffixKind& elt);

enum class msrFiguredBassParenthesesKind {
  kFiguredBassParenthesesYes, kFiguredBassParenthesesNo
};

std::string msrFiguredBassParenthesesKindAsString (
  msrFiguredBassParenthesesKind figuredBassParenthesesKind);

std::ostream& operator << (std::ostream& os, const msrFiguredBassParenthesesKind& elt);

//______________________________________________________________________________
class EXP msrBassFigure : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBassFigure> create (
                            int                     inputLineNumber,
                            const S_msrPart&               figureUpLinkToPart,
                            msrBassFigurePrefixKind figurePrefixKind,
                            int                     figureNumber,
                            msrBassFigureSuffixKind figureSuffixKind);

    SMARTP<msrBassFigure> createFigureNewbornClone (
                            const S_msrPart&  containingPart);

    SMARTP<msrBassFigure> createFigureDeepClone ( // JMI ???
                            const S_msrPart&  containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBassFigure (
                            int                     inputLineNumber,
                            const S_msrPart&               figureUpLinkToPart,
                            msrBassFigurePrefixKind figurePrefixKind,
                            int                     figureNumber,
                            msrBassFigureSuffixKind figureSuffixKind);

    virtual               ~msrBassFigure ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrPart             getFigureUpLinkToPart () const
                              { return fFigureUpLinkToPart; }

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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrPart             fFigureUpLinkToPart;

    msrBassFigurePrefixKind
                          fFigurePrefixKind;
    int                   fFigureNumber;
    msrBassFigureSuffixKind
                          fFigureSuffixKind;
};
typedef SMARTP<msrBassFigure> S_msrBassFigure;
EXP std::ostream& operator << (std::ostream& os, const S_msrBassFigure& elt);

//______________________________________________________________________________
class EXP msrFiguredBass : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFiguredBass> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    static SMARTP<msrFiguredBass> create (
                            int                 inputLineNumber);

    static SMARTP<msrFiguredBass> create (
                            int                    inputLineNumber,
                            const S_msrMeasure&    upLinkToMeasure,
                            const Rational&        figuredBassSoundingWholeNotes,
                            const Rational&        figuredBassDisplayWholeNotes,
                            msrFiguredBassParenthesesKind
                                                   figuredBassParenthesesKind,
                            const msrTupletFactor& figuredBassTupletFactor);

    static SMARTP<msrFiguredBass> create (
                            int                    inputLineNumber,
                            const Rational&        figuredBassSoundingWholeNotes,
                            const Rational&        figuredBassDisplayWholeNotes,
                            msrFiguredBassParenthesesKind
                                                   figuredBassParenthesesKind,
                            const msrTupletFactor& figuredBassTupletFactor);

    SMARTP<msrFiguredBass> createFiguredBassNewbornClone (
                            const S_msrVoice& containingVoice);

    SMARTP<msrFiguredBass> createFiguredBassDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFiguredBass (
                            int                    inputLineNumber,
                            const S_msrMeasure&    upLinkToMeasure,
                            const Rational&        figuredBassSoundingWholeNotes,
                            const Rational&        figuredBassDisplayWholeNotes,
                            msrFiguredBassParenthesesKind
                                                   figuredBassParenthesesKind,
                            const msrTupletFactor& figuredBassTupletFactor);

    virtual               ~msrFiguredBass ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to measure
    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override
                              { setFiguredBassUpLinkToMeasure (measure); }

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override
                            { upLinkToMeasure = getFiguredBassUpLinkToMeasure (); }

    void                  setFiguredBassUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getFiguredBassUpLinkToMeasure () const
                            { return fFiguredBassUpLinkToMeasure; }

    void                  setFiguredBassUpLinkToNote (
                            const S_msrNote& note);

    S_msrNote             getFiguredBassUpLinkToNote () const
                             { return fFiguredBassUpLinkToNote; }

    void                  setFiguredBassUpLinkToVoice (
                            const S_msrVoice& voice)
                              { fFiguredBassUpLinkToVoice = voice; }

    S_msrVoice            getFiguredBassUpLinkToVoice () const
                             { return fFiguredBassUpLinkToVoice; }

/* JMI v0.9.66
    S_msrPart             getFiguredBassUpLinkToPart () const
                              { return fFiguredBassUpLinkToPart; }
*/

    // whole notes
    void                  setFiguredBassDisplayWholeNotes (
                            const Rational& wholeNotes)
                              {
                                fFiguredBassDisplayWholeNotes = wholeNotes;
                              }

    Rational              getFiguredBassDisplayWholeNotes () const
                              { return fFiguredBassDisplayWholeNotes; }

    msrFiguredBassParenthesesKind
                          getFiguredBassParenthesesKind () const
                              { return fFiguredBassParenthesesKind; }

    const std::list<S_msrBassFigure>&
                          getFiguredBassFiguresList () const
                              { return fFiguredBassFiguresList;  }

    void                  setFiguredBassTupletFactor (
                            const msrTupletFactor& tupletFactor)
                              { fFiguredBassTupletFactor = tupletFactor; }

    msrTupletFactor       getFiguredBassTupletFactor () const
                              { return fFiguredBassTupletFactor; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendFigureToFiguredBass (
                            const S_msrBassFigure& bassFigure);

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

    // upLinks
    S_msrMeasure          fFiguredBassUpLinkToMeasure;

    S_msrNote             fFiguredBassUpLinkToNote;
    S_msrVoice            fFiguredBassUpLinkToVoice; // for use in figured bass voices JMI v0.9.66

    Rational              fFiguredBassDisplayWholeNotes;

    msrFiguredBassParenthesesKind
                          fFiguredBassParenthesesKind;

    std::list<S_msrBassFigure>
                          fFiguredBassFiguresList;

    msrTupletFactor       fFiguredBassTupletFactor;
};
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;
EXP std::ostream& operator << (std::ostream& os, const S_msrFiguredBass& elt);


}


#endif
