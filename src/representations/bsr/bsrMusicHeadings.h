/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrMusicHeadings___
#define ___bsrMusicHeadings___

#include "smartpointer.h"

#include "bsrLines.h"

#include "bsrCellsLists.h"
#include "bsrKeys.h"
#include "bsrTempos.h"
#include "bsrTimeSignatures.h"


namespace MusicFormats
{

//______________________________________________________________________________
/*
class   bsrKey;
typedef SMARTP<bsrKey> S_bsrKey;

class   bsrTimeSignature;
typedef SMARTP<bsrTimeSignature> S_bsrTimeSignature;

class   bsrTempo;
typedef SMARTP<bsrTempo> S_bsrTempo;
*/

//______________________________________________________________________________
class EXP bsrMusicHeading : public bsrLine
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrMusicHeading> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrMusicHeading (
                            int inputLineNumber);

    virtual               ~bsrMusicHeading ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMusicHeadingTempo (S_bsrTempo tempo)
                              { fMusicHeadingTempo = tempo; }

    S_bsrTempo            getMusicHeadingTempo () const
                              { return fMusicHeadingTempo; }

    S_bsrKey              getMusicHeadingKey () const
                              { return fMusicHeadingKey; }

    void                  setMusicHeadingKey (S_bsrKey key)
                              { fMusicHeadingKey = key; }

    S_bsrTimeSignature    getMusicHeadingTimeSignature () const
                              { return fMusicHeadingTimeSignatureSignature; }

    void                  setMusicHeadingTimeSignature (
                            const S_bsrTimeSignature& timeSignature)
                              { fMusicHeadingTimeSignatureSignature = time; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const // override JMI
                              { return buildCellsList (); }

  private:

    // private services
    // ------------------------------------------------------

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

    S_bsrTempo            fMusicHeadingTempo;
    S_bsrKey              fMusicHeadingKey;
    S_bsrTimeSignature    fMusicHeadingTimeSignatureSignature;
};
typedef SMARTP<bsrMusicHeading> S_bsrMusicHeading;
EXP std::ostream& operator << (std::ostream& os, const S_bsrMusicHeading& elt);


}


#endif // ___bsrMusicHeadings___
