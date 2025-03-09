/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrLines___
#define ___bsrLines___

#include "bsrPageElements.h"

#include "bsrCellsLists.h"
#include "bsrKeys.h"
#include "bsrLineContents.h"
#include "bsrLineContentsElements.h"
#include "bsrMeasures.h"
#include "bsrNotes.h"
#include "bsrSpaces.h"
#include "bsrTempos.h"
#include "bsrTimeSignatures.h"


namespace MusicFormats
{

//______________________________________________________________________________
class   bsrLineContents;
typedef SMARTP<bsrLineContents> S_bsrLineContents;

//______________________________________________________________________________
class EXP bsrLine : public bsrPageElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrLine> create (
                            int inputLineNumber,
                            int printLineNumber,
                            int cellsPerLine);

    SMARTP<bsrLine> createLineNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrLine (
                            int inputLineNumber,
                            int printLineNumber,
                            int cellsPerLine);

    virtual               ~bsrLine ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintLineNumber () const
                              { return fPrintLineNumber; }

    int                   getBrailleLineNumber () const
                              { return fBrailleLineNumber; }

    int                   getCellsPerLine () const
                              { return fCellsPerLine; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendKeyToLine (S_bsrKey key);

    void                  appendMeasureToLine (S_bsrMeasure measure);

    void                  appendNoteToLine (S_bsrNote note);

    void                  appendSpacesToLine (S_bsrSpaces spaces);

    void                  appendTimeSignatureToLine (S_bsrTimeSignature timeSignature);

    void                  insertTimeBeforeLastElementOfLine (S_bsrTimeSignature timeSignature);

    void                  appendTempoToLine (S_bsrTempo tempo);

    int                   fetchLineContentsNumber () const override
                              { return fLineContentsList.size (); }

    S_bsrCellsList        fetchCellsList () const
                              { return buildLineNumberCellsList (); } // JMI ???

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildLineNumberCellsList () const;

    void                  appendLineElementToLine (
                            const S_bsrLineContentsElement& lineElement);

    void                  insertElementBeforeLastElementOfLine (
                            const S_bsrLineContentsElement& lineElement);

    void                  appendLineElementToLastMeasureOfLine (
                            const S_bsrLineContentsElement& lineElement);

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

    virtual std::string   asDebugString () const;

    void                  print (std::ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    int                   fPrintLineNumber;
    int                   fBrailleLineNumber;

    int                   fCellsPerLine;

    S_bsrCellsList        fLineNumberCellsList;

    std::list <S_bsrLineContents>
                          fLineContentsList;

    Bool                  fASpaceIsNeededInLine;
};
typedef SMARTP<bsrLine> S_bsrLine;
EXP std::ostream& operator << (std::ostream& os, const S_bsrLine& elt);


}


#endif // ___bsrLines___
