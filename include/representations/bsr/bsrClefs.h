#ifndef ___bsrClefs___
#define ___bsrClefs___

#include "bsrLineContentsElements.h"

#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class bsrClefKind {
  kClefKindNone,

  kClefKindGTreble,
  kClefKindFBass,
  kClefKindCAlto,
  kClefKindGSoprano, // first line, french violin
  kClefKindCBaritone,
  kClefKindCTenor,
  kClefKindGOttavaAlta,
  kClefKindGOttavaBassa,
  kClefKindModifiedBassForRightHandPart,
  kClefKindModifiedTrebleForLeftHandPart
};

std::string bsrClefKindAsString (
  bsrClefKind clefKind);

std::string bsrClefKindAsDebugString (
  bsrClefKind clefKind);

std::ostream& operator << (std::ostream& os, const bsrClefKind& elt);

class EXP bsrClef : public bsrLineContentsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrClef> create (
                            int         inputLineNumber,
                            bsrClefKind clefKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrClef (
                            int         inputLineNumber,
                            bsrClefKind clefKind);

    virtual               ~bsrClef ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrClefKind           getClefKind () const
                              { return fClefKind; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fClefCellsList; }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildCellsList () const;

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

    std::string           asDebugString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    bsrClefKind           fClefKind;

    S_bsrCellsList        fClefCellsList;
};
typedef SMARTP<bsrClef> S_bsrClef;
EXP std::ostream& operator << (std::ostream& os, const S_bsrClef& elt);


}


#endif
