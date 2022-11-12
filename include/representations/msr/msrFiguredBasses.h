/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrFiguredBasses___
#define ___msrFiguredBasses___

#include "msrElements.h"
#include "msrMeasureElements.h"

#include "msrTypesForwardDeclarations.h"

#include "msrTupletFactors.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types
enum class msrBassFigurePrefixKind {
  kBassFigurePrefix_NO_,
  kBassFigurePrefixDoubleFlat, kBassFigurePrefixFlat, kBassFigurePrefixFlatFlat,
  kBassFigurePrefixNatural,
  kBassFigurePrefixSharpSharp, kBassFigurePrefixSharp, kBassFigurePrefixDoubleSharp
};

std::string msrBassFigurePrefixKindAsString (
  msrBassFigurePrefixKind figurePrefixKind);

std::ostream& operator << (std::ostream& os,const msrBassFigurePrefixKind& elt);

enum class msrBassFigureSuffixKind {
  kBassFigureSuffix_NO_,
  kBassFigureSuffixDoubleFlat, kBassFigureSuffixFlat, kBassFigureSuffixFlatFlat,
  kBassFigureSuffixNatural,
  kBassFigureSuffixSharpSharp, kBassFigureSuffixSharp, kBassFigureSuffixDoubleSharp,
  kBassFigureSuffixSlash
};

std::string msrBassFigureSuffixKindAsString (
  msrBassFigureSuffixKind figureSuffixKind);

std::ostream& operator << (std::ostream& os,const msrBassFigureSuffixKind& elt);

class EXP msrBassFigure : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBassFigure> create (
      int                     inputLineNumber,
      S_msrPart               figureUpLinkToPart,
      msrBassFigurePrefixKind figurePrefixKind,
      int                     figureNumber,
      msrBassFigureSuffixKind figureSuffixKind);

    SMARTP<msrBassFigure> createFigureNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrBassFigure> createFigureDeepClone ( // JMI ???
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBassFigure (
                            int                     inputLineNumber,
                            S_msrPart               figureUpLinkToPart,
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

    // upLinks
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
// data types

enum class msrFiguredBassParenthesesKind {
  kFiguredBassParenthesesYes, kFiguredBassParenthesesNo
};

std::string msrFiguredBassParenthesesKindAsString (
  msrFiguredBassParenthesesKind figuredBassParenthesesKind);

std::ostream& operator << (std::ostream& os, const msrFiguredBassParenthesesKind& elt);

class EXP msrFiguredBass : public msrMeasureElementLambda
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFiguredBass> create (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

    static SMARTP<msrFiguredBass> create (
                            int             inputLineNumber,
                            S_msrMeasure    upLinkToMeasure,
                            const Rational& figuredBassSoundingWholeNotes,
                            const Rational& figuredBassDisplayWholeNotes,
                            msrFiguredBassParenthesesKind
                                            figuredBassParenthesesKind,
                            msrTupletFactor figuredBassTupletFactor);

    SMARTP<msrFiguredBass> createFiguredBassNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrFiguredBass> createFiguredBassDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFiguredBass (
                            int             inputLineNumber,
                            S_msrMeasure    upLinkToMeasure,
                            const Rational& figuredBassSoundingWholeNotes,
                            const Rational& figuredBassDisplayWholeNotes,
                            msrFiguredBassParenthesesKind
                                            figuredBassParenthesesKind,
                            msrTupletFactor figuredBassTupletFactor);

    virtual               ~msrFiguredBass ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
    void                  setFiguredBassUpLinkToNote (
                            S_msrNote note)
                              { fFiguredBassUpLinkToNote = note; }

    S_msrNote             getFiguredBassUpLinkToNote () const
                             { return fFiguredBassUpLinkToNote; }

    void                  setFiguredBassUpLinkToVoice (
                            S_msrVoice voice)
                              { fFiguredBassUpLinkToVoice = voice; }

    S_msrVoice            getFiguredBassUpLinkToVoice () const
                             { return fFiguredBassUpLinkToVoice; }

/* JMI v0.9.66
    S_msrPart             getFiguredBassUpLinkToPart () const
                              { return fFiguredBassUpLinkToPart; }
*/

//     // position in measure
//     void                  setMeasureElementMeasurePosition (
//                             const S_msrMeasure measure,
//                             const Rational&    measurePosition,
//                             const std::string&      context) override
//                               {
//                                 setFiguredBassMeasurePosition (
//                                   measure,
//                                   measurePosition,
//                                   context);
//                               }
//
//     void                  setFiguredBassMeasurePosition (
//                             const S_msrMeasure measure,
//                             const Rational&    measurePosition,
//                             const std::string&      context);

    // whole notes
    void                  setFiguredBassDisplayWholeNotes (
                            const Rational& wholeNotes)
                              {
                                fFiguredBassDisplayWholeNotes =
                                  wholeNotes;
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
                            msrTupletFactor tupletFactor)
                              { fFiguredBassTupletFactor = tupletFactor; }

    msrTupletFactor       getFiguredBassTupletFactor () const
                              { return fFiguredBassTupletFactor; }

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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrNote             fFiguredBassUpLinkToNote;
    S_msrVoice            fFiguredBassUpLinkToVoice; // for use in figured bass voices JMI

    Rational              fFiguredBassDisplayWholeNotes;

    msrFiguredBassParenthesesKind
                          fFiguredBassParenthesesKind;

    std::list<S_msrBassFigure> fFiguredBassFiguresList;

    msrTupletFactor       fFiguredBassTupletFactor;
};
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;
EXP std::ostream& operator << (std::ostream& os, const S_msrFiguredBass& elt);


}


#endif
