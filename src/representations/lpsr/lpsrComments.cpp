/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"


#include "lpsrComments.h"

#include "oahOah.h"

#include "lpsrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrComment lpsrComment::create (
  int                inputLineNumber,
  const std::string&      contents,
  lpsrCommentGapAfterwardsKind commentGapAfterwardsKind)
{
  lpsrComment* obj = new
    lpsrComment (
      inputLineNumber, contents, commentGapAfterwardsKind);
  assert (obj != nullptr);
  return obj;
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
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrComment::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrComment::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrComment::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrComment::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrComment::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
