/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __basevisitor__
#define __basevisitor__


#ifdef WIN32
#pragma warning (disable : 4251 4275)
#endif

namespace MusicXML2 
{

class basevisitor 
{
	public:
		virtual ~basevisitor() {}
};

}


#endif
