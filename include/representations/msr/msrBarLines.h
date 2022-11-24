/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrBarLines___
#define ___msrBarLines___

#include <list>

#include "msrMeasureElements.h"

#include "msrTypesForwardDeclarations.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

// location
enum class msrBarLineLocationKind {
  kBarLineLocationNone,

  kBarLineLocationLeft,
  kBarLineLocationMiddle,
  kBarLineLocationRight // by default
};

std::string msrBarLineLocationKindAsString (
  msrBarLineLocationKind barLineLocationKind);

std::ostream& operator << (std::ostream& os, const msrBarLineLocationKind& elt);

// style
enum class msrBarLineStyleKind {
  kBarLineStyleNone,

  kBarLineStyleRegular,  // by default

  kBarLineStyleDotted, kBarLineStyleDashed, kBarLineStyleHeavy,
  kBarLineStyleLightLight, kBarLineStyleLightHeavy,
  kBarLineStyleHeavyLight, kBarLineStyleHeavyHeavy,
  kBarLineStyleTick, kBarLineStyleShort
};

std::string msrBarLineStyleKindAsString (
  msrBarLineStyleKind barLineStyleKind);

std::ostream& operator << (std::ostream& os, const msrBarLineStyleKind& elt);

// repeat direction
enum class msrBarLineRepeatDirectionKind {
  kBarLineRepeatDirectionNone,
  kBarLineRepeatDirectionForward, kBarLineRepeatDirectionBackward
};

std::string msrBarLineRepeatDirectionKindAsString (
  msrBarLineRepeatDirectionKind barLineRepeatDirectionKind);

std::ostream& operator << (std::ostream& os, const msrBarLineRepeatDirectionKind& elt);

// ending type
enum class msrBarLineEndingTypeKind {
  kBarLineEndingTypeNone,

  kBarLineEndingTypeStart,
  kBarLineEndingTypeStop,
  kBarLineEndingTypeDiscontinue
};

std::string msrBarLineEndingTypeKindAsString (
  msrBarLineEndingTypeKind barLineEndingTypeKind);

std::ostream& operator << (std::ostream& os, const msrBarLineEndingTypeKind& elt);

// category
enum class msrBarLineCategoryKind {
  kBarLineCategory_NO_,

  kBarLineCategoryStandalone,

  kBarLineCategoryRepeatStart, kBarLineCategoryRepeatEnd,

  kBarLineCategoryHookedEndingStart, kBarLineCategoryHookedEndingEnd,
  kBarLineCategoryHooklessEndingStart, kBarLineCategoryHooklessEndingEnd
};

std::string msrBarLineCategoryKindAsString (
  msrBarLineCategoryKind barLineCategoryKind);

std::ostream& operator << (std::ostream& os, const msrBarLineCategoryKind& elt);

// segno
enum class msrBarLineHasSegnoKind {
  kBarLineHasSegnoYes, kBarLineHasSegnoNo
};

std::string msrBarLineHasSegnoKindAsString (
  msrBarLineHasSegnoKind barLineHasSegnoKind);

std::ostream& operator << (std::ostream& os, const msrBarLineHasSegnoKind& elt);

// coda
enum class msrBarLineHasCodaKind {
  kBarLineHasCodaYes, kBarLineHasCodaNo
};

std::string msrBarLineHasCodaKindAsString (
  msrBarLineHasCodaKind barLineHasCodaKind);

std::ostream& operator << (std::ostream& os, const msrBarLineHasCodaKind& elt);

// repeat winged
enum class msrBarLineRepeatWingedKind {
  kBarLineRepeatWingedNone,

  kBarLineRepeatWingedStraight, kBarLineRepeatWingedCurved,
  kBarLineRepeatWingedDoubleStraight, kBarLineRepeatWingedDoubleCurved
};

std::string msrBarLineRepeatWingedKindAsString (
  msrBarLineRepeatWingedKind barLineRepeatWingedKind);

std::ostream& operator << (std::ostream& os, const msrBarLineRepeatWingedKind& elt);

class EXP msrBarLine : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarLine> create (
                            int                           inputLineNumber,
                            const S_msrMeasure&           upLinkToMeasure,
                            msrBarLineLocationKind        barLineLocationKind,
                            msrBarLineStyleKind           barLineStyleKind,
                            msrBarLineRepeatDirectionKind barLineRepeatDirectionKind,
                            msrBarLineEndingTypeKind      barLineEndingTypeKind,
                            const std::string&            endingNumber,
                            int                           barLineTimes,
                            msrBarLineCategoryKind        barLineCategoryKind,
                            msrBarLineHasSegnoKind        barLineHasSegnoKind,
                            msrBarLineHasCodaKind         barLineHasCodaKind,
                            msrBarLineRepeatWingedKind    barLineRepeatWingedKind);

    static SMARTP<msrBarLine> create (
                            int                           inputLineNumber,
                            msrBarLineLocationKind        barLineLocationKind,
                            msrBarLineStyleKind           barLineStyleKind,
                            msrBarLineRepeatDirectionKind barLineRepeatDirectionKind,
                            msrBarLineEndingTypeKind      barLineEndingTypeKind,
                            const std::string&            endingNumber,
                            int                           barLineTimes,
                            msrBarLineCategoryKind        barLineCategoryKind,
                            msrBarLineHasSegnoKind        barLineHasSegnoKind,
                            msrBarLineHasCodaKind         barLineHasCodaKind,
                            msrBarLineRepeatWingedKind    barLineRepeatWingedKind);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrBarLine> createFinalBarLine (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    static SMARTP<msrBarLine> createDoubleBarLine (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBarLine (
                            int                           inputLineNumber,
                            const S_msrMeasure&           upLinkToMeasure,
                            msrBarLineLocationKind        barLineLocationKind,
                            msrBarLineStyleKind           barLineStyleKind,
                            msrBarLineRepeatDirectionKind barLineRepeatDirectionKind,
                            msrBarLineEndingTypeKind      barLineEndingTypeKind,
                            const std::string&            endingNumber,
                            int                           barLineTimes,
                            msrBarLineCategoryKind        barLineCategoryKind,
                            msrBarLineHasSegnoKind        barLineHasSegnoKind,
                            msrBarLineHasCodaKind         barLineHasCodaKind,
                            msrBarLineRepeatWingedKind    barLineRepeatWingedKind);

    virtual               ~msrBarLine ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override
                              { setBarLineUpLinkToMeasure (measure); }

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override
                            { upLinkToMeasure = getBarLineUpLinkToMeasure (); }

    void                  setBarLineUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getBarLineUpLinkToMeasure () const
                            { return fBarLineUpLinkToMeasure; }

    msrBarLineLocationKind
                          getLocationKind () const
                              { return fLocationKind; }

    msrBarLineStyleKind   getBarLineStyleKind () const
                              { return fStyleKind; }

    msrBarLineRepeatDirectionKind
                          getRepeatDirection () const
                              { return fRepeatDirectionKind; }

    msrBarLineRepeatWingedKind
                          getRepeatWinged () const
                              { return fRepeatWingedKind; }

    msrBarLineEndingTypeKind
                          getEndingType () const
                              { return fEndingTypeKind; }

    std::string           getEndingNumber () const
                              { return fEndingNumber; }

    msrBarLineCategoryKind
                          getBarLineCategory () const
                              { return fBarLineCategoryKind; }

    void                  setBarLineCategory (
                            msrBarLineCategoryKind barLineCategoryKind);

    int                   getBarLineTimes () const
                              { return fBarLineTimes; }

    msrBarLineHasSegnoKind
                          getBarLineHasSegnoKind () const
                              { return fBarLineHasSegnoKind; }
    msrBarLineHasCodaKind getBarLineHasCodaKind () const
                              { return fBarLineHasCodaKind; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  barLineIsAFinalBar () const;

    Bool                  barLineIsADoubleBar () const;

  private:

    // private services
    // ------------------------------------------------------

    const std::list<int>& getEndingNumbersList () const
                              { return fEndingNumbersList; }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           endingNumbersListAsString () const;

    std::string           asString () const override;
    std::string           asShortString () const override;

    void                  print (std::ostream& os) const override;

    void                  printShort (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrMeasure          fBarLineUpLinkToMeasure;

    msrBarLineLocationKind
                          fLocationKind;

    msrBarLineStyleKind   fStyleKind;

    msrBarLineRepeatDirectionKind
                          fRepeatDirectionKind;

    msrBarLineRepeatWingedKind
                          fRepeatWingedKind;



    msrBarLineEndingTypeKind
                          fEndingTypeKind;
    std::string           fEndingNumber; // may be "1, 2"

    int                   fBarLineTimes;

    msrBarLineCategoryKind
                          fBarLineCategoryKind;

    msrBarLineHasSegnoKind
                          fBarLineHasSegnoKind;
    msrBarLineHasCodaKind fBarLineHasCodaKind;

    // the numbers extracted from fEndingNumber
    std::list<int>        fEndingNumbersList;
};
typedef SMARTP<msrBarLine> S_msrBarLine;
EXP std::ostream& operator << (std::ostream& os, const S_msrBarLine& elt);


}


#endif
