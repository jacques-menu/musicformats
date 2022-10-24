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
class EXP msrBarLine : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    // location
    enum msrBarLineLocationKind {
      kBarLineLocationNone,

      kBarLineLocationLeft,
      kBarLineLocationMiddle,
      kBarLineLocationRight // by default
    };

    static string barLineLocationKindAsString (
      msrBarLineLocationKind barLineLocationKind);

    // style
    enum msrBarLineStyleKind {
      kBarLineStyleNone,

      kBarLineStyleRegular,  // by default

      kBarLineStyleDotted, kBarLineStyleDashed, kBarLineStyleHeavy,
      kBarLineStyleLightLight, kBarLineStyleLightHeavy,
      kBarLineStyleHeavyLight, kBarLineStyleHeavyHeavy,
      kBarLineStyleTick, kBarLineStyleShort
    };

    static string barLineStyleKindAsString (
      msrBarLineStyleKind barLineStyleKind);

    // repeat direction
    enum msrBarLineRepeatDirectionKind {
      kBarLineRepeatDirectionNone,
      kBarLineRepeatDirectionForward, kBarLineRepeatDirectionBackward
    };

    static string barLineRepeatDirectionKindAsString (
      msrBarLineRepeatDirectionKind barLineRepeatDirectionKind);

    // ending type
    enum msrBarLineEndingTypeKind {
      kBarLineEndingNone,

      kBarLineEndingTypeStart,
      kBarLineEndingTypeStop,
      kBarLineEndingTypeDiscontinue
    };

    static string barLineEndingTypeKindAsString (
      msrBarLineEndingTypeKind barLineEndingTypeKind);

    // category
    enum msrBarLineCategoryKind {
      k_NoBarLineCategory,

      kBarLineCategoryStandalone,

      kBarLineCategoryRepeatStart, kBarLineCategoryRepeatEnd,

      kBarLineCategoryHookedEndingStart, kBarLineCategoryHookedEndingEnd,
      kBarLineCategoryHooklessEndingStart, kBarLineCategoryHooklessEndingEnd
    };

    static string barLineCategoryKindAsString (
      msrBarLineCategoryKind barLineCategoryKind);

    // segno
    enum msrBarLineHasSegnoKind {
      kBarLineHasSegnoYes, kBarLineHasSegnoNo
    };

    static string barLineHasSegnoKindAsString (
      msrBarLineHasSegnoKind barLineHasSegnoKind);

    // coda
    enum msrBarLineHasCodaKind {
      kBarLineHasCodaYes, kBarLineHasCodaNo
    };

    static string barLineHasCodaKindAsString (
      msrBarLineHasCodaKind barLineHasCodaKind);

    // repeat winged
    enum msrBarLineRepeatWingedKind {
      kBarLineRepeatWingedNone,

      kBarLineRepeatWingedStraight, kBarLineRepeatWingedCurved,
      kBarLineRepeatWingedDoubleStraight, kBarLineRepeatWingedDoubleCurved
    };

    static string barLineRepeatWingedKindAsString (
      msrBarLineRepeatWingedKind barLineRepeatWingedKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarLine> create (
                            int                           inputLineNumber,
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
                            int inputLineNumber);

    static SMARTP<msrBarLine> createDoubleBarLine (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBarLine (
                            int                           inputLineNumber,
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
