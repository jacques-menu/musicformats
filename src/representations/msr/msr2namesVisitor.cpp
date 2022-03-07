/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "oahOah.h"
#include "msrOah.h"

#include "msr2namesVisitor.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//________________________________________________________________________
msr2namesVisitor::msr2namesVisitor (
  S_msrOahGroup& msrOpts)
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
      endl;
  }

  gLogStream <<
    "MSR names in \"" <<
    gGlobalServiceRunData->getInputSourceName () <<
    "\":" <<
    endl << endl;

  ++gIndenter;
}

void msr2namesVisitor::visitEnd (S_msrScore& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrScore" <<
      endl;
  }

  gLogStream <<
    "The score contains:" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 3;

  gLogStream <<
    setw (fieldWidth) <<
    mfSingularOrPlural (
      fPartGroupsCounter,
      "part group",
      "part groups") <<
    endl <<

    setw (fieldWidth) <<
    mfSingularOrPlural (
      fPartsCounter,
      "part",
      "parts") <<
    endl <<

    setw (fieldWidth) <<
    mfSingularOrPlural (
      fStavesCounter,
      "stave",
      "staves") <<
    endl <<

    setw (fieldWidth) <<
    mfSingularOrPlural (
      fVoicesCounter,
      "voice",
      "voices") <<
    endl;

  --gIndenter;
}

//________________________________________________________________________
void msr2namesVisitor::visitStart (S_msrPartGroup& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPartGroup" <<
      endl;
  }

  ++fPartGroupsCounter;

  unsigned int partGroupElementsSize =
    elt->
      getPartGroupElementsList ().size ();

  gLogStream <<
    "PartGroup" << ' ' << elt->getPartGroupCombinedName () <<
    " contains " <<
    mfSingularOrPlural (
      partGroupElementsSize,
      " part or sub part group",
      " parts or sub part groups") <<
    endl;

  ++gIndenter;

  const int fieldWidth = 25;

  gLogStream << left <<
    setw (fieldWidth) <<
    "partGroupName" << " : \"" <<
    elt->getPartGroupName  () << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partGroupNameDisplayText" << " : \"" <<
    elt->getPartGroupNameDisplayText  () << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partGroupAbbrevation" << " : \"" <<
    elt->getPartGroupAbbreviation  () << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "partGroupInstrumentName" << " : \"" <<
      elt->getPartGroupInstrumentName () << "\"" <<
    endl << endl;
}

void msr2namesVisitor::visitEnd (S_msrPartGroup& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPartGroup" <<
      endl;
  }
}

//________________________________________________________________________
void msr2namesVisitor::visitStart (S_msrPart& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPart" <<
      endl;
  }

  ++fPartsCounter;

  unsigned int partStavesMapSize =
    elt->
      getPartStaveNumbersToStavesMap ().size ();

  gLogStream <<
    "Part " << elt->getPartCombinedName () <<
    " contains " <<
    mfSingularOrPlural (
      partStavesMapSize,
      "staff", "staves") <<
    endl;

  ++gIndenter;

  const int fieldWidth = 27;

  gLogStream << left <<
    setw (fieldWidth) <<
    "partID" << " : \"" <<
    elt->getPartID () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partMsrName" << " : \"" <<
    elt->getPartMsrName () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partName" << " : \"" <<
    elt->getPartName  () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partAbbrevation" << " : \"" <<
    elt->getPartAbbreviation () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partInstrumentName" << " : \"" <<
    elt->getPartInstrumentName () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partInstrumentAbbreviation" << " : \"" <<
    elt->getPartInstrumentAbbreviation () << "\"" <<
    endl;

  gLogStream << endl;
}

void msr2namesVisitor::visitEnd (S_msrPart& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPart" <<
      endl;
  }
}

//________________________________________________________________________
void msr2namesVisitor::visitStart (S_msrStaff& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaff" <<
      endl;
  }

  ++fStavesCounter;

  unsigned int staffAllVoicesVectorSize =
    elt->
      getStaffAllVoicesVector ().size ();

  gLogStream <<
    "Staff" << ' ' << elt->getStaffName () <<
    " contains " <<
    mfSingularOrPlural (
      staffAllVoicesVectorSize,
      "voice", "voices") <<
    endl;

  ++gIndenter;


  const int fieldWidth = 28;

  gLogStream << left <<
    setw (fieldWidth) <<
    "staffNumber" << " : " <<
    elt->getStaffNumber () <<
    endl <<

    setw (fieldWidth) <<
    "staffInstrumentName" << ": \"" <<
    elt->getStaffInstrumentName () << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "staffInstrumentAbbreviation" << ": \"" <<
    elt->getStaffInstrumentAbbreviation () << "\"" <<
    endl;

  gLogStream << endl;

  fOnGoingStaff = true;
}

void msr2namesVisitor::visitEnd (S_msrStaff& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStaff" <<
      endl;
  }

  fOnGoingStaff = false;
}

//________________________________________________________________________
void msr2namesVisitor::visitStart (S_msrVoice& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrVoice" <<
      endl;
  }

  ++fVoicesCounter;

  unsigned int voiceStanzasMapSize = elt->getVoiceStanzasMap ().size ();

  gLogStream <<
    "Voice" << ' ' << elt->getVoiceName () <<
    " has " <<
    mfSingularOrPlural (
      voiceStanzasMapSize,
      "stanza", "stanzas") <<
    endl;

  ++gIndenter;

  const int fieldWidth = 28;

  gLogStream << left <<
    setw (fieldWidth) <<
    "voiceNumber" << " : " <<
    elt->getVoiceNumber () <<
    endl <<
    setw (fieldWidth) <<
    "musicHasBeenInsertedInVoice" << " : " <<
    elt->getMusicHasBeenInsertedInVoice () <<
    endl;

  gLogStream << endl;
}

void msr2namesVisitor::visitEnd (S_msrVoice& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrVoice" <<
      endl;
  }
}


} // namespace
