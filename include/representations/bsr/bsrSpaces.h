#ifndef ___bsrSpaces___
#define ___bsrSpaces___

#include "bsrLineContentsElements.h"

#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
const bsrCellKind
  kCellSpace = bsrCellKind::kDotsNone;

//______________________________________________________________________________
class EXP bsrSpaces : public bsrLineContentsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrSpaces> create (
                            int inputLineNumber,
                            int numberOfSpaces);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrSpaces (
                            int inputLineNumber,
                            int numberOfSpaces);

    virtual               ~bsrSpaces ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getNumberOfSpaces () const
                              { return fNumberOfSpaces; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fSpacesCellsList; }

    int                   fetchCellsNumber () const override;

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

    int                   fNumberOfSpaces;

    S_bsrCellsList        fSpacesCellsList;
};
typedef SMARTP<bsrSpaces> S_bsrSpaces;
EXP std::ostream& operator << (std::ostream& os, const S_bsrSpaces& elt);


}


#endif
