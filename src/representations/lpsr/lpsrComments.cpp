/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "lpsrComments.h"

#include "oahOah.h"

#include "lpsrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrComment lpsrComment::create (
  int                inputLineNumber,
  const std::string&      contents,
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
  const std::string&      contents,
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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrComment::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrComment::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrComment::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrComment::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrComment::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrComment::browseData (basevisitor* v)
{}

std::string lpsrCommentGapAfterwardsKindAsString (
  lpsrCommentGapAfterwardsKind commentGapAfterwardsKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const lpsrCommentGapAfterwardsKind& elt)
{
  os << lpsrCommentGapAfterwardsKindAsString (elt);
  return os;
}

void lpsrComment::print (std::ostream& os) const
{
  os <<
    "Comment" <<
    std::endl;

  ++gIndenter;

  os <<
    "% " << fContents <<
    std::endl;

  if (fCommentGapKind == lpsrCommentGapAfterwardsKind::kCommentGapAfterwardsYes)
    os << std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrComment& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
