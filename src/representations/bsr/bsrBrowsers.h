/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrBrowsers___
#define ___bsrBrowsers___

#include "browser.h"


namespace MusicFormats
{

//______________________________________________________________________________
template <typename T> class   bsrBrowser : public browser<T>
{
  public:

                          bsrBrowser (basevisitor* v)
                            : fVisitor (v)
                              {}

    virtual               ~bsrBrowser ()
    {}

    virtual void          set (basevisitor* v)
                              { fVisitor = v; }

    virtual               void browse (T& t)
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


}


#endif
