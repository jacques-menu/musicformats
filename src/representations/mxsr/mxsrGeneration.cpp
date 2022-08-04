/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

//#include <iomanip>      // setw, setprecision, ...
//#include <cmath>
//#include <string>

#include <sstream>

#include "mxsrGeneration.h"
#include "xmlvisitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif


using namespace std;

namespace MusicFormats
{

//________________________________________________________________________
string mxmlelementAsString (Sxmlelement elem)
{
  int
    elemType = elem->getType ();

  const std::string&  elemName =
    elem->getName ();

  const std::vector<Sxmlattribute>& elemAttributes =
    elem->attributes();

  stringstream s;

  s <<
    "[mxmlelement " <<
    ", elemType: " << elemType <<
    ", elemName: " << elemName <<
    ", elemAttributes.size ():" << elemAttributes.size () <<
    "]";

  return s.str ();
}

void printMxsr (const Sxmlelement theMxsr, ostream& os)
{
  xmlvisitor v (os);
  tree_browser<xmlelement> browser (&v);
  browser.browse (*theMxsr);
}

//------------------------------------------------------------------------
Sxmlelement createMxmlelement (int type, const string& value)
{
  Sxmlelement result = factory::instance().create(type);

  result->setValue (value);

  return result;
}

Sxmlelement createMxmlIntegerElement (int type, int value)
{
  Sxmlelement result = factory::instance().create(type);

  result->setValue (value);

  return result;
}

Sxmlelement createMxmlFloatElement (int type, float value)
{
  Sxmlelement result = factory::instance().create(type);

  result->setValue (value);

  return result;
}

//------------------------------------------------------------------------
Sxmlattribute createMxmlAttribute (const string& name, const string& value)
{
  Sxmlattribute result = xmlattribute::create();

  result->setName(name);
  result->setValue(value);

  return result;
}

Sxmlattribute createMxmlIntegerAttribute (const string& name, int value)
{
  Sxmlattribute result = xmlattribute::create();

  result->setName(name);
  result->setValue(value);

  return result;
}

Sxmlattribute createMxmlFloatAttribute (const string& name, float value)
{
  Sxmlattribute result = xmlattribute::create();

  result->setName(name);
  result->setValue(value);

  return result;
}

//------------------------------------------------------------------------
SXMLFile createSxmlFile ()
{
  SXMLFile result = TXMLFile::create ();
  assert (result != nullptr);

  TXMLDecl * xmlDecl = new TXMLDecl ("1.0", "UTF-8", TXMLDecl::kNo);
  assert (xmlDecl != nullptr);

  result->set (xmlDecl);

  TDocType * docType = new TDocType ("score-partwise");
  assert (docType != nullptr);

  result->set (docType);

  return result;
}

//------------------------------------------------------------------------
Sxmlelement createMxmlScorePartWiseElement ()
{
  Sxmlelement result = factory::instance ().create (k_score_partwise);

  Sxmlattribute versionAttribute = createMxmlAttribute("version", "3.1");
  result->add (versionAttribute);

  return result;
}


} // namespace

