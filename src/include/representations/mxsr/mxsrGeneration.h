/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrGeneration___
#define ___mxsrGeneration___

#include <string>

#include "elements.h"
#include "factory.h"
#include "xml.h"
#include "xmlfile.h"
#include "exports.h"


using namespace std;
using namespace MusicXML2;

namespace MusicFormats
{

//------------------------------------------------------------------------
string mxmlelementAsString (Sxmlelement elem);

void printMxsr (const Sxmlelement theMxsr, ostream& os);

//------------------------------------------------------------------------
Sxmlelement createMxmlelement (int type, const string& value);

Sxmlelement createMxmlIntegerElement (int type, int value);

Sxmlelement createMxmlFloatElement (int type, float value);

//------------------------------------------------------------------------
Sxmlattribute createMxmlAttribute (const string& name, const string& value);

Sxmlattribute createMxmlIntegerAttribute (const string& name, int value);

Sxmlattribute createMxmlFloatAttribute (const string& name, float value);

//------------------------------------------------------------------------
EXP SXMLFile createSxmlFile ();

//------------------------------------------------------------------------
Sxmlelement createMxmlScorePartWiseElement ();

}


#endif
