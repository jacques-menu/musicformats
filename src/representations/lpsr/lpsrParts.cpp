/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "lpsrWae.h"

#include "lpsrParts.h"

#include "oahOah.h"

#include "lpsrOah.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrPartBlock lpsrPartBlock::create (
  S_msrPart part)
{
  lpsrPartBlock* o = new lpsrPartBlock (
    part);
  assert (o != nullptr);
  return o;
}

lpsrPartBlock::lpsrPartBlock (
  S_msrPart part)
    : lpsrElement (0) // JMI
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    part != nullptr,
    "part is null");

  fPart = part;

  // set part block instrument names default values // JMI
  fPartBlockInstrumentName =
    fPart->
      getPartName ();
  fPartBlockShortInstrumentName =
    fPart->
      getPartAbbreviation ();
}

lpsrPartBlock::~lpsrPartBlock ()
{}

bool lpsrPartBlock::compareStaffBlockWithOtherElement (
  const S_lpsrStaffBlock& staffBlock,
  const S_msrElement&     otherElement)
{
  bool result = true;

  if (
    S_lpsrStaffBlock
      secondStaffBlock =
        dynamic_cast<lpsrStaffBlock*>(&(*otherElement))
    ) {
    // otherElement is a staff block
    result =
      staffBlock->getStaff ()->getStaffNumber ()
        <
      secondStaffBlock->getStaff ()->getStaffNumber ();
  }

  else if (
    S_lpsrChordNamesContext
      secondChordNamesContext =
        dynamic_cast<lpsrChordNamesContext*>(&(*otherElement))
    ) {
    // otherElement is a chord names context
    result =
      staffBlock->getStaff ()->getStaffNumber ()
        <
      secondChordNamesContext->
        getContextVoice ()->
          getVoiceUpLinkToStaff ()->
            getStaffNumber ();
  }

  else if (
    S_lpsrFiguredBassContext
      secondFiguredBassContext =
        dynamic_cast<lpsrFiguredBassContext*>(&(*otherElement))
    ) {
    // otherElement is a figured bass context
  }

  else {
    std::stringstream s;

    s <<
      "part block element '" <<
      otherElement->asShortString () <<
      " is not a staff nor a chord names or figured bass context";

    lpsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      otherElement->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

/* JMI
  gLogStream <<
    std::endl <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    std::endl <<
    "compareStaffBlockWithOtherElement:" <<
    std::endl <<
    staffBlock <<
    std::endl <<
    otherElement <<
    std::endl <<
    "===> " << result <<
    std::endl << std::endl;
    */

  return result;
}

bool lpsrPartBlock::compareChordNamesContextWithOtherElement (
  const S_lpsrChordNamesContext& chordNamesContext,
  const S_msrElement&            otherElement)
{
  bool result = true;

  if (
    S_lpsrStaffBlock
      secondStaffBlock =
        dynamic_cast<lpsrStaffBlock*>(&(*otherElement))
    ) {
    // otherElement is a staff block
    S_msrVoice
      chordNamesVoice =
        chordNamesContext->
          getContextVoice ();

    int
      chordNamesContextStaffNumber =
        chordNamesVoice->
          getVoiceUpLinkToStaff ()->
            getStaffNumber (),
      secondStaffBlockStaffNumber =
        secondStaffBlock->
          getStaff ()->getStaffNumber ();

    if (chordNamesContextStaffNumber == secondStaffBlockStaffNumber) {
      // chord names should precede the staff
      result = true;
    }
    else {
      result =
        chordNamesContextStaffNumber
          <
        secondStaffBlockStaffNumber;
    }
  }

  else if (
    S_lpsrChordNamesContext
      secondChordNamesContext =
        dynamic_cast<lpsrChordNamesContext*>(&(*otherElement))
    ) {
    // otherElement is a chord names context
    S_msrVoice
      chordNamesVoice =
        chordNamesContext->
          getContextVoice (),
      secondChordNamesVoice =
        secondChordNamesContext->
          getContextVoice ();

    int
      chordNamesContextStaffNumber =
        chordNamesVoice->
          getVoiceUpLinkToStaff ()->
            getStaffNumber (),
      secondChordNamesContextStaffNumber =
        secondChordNamesVoice->
          getVoiceUpLinkToStaff ()->
            getStaffNumber ();

    if (chordNamesContextStaffNumber == secondChordNamesContextStaffNumber) {
      result =
        chordNamesVoice->getVoiceNumber ()
          <
        secondChordNamesVoice->getVoiceNumber ();
    }
    else {
      result =
        chordNamesContextStaffNumber
          <
        secondChordNamesContextStaffNumber;
    }
  }

  else if (
    S_lpsrFiguredBassContext
      secondFiguredBassContext =
        dynamic_cast<lpsrFiguredBassContext*>(&(*otherElement))
    ) {
    // otherElement is a figured bass context
  }

  else {
    std::stringstream s;

    s <<
      "part block element '" <<
      otherElement->asShortString () <<
      " is not a staff nor a chord names or figured bass context";

    lpsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      otherElement->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

/* JMI
  gLogStream <<
    std::endl <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    std::endl <<
    "compareChordNamesContextWithOtherElement:" <<
    std::endl <<
    chordNamesContext <<
    std::endl <<
    otherElement <<
    std::endl <<
    "===> " << result <<
    std::endl << std::endl;
*/

  return result;
}

bool lpsrPartBlock::compareElementsToHaveHarmoniesAboveCorrespondingStaff (
  const S_msrElement& first,
  const S_msrElement& second)
{
  bool result = true;

  if (
    S_lpsrStaffBlock
      firstStaffBlock =
        dynamic_cast<lpsrStaffBlock*>(&(*first))
    ) {
    // first is a staff block
    result =
      compareStaffBlockWithOtherElement (
        firstStaffBlock,
        second);
  }

  else if (
    S_lpsrChordNamesContext
      firstChordNamesContext =
        dynamic_cast<lpsrChordNamesContext*>(&(*first))
    ) {
    // first is a chord names context
    result =
      compareChordNamesContextWithOtherElement (
        firstChordNamesContext,
        second);
  }

  else if (
    S_lpsrFiguredBassContext
      firstFiguredBassContext =
        dynamic_cast<lpsrFiguredBassContext*>(&(*first))
    ) {
    // first is a figured bass context
    result = true;
  }

  else {
    std::stringstream s;

    s <<
      "part block element '" <<
      first->asShortString () <<
      " is not a staff nor a chord names or figured bass context";

    lpsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      first->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

void lpsrPartBlock::appendStaffBlockToPartBlock (
  S_lpsrStaffBlock staffBlock)
{
  fPartBlockElementsList.push_back (staffBlock);
}

void lpsrPartBlock::appendChordNamesContextToPartBlock (
  int                     inputLineNumber,
  S_lpsrChordNamesContext chordNamesContext)
{
  // appent chordNamesContext to the part block elements list
  fPartBlockElementsList.push_back (chordNamesContext);

  // sort the std::list if necessary
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Sorting the voices in part block for part \"" <<
      fPart->getPartCombinedName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

/* JMI
  gLogStream <<
    std::endl << std::endl <<
    "@@@@@@@@@@@@@@@@ fPartBlockElementsList contains initially:" <<
    std::endl << std::endl;

  for (
    std::list<S_msrElement>::const_iterator i = fPartBlockElementsList.begin ();
    i != fPartBlockElementsList.end ();
    ++i
  ) {
    S_msrElement
      element = (*i);

    gLogStream <<
      element->asShortString () <<
      std::endl;
  } // for
  gLogStream <<
    std::endl << std::endl;
*/

  // sort fPartBlockElementsList, to have harmonies just before
  // the corresponding voice
  if (fPartBlockElementsList.size ()) {
    fPartBlockElementsList.sort (
      compareElementsToHaveHarmoniesAboveCorrespondingStaff);
  }

/* JMI
  gLogStream <<
    std::endl << std::endl <<
    "@@@@@@@@@@@@@@@@ fPartBlockElementsList contains after sort:" <<
    std::endl << std::endl;

  for (
    std::list<S_msrElement>::const_iterator i = fPartBlockElementsList.begin ();
    i != fPartBlockElementsList.end ();
    ++i
  ) {
    S_msrElement
      element = (*i);

    gLogStream <<
      element->asShortString () <<
      std::endl;
  } // for
  gLogStream <<
    std::endl << std::endl;
*/
}

void lpsrPartBlock::appendFiguredBassContextToPartBlock (
  S_lpsrFiguredBassContext context)
{
  fPartBlockElementsList.push_back (context);
}

void lpsrPartBlock::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrPartBlock::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartBlock>*> (v)) {
        S_lpsrPartBlock elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrPartBlock::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrPartBlock::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrPartBlock::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartBlock>*> (v)) {
        S_lpsrPartBlock elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrPartBlock::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrPartBlock::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrPartBlock::browseData ()" <<
      std::endl;
  }
#endif

  for (
    std::list<S_msrElement>::const_iterator i = fPartBlockElementsList.begin ();
    i != fPartBlockElementsList.end ();
    ++i
  ) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% <== lpsrPartBlock::browseData ()" <<
      std::endl;
  }
#endif
}

void lpsrPartBlock::print (std::ostream& os) const
{
  os <<
    "PartBlock" << ' ' <<
    "for part " << fPart->getPartCombinedName () <<
    ", " <<
    mfSingularOrPlural (
      fPartBlockElementsList.size (), "element", "elements") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 28;

  os << std::left <<
    std::setw (fieldWidth) << std::string ("") + "partName" << " = \"" <<
    fPart->getPartName () << "\"" <<
    std::endl <<
    std::setw (fieldWidth) << std::string ("") + "partAbbreviation" << " = \"" <<
    fPart->getPartAbbreviation () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) << "partBlockInstrumentName" << " = \"" <<
    fPartBlockInstrumentName <<
    "\"" <<
    std::endl <<
    std::setw (fieldWidth) << "partBlockShortInstrumentName" << " = \"" <<
    fPartBlockShortInstrumentName <<
    "\"" <<
    std::endl;

  os << std::endl;

  if (fPartBlockElementsList.size ()) {
    std::list<S_msrElement>::const_iterator
      iBegin = fPartBlockElementsList.begin (),
      iEnd   = fPartBlockElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrPartBlock& scr)
{
  scr->print (os);
  return os;
}


}
