/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahOah___
#define ___oahOah___

#include "exports.h"

#include "languages.h"

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP languageOahAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<languageOahAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            mfLanguageKind&    languageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          languageOahAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            mfLanguageKind&    languageKindVariable);

    virtual               ~languageOahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;


    // private fields
    // ------------------------------------------------------

  mfLanguageKind&         fLanguageKindVariable;
};
typedef SMARTP<languageOahAtom> S_languageOahAtom;
EXP std::ostream& operator << (std::ostream& os, const S_languageOahAtom& elt);

//______________________________________________________________________________
class EXP insiderOahAtom : public oahValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<insiderOahAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          insiderOahAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description);

    virtual               ~insiderOahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<insiderOahAtom> S_insiderOahAtom;
EXP std::ostream& operator << (std::ostream& os, const S_insiderOahAtom& elt);

//______________________________________________________________________________
class EXP regularOahAtom : public oahValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<regularOahAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          regularOahAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description);

    virtual               ~regularOahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<regularOahAtom> S_regularOahAtom;
EXP std::ostream& operator << (std::ostream& os, const S_regularOahAtom& elt);

//______________________________________________________________________________
class EXP passIDOahAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<passIDOahAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            mfPassIDKind&      passIDKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          passIDOahAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            mfPassIDKind&      passIDKind);

    virtual               ~passIDOahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

    // private fields
    // ------------------------------------------------------

    mfPassIDKind&         fPassIDKindVariable;
};
typedef SMARTP<passIDOahAtom> S_passIDOahAtom;
EXP std::ostream& operator << (std::ostream& os, const S_passIDOahAtom& elt);

//______________________________________________________________________________
class EXP oahOahGroup : public oahGroup
{
  public:

    static SMARTP<oahOahGroup> create (
                            const std::string& serviceName);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeOahOahGroup (
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahOahGroup (
                            const std::string& serviceName);

    virtual               ~oahOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // command line
    std::string           getOahOahGroupServiceName () const
                              { return fOahOahGroupServiceName; }

    S_mfcMultiComponent   fetchOahOahGroupHandlerMultiComponent () const;

    // OAH verbose mode
    const S_oahBooleanAtom&
                          getOahVerboseModeAtom () const
                              { return fOahVerboseModeAtom; }

    // options and arguments
    void                  setdisplayOptionsAndArguments ()
                              { fdisplayOptionsAndArguments = true; }
    Bool                  getdisplayOptionsAndArguments () const
                              { return fdisplayOptionsAndArguments; }

    // help about option name
    std::string           getOptionNameToProvideHelpAbout () const
                              { return fOptionNameToProvideHelpAbout; }
    S_oahOptionNameHelpAtom
                          getOptionNameHelpAtom () const
                              { return fOptionNameHelpAtom; }

    // query about option name
    std::string           getOptionNameToProvideQueryAbout () const
                              { return fOptionNameToProvideQueryAbout; }
    S_oahQueryOptionNameAtom
                          getQueryOptionNameAtom () const
                              { return fQueryOptionNameAtom; }

    // find std::string
//     std::string           getStringToBeFound () const
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

    // trace
    void                  setTraceOahVisitors ()
                              { fTraceOahVisitors = true; }
    Bool                  getTraceOahVisitors () const
                              { return fTraceOahVisitors; }

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
                            const std::string& serviceName);

    void                  initializeOahOahAndArgumentsOptions ();

#ifdef MF_TRACE_IS_ENABLED
    void                  initializeOahTraceOah ();
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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const;

  private:

    // private work fields
    // --------------------------------------

    // command line
    std::string           fOahOahGroupServiceName;

    // language
    // JMI v0.9.66

    // trace early options

    S_oahBooleanAtom      fTraceEarlyOptionsAtom;

    // OAH verbose mode

    S_oahBooleanAtom      fOahVerboseModeAtom;

    // help about option name
    std::string           fOptionNameToProvideHelpAbout;
    S_oahOptionNameHelpAtom
                          fOptionNameHelpAtom;

    // query about option name
    std::string           fOptionNameToProvideQueryAbout;
    S_oahQueryOptionNameAtom
                          fQueryOptionNameAtom;

    // find std::string
//     std::string           fStringToBeFound; JMI ??? v0.9.66
    S_oahFindStringAtom   fFindStringAtom;

    // include options file
    S_oahIncludeOptionsAndArgumentsFileAtom
                          fIncludeOptionsAndArgumentsFileAtom;

  private:

    // private fields
    // --------------------------------------

    // options and arguments
    Bool                  fdisplayOptionsAndArguments;

    // display
    Bool                  fReverseNamesDisplayOrder;

    // messages positions
    Bool                  fDisplaySourceCodePositions;

    // trace
    Bool                  fTraceOahVisitors;
};
typedef SMARTP<oahOahGroup> S_oahOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_oahOahGroup& elt);

EXP extern S_oahOahGroup gGlobalOahOahGroup;

//______________________________________________________________________________
EXP S_oahOahGroup createGlobalOahOahGroup (
  const std::string& serviceName);


}


#endif
