/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfStaticSettings.h"

#include "lpsrHeaders.h"

#include "oahOah.h"

#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrHeader lpsrHeader::create (
  int inputLineNumber)
{
  lpsrHeader* o =
    new lpsrHeader (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

lpsrHeader::lpsrHeader (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrHeader::~lpsrHeader ()
{}

size_t lpsrHeader::maxLilypondVariablesNamesLength ()
{
  size_t result = 0;

  // LilyPond informations

  if (fLilypondDedication.size () > result) {
      result = fLilypondDedication.size ();
  }

  if (fLilypondPiece.size () > result) {
      result = fLilypondPiece.size ();
  }

  if (fLilypondOpus.size () > result) {
      result = fLilypondOpus.size ();
  }

  if (fLilypondTitle.size () > result) {
      result = fLilypondTitle.size ();
  }

  if (fLilypondSubTitle.size () > result) {
      result = fLilypondSubTitle.size ();
  }

  if (fLilypondSubSubTitle.size () > result) {
      result = fLilypondSubSubTitle.size ();
  }

  if (fLilypondInstrument.size () > result) {
      result = fLilypondInstrument.size ();
  }

  if (fLilypondMeter.size () > result) {
      result = fLilypondMeter.size ();
  }

  if (fLilypondCopyright.size () > result) {
      result = fLilypondCopyright.size ();
  }

  if (fLilypondTagline.size () > result) {
      result = fLilypondTagline.size ();
  }

  return result;
}

void lpsrHeader::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrHeader::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrHeader::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrHeader::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrHeader::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrHeader::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrHeader::browseData (basevisitor* v)
{}

std::string lpsrHeader::asString () const
{
  std::stringstream ss;

  ss <<
    "[LpsrHeader" <<
    ", lilypondTitle: \" " <<  fLilypondTitle << "\"" <<
//    ", workNumber: \" " <<  fWorkNumber << "\"" <<
//    ", workCreditTypeTitle: \" " <<  fWorkCreditTypeTitle << "\"" <<
    ", lilypondOpus: \" " <<  fLilypondOpus << "\"" <<
//    ", movementNumber: \" " <<  fMovementNumber << "\"" <<
//    ", movementTitle: \" " <<  fMovementTitle << "\"" <<
    ", ..." <<
    ']';

  return ss.str ();
}

void lpsrHeader::print (std::ostream& os) const
{
  os <<
    "Header" <<
    std::endl;

  Bool emptyHeader (true);

  ++gIndenter;

  const int fieldWidth = 16;

  // identification

  os <<
    "HeaderIdentification:";

  if (fHeaderIdentification) {
    os << std::endl;
    ++gIndenter;
    os <<
      fHeaderIdentification;
    --gIndenter;
  }
  else {
    os << " [NONE]" << std::endl;
  }

  // LilyPond informations

  if (fLilypondDedication.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondDedication" << ": " <<
      fLilypondDedication <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondPiece.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondPiece" << ": " <<
      fLilypondPiece <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondOpus.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondOpus" << ": " <<
      fLilypondOpus <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondTitle.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondTitle" << ": " <<
      fLilypondTitle <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondSubTitle.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondSubTitle" << ": " <<
      fLilypondSubTitle <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondSubSubTitle.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondSubSubTitle" << ": " <<
      fLilypondSubSubTitle <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondInstrument.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondInstrument" << ": " <<
      fLilypondInstrument <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondMeter.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondMeter" << ": " <<
      fLilypondMeter <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondCopyright.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondCopyright" << ": " <<
      fLilypondCopyright <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondTagline.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondTagline" << ": " <<
      fLilypondTagline <<
      std::endl;

    emptyHeader = false;
  }

  if (emptyHeader) {
    os <<
      ' ' << "nothing specified" <<
      std::endl << std::endl; // JMI
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrHeader& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const S_lpsrHeader& elt)
{
//   iss.getStringstream () <<
//     gIndenter.indentMultiLineStringWithCurrentOffset (
//       S_lpsrHeader.asString ());

  if (elt) {
    elt->print (iss.getStringstream ());
  }
  else {
    iss.getStringstream () << "[NONE]" << '\n';
  }

  return iss;
}


}

/*
void lpsrHeader::appendRight (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
		std::stringstream ss;

    ss <<
      "Appending right \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fRightsList.push_back (value);
}

void lpsrHeader::appendComposer (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
		std::stringstream ss;

    ss <<
      "Appending composer \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fComposersList.push_back (value);
}

void lpsrHeader::appendArranger (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
		std::stringstream ss;

    ss <<
      "Appending arranger \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fArrangersList.push_back (value);
}

void lpsrHeader::appendLyricist (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
		std::stringstream ss;

    ss <<
      "Appending lyricist \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fLyricistsList.push_back (value);
}

void lpsrHeader::appendPoet (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
		std::stringstream ss;

    ss <<
      "Appending poet \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPoetsList.push_back (value);
}

void lpsrHeader::removeAllPoets (
  int    inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
		std::stringstream ss;

    ss <<
      "Removing all poets from lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPoetsList.clear ();
}

void lpsrHeader::appendTranslator (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
		std::stringstream ss;

    ss <<
      "Appending translator \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTranslatorsList.push_back (value);
}

void lpsrHeader::appendArtist (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
		std::stringstream ss;

    ss <<
      "Appending artist \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fArtistsList.push_back (value);
}

void lpsrHeader::appendSoftware (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
		std::stringstream ss;

    ss <<
      "Appending software \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSoftwaresList.push_back (value);
}

*/
