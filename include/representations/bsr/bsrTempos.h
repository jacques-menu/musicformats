/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrTempos___
#define ___bsrTempos___

#include "msrTempos.h"

#include "bsrLineContentsElements.h"

#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
// std::string tempoKindAsDebugString ( // JMI v0.9.66
//   msrTempoKBeatUnitsKind tempoKind);

//______________________________________________________________________________
const bsrCellKind
  kCellTempoEquals = bsrCellKind::kDots2356,
  kCellTempoHyphen = bsrCellKind::kDots36;

class EXP bsrTempo : public bsrLineContentsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTempo> create (
                            int        inputLineNumber,
                            const S_msrTempo& mTempo);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrTempo (
                            int        inputLineNumber,
                            const S_msrTempo& mTempo);

    virtual               ~bsrTempo ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrTempo            getMsrTempo () const
                              { return fMsrTempo; }

    S_bsrCellsList        getTempoCellsList () const
                              { return fTempoCellsList; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fTempoCellsList; }

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

    std::string           asString () const override;

    std::string           asDebugString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrTempo            fMsrTempo;

    S_bsrCellsList        fTempoCellsList;
};
typedef SMARTP<bsrTempo> S_bsrTempo;
EXP std::ostream& operator << (std::ostream& os, const S_bsrTempo& elt);


}


#endif // ___bsrTempos___
