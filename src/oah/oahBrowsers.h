/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahBrowsers___
#define ___oahBrowsers___

#include "browser.h"


namespace MusicFormats
{
/* JMI
//______________________________________________________________________________
template <typename T> class   oahBrowser : public browser<T>
{
  protected:

    basevisitor*  fVisitor;

    virtual void enter (T& t) { t.acceptIn  (fVisitor); }
    virtual void leave (T& t) { t.acceptOut (fVisitor); }

  public:

    oahBrowser (basevisitor* v) : fVisitor (v)
    {}

    virtual               ~oahBrowser ()
    {}

    virtual void          set (basevisitor* v)
                            { fVisitor = v; }

    virtual               void browse (T& t)
                            {
                              enter (t);

                              t.browseData (fVisitor);

                              leave (t);
                            }
};
*/

template <typename T> class   oahBrowser : public browser <T>
{
  public:

                          oahBrowser (basevisitor* v)
                            : fVisitor (v)
                              {}

    virtual               ~oahBrowser ()
                              {}

  public:

    virtual void          set (basevisitor* v)
                              { fVisitor = v; }

    virtual void          browse (T& t)
                              {
/* JMI
#ifdef MF_TRACING_IS_ENABLED
                              if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
                                gOutputStream <<
                                  std::endl <<
                                  ".\\\" --> browse()" <<
                                  std::endl;
                              }
#endif
*/

                                enter (t);

                                t.browseData (fVisitor);

                                leave (t);
                              }

  protected:

    basevisitor*          fVisitor;

    virtual void          enter (T& t)
                              {
/* JMI
#ifdef MF_TRACING_IS_ENABLED
                          if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
                            gOutputStream <<
                              std::endl <<
                              ".\\\" --> enter()" <<
                              std::endl;
                          }
#endif
*/

                                t.acceptIn  (fVisitor);
                              }

    virtual void          leave (T& t)
                              {
/* JMI
#ifdef MF_TRACING_IS_ENABLED
                                if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
                                  gOutputStream <<
                                    std::endl <<
                                    ".\\\" --> leave()" <<
                                    std::endl;
                                }
#endif
*/

                                t.acceptOut (fVisitor);
                              }
};


}


#endif
