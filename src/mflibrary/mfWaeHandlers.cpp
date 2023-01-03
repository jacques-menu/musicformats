/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "msdlKeywords.h"
#include "msdlTokens.h"

#include "mfWaeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
/* this class is purely virtual
S_mfWaeHandler mfWaeHandler::create ()
{
  mfWaeHandler* o =
    new mfWaeHandler ();
  assert (o != nullptr);
  return o;
}
*/

mfWaeHandler::mfWaeHandler ()
{}

mfWaeHandler::~mfWaeHandler ()
{}

std::string mfWaeHandler::asString () const
{
  std::stringstream s;

  s <<
    "MSDL WAE lexical handler";

  return s.str ();
}

void mfWaeHandler::print (std::ostream& os) const
{
  os <<
    "mfWaeHandler" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mfWaeHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
// pass numbers

std::string mfWaeHandler::pass1 () const
{
  return "Pass 1";
}

std::string mfWaeHandler::pass2 () const
{
  return "Pass 2";
}
std::string mfWaeHandler::pass2a () const
{
  return "Pass 2a";
}
std::string mfWaeHandler::pass2b () const
{
  return "Pass 2b";
}

std::string mfWaeHandler::pass3 () const
{
  return "Pass 3";
}
std::string mfWaeHandler::pass3a () const
{
  return "Pass 3a";
}
std::string mfWaeHandler::pass3b () const
{
  return "Pass 3b";
}

std::string mfWaeHandler::pass4 () const
{
  return "Pass 4";
}
std::string mfWaeHandler::pass4a () const
{
  return "Pass 4a";
}
std::string mfWaeHandler::pass4b () const
{
  return "Pass 4b";
}

std::string mfWaeHandler::pass5 () const
{
  return "Pass 5";
}

std::string mfWaeHandler::passOptional () const
{
  return "Pass (optional)";
}

std::string mfWaeHandler::fullVersion () const
{
  return "full version";
}
std::string mfWaeHandler::summary () const
{
  return "summary";
}
std::string mfWaeHandler::names () const
{
  return "names";
}
std::string mfWaeHandler::slices () const
{
  return "slices";
}

//_______________________________________________________________________________
// OAH

std::string mfWaeHandler::handleOptionsAndArgumentsFromArgcArgv () const
{
  return "Handle the options and arguments from argc/argv";
}

//_______________________________________________________________________________
// passes

std::string mfWaeHandler::createAnMXSRFromAMusicXMLFile () const
{
  return "Create an MXSR from a MusicXML file";
}
std::string mfWaeHandler::createAnMXSRFromAMusicXMLDescriptor () const
{
  return "Create an MXSR from a MusicXML descriptor";
}
std::string mfWaeHandler::createAnMXSRFromAMusicXMLBuffer () const
{
  return "Create an MXSR from a MusicXML buffer";
}

std::string mfWaeHandler::createAnMSRSqueletonFromTheMXSR () const
{
  return "Create an MSR skeleton from the MXSR";
}

std::string mfWaeHandler::populateTheMSRSqueletonFromMusicXMLData () const
{
  return "Populate the MSR skeleton from MusicXML data";
}

std::string mfWaeHandler::convertTheFirstMSRIntoASecondMSR () const
{
  return "Convert the first MSR into a second MSR";
}

std::string mfWaeHandler::convertTheSecondMSRIntoAnLPSR () const
{
  return "Convert the second MSR into an LPSR";
}

std::string mfWaeHandler::convertTheLPSRIntoLilyPondCode () const
{
  return "Convert the LPSR into LilyPond code";
}

//_______________________________________________________________________________
// display

std::string mfWaeHandler::displayTheFirstMSRSkeletonAsText () const
{
  return "Display the first MSR skeleton as text";
}
std::string mfWaeHandler::displayTheFirstMSRAsText () const
{
  return "Display the second MSR as text";
}
std::string mfWaeHandler::displayTheSecondMSRAsText () const
{
  return "Timing information";
}
std::string mfWaeHandler::displayTheLPSRAsText () const
{
  return "Display the LPSR as text";
}

std::string mfWaeHandler::displayTheNamesInTheFirstMSR () const
{
  return "Display the names in the first MSR";
}
std::string mfWaeHandler::displayTheNamesInTheSecondMSR () const
{
  return "Display the names in the second MSR";
}

std::string mfWaeHandler::displayASummaryOfTheFirstMSR () const
{
  return "Display a summary of the first MSR";
}
std::string mfWaeHandler::displayASummaryOfTheSecondMSR () const
{
  return "Display a summary of the second MSR";
}

//_______________________________________________________________________________
// timing

std::string mfWaeHandler::timingInformation () const
{
  return "Timing information";
}
std::string mfWaeHandler::activity () const
{
  return "Activity";
}
std::string mfWaeHandler::description () const
{
  return "Description";
}
std::string mfWaeHandler::kind () const
{
  return "Kind";
}
std::string mfWaeHandler::CPUSeconds () const
{
  return "CPU (sec)";
}
std::string mfWaeHandler::mandatory () const
{
  return "mandatory";
}
std::string mfWaeHandler::optional () const
{
  return "optional";
}
std::string mfWaeHandler::totalSeconds () const
{
  return "Total (sec)";
}


} // namespace
