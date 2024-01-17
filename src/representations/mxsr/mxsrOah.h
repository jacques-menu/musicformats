/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrOah___
#define ___mxsrOah___

#include "mfPreprocessorSettings.h"

#include "oahBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP mxsrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxsrOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMxsrOah ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrOahGroup ();

    virtual               ~mxsrOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
    // specific trace
    // --------------------------------------

    // encoding
    void                  setTraceEncoding ()
                              { fTraceEncoding = true; }
    Bool                  getTraceEncoding () const
                              { return fTraceEncoding; }

    // divisions
    void                  setTraceDivisions ()
                              { fTraceDivisions = true; }
    Bool                  getTraceDivisions () const
                              { return fTraceDivisions; }

    // backup & forward
    void                  setTraceBackup ()
                              { fTraceBackup = true; }
    Bool                  getTraceBackup () const
                              { return fTraceBackup; }

    void                  setTraceForward ()
                              { fTraceForward = true; }
    Bool                  getTraceForward () const
                              { return fTraceForward; }

    // specific trace JMI move to traceOah
    // --------------------------------------

    void                  setDisplayMxsr ()
                              { fDisplayMxsr = true; }
    Bool                  getDisplayMxsr () const
                              { return fDisplayMxsr; }

    void                  setTraceMxsr ()
                              { fTraceMxsr = true; }
    Bool                  getTraceMxsr () const
                              { return fTraceMxsr; }

    // visitors
    void                  setTraceMxsrVisitors ()
                              { fTraceMxsrVisitors = true; }
    Bool                  getTraceMxsrVisitors () const
                              { return fTraceMxsrVisitors; }
#endif // MF_TRACE_IS_ENABLED

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
    void                  initializeMxsrTraceOah ();
#endif // MF_TRACE_IS_ENABLED

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMxsrOahHelp ();

    void                  displayMxsrOahValues (int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
    // specific trace
    // --------------------------------------

    // encoding
    Bool                  fTraceEncoding;

    // divisions
    Bool                  fTraceDivisions;

    // backup & forward
    Bool                  fTraceBackup;
    Bool                  fTraceForward;

    // specific trace JMI move to traceOah JMI ??? v0.9.70
    // --------------------------------------

    Bool                  fDisplayMxsr;
    Bool                  fTraceMxsr;

    // visitors
    Bool                  fTraceMxsrVisitors;
#endif // MF_TRACE_IS_ENABLED
};
typedef SMARTP<mxsrOahGroup> S_mxsrOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_mxsrOahGroup& elt);

EXP extern S_mxsrOahGroup gGlobalMxsrOahGroup;

//______________________________________________________________________________
EXP S_mxsrOahGroup createGlobalMxsrOahGroup ();


}


#endif // ___mxsrOah___
