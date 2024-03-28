/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrLinesContents___
#define ___bsrLinesContents___

#include "bsrElements.h"

#include "bsrCellsLists.h"
#include "bsrLines.h"
#include "bsrLineContentsElements.h"


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

std::string bsrLineContentsKindAsString (
  bsrLineContentsKind lineContentsKind);

std::ostream& operator << (std::ostream& os, const bsrLineContentsKind& elt);

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
                            const S_bsrLine& upLinkToLine);

    S_bsrLine             getLineContentsUpLinkToLine () const;

    bsrLineContentsKind   getLineContentsKind () const
                              { return fLineContentsKind; }

    const std::list<S_bsrLineContentsElement>&
                          getLineContentsLineElementsList () const
                              { return fLineContentsLineElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendLineElementToLineContents (
                            const S_bsrLineContentsElement& lineElement);

    void                  insertLineElementBeforeLastElementOfLineContents (
                            const S_bsrLineContentsElement& lineElement);

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

    std::string           asString () const override;

    std::string           asShortString () const override;

    virtual std::string   asDebugString () const;

    void                  print (std::ostream& os) const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildLineContentsElementsList () const;

  protected:

    // protected fields
    // ------------------------------------------------------

    S_bsrLine             fLineContentsUpLinkToLine;

    bsrLineContentsKind   fLineContentsKind;

    std::list<S_bsrLineContentsElement>
                          fLineContentsLineElementsList;
};
typedef SMARTP<bsrLineContents> S_bsrLineContents;
EXP std::ostream& operator << (std::ostream& os, const S_bsrLineContents& elt);


}


#endif // ___bsrLinesContents___
