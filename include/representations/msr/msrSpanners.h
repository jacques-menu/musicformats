/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSpanners___
#define ___msrSpanners___

#include "msrElements.h"

#include "msrNotes.h"


namespace MusicFormats
{

// spanner types
//______________________________________________________________________________
enum class msrSpannerKind {
  kSpannerDashes, kSpannerWavyLine
};

std::string msrSpannerKindAsString (
  msrSpannerKind spannerKind);

std::ostream& operator << (std::ostream& os, const msrSpannerKind& elt);

enum class msrSpannerTypeKind {
  kSpannerType_UNKNOWN_,
  kSpannerTypeStart, kSpannerTypeContinue, kSpannerTypeStop
};

std::string msrSpannerTypeKindAsString (
  msrSpannerTypeKind spannerTypeKind);

std::ostream& operator << (std::ostream& os, const msrSpannerTypeKind& elt);

//______________________________________________________________________________
// PRE-declaration for two-way sideLinks
class   msrSpanner;
typedef SMARTP<msrSpanner> S_msrSpanner;

//______________________________________________________________________________
class EXP msrSpanner : public msrElement
{
  public:

    // data types

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSpanner> create (
                            const mfInputLineNumber& inputLineNumber,
                            int                spannerNumber,
                            msrSpannerKind     spannerKind,
                            msrSpannerTypeKind spannerTypeKind,
                            msrPlacementKind   spannerPlacementKind,
                            const S_msrNote&          spannerUpLinkToNote);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSpanner (
                            const mfInputLineNumber& inputLineNumber,
                            int                spannerNumber,
                            msrSpannerKind     spannerKind,
                            msrSpannerTypeKind spannerTypeKind,
                            msrPlacementKind   spannerPlacementKind,
                            const S_msrNote&          spannerUpLinkToNote);

    virtual               ~msrSpanner ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    void                  setSpannerUpLinkToNote (
                            const S_msrNote& spannerUpLinkToNote)
                              { fSpannerUpLinkToNote = spannerUpLinkToNote; }

    S_msrNote             getSpannerUpLinkToNote () const
                              { return fSpannerUpLinkToNote; }

    // sideLinks
    void                  setSpannerSideLinkToOtherEnd (
                            const S_msrSpanner& sideLinkToOtherEnd);

    S_msrSpanner          getSpannerSideLinkToOtherEnd () const
                              { return fSpannerSideLinkToOtherEnd; }

    // fields
    void                  setSpannerNumber (int spannerNumber)
                              { fSpannerNumber = spannerNumber; }

    int                   getSpannerNumber () const
                              { return fSpannerNumber; }

    void                  setSpannerKind (msrSpannerKind spannerKind)
                              { fSpannerKind = spannerKind; }

    msrSpannerKind        getSpannerKind () const
                              { return fSpannerKind; }

    void                  setSpannerTypeKind (msrSpannerTypeKind spannerTypeKind)
                              { fSpannerTypeKind = spannerTypeKind; }

    msrSpannerTypeKind    getSpannerTypeKind () const
                              { return fSpannerTypeKind; }

    void                  setSpannerPlacementKind (msrPlacementKind placementKind)
                              { fSpannerPlacementKind = placementKind; }

    msrPlacementKind      getSpannerPlacementKind () const
                              { return fSpannerPlacementKind; }

    void                  setSpannerBeginText (std::string beginText)
                              { fSpannerBeginText = beginText; }

    std::string           getSpannerBeginText () const
                              { return fSpannerBeginText; }

    void                  setSpannerMiddleText (std::string middleText)
                              { fSpannerMiddleText = middleText; }

    std::string           getSpannerMiddleText () const
                              { return fSpannerMiddleText; }

    void                  setSpannerEndText (std::string endText)
                              { fSpannerEndText = endText; }

    std::string           getSpannerEndText () const
                              { return fSpannerEndText; }


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

  protected:

    // upLinks
    // ------------------------------------------------------
    S_msrNote             fSpannerUpLinkToNote;

    // sideLinks
    // ------------------------------------------------------
    S_msrSpanner          fSpannerSideLinkToOtherEnd; // two-way

    // fields
    // ------------------------------------------------------

    int                   fSpannerNumber;

    msrSpannerKind        fSpannerKind;

    msrSpannerTypeKind    fSpannerTypeKind;

    msrPlacementKind      fSpannerPlacementKind;

    std::string           fSpannerBeginText;
    std::string           fSpannerMiddleText;
    std::string           fSpannerEndText;
};
typedef SMARTP<msrSpanner> S_msrSpanner;
EXP std::ostream& operator << (std::ostream& os, const S_msrSpanner& elt);


}


#endif // ___msrSpanners___
