/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrBrowsers___
#define ___msrBrowsers___

#include "browser.h"

#include "msrPathToVoice.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
template <typename T> class   msrBrowser : public browser <T>
{
  public:

                          msrBrowser (basevisitor* v)
                            : fVisitor (v)
                              {}

    virtual               ~msrBrowser ()
                              {}

  public:

    virtual void          set (basevisitor* v)
                              { fVisitor = v; }

    virtual void          browse (T& t)
                              {
                                enter (t);

                                t.browseData (fVisitor);

                                leave (t);
                              }

  protected:

    basevisitor*          fVisitor;

    virtual void          enter (T& t)
                              { t.acceptIn  (fVisitor); }

    virtual void          leave (T& t)
                              { t.acceptOut (fVisitor); }
};

//______________________________________________________________________________
template <typename T> class   msrBrowserAlongPathToVoice : public browser <T>
{
  public:

                          msrBrowserAlongPathToVoice (
                            basevisitor*     v,
                            const S_msrPathToVoice& pathToVoice)
                              : fVisitor (v),
                                fPathToVoice (pathToVoice)
                              {}

    virtual               ~msrBrowserAlongPathToVoice ()
                              {}

  public:

    virtual void          set (basevisitor* v)
                              { fVisitor = v; }

    virtual void          browse (T& t)
                              {
                                enter (t);

                                t.browseDataAlongPathToVoice (
                                  fVisitor,
                                  fPathToVoice);

                                leave (t);
                              }

  protected:

    basevisitor*          fVisitor;
    S_msrPathToVoice      fPathToVoice;

    virtual void          enter (T& t)
                              { t.acceptIn  (fVisitor); }

    virtual void          leave (T& t)
                              { t.acceptOut (fVisitor); }
};


}


#endif
