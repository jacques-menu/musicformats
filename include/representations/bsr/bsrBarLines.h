#ifndef ___bsrBarLines___
#define ___bsrBarLines___

#include "bsrCellsLists.h"
#include "bsrLineContentsElements.h"

namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class bsrBarLineKind {
  kBarLineKindNone,
  kBarLineKindSpecial, // for dotted or dashed barLines
  kBarLineKindUnusual, // irregular spacing, ...
  kBarLineKindFinalDouble, kBarLineKindSectionalDouble
};

string bsrBarLineKindAsString (
  bsrBarLineKind barLineKind);

string bsrBarLineKindAsDebugString (
  bsrBarLineKind barLineKind);

ostream& operator << (ostream& os, const bsrBarLineKind& elt);

class EXP bsrBarLine : public bsrLineContentsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrBarLine> create (
                            int            inputLineNumber,
                            bsrBarLineKind barLineKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrBarLine (
                            int            inputLineNumber,
                            bsrBarLineKind barLineKin);

    virtual               ~bsrBarLine ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrBarLineKind        getBarLineKind () const
                              { return fBarLineKind; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fBarLineCellsList; }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        bsrBarLineKindAsCellsList ();

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

    string                asString () const override;

    string                asDebugString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    bsrBarLineKind        fBarLineKind;

    S_bsrCellsList        fBarLineCellsList;
};
typedef SMARTP<bsrBarLine> S_bsrBarLine;
EXP ostream& operator << (ostream& os, const S_bsrBarLine& elt);

}


#endif
