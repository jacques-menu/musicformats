/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef MSVC
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include "libmusicxml.h"
#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"
#include "xml2guidovisitor.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
static xmlErr xml2guido(SXMLFile& xmlfile, bool generateBars, int partFilter, ostream& out, const char* file)
{
	Sxmlelement st = xmlfile->elements();
	if (st) {
		if (st->getName() == "score-timewise") return kUnsupported;
		
		xml2guidovisitor v(true, true, generateBars, partFilter);
		Sguidoelement gmn = v.convert(st);
		if (file) {
			out << "(*\n  gmn code converted from '" << file << "'"
				<< "\n  using libmusicxml v." << musicxmllibVersionStr();
		}
		else out << "(*\n  gmn code converted using libmusicxml v." << musicxmllibVersionStr();
		out << "\n  and the embedded xml2guido converter v." << musicxml2guidoVersionStr()
			<< "\n*)" << endl;
		out << gmn << endl;
		return kNoErr;
	}
	return kInvalidFile;
}

//_______________________________________________________________________________
static xmlErr partialxml2guido(SXMLFile& xmlfile, bool generateBars, int partFilter, int beginMeasure, int endMeasure, ostream& out, const char* file)
{
    Sxmlelement st = xmlfile->elements();
    if (st) {
        if (st->getName() == "score-timewise") return kUnsupported;
        
        xml2guidovisitor v(true, true, generateBars, partFilter, beginMeasure, endMeasure);
        Sguidoelement gmn = v.convert(st);
        if (file) {
            out << "(*\n  gmn code converted from '" << file << "'"
                << "\n  using libmusicxml v." << musicxmllibVersionStr();
        }
        else out << "(*\n  gmn code converted using libmusicxml v." << musicxmllibVersionStr();
        out << "\n  and the embedded xml2guido converter v." << musicxml2guidoVersionStr()
            << "\n*)" << endl;
        out << gmn << endl;
        return kNoErr;
    }
    return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfile2guido(const char *file, bool generateBars, int beginMeasure, int endMeasure, int partFilter, ostream& out)
{
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.read(file);
	if (xmlfile) {
        if ((beginMeasure != 0) || (endMeasure != 0)) {
            return partialxml2guido(xmlfile, generateBars, partFilter, beginMeasure, endMeasure, out, 0);
        }
		return xml2guido(xmlfile, generateBars, partFilter, out, file);
	}
	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2guido(FILE * fd, bool generateBars, int beginMeasure, int endMeasure, int partFilter, ostream& out)
{
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.read(fd);
	if (xmlfile) {
        if ((beginMeasure != 0) || (endMeasure != 0)) {
            return partialxml2guido(xmlfile, generateBars, partFilter, beginMeasure, endMeasure, out, 0);
        }
		return xml2guido(xmlfile, generateBars, partFilter, out, 0);
	}
	return kInvalidFile;
}

//_______________________________________________________________________________
    //  partFilter: 0 to get all parts, 1 to get only "P1", etc.
EXP xmlErr musicxmlstring2guido(const char * buffer, bool generateBars, ostream& out)
    {
        return musicxmlstring2guidoOnPart(buffer, generateBars, 0, out);
    }

EXP xmlErr musicxmlstring2guidoOnPart(const char * buffer, bool generateBars, int partFilter, std::ostream& out)
{
	xmlreader r;
	SXMLFile xmlfile;
	xmlfile = r.readbuff(buffer);
	if (xmlfile) {
		return xml2guido(xmlfile, generateBars, partFilter, out, 0);
	}
	return kInvalidFile;
}

EXP xmlErr      musicxmlstring2guidoPartial(const char * buffer, bool generateBars, int partFilter, int beginMeasure, int endMeasure, std::ostream& out)
{
    xmlreader r;
    SXMLFile xmlfile;
    xmlfile = r.readbuff(buffer);
    if (xmlfile) {
        return partialxml2guido(xmlfile, generateBars, partFilter, beginMeasure, endMeasure, out, 0);
    }
    return kInvalidFile;
}
    

}
