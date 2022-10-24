#ifndef ___bsrDynamic___
#define ___bsrDynamic___

#include "msrDynamics.h"

#include "bsrCellsLists.h"
#include "bsrLineContentsElements.h"

namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrDynamic : public bsrLineContentsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrDynamic> create (
                            int             inputLineNumber,
                            msrDynamicKind dynamicKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrDynamic (
                            int             inputLineNumber,
                            msrDynamicKind dynamicKind);

    virtual               ~bsrDynamic ();

  public:

    // set and get
    // ------------------------------------------------------

    msrDynamicKind       getDynamicKind () const
                              { return fDynamicKind; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fDynamicsCellsList; }


    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        noteValueKindAsCellsList () const;

    S_bsrCellsList        noteOctaveKindAsCellsList () const;

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

    msrDynamicKind        fDynamicKind;

    S_bsrCellsList        fDynamicsCellsList;
};
typedef SMARTP<bsrDynamic> S_bsrDynamic;
EXP ostream& operator << (ostream& os, const S_bsrDynamic& elt);


}


#endif
