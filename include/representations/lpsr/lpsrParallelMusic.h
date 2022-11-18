/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrParallelMusic___
#define ___lpsrParallelMusic___

#include <string>
#include <list>

#include "lpsrElements.h"

#include "lpsrPartGroups.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class lpsrElementsSeparatorKind {
  kElementsSeparatorEndOfLine, kElementsSeparatorSpace
};

std::string lpsrElementsSeparatorKindAsString (
  lpsrElementsSeparatorKind elementsSeparatorKind);

std::ostream& operator << (std::ostream& os, const lpsrElementsSeparatorKind& elt);

//______________________________________________________________________________
class EXP lpsrParallelMusicBLock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrParallelMusicBLock> create (
                            int                       inputLineNumber,
                            lpsrElementsSeparatorKind elementsSeparatorKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrParallelMusicBLock (
                            int                       inputLineNumber,
                            lpsrElementsSeparatorKind elementsSeparatorKind);

    virtual               ~lpsrParallelMusicBLock ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::list<S_lpsrPartGroupBlock>&
                          getParallelMusicBLockPartGroupBlocks () const
                              { return fParallelMusicBLockPartGroupBlocks; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToParallelMusicBLock ( // JMI
                            const S_lpsrPartGroupBlock& partGroupBlock)
                              {
                                fParallelMusicBLockPartGroupBlocks.push_back (
                                  partGroupBlock);
                              }

/* JMI
    S_lpsrPartGroupBlock  getLastPartGroupBlockOfParallelMusicBLock ()
                              {
                                return
                                  fParallelMusicBLockPartGroupBlocks.back ();
                              }

    void                  removeLastPartGroupBlockOfParallelMusicBLock () // JMI
                              {
                                fParallelMusicBLockPartGroupBlocks.pop_back ();
                              }
*/

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    std::list<S_lpsrPartGroupBlock>
                          fParallelMusicBLockPartGroupBlocks;

    lpsrElementsSeparatorKind
                          fElementsSeparatorKind;
};
typedef SMARTP<lpsrParallelMusicBLock> S_lpsrParallelMusicBLock;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrParallelMusicBLock& elt);


}


#endif
