/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfServices.h"

#include "mfStringsHandling.h"

#include "oahEarlyOptions.h"

#include "msrScores.h"

#include "oahOah.h"
#include "msrOah.h"
#include "lpsrOah.h"

#include "lpsr2namesVisitor.h"

#include "lpsrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
lpsr2namesVisitor::lpsr2namesVisitor (
  const S_lpsrOahGroup& lpsrOpts)
{
  fLpsrOahGroup = lpsrOpts;

  // part groups
  fPartGroupsCounter = 0;

  // parts
  fPartsCounter = 0;

  // staves
  fStavesCounter = 0;
  fOnGoingStaff = false;

  // voices
  fVoicesCounter = 0;
};

lpsr2namesVisitor::~lpsr2namesVisitor ()
{}

//________________________________________________________________________
void lpsr2namesVisitor::printNamesFromLpsrScore (
  const S_lpsrScore& score)
{
  if (score) {
    // set the parts browsing order
    score->
      getEmbeddedMsrScore ()->
        setStavesBrowingOrderKind (
          msrStavesBrowingOrderKind::kStavesBrowingOrderHarmoniesRegularsFiguredBasses);

    // create a lpsrScore browser
    lpsrBrowser<lpsrScore> browser (this);

    // browse the score with the browser
    browser.browse (*score);
  }
}

//________________________________________________________________________
void lpsr2namesVisitor::visitStart (S_msrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting lpsrScore";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  gLog <<
    "MSR names in \"" <<
    gServiceRunData->getInputSourceName () <<
    "\":" <<
    std::endl << std::endl;

  ++gIndenter;
}

void lpsr2namesVisitor::visitEnd (S_msrScore& elt)
{
  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting lpsrScore";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  gLog <<
    "The score contains:" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 3;

  gLog <<
    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fPartGroupsCounter,
      "part group",
      "part groups") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fPartsCounter,
      "part",
      "parts") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fStavesCounter,
      "staff",
      "staves") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fVoicesCounter,
      "voice",
      "voices") <<
    std::endl;

  --gIndenter;
}

//________________________________________________________________________
void lpsr2namesVisitor::visitStart (S_msrPartGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting lpsrPartGroup";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fPartGroupsCounter;

  size_t partGroupElementsSize =
    elt->
      getPartGroupElementsList ().size ();

  gLog <<
    "PartGroup" << ' ' << elt->getPartGroupCombinedName () <<
    " contains " <<
    mfSingularOrPlural (
      partGroupElementsSize,
      " part or sub part group",
      " parts or sub part groups") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 25;

  gLog << std::left <<
    std::setw (fieldWidth) <<
    "partGroupName" << ": \"" <<
    elt->getPartGroupName  () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupNameDisplayText" << ": \"" <<
    elt->getPartGroupNameDisplayText  () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupAbbrevation" << ": \"" <<
    elt->getPartGroupAbbreviation  () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupInstrumentName" << ": \"" <<
      elt->getPartGroupInstrumentName () << "\"" <<
    std::endl << std::endl;
}

void lpsr2namesVisitor::visitEnd (S_msrPartGroup& elt)
{
  --gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting lpsrPartGroup";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2namesVisitor::visitStart (S_msrPart& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting lpsrPart";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fPartsCounter;

  size_t partStavesMapSize =
    elt->
      getPartStaveNumbersToStavesMap ().size ();

  gLog <<
    "Part " << elt->getPartCombinedName () <<
    " contains " <<
    mfSingularOrPlural (
      partStavesMapSize,
      "staff", "staves") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 27;

  gLog << std::left <<
    std::setw (fieldWidth) <<
    "partID" << ": \"" <<
    elt->getPartID () << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "partMsrName" << ": \"" <<
    elt->getPartMsrName () << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "partName" << ": \"" <<
    elt->getPartName  () << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "partAbbrevation" << ": \"" <<
    elt->getPartAbbreviation () << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "partInstrumentName" << ": \"" <<
    elt->getPartInstrumentName () << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "partInstrumentAbbreviation" << ": \"" <<
    elt->getPartInstrumentAbbreviation () << "\"" <<
    std::endl << std::endl;
}

void lpsr2namesVisitor::visitEnd (S_msrPart& elt)
{
  --gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting lpsrPart";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2namesVisitor::visitStart (S_msrStaff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting lpsrStaff";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fStavesCounter;

  size_t staffAllVoicesVectorSize =
    elt->
      getStaffAllVoicesVector ().size ();

  gLog <<
    "Staff" << ' ' << elt->getStaffName () <<
    " contains " <<
    mfSingularOrPlural (
      staffAllVoicesVectorSize,
      "voice", "voices") <<
    std::endl;

  ++gIndenter;


  const int fieldWidth = 28;

  gLog << std::left <<
    std::setw (fieldWidth) <<
    "staffNumber" << ": " <<
    elt->getStaffNumber () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "staffInstrumentName" << ": \"" <<
    elt->getStaffInstrumentName () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "staffInstrumentAbbreviation" << ": \"" <<
    elt->getStaffInstrumentAbbreviation () << "\"" <<
    std::endl;

  gLog << std::endl;

  fOnGoingStaff = true;
}

void lpsr2namesVisitor::visitEnd (S_msrStaff& elt)
{
  --gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting lpsrStaff";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingStaff = false;
}

//________________________________________________________________________
void lpsr2namesVisitor::visitStart (S_msrVoice& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting lpsrVoice";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fVoicesCounter;

  size_t voiceStanzasMapSize = elt->getVoiceStanzasMap ().size ();

  gLog <<
    "Voice" << ' ' << elt->getVoiceName () <<
    " has " <<
    mfSingularOrPlural (
      voiceStanzasMapSize,
      "stanza", "stanzas") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 28;

  gLog << std::left <<
    std::setw (fieldWidth) <<
    "voiceNumber" << ": " <<
    elt->getVoiceNumber () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "musicHasBeenInsertedInVoice" << ": " <<
    elt->getMusicHasBeenInsertedInVoice () <<
    std::endl;

  gLog << std::endl;
}

void lpsr2namesVisitor::visitEnd (S_msrVoice& elt)
{
  --gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting lpsrVoice";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}


} // namespace
