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

#include "msrBasicTypes.h"
#include "msrMeasureElements.h"


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

string msrBarLineLocationKindAsString (
  msrBarLineLocationKind barLineLocationKind);

ostream& operator << (ostream& os, const msrBarLineLocationKind& elt);

// style
enum class msrBarLineStyleKind {
  kBarLineStyleNone,

  kBarLineStyleRegular,  // by default

  kBarLineStyleDotted, kBarLineStyleDashed, kBarLineStyleHeavy,
  kBarLineStyleLightLight, kBarLineStyleLightHeavy,
  kBarLineStyleHeavyLight, kBarLineStyleHeavyHeavy,
  kBarLineStyleTick, kBarLineStyleShort
};

string msrBarLineStyleKindAsString (
  msrBarLineStyleKind barLineStyleKind);

ostream& operator << (ostream& os, const msrBarLineStyleKind& elt);

// repeat direction
enum class msrBarLineRepeatDirectionKind {
  kBarLineRepeatDirectionNone,
  kBarLineRepeatDirectionForward, kBarLineRepeatDirectionBackward
};

string msrBarLineRepeatDirectionKindAsString (
  msrBarLineRepeatDirectionKind barLineRepeatDirectionKind);

ostream& operator << (ostream& os, const msrBarLineRepeatDirectionKind& elt);

// ending type
enum class msrBarLineEndingTypeKind {
  kBarLineEndingNone,

  kBarLineEndingTypeStart,
  kBarLineEndingTypeStop,
  kBarLineEndingTypeDiscontinue
};

string msrBarLineEndingTypeKindAsString (
  msrBarLineEndingTypeKind barLineEndingTypeKind);

ostream& operator << (ostream& os, const msrBarLineEndingTypeKind& elt);

// category
enum class msrBarLineCategoryKind {
  k_NoBarLineCategory,

  kBarLineCategoryStandalone,

  kBarLineCategoryRepeatStart, kBarLineCategoryRepeatEnd,

  kBarLineCategoryHookedEndingStart, kBarLineCategoryHookedEndingEnd,
  kBarLineCategoryHooklessEndingStart, kBarLineCategoryHooklessEndingEnd
};

string msrBarLineCategoryKindAsString (
  msrBarLineCategoryKind barLineCategoryKind);

ostream& operator << (ostream& os, const msrBarLineCategoryKind& elt);

// segno
enum class msrBarLineHasSegnoKind {
  kBarLineHasSegnoYes, kBarLineHasSegnoNo
};

string msrBarLineHasSegnoKindAsString (
  msrBarLineHasSegnoKind barLineHasSegnoKind);

ostream& operator << (ostream& os, const msrBarLineHasSegnoKind& elt);

// coda
enum class msrBarLineHasCodaKind {
  kBarLineHasCodaYes, kBarLineHasCodaNo
};

string msrBarLineHasCodaKindAsString (
  msrBarLineHasCodaKind barLineHasCodaKind);

ostream& operator << (ostream& os, const msrBarLineHasCodaKind& elt);

// repeat winged
enum class msrBarLineRepeatWingedKind {
  kBarLineRepeatWingedNone,

  kBarLineRepeatWingedStraight, kBarLineRepeatWingedCurved,
  kBarLineRepeatWingedDoubleStraight, kBarLineRepeatWingedDoubleCurved
};

string msrBarLineRepeatWingedKindAsString (
  msrBarLineRepeatWingedKind barLineRepeatWingedKind);

ostream& operator << (ostream& os, const msrBarLineRepeatWingedKind& elt);

class EXP msrBarLine : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarLine> create (
                            int                           inputLineNumber,
                            S_msrMeasure                  upLinkToMeasure,
                            msrBarLineLocationKind        barLineLocationKind,
                            msrBarLineStyleKind           barLineStyleKind,
                            msrBarLineRepeatDirectionKind barLineRepeatDirectionKind,
                            msrBarLineEndingTypeKind      barLineEndingTypeKind,
                            const string&                 endingNumber,
                            int                           barLineTimes,
                            msrBarLineCategoryKind        barLineCategoryKind,
                            msrBarLineHasSegnoKind        barLineHasSegnoKind,
                            msrBarLineHasCodaKind         barLineHasCodaKind,
                            msrBarLineRepeatWingedKind    barLineRepeatWingedKind);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrBarLine> createFinalBarLine (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

    static SMARTP<msrBarLine> createDoubleBarLine (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBarLine (
                            int                           inputLineNumber,
                            S_msrMeasure                  upLinkToMeasure,
                            msrBarLineLocationKind        barLineLocationKind,
                            msrBarLineStyleKind           barLineStyleKind,
                            msrBarLineRepeatDirectionKind barLineRepeatDirectionKind,
                            msrBarLineEndingTypeKind      barLineEndingTypeKind,
                            const string&                 endingNumber,
                            int                           barLineTimes,
                            msrBarLineCategoryKind        barLineCategoryKind,
                            msrBarLineHasSegnoKind        barLineHasSegnoKind,
                            msrBarLineHasCodaKind         barLineHasCodaKind,
                            msrBarLineRepeatWingedKind    barLineRepeatWingedKind);

    virtual               ~msrBarLine ();

  public:

    // set and get
    // ------------------------------------------------------

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

    string                getEndingNumber () const
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

    const list<int>&      getEndingNumbersList () const
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

    string                endingNumbersListAsString () const;

    string                asString () const override;
    string                asShortString () const override;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrBarLineLocationKind
                          fLocationKind;

    msrBarLineStyleKind   fStyleKind;

    msrBarLineRepeatDirectionKind
                          fRepeatDirectionKind;

    msrBarLineRepeatWingedKind
                          fRepeatWingedKind;



    msrBarLineEndingTypeKind
                          fEndingTypeKind;
    string                fEndingNumber; // may be "1, 2"

    int                   fBarLineTimes;

    msrBarLineCategoryKind
                          fBarLineCategoryKind;

    msrBarLineHasSegnoKind
                          fBarLineHasSegnoKind;
    msrBarLineHasCodaKind fBarLineHasCodaKind;

    // the numbers extracted from fEndingNumber
    list<int>             fEndingNumbersList;
};
typedef SMARTP<msrBarLine> S_msrBarLine;
EXP ostream& operator << (ostream& os, const S_msrBarLine& elt);


} // namespace MusicFormats


#endif
