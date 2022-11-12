#ifndef ___bsrBarLines___
#define ___bsrBarLines___

#include "bsrLineContentsElements.h"

#include "bsrCellsLists.h"

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

std::string bsrBarLineKindAsString (
  bsrBarLineKind barLineKind);

std::string bsrBarLineKindAsDebugString (
  bsrBarLineKind barLineKind);

std::ostream& operator << (std::ostream& os, const bsrBarLineKind& elt);

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

    std::string           asString () const override;

    std::string           asDebugString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    bsrBarLineKind        fBarLineKind;

    S_bsrCellsList        fBarLineCellsList;
};
typedef SMARTP<bsrBarLine> S_bsrBarLine;
EXP std::ostream& operator << (std::ostream& os, const S_bsrBarLine& elt);

}


#endif
