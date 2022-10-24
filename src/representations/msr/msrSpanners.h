#ifndef ___msrSpanners___
#define ___msrSpanners___

#include "msrBasicTypes.h"
#include "msrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class msrNote;
typedef SMARTP<msrNote> S_msrNote;

//______________________________________________________________________________
// pre-declaration for two-way sideLinks
class msrSpanner;
typedef SMARTP<msrSpanner> S_msrSpanner;

//______________________________________________________________________________
class EXP msrSpanner : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrSpannerKind {
      kSpannerDashes, kSpannerWavyLine
    };

    static string spannerKindAsString (
      msrSpannerKind spannerKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSpanner> create (
                            int                inputLineNumber,
                            int                spannerNumber,
                            msrSpannerKind     spannerKind,
                            msrSpannerTypeKind spannerTypeKind,
                            msrPlacementKind   spannerPlacementKind,
                            S_msrNote          spannerUpLinkToNote);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSpanner (
                            int                inputLineNumber,
                            int                spannerNumber,
                            msrSpannerKind     spannerKind,
                            msrSpannerTypeKind spannerTypeKind,
                            msrPlacementKind   spannerPlacementKind,
                            S_msrNote          spannerUpLinkToNote);

    virtual               ~msrSpanner ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    void                  setSpannerUpLinkToNote (
                            S_msrNote spannerUpLinkToNote)
                              { fSpannerUpLinkToNote = spannerUpLinkToNote; }

    S_msrNote             getSpannerUpLinkToNote () const
                              { return fSpannerUpLinkToNote; }

    // sideLinks
    void                  setSpannerSideLinkToOtherEnd (
                            S_msrSpanner sideLinkToOtherEnd);

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

    void                  setSpannerBeginText (string beginText)
                              { fSpannerBeginText = beginText; }

    string                getSpannerBeginText () const
                              { return fSpannerBeginText; }

    void                  setSpannerMiddleText (string middleText)
                              { fSpannerMiddleText = middleText; }

    string                getSpannerMiddleText () const
                              { return fSpannerMiddleText; }

    void                  setSpannerEndText (string endText)
                              { fSpannerEndText = endText; }

    string                getSpannerEndText () const
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

    string                spannerKindAsString () const;

    string                spannerTypeKindAsString () const;

    string                spannerPlacementKindAsString () const;

    string                asShortString () const override;

    void                  print (ostream& os) const override;

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

    string                fSpannerBeginText;
    string                fSpannerMiddleText;
    string                fSpannerEndText;
};
typedef SMARTP<msrSpanner> S_msrSpanner;
EXP ostream& operator << (ostream& os, const S_msrSpanner& elt);


}


#endif
