/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
  const string& theString)
{
  string regularExpression (
    "([[:digit:]]*.[[:digit:]]*)" // RString
    ","
    "([[:digit:]]*.[[:digit:]]*)" // GString
    ","
    "([[:digit:]]*.[[:digit:]]*)" // BString
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for RGB color string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 4) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          '[' << sm [i] << "] ";
      } // for
      gLogStream << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "msrColorRGB string '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    RString = sm [1],
    GString = sm [2],
    BString = sm [3];

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "--> RString = \"" << RString << "\", " <<
      "--> GString = \"" << GString << "\"" <<
      "--> BString = \"" << BString << "\"" <<
      endl;
  }
#endif

  // are these strings alright?
  {
    stringstream s;

    s << RString;
    s >> fR;

    if (fR < 0.0 || fR > 1.0) {
      gLogStream <<
        "### ERROR: the R component " << fR <<
        " is not in the [0.0..1.0] interval in RGB color '" << theString << "'" <<
        endl;
    }
  }
  {
    stringstream s;

    s << GString;
    s >> fG;

    if (fG < 0.0 || fG > 1.0) {
      gLogStream <<
        "### ERROR: the G component " << fG <<
        " is not in the [0.0..1.0] interval in RGB color '" << theString << "'" <<
        endl;
    }
  }
  {
    stringstream s;

    s << BString;
    s >> fB;

    if (fB < 0.0 || fB > 1.0) {
      gLogStream <<
        "### ERROR: the B component " << fB <<
        " is not in the [0.0..1.0] interval in RGB color '" << theString << "'" <<
        endl;
    }
  }

}

string msrColorRGB::asString (int precision) const
{
  stringstream s;

  s <<
    "[ColorRGB " <<
    setprecision (precision) <<
    '[' <<
    fR <<
    "," <<
    fG <<
    "," <<
    fB <<
    "]]";

  return s.str ();
}

string msrColorRGB::asSpaceSeparatedString (int precision) const
{
  stringstream s;

  s <<
    setprecision (precision) <<
    fR <<
    ' ' <<
    fG <<
    ' ' <<
    fB;

  return s.str ();
}

void msrColorRGB::print (ostream& os) const
{
  os << asString () << endl;
};

ostream& operator << (ostream& os, const msrColorRGB& elt)
{
  elt.print (os);
  return os;
}

// AlphaRGB colors
//______________________________________________________________________________
msrColorAlphaRGB::msrColorAlphaRGB (
  const string& colorRGB,
  const string& colorAlpha)
{
  fColorRGB   = colorRGB;
  fColorAlpha = colorAlpha;
}

msrColorAlphaRGB::msrColorAlphaRGB (
  const string& colorRGB)
{
  fColorRGB   = colorRGB;
  fColorAlpha = "FF";
}

msrColorAlphaRGB::~msrColorAlphaRGB ()
{}

string msrColorAlphaRGB::asString () const
{
  stringstream s;

  s <<
    "[ColorAlphaRGB" <<
    ", fColorRGB: \"" << fColorRGB <<
    "\", fColorAlpha: \"" << fColorAlpha <<
    "\"]";

  return s.str ();
}

void msrColorAlphaRGB::print (ostream& os) const
{
  os << asString () << endl;
};

ostream& operator << (ostream& os, const msrColorAlphaRGB& elt)
{
  elt.print (os);
  return os;
}


}

