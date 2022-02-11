/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahVisitor___
#define ___oahVisitor___

#include "visitor.h"

#include "oahBasicTypes.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP oahVisitor : public smartable,

	public visitor<S_oahHandler>

{
  public:

    oahVisitor (
      const S_oahHandler handler);

    virtual               ~oahVisitor ();

    virtual void          visitTheHandler () = 0;

  protected:

    S_oahHandler          fVisitedOahHandler;
};
typedef SMARTP<oahVisitor> S_oahVisitor;


} // namespace MusicFormats


#endif
