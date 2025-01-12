/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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
                            int           inputLineNumber,
                            const std::string& printMeasureNumber);

    SMARTP<bsrMeasure> createMeasureNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrMeasure (
                            int           inputLineNumber,
                            const std::string& printMeasureNumber);

    virtual               ~bsrMeasure ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getPrintMeasureNumber () const
                              { return fPrintMeasureNumber; }

    std::string           getBrailleMeasureNumber () const
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

    std::string           fPrintMeasureNumber;
    std::string           fBrailleMeasureNumber;

    std::list <S_bsrLineContentsElement>
                          fMeasureLineElementsList;
};
typedef SMARTP<bsrMeasure> S_bsrMeasure;
EXP std::ostream& operator << (std::ostream& os, const S_bsrMeasure& elt);


}


#endif // ___bsrMeasures___
