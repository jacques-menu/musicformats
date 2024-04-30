/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfPreprocessorSettings.h"

#include "mfConstants.h"
#include "msrMeasureConstants.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "languages.h"

#include "oahEarlyOptions.h"

#include "oahWae.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::map<std::string, mfServiceKind>
  gGlobalMusicFormatsServiceKindsMap;

std::string mfServiceKindAsString (
  mfServiceKind serviceKind)
{
  std::string result;

  // these strings are used in the command line options
  switch (serviceKind) {
    case mfServiceKind::kMfService_UNKNOWN_:
      result = "*kMfService_UNKNOWN_*";
      break;

    case mfServiceKind::kMfService_xml2ly:
      result = "kMfService_xml2ly";
      break;

    case mfServiceKind::kMfService_xml2brl:
      result = "kMfService_xml2brl";
      break;

    case mfServiceKind::kMfService_xml2xml:
      result = "kMfService_xml2xml";
      break;

    case mfServiceKind::kMfService_xml2gmn:
      result = "kMfService_xml2gmn";
      break;

    case mfServiceKind::kMfService_stringMatcher:
      result = "kMfService_stringMatcher";
      break;

    case mfServiceKind::kMfService_msdl:
      result = "kMfService_msdl";
      break;

    case mfServiceKind::kMfService_mfsl:
      result = "kMfService_mfsl";
      break;

    case mfServiceKind::kMfService_ischeme:
      result = "kMfService_ischeme";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mfServiceKind elt)
{
  os << mfServiceKindAsString (elt);
  return os;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const mfServiceKind& elt)
{
  iss << mfServiceKindAsString (elt);
  return iss;
}

mfServiceKind mfServiceKindFromString (const std::string& theString)
{
  mfServiceKind result =
    mfServiceKind::kMfService_UNKNOWN_;

  std::map<std::string, mfServiceKind>::const_iterator
    it =
      gGlobalMusicFormatsServiceKindsMap.find (
        theString);

  if (it == gGlobalMusicFormatsServiceKindsMap.end ()) {
    // no, service kind is unknown in the map
    std::stringstream ss;

    ss <<
      "MusicFormats service kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMusicFormatsServiceKindsMap.size () - 1 <<
      " known MusicFormats service kinds are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableMusicFormatsServiceKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  result = (*it).second;

  return result;
}

std::string availableMusicFormatsServiceKinds (size_t namesListMaxLength)
{
  std::stringstream ss;

  size_t
    mfServiceKindsMapSize =
      gGlobalMusicFormatsServiceKindsMap.size ();

  if (mfServiceKindsMapSize) {
    size_t nextToLast =
      mfServiceKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::pair<std::string, mfServiceKind>
        thePair :
          gGlobalMusicFormatsServiceKindsMap
    ) {
      std::string theString = thePair.first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        ss << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
        break;
      }

      if (count == 1) {
        ss << gIndenter.getSpacer ();
      }
      ss << theString;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != mfServiceKindsMapSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}

void initializeMusicFormatsServiceKindsMap ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
    gGlobalMusicFormatsServiceKindsMap ["xml2ly"] =
      mfServiceKind::kMfService_xml2ly;

    gGlobalMusicFormatsServiceKindsMap ["xml2brl"] =
      mfServiceKind::kMfService_xml2brl;

    gGlobalMusicFormatsServiceKindsMap ["xml2xml"] =
      mfServiceKind::kMfService_xml2xml;

    gGlobalMusicFormatsServiceKindsMap ["xml2gmn"] =
      mfServiceKind::kMfService_xml2gmn;

    gGlobalMusicFormatsServiceKindsMap ["filter"] =
      mfServiceKind::kMfService_stringMatcher;

    gGlobalMusicFormatsServiceKindsMap ["msdl"] =
      mfServiceKind::kMfService_msdl;

    gGlobalMusicFormatsServiceKindsMap ["mfsl"] =
      mfServiceKind::kMfService_mfsl;

    gGlobalMusicFormatsServiceKindsMap ["ischeme"] =
      mfServiceKind::kMfService_ischeme;

    pPrivateThisMethodHasBeenRun = true;
  }
}

void initializeMusicFormatsServices ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
#ifdef MF_TRACE_IS_ENABLED
    if (
      gEarlyOptions.getTraceEarlyOptions ()
        &&
      ! gEarlyOptions.getEarlyQuietOption ()
    ) {
      std::stringstream ss;

      ss <<
        "Initializing MusicFormats services" <<
        std::endl;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

    // services handling
    // ------------------------------------------------------

    initializeMusicFormatsServiceKindsMap ();

    pPrivateThisMethodHasBeenRun = true;
  }
}

//_______________________________________________________________________________
S_mfService mfService::create (
  const std::string& serviceName)
{
  mfService* obj = new
    mfService (
      serviceName);
  assert (obj != nullptr);
  return obj;
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
  for (S_mfPass pass : fServicePassesList) {
    passIDKindList.push_back (
      pass-> getPassIDKind());
  } // for
}

size_t mfService::fetchMaxPassIDKindAsStringLength () const
{
  size_t result = 0;

  for (S_mfPass pass : fServicePassesList) {
    std::string
      passIDKindAsString =
        mfPassIDKindAsString (
          pass->getPassIDKind ());

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

  for (S_mfPass pass : fServicePassesList) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "passIDKind:" <<
      std::endl;

    ++gIndenter;
    os <<
      gLanguage->
        passIDKindAsString (
          pass->getPassIDKind ()) <<
          std::endl;
    --gIndenter;

    os <<
      std::setw (fieldWidth) <<
      "pass " << ": " <<
      std::endl;

    ++gIndenter;
    os <<
      gIndenter.indentMultiLineStringWithCurrentOffset (
        pass->getPassDescription ()) <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

  os << ']' << std::endl;
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
    fServicePassesList.size () <<
    " passes:" <<
    '\n';

  ++gIndenter;

//   const int fieldWidth = 10;

  for (S_mfPass pass : fServicePassesList) {
    iss << // JMI std::left <<
//       std::setw (fieldWidth) <<
      pass->getPassIDKind () << ": " <<
      pass->getPassDescription () <<
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
      fServicePassesList.size (),
      "pass", "passes") <<
    ':' <<
    std::endl;

  ++gIndenter;

//   const int fieldWidth = 21;

  for (S_mfPass pass : fServicePassesList) {
    os << // std::left <<
//       std::setw (fieldWidth) <<
      gLanguage->
        passIDKindAsString (
          pass->getPassIDKind ()) <<
      ":" <<
      std::endl;

//       ++gIndenter;
      os <<
//         gIndenter.indentMultiLineStringWithCurrentOffset (
//           pass->getPassDescription ()) <<
        pass->getPassDescription () <<
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
S_mfServiceRunData mfServiceRunData::create (
  const std::string& serviceName)
{
  mfServiceRunData* obj = new
    mfServiceRunData (
      serviceName);
  assert (obj != nullptr);
  return obj;
}

// S_mfServiceRunData mfServiceRunData::create (
//   const std::string& serviceName,
//   S_oahHandler  runOahHandler)
// {
//   mfServiceRunData* obj = new
//     mfServiceRunData (
//       serviceName,
//       runOahHandler);
//   assert (obj != nullptr);
//   return obj;
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

  // parts
  fCurrentPartIDAndName = "PartIDAndName_???";

  // staves
  fCurrentStaveNumber = K_STAFF_NUMBER_UNKNOWN_;

  // voices
  fCurrentVoiceNumber = K_VOICE_NUMBER_UNKNOWN_;

  // measure numbers
  fCurrentMeasureNumber = K_MEASURE_NUMBER_UNKNOWN_;
  fScoreMeasuresNumber  = 0;
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
std::map<mfServiceKind, S_mfService> pGlobalServicesMap;

// hidden global service variable
S_mfService pGlobalService;

EXP void setGlobalService (mfServiceKind serviceKind)
{
  S_mfService
    service =
      pGlobalServicesMap [serviceKind];

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

//________________________________________________________________________
EXP void initializeServices ()
{
  // xml2ly
  // ------------------------------

  {
    std::string serviceName = "xml2ly";

    S_mfService
      xml2lyService =
        mfService::create (serviceName);

  //     This multi-pass converter basically performs 5 passes:
  //         Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
  //                  and converts it to a MusicXML tree;
  //         Pass 2a: converts that MusicXML tree into
  //                  a first Music Score Representation (MSR) skeleton;
  //         Pass 2b: populates the first MSR skeleton from the MusicXML tree
  //                  to get a full MSR;
  //         Pass 3:  converts the first MSR into a second MSR to apply options
  //         Pass 4:  converts the second MSR into a
  //                  LilyPond Score Representation (LPSR);
  //         Pass 5:  converts the LPSR to LilyPond code
  //                  and writes it to standard output.

    xml2lyService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_1,
          gLanguage->convertAMusicXMLStreamIntoAnMXSR ()));

    xml2lyService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_2,
          gLanguage->convertTheMXSRIntoAnMSRSkeleton ()));

    xml2lyService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_3,
          gLanguage->convertTheFirstMSRIntoASecondMSR ()));

    xml2lyService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_4,
          gLanguage->convertTheSecondMSRIntoAnLPSR ()));

    xml2lyService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_4,
          gLanguage->convertTheFirstMSRIntoAnLPSR ())); // JMI ??? v0.9.70

    xml2lyService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_5,
          gLanguage->convertTheLPSRIntoLilyPondCode ()));

    xml2lyService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_6,
          gLanguage->convertTheLPSRIntoLilyPondCode ()));

    pGlobalServicesMap [mfServiceKind::kMfService_xml2ly] = xml2lyService;

  //   gLog << "--------------" << std::endl;
  //
  //   gLog <<
  //     "xml2lyService::print ()" <<
  //     std::endl;
  //
  //   ++gIndenter;
  //   gLog <<
  //     xml2lyService <<
  //     std::endl;
  //   --gIndenter;
  //
  //   gLog << "--------------" << std::endl;
  //
  //   gLog <<
  //     "xml2lyService::printServiceForAboutOption ()" <<
  //     std::endl;
  //
  //   ++gIndenter;
  //   xml2lyService-> printServiceForAboutOption (gLog);
  //   --gIndenter;
  //
  //   gLog << "--------------" << std::endl;
  //
  //   gLog <<
  //     "xml2lyService::fetchServicePassDescriptionsAsString ()" <<
  //     std::endl;
  //
  //   ++gIndenter;
  //   gLog <<
  //     xml2lyService-> fetchServicePassDescriptionsAsString () <<
  //     std::endl;
  //   --gIndenter;
  //
  //   gLog << "--------------" << std::endl;
  }

  // xml2brl
  // ------------------------------

  {
    std::string serviceName = "xml2brl";

//         Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
//                  and converts it to a MusicXML tree;
//         Pass 2a: converts that MusicXML tree into
//                  a first Music Score Representation (MSR) skeleton;
//         Pass 2b: populates the MSR skeleton from the MusicXML tree
//                  to get a full MSR;
//         Pass 3:  converts the first MSR into a second MSR, to apply options
//         Pass 4:  converts the second MSR into
//                  a first Braille Score Representation (BSR)
//                  containing one Braille page per MusicXML page;
//         Pass 5:  converts the first BSR into a second BSR
//                  with as many Braille pages as needed
//                  to fit the line and page lengthes;
//         Pass 6:  converts the BSR to Braille text
//                  and writes it to standard output.

    S_mfService
      xml2brlService =
        mfService::create (serviceName);

    xml2brlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_1,
          gLanguage->convertAMusicXMLStreamIntoAnMXSR ()));

    xml2brlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_2,
          gLanguage->convertTheMXSRIntoAnMSRSkeleton ()));

    xml2brlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_3,
          gLanguage->populateTheMSRSkeletonFromMusicXMLData ()));

    xml2brlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_4,
          gLanguage->convertTheFirstMSRIntoASecondMSR ()));

    xml2brlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_5,
          gLanguage->convertTheSecondMSRIntoAnLPSR ()));

    xml2brlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_6,
          gLanguage->convertTheLPSRIntoLilyPondCode ()));

    pGlobalServicesMap [mfServiceKind::kMfService_xml2brl] = xml2brlService;
  }

  // xml2xml
  // ------------------------------

  {
    std::string serviceName = "xml2xml";

//         Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
//                  and converts it to a MusicXML tree;
//         Pass 2a: converts that MusicXML tree into
//                  a first Music Score Representation (MSR) skeleton;
//         Pass 2b: populates the MSR skeleton from the MusicXML tree
//                  to get a full MSR;
//         Pass 3:  converts the first MSR into a second MSR, to apply options;
//         Pass 4:  converts the second MSR into a second MusicXML tree;
//         Pass 5:  converts the second MusicXML tree to MusicXML code
//                  and writes it to standard output.

    S_mfService
      xml2xmlService =
        mfService::create (serviceName);

    xml2xmlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_1,
          gLanguage->convertAMusicXMLStreamIntoAnMXSR ()));

    xml2xmlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_2,
          gLanguage->convertTheMXSRIntoAnMSRSkeleton ()));

    xml2xmlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_3,
          gLanguage->populateTheMSRSkeletonFromMusicXMLData ()));

    xml2xmlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_4,
          gLanguage->convertTheFirstMSRIntoASecondMSR ()));

    xml2xmlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_5,
          gLanguage->convertTheSecondMSRIntoAnLPSR ()));

    xml2xmlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_6,
          gLanguage->convertTheLPSRIntoLilyPondCode ()));

    pGlobalServicesMap [mfServiceKind::kMfService_xml2xml] = xml2xmlService;
  }

  // xml2gmn
  // ------------------------------

  {
    std::string serviceName = "xml2gmn";

    S_mfService
      xml2gmnService =
        mfService::create (serviceName);

//         Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
//                  and converts it to a first MusicXML tree;
//         Pass 2a: converts that MusicXML tree into
//                  a first Music Score Representation (MSR) skeleton;
//         Pass 2b: populates the MSR skeleton from the MusicXML tree
//                  to get a full MSR;
//         Pass 3:  converts the first MSR a second MSR, to apply options;
//         Pass 4:  converts the second MSR into a second MusicXML tree;
//         Pass 5:  converts the second MusicXML tree to Guido code
//                  and writes it to standard output.

    xml2gmnService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_1,
          gLanguage->convertAMusicXMLStreamIntoAnMXSR ()));

    xml2gmnService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_2,
          gLanguage->convertTheMXSRIntoAnMSRSkeleton ()));

    xml2gmnService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_3,
          gLanguage->populateTheMSRSkeletonFromMusicXMLData ()));

    xml2gmnService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_4,
          gLanguage->convertTheFirstMSRIntoASecondMSR ()));

    xml2gmnService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_5,
          gLanguage->convertTheSecondMSRIntoAnLPSR ()));

    xml2gmnService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_6,
          gLanguage->convertTheLPSRIntoLilyPondCode ()));

    pGlobalServicesMap [mfServiceKind::kMfService_xml2gmn] = xml2gmnService;
  }

  // filter
  // ------------------------------

  {
    std::string serviceName = "filter";

// What stringMatcherInterpreter does:
//
//     This multi-pass converter performs various passes depending on the output generated,
//     which should be specified by an option
//
//     Other passes are performed according to the options, such as
//     displaying views of the internal data or printing a summary of the score.
//
//     The activity log and warning/error messages go to standard error.

    S_mfService
      filterService =
        mfService::create (serviceName);

    filterService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_1,
          gLanguage->convertAMusicXMLStreamIntoAnMXSR ()));

    filterService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_2,
          gLanguage->convertTheMXSRIntoAnMSRSkeleton ()));

    filterService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_3,
          gLanguage->populateTheMSRSkeletonFromMusicXMLData ()));

    filterService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_4,
          gLanguage->convertTheFirstMSRIntoASecondMSR ()));

    filterService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_5,
          gLanguage->convertTheSecondMSRIntoAnLPSR ()));

    filterService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_6,
          gLanguage->convertTheLPSRIntoLilyPondCode ()));

    pGlobalServicesMap [mfServiceKind::kMfService_stringMatcher] = filterService;
  }

  // msdl
  // ------------------------------

  {
    std::string serviceName = "msdl";

// What msdlConverter does:
//
//     This multi-pass converter performs various passes depending on the output generated,
//     which should be specified by an option
//
//     Other passes are performed according to the options, such as
//     displaying views of the internal data or printing a summary of the score.
//
//     The activity log and warning/error messages go to standard error.

    S_mfService
      msdlService =
        mfService::create (serviceName);

    msdlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_1,
          gLanguage->convertAMusicXMLStreamIntoAnMXSR ()));

    msdlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_2,
          gLanguage->convertTheMXSRIntoAnMSRSkeleton ()));

    msdlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_3,
          gLanguage->populateTheMSRSkeletonFromMusicXMLData ()));

    msdlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_4,
          gLanguage->convertTheFirstMSRIntoASecondMSR ()));

    msdlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_5,
          gLanguage->convertTheSecondMSRIntoAnLPSR ()));

    msdlService->
      appendPassToService (
        mfPass::create (
          mfPassIDKind::kMfPassID_6,
          gLanguage->convertTheLPSRIntoLilyPondCode ()));

    pGlobalServicesMap [mfServiceKind::kMfService_msdl] = msdlService;
  }

  // JMI v0.9.67 mfsl ??? ischeme ???
}


}
