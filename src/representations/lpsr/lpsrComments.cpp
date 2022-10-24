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
  lpsrCommentGapKind commentGapKind)
{
  lpsrComment* o = new
    lpsrComment (
      inputLineNumber, contents, commentGapKind);
  assert (o != nullptr);
  return o;
}

lpsrComment::lpsrComment (
  int                inputLineNumber,
  const string&      contents,
  lpsrCommentGapKind commentGapKind)
    : lpsrElement (inputLineNumber)
{
  fContents       = contents;
  fCommentGapKind = commentGapKind;
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

string lpsrComment::commentGapKindAsString (
  lpsrCommentGapKind commentGapKind)
{
  string result;

  switch (commentGapKind) {
    case lpsrComment::kGapAfterwardsYes:
      result = "kGapAfterwardsYes";
    break;
    case lpsrComment::kGapAfterwardsNo:
      result = "kGapAfterwardsNo";
    break;
  } // switch

  return result;
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

  if (fCommentGapKind == kGapAfterwardsYes)
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
