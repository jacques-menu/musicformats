#ifndef ___bsrLinesContents___
#define ___bsrLinesContents___

#include "bsrElements.h"
#include "bsrLines.h"
#include "bsrLineContentsElements.h"

#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
class   bsrLine;
typedef SMARTP<bsrLine> S_bsrLine;

//______________________________________________________________________________
// data types

enum class bsrLineContentsKind {
  kLineContentsRegular,
  kLineContentsContinuation
};

string bsrLineContentsKindAsString (
  bsrLineContentsKind lineContentsKind);

ostream& operator << (ostream& os, const bsrLineContentsKind& elt);

class EXP bsrLineContents : public bsrElement
{
  public:

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrLineContents> create (
                            int                 inputLineNumber,
                            bsrLineContentsKind lineContentsKind);

    SMARTP<bsrLineContents> createLineNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrLineContents (
                            int                 inputLineNumber,
                            bsrLineContentsKind lineContentsKind);

    virtual               ~bsrLineContents ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLineContentsUpLinkToLine (
                            S_bsrLine upLinkToLine);

    S_bsrLine             getLineContentsUpLinkToLine () const;

    bsrLineContentsKind   getLineContentsKind () const
                              { return fLineContentsKind; }

    const list<S_bsrLineContentsElement>&
                          getLineContentsLineElementsList () const
                              { return fLineContentsLineElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendLineElementToLineContents (
                            S_bsrLineContentsElement lineElement);

    void                  insertLineElementBeforeLastElementOfLineContents (
                            S_bsrLineContentsElement lineElement);

    S_bsrCellsList        fetchCellsList () const
                              { return buildLineContentsElementsList (); } // JMI ???

    int                   fetchCellsNumber () const;

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

    string                asShortString () const override;

    virtual string        asDebugString () const;

    void                  print (ostream& os) const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildLineContentsElementsList () const;

  protected:

    // protected fields
    // ------------------------------------------------------

    S_bsrLine             fLineContentsUpLinkToLine;

    bsrLineContentsKind   fLineContentsKind;

    list<S_bsrLineContentsElement>
                          fLineContentsLineElementsList;
};
typedef SMARTP<bsrLineContents> S_bsrLineContents;
EXP ostream& operator << (ostream& os, const S_bsrLineContents& elt);


}


#endif
