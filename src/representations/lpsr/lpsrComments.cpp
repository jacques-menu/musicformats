/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "lpsrComments.h"

#include "oahOah.h"

#include "lpsrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrComment lpsrComment::create (
  int                inputLineNumber,
  const string&      contents,
  lpsrCommentGapAfterwardsKind commentGapAfterwardsKind)
{
  lpsrComment* o = new
    lpsrComment (
      inputLineNumber, contents, commentGapAfterwardsKind);
  assert (o != nullptr);
  return o;
}

lpsrComment::lpsrComment (
  int                inputLineNumber,
  const string&      contents,
  lpsrCommentGapAfterwardsKind commentGapAfterwardsKind)
    : lpsrElement (inputLineNumber)
{
  fContents       = contents;
  fCommentGapKind = commentGapAfterwardsKind;
}

lpsrComment::~lpsrComment ()
{}

void lpsrComment::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrComment::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrComment::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrComment::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrComment::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrComment::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrComment::browseData (basevisitor* v)
{}

string lpsrComment::lpsrCommentGapAfterwardsKindAsString (
  lpsrCommentGapAfterwardsKind commentGapAfterwardsKind)
{
  string result;

  switch (commentGapAfterwardsKind) {
    case lpsrCommentGapAfterwardsKind::kCommentGapAfterwardsYes:
      result = "kCommentGapAfterwardsYes";
    break;
    case lpsrCommentGapAfterwardsKind::kCommentGapAfterwardsNo:
      result = "kCommentGapAfterwardsNo";
    break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const lpsrCommentGapAfterwardsKind& elt)
{
  os << lpsrCommentGapAfterwardsKindAsString (elt);
  return os;
}

void lpsrComment::print (ostream& os) const
{
  os <<
    "Comment" <<
    endl;

  ++gIndenter;

  os <<
    "% " << fContents <<
    endl;

  if (fCommentGapKind == kCommentGapAfterwardsYes)
    os << endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_lpsrComment& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}


}
