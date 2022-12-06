/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "oahOah.h"
#include "msrOah.h"

#include "msr2namesVisitor.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//________________________________________________________________________
msr2namesVisitor::msr2namesVisitor (
  const S_msrOahGroup& msrOpts)
{
  fMsrOahGroup = msrOpts;

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

msr2namesVisitor::~msr2namesVisitor ()
{}

//________________________________________________________________________
void msr2namesVisitor::printNamesFromMsrScore (
  const S_msrScore& score)
{
  if (score) {
    // create a msrScore browser
    msrBrowser<msrScore> browser (this);

    // browse the score with the browser
    browser.browse (*score);
  }
}

//________________________________________________________________________
void msr2namesVisitor::visitStart (S_msrScore& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScore" <<
      std::endl;
  }

  gLogStream <<
    "MSR names in \"" <<
    gGlobalServiceRunData->getInputSourceName () <<
    "\":" <<
    std::endl << std::endl;

  ++gIndenter;
}

void msr2namesVisitor::visitEnd (S_msrScore& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrScore" <<
      std::endl;
  }

  gLogStream <<
    "The score contains:" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 3;

  gLogStream <<
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
      "stave",
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
void msr2namesVisitor::visitStart (S_msrPartGroup& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPartGroup" <<
      std::endl;
  }

  ++fPartGroupsCounter;

  size_t partGroupElementsSize =
    elt->
      getPartGroupElementsList ().size ();

  gLogStream <<
    "PartGroup" << ' ' << elt->getPartGroupCombinedName () <<
    " contains " <<
    mfSingularOrPlural (
      partGroupElementsSize,
      " part or sub part group",
      " parts or sub part groups") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 25;

  gLogStream << std::left <<
    std::setw (fieldWidth) <<
    "partGroupName" << " : \"" <<
    elt->getPartGroupName  () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupNameDisplayText" << " : \"" <<
    elt->getPartGroupNameDisplayText  () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupAbbrevation" << " : \"" <<
    elt->getPartGroupAbbreviation  () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupInstrumentName" << " : \"" <<
      elt->getPartGroupInstrumentName () << "\"" <<
    std::endl << std::endl;
}

void msr2namesVisitor::visitEnd (S_msrPartGroup& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPartGroup" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2namesVisitor::visitStart (S_msrPart& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPart" <<
      std::endl;
  }

  ++fPartsCounter;

  size_t partStavesMapSize =
    elt->
      getPartStaveNumbersToStavesMap ().size ();

  gLogStream <<
    "Part " << elt->getPartCombinedName () <<
    " contains " <<
    mfSingularOrPlural (
      partStavesMapSize,
      "staff", "staves") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 27;

  gLogStream << std::left <<
    std::setw (fieldWidth) <<
    "partID" << " : \"" <<
    elt->getPartID () << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "partMsrName" << " : \"" <<
    elt->getPartMsrName () << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "partName" << " : \"" <<
    elt->getPartName  () << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "partAbbrevation" << " : \"" <<
    elt->getPartAbbreviation () << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "partInstrumentName" << " : \"" <<
    elt->getPartInstrumentName () << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "partInstrumentAbbreviation" << " : \"" <<
    elt->getPartInstrumentAbbreviation () << "\"" <<
    std::endl;

  gLogStream << std::endl;
}

void msr2namesVisitor::visitEnd (S_msrPart& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPart" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2namesVisitor::visitStart (S_msrStaff& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaff" <<
      std::endl;
  }

  ++fStavesCounter;

  size_t staffAllVoicesVectorSize =
    elt->
      getStaffAllVoicesVector ().size ();

  gLogStream <<
    "Staff" << ' ' << elt->getStaffName () <<
    " contains " <<
    mfSingularOrPlural (
      staffAllVoicesVectorSize,
      "voice", "voices") <<
    std::endl;

  ++gIndenter;


  const int fieldWidth = 28;

  gLogStream << std::left <<
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

  gLogStream << std::endl;

  fOnGoingStaff = true;
}

void msr2namesVisitor::visitEnd (S_msrStaff& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStaff" <<
      std::endl;
  }

  fOnGoingStaff = false;
}

//________________________________________________________________________
void msr2namesVisitor::visitStart (S_msrVoice& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrVoice" <<
      std::endl;
  }

  ++fVoicesCounter;

  size_t voiceStanzasMapSize = elt->getVoiceStanzasMap ().size ();

  gLogStream <<
    "Voice" << ' ' << elt->getVoiceName () <<
    " has " <<
    mfSingularOrPlural (
      voiceStanzasMapSize,
      "stanza", "stanzas") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 28;

  gLogStream << std::left <<
    std::setw (fieldWidth) <<
    "voiceNumber" << ": " <<
    elt->getVoiceNumber () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "musicHasBeenInsertedInVoice" << ": " <<
    elt->getMusicHasBeenInsertedInVoice () <<
    std::endl;

  gLogStream << std::endl;
}

void msr2namesVisitor::visitEnd (S_msrVoice& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrVoice" <<
      std::endl;
  }
}


} // namespace
