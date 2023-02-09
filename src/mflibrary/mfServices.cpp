/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfStaticSettings.h"

#include "mfServices.h"
#include "mfStringsHandling.h"

#include "languages.h"

#include "oahEarlyOptions.h"

#include "oahWae.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_mfService mfService::create (
  const std::string& serviceName)
{
  mfService* o = new
    mfService (
      serviceName);
  assert (o != nullptr);
  return o;
}

mfService::mfService (
  const std::string& serviceName)
{
  // service name
  fServiceName = serviceName;
}

mfService::~mfService ()
{}

void mfService::fetchPassIDKindList (
  std::list<mfPassIDKind>& passIDKindList) const
{
  for (S_mfPassDescription passDescription : fServicePassDescriptionsList) {
    passIDKindList.push_back (
      passDescription-> getPassIDKind());
  } // for
}

size_t mfService::fetchMaxPassIDKindAsStringLength () const
{
  size_t result = 0;

  for (S_mfPassDescription passDescription : fServicePassDescriptionsList) {
    std::string
      passIDKindAsString =
        mfPassIDKindAsString (
          passDescription->getPassIDKind ());

    size_t
      passIDKindAsStringSize =
        passIDKindAsString.size ();

    if (passIDKindAsStringSize > result) {
      result = passIDKindAsStringSize;
    }
  } // for

  return result;
}

void mfService::print (std::ostream& os) const
{
  os <<
    "[mfService" <<
    ", fServiceName: " << fServiceName << ":" <<
    std::endl;

  ++gIndenter;

  // service passIDs
  // --------------------------------------

  const int fieldWidth = fetchMaxPassIDKindAsStringLength ();

  for (S_mfPassDescription passDescription : fServicePassDescriptionsList) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "passIDKind:" <<
      std::endl;

    ++gIndenter;
    os <<
      gLanguage->
        passIDKindAsString (
          passDescription->getPassIDKind ()) <<
          std::endl;
    --gIndenter;

    os <<
      std::setw (fieldWidth) <<
      "passDescription " << ": " <<
      std::endl;

    ++gIndenter;
    os <<
      gIndenter.indentMultiLineStringWithCurrentOffset (
        passDescription->getPassDescription ()) <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

  os << "]" << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mfService& elt)
{
  elt->print (os);
  return os;
}

std::string mfService::fetchServicePassDescriptionsAsString () const
{
  mfIndentedStringStream iss;

  iss <<
    "What " << fServiceName << " does:" <<
    "\n\n";

  ++gIndenter;

  iss <<
    "This multi-pass converter basically performs " <<
    fServicePassDescriptionsList.size () <<
    " passes:" <<
    '\n';

  ++gIndenter;

//   const int fieldWidth = 10;

  for (S_mfPassDescription passDescription : fServicePassDescriptionsList) {
    iss << // JMI std::left <<
//       std::setw (fieldWidth) <<
      passDescription->getPassIDKind () << ": " <<
      passDescription->getPassDescription () <<
      '\n';
  } // for

  --gIndenter;

  iss <<
    "Other passes are performed according to the options, such as" <<
    '\n' <<
    "displaying views of the internal data or printing a summary of the score."
    "\n\n" <<

    "The activity log and warning/error messages go to standard error." <<
    '\n';

  --gIndenter;

  return iss.str ();
}

void mfService::printServiceForAboutOption (std::ostream& os) const
{
  os <<
    "What " << fServiceName << " does:" <<
    std::endl << std::endl;

  ++gIndenter;

  os <<
    "This multi-pass converter basically performs " <<
    mfSingularOrPlural (
      fServicePassDescriptionsList.size (),
      "pass", "passes") <<
    ':' <<
    std::endl;

  ++gIndenter;

//   const int fieldWidth = 21;

  for (S_mfPassDescription passDescription : fServicePassDescriptionsList) {
    os << // std::left <<
//       std::setw (fieldWidth) <<
      gLanguage->
        passIDKindAsString (
          passDescription->getPassIDKind ()) <<
      ":" <<
      std::endl;

//       ++gIndenter;
      os <<
//         gIndenter.indentMultiLineStringWithCurrentOffset (
//           passDescription->getPassDescription ()) <<
        passDescription->getPassDescription () <<
        std::endl;
//       --gIndenter;
  } // for

  --gIndenter;

  os <<
    std::endl <<
    "Other passes are performed according to the options, such as" <<
    std::endl <<
    "displaying views of the internal data or printing a summary of the score." <<
    std::endl << std::endl <<

    "The activity log and warning/error messages go to standard error." <<
    std::endl;

  --gIndenter;
}

//_______________________________________________________________________________
S_mfServiceRunData gServiceRunData;
S_mfService gGlobalCurrentService;

S_mfServiceRunData mfServiceRunData::create (
  const std::string& serviceName)
{
  mfServiceRunData* o = new
    mfServiceRunData (
      serviceName);
  assert (o != nullptr);
  return o;
}

// S_mfServiceRunData mfServiceRunData::create (
//   const std::string& serviceName,
//   S_oahHandler  runOahHandler)
// {
//   mfServiceRunData* o = new
//     mfServiceRunData (
//       serviceName,
//       runOahHandler);
//   assert (o != nullptr);
//   return o;
// }

mfServiceRunData::mfServiceRunData (
  const std::string& serviceName)
{
  // service name
  fServiceName = serviceName;

//   // OAH handler
//   fRunOahHandler = runOahHandler;

  // run date
  initializeRunDate ();
}

// mfServiceRunData::mfServiceRunData (
//   const std::string& serviceName,
//   S_oahHandler  runOahHandler)
// {
//   // service name
//   fServiceName = serviceName;
//
//   // OAH handler
//   fRunOahHandler = runOahHandler;
//
//   // run date
//   initializeRunDate ();
// }

mfServiceRunData::~mfServiceRunData ()
{}

void mfServiceRunData::initializeRunDate ()
{
  time_t      runRawtime;
  struct tm*  runTimeinfo;
  char buffer [80];

  time (&runRawtime);
  runTimeinfo = localtime (&runRawtime);

  strftime (buffer, 80, "%A %F @ %T %Z", runTimeinfo);
  fRunDateFull = buffer;

  strftime (buffer, 80, "%Y-%m-%d", runTimeinfo);
  fRunDateYYYYMMDD = buffer;
}

void mfServiceRunData::print (std::ostream& os) const
{
  os <<
    "The service run data are:" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 21;

  // service name
  // --------------------------------------

  os << std::left <<
    std::setw (fieldWidth) << "Service name" << ": " <<
    fServiceName <<
    std::endl;

//   // OAH handler
//   // --------------------------------------
//
//   os << std::left <<
//     std::setw (fieldWidth) << "OAH handler" << ": " <<
//     fRunOahHandler->getHandlerHeader () <<
//     std::endl;

  // input source name
  // --------------------------------------

  os << std::left <<
    std::setw (fieldWidth) << "fInputSourceName" << ": " << // JMIJMIJMI in oahHandler???
    fInputSourceName <<
    std::endl;

  // run date
  // --------------------------------------

  os << std::left <<
    std::setw (fieldWidth) << "Run date:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (fieldWidth) << "fRunDateFull" << ": " <<
    fRunDateFull <<
    std::endl <<
    std::setw (fieldWidth) << "fRunDateYYYYMMDD" << ": " <<
    fRunDateYYYYMMDD <<
    std::endl;

  --gIndenter;


  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const mfServiceRunData& elt)
{
  elt.print (os);
  return os;
}

//________________________________________________________________________
// hidden global service variable
S_mfService pGlobalService;

EXP void setGlobalService (S_mfService service)
{
  pGlobalService = service;
}

EXP S_mfService getGlobalService ()
{
  return pGlobalService;
}

// hidden global current service run data variable
S_mfServiceRunData pGlobalServiceRunData;

EXP void setGlobalServiceRunData (S_mfServiceRunData serviceRunData)
{
  pGlobalServiceRunData = serviceRunData;
}

EXP S_mfServiceRunData getGlobalServiceRunData ()
{
  return pGlobalServiceRunData;
}


}
