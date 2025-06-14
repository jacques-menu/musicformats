/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrBookBlockElements___
#define ___lpsrBookBlockElements___

#include "lpsrElements.h"

#include "lpsrComments.h"
#include "lpsrHeaders.h"
#include "lpsrLayouts.h"
#include "lpsrPapers.h"
#include "lpsrParallelMusic.h"
#include "lpsrPartGroups.h"
#include "lpsrScheme.h"

#include "msrMidi.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrBookBlockElement : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBookBlockElement> create (
                            const mfInputLineNumber& inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrBookBlockElement (
                            const mfInputLineNumber& inputLineNumber);

    virtual               ~lpsrBookBlockElement ();

  public:

    // set and get
    // ------------------------------------------------------

    S_lpsrHeader          getBookBlockElementHeader () const
                              { return fBookBlockElementHeader; }

    S_lpsrParallelMusicBLock
                          getBlockElementParallelMusicBLock () const
                              {
                                return
                                  fBookBlockElementParallelMusicBlock;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToBlockElement (
                            const S_lpsrPartGroupBlock& partGroupBlock);

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

  protected:

    // protected fields
    // ------------------------------------------------------

    S_lpsrHeader          fBookBlockElementHeader;

    S_lpsrParallelMusicBLock
                          fBookBlockElementParallelMusicBlock;
};
typedef SMARTP<lpsrBookBlockElement> S_lpsrBookBlockElement;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrBookBlockElement& elt);

//______________________________________________________________________________
class EXP lpsrScoreBlock : public lpsrBookBlockElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrScoreBlock> create (
                            const mfInputLineNumber& inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrScoreBlock (
                            const mfInputLineNumber& inputLineNumber);

    virtual               ~lpsrScoreBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    S_lpsrLayout          getScoreBlockLayout () const
                              { return fScoreBlockLayout; }

    S_msrMidiTempo        getScoreBlockMidiTempo () const
                              { return fScoreBlockMidiTempo; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToScoreBlock (
                            const S_lpsrPartGroupBlock& partGroupBlock);

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

    // private fields
    // ------------------------------------------------------

    S_lpsrLayout          fScoreBlockLayout;

    S_msrMidiTempo        fScoreBlockMidiTempo;
};
typedef SMARTP<lpsrScoreBlock> S_lpsrScoreBlock;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrScoreBlock& elt);

//______________________________________________________________________________
class EXP lpsrBookPartBlock : public lpsrBookBlockElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBookPartBlock> create (
                            const mfInputLineNumber& inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrBookPartBlock (
                            const mfInputLineNumber& inputLineNumber);

    virtual               ~lpsrBookPartBlock ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToBookPartBlock (
                            const S_lpsrPartGroupBlock& partGroupBlock);

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

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<lpsrBookPartBlock> S_lpsrBookPartBlock;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrBookPartBlock& elt);

//______________________________________________________________________________
class EXP lpsrBookBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBookBlock> create (
                            const mfInputLineNumber& inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrBookBlock (
                            const mfInputLineNumber& inputLineNumber);

    virtual               ~lpsrBookBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBookBlockHeader (
                            const S_lpsrHeader& bookBlockHeader)
                              { fBookBlockHeader = bookBlockHeader; }

    S_lpsrHeader          getBookBlockHeader () const
                              { return fBookBlockHeader; }

    void                  setBookBlockPaper (
                            const S_lpsrPaper& bookBlockPaper)
                              { fBookBlockPaper = bookBlockPaper; }

    S_lpsrPaper           getBookBlockPaper () const
                              { return fBookBlockPaper; }

    const std::list <S_lpsrBookBlockElement>&
                          getBookBlockElementsList () const
                              { return fBookBlockElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendLpsrScoreBlockToBookBlockElementsList (
                            const S_lpsrScoreBlock& scoreBlock);

    void                  appendLpsrBookPartBlockToBookBlockElementsList (
                            const S_lpsrBookPartBlock& bookPartBlock);

  protected:

    // protected services
    // ------------------------------------------------------

    void                  appendBookBlockElementToBookBlockElementsList (
                            const S_lpsrBookBlockElement& bookBlockElement);

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

    // private fields
    // ------------------------------------------------------

    S_lpsrHeader          fBookBlockHeader;

    S_lpsrPaper           fBookBlockPaper;

    std::list <S_lpsrBookBlockElement>
                          fBookBlockElementsList;
};
typedef SMARTP<lpsrBookBlock> S_lpsrBookBlock;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrBookBlock& elt);


}


#endif // ___lpsrBookBlockElements___
