/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "bsrLineContentsElements.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
/* this class is purely virtual
S_bsrLineContentsElement bsrLineContentsElement::create (
  const mfInputLineNumber& inputLineNumber)
{
  bsrLineContentsElement* obj =
    new bsrLineContentsElement (
      inputLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/

bsrLineContentsElement::bsrLineContentsElement (
  const mfInputLineNumber& inputLineNumber)
    : bsrElement (inputLineNumber)
{
  fSpacesBefore = 0;
}

bsrLineContentsElement::~bsrLineContentsElement ()
{}

/*
void bsrLineContentsElement::setUpLinkToBsrLine (
  S_bsrLine upLinkToBsrLine)
{
  fUpLinkToBsrLine = upLinkToBsrLine;
}

S_bsrLine bsrLineContentsElement::getUpLinkToBsrLine () const
{
  S_msrVoice result;

  if (fMeasureElementUpLinkToMeasure) {
    result =
  return fUpLinkToBsrLine;
  }

  return result;
}
*/

void bsrLineContentsElement::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrLineContentsElement::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrLineContentsElement>*
    p =
      dynamic_cast<visitor<S_bsrLineContentsElement>*> (v)) {
        S_bsrLineContentsElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrLineContentsElement::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrLineContentsElement::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrLineContentsElement::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrLineContentsElement>*
    p =
      dynamic_cast<visitor<S_bsrLineContentsElement>*> (v)) {
        S_bsrLineContentsElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrLineContentsElement::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrLineContentsElement::browseData (basevisitor* v)
{}


std::string bsrLineContentsElement::asString () const
{
  // this is overriden all in actual elements
  return "??? bsrLineContentsElement::asString () ???";
}

std::string bsrLineContentsElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

/* JMI
std::string bsrLineContentsElement::asDebugString () const
{
  // this can be overriden in actual elements
  return asShortString ();
}
*/

void bsrLineContentsElement::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_bsrLineContentsElement& elt)
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
