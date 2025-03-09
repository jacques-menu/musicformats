/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <string.h> // for strlen()
#include <fstream>      // std::ofstream, std::ofstream::open(), std::ofstream::close()
#include <regex>

#include <iomanip> // for setw()

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "musicxml2mxsrWae.h"

#include "mfPreprocessorSettings.h"

#include "mfServices.h"

#include "mfTiming.h"
#include "mfStringsHandling.h"

#include "oahOah.h"

#include "musicxml2mxsrInterface.h"

#include "msrOah.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
void displayXMLDeclaration (
  TXMLDecl* xmlDeclaration)
{
  std::string xmlVersion    = xmlDeclaration->getVersion ();
  std::string xmlEncoding   = xmlDeclaration->getEncoding ();
  int         xmlStandalone = xmlDeclaration->getStandalone ();

  constexpr int fieldWidth = 14;

  gLog <<
    "XML Declaration:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) <<
    "xmlVersion" << " = \"" << xmlVersion << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "xmlEncoding" << " = \"" << xmlEncoding << "\"" <<
    std::endl  <<
    std::setw (fieldWidth) <<
    "xmlStandalone" << " = \"" << xmlStandalone << "\"" <<
    std::endl << std::endl;

  --gIndenter;
}

//_______________________________________________________________________________
void displayMusicXMLDocumentType (
  TDocType* documentType)
{
  constexpr int fieldWidth = 16;

  gLog <<
    "Document Type:" <<
    std::endl;

  ++gIndenter;

  std::string xmlStartElement = documentType->getStartElement ();
  Bool        xmlPublic       = documentType->getPublic ();
  std::string xmlPubLitteral  = documentType->getPubLitteral ();
  std::string xmlSysLitteral  = documentType->getSysLitteral ();

  gLog << std::left <<
    std::setw (fieldWidth) <<
    "xmlStartElement" << " = \"" << xmlStartElement << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "xmlPublic" << " = \"" << xmlPublic << "\"" <<
    std::endl  <<
    std::setw (fieldWidth) <<
    "xmlPubLitteral" << " = \"" << xmlPubLitteral << "\"" <<
    std::endl  <<
    std::setw (fieldWidth) <<
    "xmlSysLitteral" << " = \"" << xmlSysLitteral << "\"" <<
    std::endl << std::endl;

  --gIndenter;
}

//_______________________________________________________________________________
std::string uncompressMXLFile (
  std::string mxlFileName)
{
  std::string fileBaseName = mfBaseName (mxlFileName);

  gLog <<
    "The compressed file name is '" <<
    mxlFileName <<
    '\'' <<
    std::endl << std::endl;

  std::string uncompressedFileName;

#ifdef WIN32
  // JMI
#else
  {
    // build shell command to list the contents of the uncompress file
    std::stringstream s1;

    s1 <<
      "unzip -l " <<
      mxlFileName;

    std::string listContentsShellCommand = s1.str ();

    if (true) { // JMI
      gLog <<
        "Listing the contents of the compressed file '" <<
        mxlFileName <<
        "' with command:" <<
        std::endl;

      ++gIndenter;

      gLog <<
        listContentsShellCommand <<
        std::endl << std::endl;

      --gIndenter;
    }

      // create a stream to receive the result of listContentsShellCommand
    FILE* inputStream =
      popen (
        listContentsShellCommand.c_str (),
        "r");

    if (inputStream == nullptr) {
      std::stringstream ss;

      ss <<
        "Cannot list the contents of compressed file '" <<
        mxlFileName <<
        "' with 'popen ()'";

      musicxml2mxsrInternalError (
        gServiceRunData->getInputSourceName (),
        0, // inputLineNumber
        __FILE__, __LINE__,
        ss.str ());
    }

    else {
      std::string contentsList;

      // read the list from inputStream
      char tampon [1024];

      while (
        ! feof (inputStream)
          &&
        ! ferror (inputStream)
          &&
        fgets (tampon, sizeof (tampon), inputStream) != NULL
        ) {
        // append the contents of tampon to contentsList
        contentsList += tampon;
      } // while
      // terminate the std::string in tampon
      tampon [strlen (tampon) -1] = '\0';

      // close the stream
      if (pclose (inputStream) < 0) {
        musicxml2mxsrInternalError (
          gServiceRunData->getInputSourceName (),
          0, // inputLineNumber
          __FILE__, __LINE__,
          "Cannot close the input stream after 'popen ()'");
      }

      gLog <<
        "The contents of the compressed file '" <<
        mxlFileName <<
        "' is:" <<
        std::endl;

      ++gIndenter;

      gLog <<
        contentsList <<
        std::endl;

      --gIndenter;

      // analyze the contents list
      std::list <std::string> linesList;

      std::istringstream inputStream (contentsList);
      std::string        currentLine;

      while (getline (inputStream, currentLine)) {

        if (inputStream.eof ()) break;

#ifdef MF_TRACE_IS_ENABLED
        {
          gLog <<
            "*** currentLine:" <<
            std::endl;

          ++gIndenter;

          gLog <<
            currentLine <<
            std::endl;

          --gIndenter;
        }
#endif // MF_TRACE_IS_ENABLED

        /*
        user@lilydev: ~/libmusicxml-git/musicxml > unzip -l UnofficialTestSuite/90a-Compressed-MusicXML.mxl
        Archive:  UnofficialTestSuite/90a-Compressed-MusicXML.mxl
          Length      Date    Time    Name
        ---------  ---------- -----   ----
                0  2007-11-14 16:04   META-INF/
              246  2007-11-14 16:02   META-INF/container.xml
             2494  2008-11-14 23:03   20a-Compressed-MusicXML.xml
            30903  2007-11-14 15:51   20a-Compressed-MusicXML.pdf
        ---------                     -------
            33643                     4 files
        */

        std::string regularExpression (
          "[[:space:]]*"
          ".*"            // length
          "[[:space:]]+"
          ".*"            // date
          "[[:space:]]+"
          ".*"            // time
          "[[:space:]]+"
          "(.*)"          // name
          );

        std::regex  e (regularExpression);
        std::smatch sm;

        regex_match (currentLine, sm, e);

        if (sm.size ()) {
#ifdef MF_TRACE_IS_ENABLED
          if (gEarlyOptions.getEarlyTracePasses ()) { // JMI ???
            gLog <<
              "There are " << sm.size () - 1 << " match(es) " <<
              "with std::regex '" << regularExpression <<
              "':" <<
              std::endl;

            for (unsigned i = 1; i < sm.size (); ++i) {
              gLog <<
                '[' << sm [i] << "] " <<
                std::endl;
            } // for

            gLog <<
              std::endl << std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          std::string stringFromLine = sm [1];

          // has stringFromLine a ".xml" suffix?
          size_t
            posInString =
              stringFromLine.rfind (".xml");

          // JMI if (posInString == stringFromLine.size () - 4) {
          if (posInString != stringFromLine.npos) {  // JMI STRANGISSIMO!!!
    //      if (posInString != stringFromLine.npos && stringFromLine != "files") {  // JMI STRANGISSIMO!!!
            // yes, this is a MusicXML file

            // is this file part of META-INF?
            size_t
              posInString =
                stringFromLine.find ("META-INF");

            if (posInString == stringFromLine.npos) {
              // no, this is an actual MusicXML file

              if (uncompressedFileName.size ()) {
                std::stringstream ss;

                ss <<
                  "Compressed file '" << mxlFileName <<
                  "' contains multiple MusicMXL files" <<
                  ", found '" << uncompressedFileName <<
                  "' and then '" << stringFromLine << "'";

                musicxml2mxsrInternalError (
                  gServiceRunData->getInputSourceName (),
                  0, // inputLineNumber
                  __FILE__, __LINE__,
                  ss.str ());
              }

              else {
                // we've got the uncompressed file name
                uncompressedFileName = stringFromLine;

                gLog <<
                  "The uncompressed file name is '" <<
                  uncompressedFileName <<
                  '\'' <<
                  std::endl << std::endl;
              }
            }
          }
        }
      } // while
    }
  }

  {
    // build shell command to uncompress the file
    std::stringstream s2;

    s2 <<
      "unzip -u -d /tmp " <<
      mxlFileName;

    std::string uncompressShellCommand = s2.str ();

    if (true) { // JMI
      gLog <<
        "Uncompressing '" <<
        mxlFileName <<
        "' into '/tmp/" <<
        uncompressedFileName <<
        "' with command:" <<
        std::endl;

      ++gIndenter;

      gLog <<
        uncompressShellCommand <<
        std::endl << std::endl;

      --gIndenter;
    }

    // create a stream to receive the result of uncompressShellCommand
    FILE* inputStream =
      popen (
        uncompressShellCommand.c_str (),
        "r");

    if (inputStream == nullptr) {
      std::stringstream ss;

      ss <<
        "Cannot uncompress the file '" <<
        mxlFileName <<
        "' with 'popen ()'";

      musicxml2mxsrInternalError (
        gServiceRunData->getInputSourceName (),
        0, // inputLineNumber
        __FILE__, __LINE__,
        ss.str ());
    }
  }
#endif // WIN32

  return uncompressedFileName;
}

//_______________________________________________________________________________
void checkDesiredEncoding (
  const std::string& encoding,
  const std::string& desiredEncoding)
{
  if (encoding != desiredEncoding) {
    if (encoding.empty ()) {
      std::stringstream ss;

      ss <<
        "MusicXML data in this file" <<
        " doesn't contain any encoding specification; assuming it is " <<
        desiredEncoding;

//       musicxml2mxsrWarning (
      musicxml2mxsrError (
        gServiceRunData->getInputSourceName (),
        1, // inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }

    else {
      std::stringstream ss;

      ss <<
        "This MusicXML data is encoded in " <<
        encoding <<
        ", which LilyPond cannot handle, so this is rejected by this version. " <<
        "You may wich to convert it to " <<
        desiredEncoding <<
        " encoding prior to running xml2ly" <<
        ", for example with iconv or using a text editor" <<
        ", in which case " <<
        encoding <<
        " should be replaced by " <<
        desiredEncoding <<
        " manually" <<
        " - handling it as is";

//       musicxml2mxsrWarning ( // JMI v0.9.72 NOT OK for Windows users???
      musicxml2mxsrError ( // JMI v0.9.67 NOT OK for Windows users ???
        gServiceRunData->getInputSourceName (),
        1, // inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
  }
}

//_______________________________________________________________________________
void checkSXMLFile (
  SXMLFile      sxmlfile,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceEncoding ()) {
    gLog <<
      std::endl <<
      "!!!!! sxmlfile contents from " <<
      context <<
      std::endl;

    sxmlfile->print (gLog);

    gLog <<
      std::endl; // twice ??? JMI
  }
#endif // MF_TRACE_IS_ENABLED

  // get the xmlDecl
  TXMLDecl * xmlDecl = sxmlfile->getXMLDecl ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceEncoding ()) {
    gLog <<
      std::endl <<
      "!!!!! xmlDecl contents from file:" <<
      std::endl << std::endl;
    xmlDecl->print (gLog);

    displayXMLDeclaration (
      xmlDecl);
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  // get the docType
  TDocType * docType = sxmlfile->getDocType ();

  if (gGlobalMxsr2msrOahGroup->getTraceEncoding ()) {
    gLog <<
      std::endl <<
      "!!!!! docType from file:" <<
      std::endl << std::endl;
    docType->print (gLog);

    displayMusicXMLDocumentType (
      docType);
  }
#endif // MF_TRACE_IS_ENABLED

  // get the encoding type
  std::string encoding = xmlDecl->getEncoding ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceEncoding ()) {
    std::stringstream ss;

    ss <<
      "% MusicXML data uses " <<
      encoding <<
      " encoding";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is the encoding UTF-8?
  checkDesiredEncoding (
    encoding,
    "UTF-8");
}

//_______________________________________________________________________________
SXMLFile createSXMLFileFromFile (
  const char*        fileName,
  mfPassIDKind       passIDKind,
  const std::string& passDescription)
{
  SXMLFile sxmlfile;

  // start the clock
  clock_t startClock = clock ();

  // set the global current passID
  setGlobalCurrentPassIDKind (passIDKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      gLanguage->passIDKindAsString (passIDKind) << ": " << passDescription <<
      std::endl <<
      separator;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // read the input MusicXML data from the file
#ifdef TRACE_OAH
      if (gTraceOah->fTracePasses) {
        std::stringstream ss;

        ss <<
          std::endl <<
          "Reading MusicXML data from file \"" << outputFileName << "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

  xmlreader r;

  sxmlfile = r.read (fileName);

  // has there been a problem?
  if (! sxmlfile) {
    return SXMLFile (nullptr);
  }

  // check the sxmlfile
  checkSXMLFile (
    sxmlfile,
    "file");

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  return sxmlfile;
}

//_______________________________________________________________________________
SXMLFile createSXMLFileFromFd (
  FILE*              fd,
  mfPassIDKind       passIDKind,
  const std::string& passDescription)
{
  SXMLFile sxmlfile;

  // start the clock
  clock_t startClock = clock ();

  // set the global current passID
  setGlobalCurrentPassIDKind (passIDKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      gLanguage->passIDKindAsString (passIDKind) << ": " << passDescription <<
      std::endl <<
      separator;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // read the input MusicXML data from the file descriptor
#ifdef TRACE_OAH
      if (gTraceOah->fTracePasses) {
        std::stringstream ss;

        ss <<
          std::endl <<
          "Reading MusicXML data from file descriptor \"" << fd << "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

  xmlreader r;

  sxmlfile = r.read (fd);

  // has there been a problem?
  if (! sxmlfile) {
    return SXMLFile (nullptr);
  }

  // check the sxmlfile
  checkSXMLFile (
    sxmlfile,
    "file descriptor");

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  return sxmlfile;
}

//_______________________________________________________________________________
SXMLFile createSXMLFileFromString (
  const char*        buffer,
  mfPassIDKind       passIDKind,
  const std::string& passDescription)
{
  SXMLFile sxmlfile;

  // start the clock
  clock_t startClock = clock ();

  // read the input MusicXML data from the buffer
  xmlreader r;

  if (buffer [0] != '\0') { // superflous ??? JMI
#ifdef TRACE_OAH
    if (gTraceOah->fTracePasses) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "Reading MusicXML data from a string " << buffer.size () << " characters long";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    xmlreader r;

    sxmlfile = r.readbuff (buffer);

    // has there been a problem?
    if (! sxmlfile) {
      return SXMLFile (nullptr);
    }

    // check the sxmlfile
    checkSXMLFile (
      sxmlfile,
      "buffer");
  }

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  return sxmlfile;
}

//_______________________________________________________________________________
EXP Sxmlelement musicxmlFile2mxsr (
  const char*        fileName,
  S_mxsr2msrOahGroup mxsr2msrOpts,
  mfPassIDKind       passIDKind,
  const std::string& passDescription)
{
  // start the clock
  clock_t startClock = clock ();

  std::string fileNameAsString = fileName;

  // set the global current passID
  setGlobalCurrentPassIDKind (passIDKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      gLanguage->passIDKindAsString (passIDKind) << ": " << passDescription <<
      std::endl <<
      separator;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // has the input file name a ".mxl" suffix?
  size_t
    posInString =
      fileNameAsString.rfind (".mxl");

  if (posInString == fileNameAsString.size () - 4) {
 // JMI  if (posInString != fileNameAsString.npos) {
    // yes, this is a compressed file

    /* JMI OS dependent
    std::string uncompressedFileName =
      uncompressMXLFile (
        fileNameAsString);

    // the incompressed file in /tmp will be handled
    // instead of the compressed one
    fileName = uncompressedFileName.c_str ();
    */

    std::stringstream ss;

    ss <<
      "you should uncompress file " <<
      fileNameAsString <<
      " prior to running xml2ly";

    std::string message = ss.str ();

    musicxml2mxsrError (
      gServiceRunData->getInputSourceName (),
      1, // inputLineNumber,
      __FILE__, __LINE__,
      message);

    gLog <<
      message <<
      std::endl;

    throw musicxml2mxsrException (message);
  }

  // read the input MusicXML data from the file
  xmlreader r;

  SXMLFile sxmlfile = r.read (fileName);

  // has there been a problem?
  if (! sxmlfile) {
    return Sxmlelement (nullptr);
  }

  // check the sxmlfile
  checkSXMLFile (
    sxmlfile,
    "file");

  // get the MXSR
  Sxmlelement
    theMxsr =
      sxmlfile->elements ();

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  return theMxsr;
}

//_______________________________________________________________________________
EXP Sxmlelement musicxmlFd2mxsr (
  FILE*              fd,
  S_mxsr2msrOahGroup mxsr2msrOpts,
  mfPassIDKind       passIDKind,
  const std::string& passDescription)
{
  // start the clock
  clock_t startClock = clock ();

  // set the global current passID
  setGlobalCurrentPassIDKind (passIDKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      gLanguage->passIDKindAsString (passIDKind) << ": " << passDescription <<
      std::endl <<
      separator;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // read the input MusicXML data from the file descriptor
  xmlreader r;

  SXMLFile sxmlfile = r.read (fd);

  // has there been a problem?
  if (! sxmlfile) {
    return Sxmlelement (nullptr);
  }

  // check the sxmlfile
  checkSXMLFile (
    sxmlfile,
    "file descriptor");

  // get the xmlDecl
  TXMLDecl *xmlDecl = sxmlfile->getXMLDecl ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceEncoding ()) {
    gLog <<
      std::endl <<
      "xmlDecl contents:" <<
      std::endl << std::endl;
    xmlDecl->print (gLog);

    displayXMLDeclaration (
      xmlDecl);
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  // get the docType
  TDocType * docType = sxmlfile->getDocType ();

  if (gGlobalMxsr2msrOahGroup->getTraceEncoding ()) {
    gLog <<
      std::endl <<
      "!!!!! docType from stream:" <<
      std::endl << std::endl;
    docType->print (gLog);

    displayMusicXMLDocumentType (
      docType);
  }
#endif // MF_TRACE_IS_ENABLED

  // get the encoding type JMI
  std::string encoding = xmlDecl->getEncoding ();
//
//   // should the encoding be converted to UTF-8?
//   std::string desiredEncoding = "UTF-8";
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gGlobalMxsr2msrOahGroup->getTraceEncoding ()) {
//     gLog <<
//       "% MusicXML data uses \"" <<
//       desiredEncoding <<
//       "\" encoding" <<
//       ", desired encoding is \"" << desiredEncoding << "\"" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (encoding != desiredEncoding) {
//      std::stringstream ss;
//
//     ss <<
//       "you should convert this stream to " <<
//       desiredEncoding <<
//       "\" encoding prior to running xml2ly" <<
//       ", for example with iconv or using a text editor - handling it as is";
//
//     musicxml2mxsrWarning (
//       gServiceRunData->getInputSourceName (),
//       1, // inputLineNumber,
//       ss.str ());
//   }

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  // get the MXSR
  Sxmlelement theMxsr = sxmlfile->elements ();

  return theMxsr;
}

//_______________________________________________________________________________
EXP Sxmlelement musicxmlString2mxsr ( // JMI UNUSED SAX ???
  const char*        buffer,
  S_mxsr2msrOahGroup mxsr2msrOpts,
  mfPassIDKind       passIDKind,
  const std::string& passDescription)
{
  // start the clock
  clock_t startClock = clock ();

  // set the global current passID
  setGlobalCurrentPassIDKind (passIDKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      gLanguage->passIDKindAsString (passIDKind) << ": " << passDescription <<
      std::endl <<
      separator;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // read the input MusicXML data from the buffer
  xmlreader r;

  SXMLFile sxmlfile = r.readbuff (buffer);

  // has there been a problem?
  if (! sxmlfile) {
    return Sxmlelement (nullptr);
  }

  // check the sxmlfile
  checkSXMLFile (
    sxmlfile,
    "file");

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  // get the MXSR
  Sxmlelement theMxsr = sxmlfile->elements();

  return theMxsr;
}

//_______________________________________________________________________________
Sxmlelement convertMusicXMLToMxsr ( // JMI UNUSED SAX ???
  const std::string& inputSourceName,
  mfPassIDKind       passIDKind,
  const std::string& passDescription)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      gLanguage->passIDKindAsString (passIDKind) << ": " << passDescription <<
      std::endl <<
      separator;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the input source name
  gServiceRunData->setInputSourceName (
    inputSourceName);

  // create the MXSR
  Sxmlelement theMxsr;

  if (inputSourceName == "-") {
    // MusicXML data comes from standard input
    theMxsr =
      musicxmlFd2mxsr (
        stdin,
        gGlobalMxsr2msrOahGroup,
        passIDKind,
        passDescription);
  }

  else {
    // input comes from a file

    // has the input file name a ".mxl" suffix?
    size_t
      posInString =
        inputSourceName.rfind (".mxl");

    if (posInString == inputSourceName.size () - 4) {
      std::stringstream ss;

      ss <<
        "compressed MusicXML files are not handled by xml2ly currently - quitting";

      musicxml2mxsrError (
        inputSourceName,
        0, // inputLineNumber
        __FILE__, __LINE__,
        ss.str ());
    }

    // OK, let's go ahead
    theMxsr =
      musicxmlFile2mxsr (
        inputSourceName.c_str(),
        gGlobalMxsr2msrOahGroup,
        passIDKind,
        passDescription);
  }

  return theMxsr;
}


} // namespace
