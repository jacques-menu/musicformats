/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahOah___
#define ___oahOah___

#include "exports.h"

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP insiderOahAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<insiderOahAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          insiderOahAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description);

    virtual               ~insiderOahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;


    // private fields
    // ------------------------------------------------------

    Bool                  fOptionHasBeenSelected;
};
typedef SMARTP<insiderOahAtom> S_insiderOahAtom;
EXP ostream& operator<< (ostream& os, const S_insiderOahAtom& elt);

//______________________________________________________________________________
class EXP regularOahAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<regularOahAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          regularOahAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description);

    virtual               ~regularOahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    Bool                  fOptionHasBeenSelected;
};
typedef SMARTP<regularOahAtom> S_regularOahAtom;
EXP ostream& operator<< (ostream& os, const S_regularOahAtom& elt);

//______________________________________________________________________________
class EXP oahOahGroup : public oahGroup
{
  public:

    static SMARTP<oahOahGroup> create (
                            const string& serviceName);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeOahOahGroup (
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahOahGroup (
                            const string& serviceName);

    virtual               ~oahOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // command line
    string                getOahOahGroupServiceName () const
                              { return fOahOahGroupServiceName; }

    S_mfcMultiComponent   fetchOahOahGroupHandlerMultiComponent () const;

    // OAH verbose mode

// replaced by getEarlyOahVerboseMode()
//     Bool                  getOahVerboseMode () const
//                               { return fOahVerboseMode; }

    S_oahBooleanAtom      getOahVerboseModeAtom () const
                              { return fOahVerboseModeAtom; }

    // options and arguments
    void                  setShowOptionsAndArguments ()
                              { fShowOptionsAndArguments = true; }
    Bool                  getShowOptionsAndArguments () const
                              { return fShowOptionsAndArguments; }

    // help about option name
    string                getOptionNameToProvideHelpAbout () const
                              { return fOptionNameToProvideHelpAbout; }
    S_oahOptionNameHelpAtom
                          getOptionNameHelpAtom () const
                              { return fOptionNameHelpAtom; }

    // query about option name
    string                getOptionNameToProvideQueryAbout () const
                              { return fOptionNameToProvideQueryAbout; }
    S_oahQueryOptionNameAtom
                          getQueryOptionNameAtom () const
                              { return fQueryOptionNameAtom; }

    // find string
//     string                getStringToBeFound () const
//                               { return fStringToBeFound; }
    S_oahFindStringAtom   getFindStringAtom () const
                              { return fFindStringAtom; }

    // include options file
    S_oahIncludeOptionsAndArgumentsFileAtom
                          getIncludeOptionsAndArgumentsFileAtom () const
                              { return fIncludeOptionsAndArgumentsFileAtom; }

    // display
    void                  setReverseNamesDisplayOrder ()
                              { fReverseNamesDisplayOrder = true; }
    Bool                  getReverseNamesDisplayOrder () const
                              { return fReverseNamesDisplayOrder; }

    // messages positions
    void                  setDisplaySourceCodePositions ()
                              { fDisplaySourceCodePositions = true; }
    Bool                  getDisplaySourceCodePositions () const
                              { return fDisplaySourceCodePositions;  }

    // tracing
    void                  setEarlyTracingOahVisitors ()
                              { fTracingOahVisitors = true; }
    Bool                  getTracingOahVisitors () const
                              { return fTracingOahVisitors; }

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeOahBasicHelpOptions (
                            const string& serviceName);

    void                  initializeOahOahAndArgumentsOptions ();

#ifdef TRACING_IS_ENABLED
    void                  initializeOahtracingOah ();
#endif

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printOahOahHelp ();

    void                  printOahOahValues (int valueFieldWidth);

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private work fields
    // --------------------------------------

    // command line
    string                fOahOahGroupServiceName;

    // trace early options

    Bool                  fTraceEarlyOptions;
    S_oahBooleanAtom      fTraceEarlyOptionsAtom;

    // OAH verbose mode

    Bool                  fOahVerboseMode;
    S_oahBooleanAtom      fOahVerboseModeAtom;

    // help about option name
    string                fOptionNameToProvideHelpAbout;
    S_oahOptionNameHelpAtom
                          fOptionNameHelpAtom;

    // query about option name
    string                fOptionNameToProvideQueryAbout;
    S_oahQueryOptionNameAtom
                          fQueryOptionNameAtom;

    // find string
//     string                fStringToBeFound; JMI ???
    S_oahFindStringAtom   fFindStringAtom;

    // include options file
    S_oahIncludeOptionsAndArgumentsFileAtom
                          fIncludeOptionsAndArgumentsFileAtom;

  private:

    // private fields
    // --------------------------------------

    // options and arguments
    Bool                  fShowOptionsAndArguments;

    // display
    Bool                  fReverseNamesDisplayOrder;

    // messages positions
    Bool                  fDisplaySourceCodePositions;

    // tracing
    Bool                  fTracingOahVisitors;
};
typedef SMARTP<oahOahGroup> S_oahOahGroup;
EXP ostream& operator<< (ostream& os, const S_oahOahGroup& elt);

EXP extern S_oahOahGroup gGlobalOahOahGroup;

//______________________________________________________________________________
EXP S_oahOahGroup createGlobalOahOahGroup (
  const string& serviceName);


}


#endif
