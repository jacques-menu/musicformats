/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrComments___
#define ___lpsrComments___

#include "lpsrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class lpsrCommentGapAfterwardsKind {
  kCommentGapAfterwardsYes, kCommentGapAfterwardsNo
};

std::string lpsrCommentGapAfterwardsKindAsString (
  lpsrCommentGapAfterwardsKind commentGapAfterwardsKind);

std::ostream& operator << (std::ostream& os, const lpsrCommentGapAfterwardsKind& elt);

class EXP lpsrComment : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrComment> create (
                            int                inputLineNumber,
                            const std::string&      contents,
                            lpsrCommentGapAfterwardsKind commentGapAfterwardsKind =
                                                 lpsrCommentGapAfterwardsKind::kCommentGapAfterwardsNo);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrComment (
                            int                inputLineNumber,
                            const std::string&      contents,
                            lpsrCommentGapAfterwardsKind commentGapAfterwardsKind =
                                                 lpsrCommentGapAfterwardsKind::kCommentGapAfterwardsNo);

    virtual               ~lpsrComment ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getContents () const
                              { return fContents; }

    lpsrCommentGapAfterwardsKind    getCommentGapKind  () const
                              { return fCommentGapKind; }

  public:

    // public services
    // ------------------------------------------------------

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

    std::string              fContents;
    lpsrCommentGapAfterwardsKind  fCommentGapKind;
};
typedef SMARTP<lpsrComment> S_lpsrComment;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrComment& elt);


}


#endif
