/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...
#include <sstream>
#include <regex>

#include "mfIndentedTextOutput.h"

#include "msrColors.h"

#include "oahWae.h"


namespace MusicFormats
{

// RGB colors
//______________________________________________________________________________
msrColorRGB::msrColorRGB ()
{
  // initializing to negative values for isEmpty()
  fR = -1.0;
  fG = -1.0;
  fB = -1.0;
}

msrColorRGB::msrColorRGB (
  float theR,
  float theG,
  float theB)
{
  fR = theR;
  fG = theG;
  fB = theB;
}

msrColorRGB::msrColorRGB (
  const std::string& theString)
{
  std::string regularExpression (
    "([[:digit:]]*.[[:digit:]]*)" // RString
    ","
    "([[:digit:]]*.[[:digit:]]*)" // GString
    ","
    "([[:digit:]]*.[[:digit:]]*)" // BString
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for RGB color string '" << theString <<
      "' with std::regex '" << regularExpression <<
      "'" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (smSize == 4) {
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          '[' << sm [i] << "] ";
      } // for
      gLogStream << std::endl;
    }
#endif
  }

  else {
    std::stringstream ss;

    ss <<
      "msrColorRGB string '" << theString <<
      "' is ill-formed";

    oahError (ss.str ());
  }

  std::string
    RString = sm [1],
    GString = sm [2],
    BString = sm [3];

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "--> RString = \"" << RString << "\", " <<
      "--> GString = \"" << GString << "\"" <<
      "--> BString = \"" << BString << "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  // are these strings alright?
  {
    std::stringstream ss;

    ss << RString;
    ss >> fR;

    if (fR < 0.0 || fR > 1.0) {
      gLogStream <<
        "### ERROR: the R component " << fR <<
        " is not in the [0.0..1.0] interval in RGB color '" << theString << "'" <<
        std::endl;
    }
  }
  {
    std::stringstream ss;

    ss << GString;
    ss >> fG;

    if (fG < 0.0 || fG > 1.0) {
      gLogStream <<
        "### ERROR: the G component " << fG <<
        " is not in the [0.0..1.0] interval in RGB color '" << theString << "'" <<
        std::endl;
    }
  }
  {
    std::stringstream ss;

    ss << BString;
    ss >> fB;

    if (fB < 0.0 || fB > 1.0) {
      gLogStream <<
        "### ERROR: the B component " << fB <<
        " is not in the [0.0..1.0] interval in RGB color '" << theString << "'" <<
        std::endl;
    }
  }

}

std::string msrColorRGB::asString (int precision) const
{
  std::stringstream ss;

  ss <<
    "[ColorRGB " <<
    std::setprecision (precision) <<
    '[' <<
    fR <<
    "," <<
    fG <<
    "," <<
    fB <<
    "]]";

  return ss.str ();
}

std::string msrColorRGB::asSpaceSeparatedString (int precision) const
{
  std::stringstream ss;

  ss <<
    std::setprecision (precision) <<
    fR <<
    ' ' <<
    fG <<
    ' ' <<
    fB;

  return ss.str ();
}

void msrColorRGB::print (std::ostream& os) const
{
  os << asString () << std::endl;
};

std::ostream& operator << (std::ostream& os, const msrColorRGB& elt)
{
  elt.print (os);
  return os;
}

// AlphaRGB colors
//______________________________________________________________________________
msrColorAlphaRGB::msrColorAlphaRGB (
  const std::string& colorRGB,
  const std::string& colorAlpha)
{
  fColorRGB   = colorRGB;
  fColorAlpha = colorAlpha;
}

msrColorAlphaRGB::msrColorAlphaRGB (
  const std::string& colorRGB)
{
  fColorRGB   = colorRGB;
  fColorAlpha = "FF";
}

msrColorAlphaRGB::~msrColorAlphaRGB ()
{}

std::string msrColorAlphaRGB::asString () const
{
  std::stringstream ss;

  ss <<
    "[ColorAlphaRGB" <<
    ", fColorRGB: \"" << fColorRGB <<
    "\", fColorAlpha: \"" << fColorAlpha <<
    "\"]";

  return ss.str ();
}

void msrColorAlphaRGB::print (std::ostream& os) const
{
  os << asString () << std::endl;
};

std::ostream& operator << (std::ostream& os, const msrColorAlphaRGB& elt)
{
  elt.print (os);
  return os;
}


}

