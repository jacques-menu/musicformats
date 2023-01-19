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

#include "oahEarlyOptions.h"

#include "oahWae.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_mfService gGlobalService;

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

void mfService::print (std::ostream& os) const
{
  os <<
    "[Service \"" << fServiceName << " \":" <<
    std::endl;

  ++gIndenter;

  // service passIDs
  // --------------------------------------

  const int fieldWidth = 21;

  for (S_mfPassDescription passDescription : fServicePassDescriptionsList) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "passDescription " << ": " <<
      passDescription->asString () <<
      std::endl;
  } // for

  --gIndenter;

  os << "]" << std::endl;
}

std::ostream& operator << (std::ostream& os, const mfService& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
S_mfServiceRunData gGlobalServiceRunData;

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


}
