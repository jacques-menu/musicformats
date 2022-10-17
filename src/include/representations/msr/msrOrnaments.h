#ifndef ___msrOrnaments___
#define ___msrOrnaments___

#include "msrElements.h"
#include "msrNotes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class msrNote;
typedef SMARTP<msrNote> S_msrNote;

//______________________________________________________________________________
class EXP msrOrnament : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrOrnamentKind {
      kOrnamentTrill, // JMI kOrnamentDashes,
      kOrnamentTurn, kOrnamentInvertedTurn, kOrnamentDelayedTurn,
      kOrnamentDelayedInvertedTurn, kOrnamentVerticalTurn,
      kOrnamentMordent, kOrnamentInvertedMordent,
      kOrnamentSchleifer,
      kOrnamentShake,
      kOrnamentAccidentalKind
    };

    static string ornamentKindAsString (
      msrOrnamentKind ornamentKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOrnament> create (
                            int              inputLineNumber,
                            msrOrnamentKind  ornamentKind,
                            msrPlacementKind ornamentPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrOrnament (
                            int              inputLineNumber,
                            msrOrnamentKind  ornamentKind,
                            msrPlacementKind ornamentPlacementKind);

    virtual               ~msrOrnament ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOrnamentUpLinkToNote (S_msrNote note)
                              { fOrnamentUpLinkToNote = note; }

    S_msrNote             getOrnamentUpLinkToNote () const
                              { return fOrnamentUpLinkToNote; }

    msrOrnamentKind       getOrnamentKind () const
                              { return fOrnamentKind; }

    void                  setOrnamentPlacementKind (
                            msrPlacementKind
                              ornamentPlacementKind)
                              {
                                fOrnamentPlacementKind =
                                  ornamentPlacementKind;
                              }

    msrPlacementKind      getOrnamentPlacementKind () const
                              { return fOrnamentPlacementKind; }

    void                  setOrnamentAccidentalKind (
                            msrAccidentalKind
                              ornamentAccidentalKind)
                              {
                                fOrnamentAccidentalKind =
                                  ornamentAccidentalKind;
                              }

    msrAccidentalKind getOrnamentAccidentalKind () const
                              { return fOrnamentAccidentalKind; }

  public:

    // public services
    // ------------------------------------------------------

    string                ornamentKindAsString () const;

    string                ornamentPlacementKindAsString () const;

    string                ornamentAccidentalKindAsString () const;

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

    S_msrNote             fOrnamentUpLinkToNote;

    msrOrnamentKind       fOrnamentKind;

    msrPlacementKind      fOrnamentPlacementKind;

    msrAccidentalKind     fOrnamentAccidentalKind;
};
typedef SMARTP<msrOrnament> S_msrOrnament;
EXP ostream& operator<< (ostream& os, const S_msrOrnament& elt);


}


#endif
