/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrMeasures___
#define ___bsrMeasures___

#include "bsrLineContentsElements.h"

#include "bsrBarLines.h"
#include "bsrClefs.h"
#include "bsrCellsLists.h"
#include "bsrDynamics.h"
#include "bsrNotes.h"
#include "bsrNumbers.h"
#include "bsrWords.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrMeasure : public bsrLineContentsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrMeasure> create (
                            const mfInputLineNumber& inputLineNumber,
                            const mfMeasureNumber&   printMeasureNumber);

    SMARTP<bsrMeasure> createMeasureNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrMeasure (
                            const mfInputLineNumber& inputLineNumber,
                            const mfMeasureNumber&   printMeasureNumber);

    virtual               ~bsrMeasure ();

  public:

    // set and get
    // ------------------------------------------------------

    mfMeasureNumber       getPrintMeasureNumber () const
                              { return fPrintMeasureNumber; }

    mfMeasureNumber       getBrailleMeasureNumber () const
                              { return fBrailleMeasureNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendClefToMeasure (S_bsrClef clef);

    void                  appendBarLineToMeasure (S_bsrBarLine barLine);

    void                  appendNumberToMeasure (S_bsrNumber number);

    void                  appendWordsToMeasure (S_bsrWords str);

    void                  appendNoteToMeasureAtPosition (S_bsrNote note);

    void                  appendPaddingSkipNoteToMeasure (S_bsrNote note);

    void                  appendDynamicToMeasure (S_bsrDynamic dynamic);

    S_bsrCellsList        fetchCellsList () const override
                              { return buildCellsList (); }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    void                  appendLineElementToMeasure (
                            const S_bsrLineContentsElement& lineElement);

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

    mfMeasureNumber       fPrintMeasureNumber;
    mfMeasureNumber       fBrailleMeasureNumber;

    std::list <S_bsrLineContentsElement>
                          fMeasureLineElementsList;
};
typedef SMARTP<bsrMeasure> S_bsrMeasure;
EXP std::ostream& operator << (std::ostream& os, const S_bsrMeasure& elt);


}


#endif // ___bsrMeasures___
