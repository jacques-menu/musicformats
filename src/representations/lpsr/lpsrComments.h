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


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrComment : public lpsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum lpsrCommentGapKind {
      kGapAfterwardsYes, kGapAfterwardsNo
    };

    static string commentGapKindAsString (
      lpsrCommentGapKind commentGapKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrComment> create (
                            int                inputLineNumber,
                            const string&      contents,
                            lpsrCommentGapKind commentGapKind = kGapAfterwardsNo);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrComment (
                            int                inputLineNumber,
                            const string&      contents,
                            lpsrCommentGapKind commentGapKind = kGapAfterwardsNo);

    virtual               ~lpsrComment ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getContents () const
                              { return fContents; }

    lpsrCommentGapKind    getCommentGapKind  () const
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

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string              fContents;
    lpsrCommentGapKind  fCommentGapKind;
};
typedef SMARTP<lpsrComment> S_lpsrComment;
EXP ostream& operator<< (ostream& os, const S_lpsrComment& elt);


}


#endif
