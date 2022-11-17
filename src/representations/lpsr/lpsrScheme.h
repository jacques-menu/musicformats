/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrScheme___
#define ___lpsrScheme___

#include <string>

#include "lpsrElements.h"

#include "lpsrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class lpsrCommentedKind {
  kCommentedYes, kCommentedNo
};

std::string lpsrCommentedKindAsString (
  lpsrCommentedKind commentedKind);

std::ostream& operator << (std::ostream& os, const lpsrCommentedKind& elt);

enum class lpsrEndOfLineKind {
  kEndOfLineNone, kEndOfLineOnce, kEndOfLineTwice
};

std::string lpsrEndOfLineKindAsString (
  lpsrEndOfLineKind endOfLineKind);

std::ostream& operator << (std::ostream& os, const lpsrEndOfLineKind& elt);

class EXP lpsrSchemeVariable : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrSchemeVariable> create (
                            int               inputLineNumber,
                            lpsrCommentedKind commentedKind,
                            const std::string&          variableName,
                            const std::string&          value,
                            const std::string&          comment,
                            lpsrEndOfLineKind endOfLineKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrSchemeVariable (
                            int               inputLineNumber,
                            lpsrCommentedKind commentedKind,
                            const std::string&          variableName,
                            const std::string&          value,
                            const std::string&          comment,
                            lpsrEndOfLineKind endOfLineKind);

    virtual               ~lpsrSchemeVariable ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getVariableName  () const { return fVariableName; }

    void                  setVariableValue (const std::string& value)
                              { fVariableValue = value; }

    std::string           getVariableValue () const { return fVariableValue; }

    lpsrCommentedKind     getCommentedKind () const { return fCommentedKind; }

    std::string           getComment  () const
                              { return fComment; }

    lpsrEndOfLineKind          getEndOfLineKind () const
                              { return fEndOfLineKind; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    lpsrCommentedKind fCommentedKind;

    std::string           fVariableName;
    std::string           fVariableValue;

    std::string           fComment;

    lpsrEndOfLineKind          fEndOfLineKind;

};
typedef SMARTP<lpsrSchemeVariable> S_lpsrSchemeVariable;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrSchemeVariable& schemeVariable);

//______________________________________________________________________________
class EXP lpsrSchemeFunction : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrSchemeFunction> create (
                            int           inputLineNumber,
                            const std::string& functionName,
                            const std::string& functionDescription,
                            const std::string& functionCode);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrSchemeFunction (
                            int          inputLineNumber,
                            const std::string& functionName,
                            const std::string& functionDescription,
                            const std::string& functionCode);

    virtual               ~lpsrSchemeFunction ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getFunctionName () const
                              { return fFunctionName; }

    std::string           getFunctionDescription () const
                              { return fFunctionDescription; }

    std::string           getFunctionCode () const
                              { return fFunctionCode; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::string       fFunctionName;

    std::string       fFunctionDescription;

    std::string       fFunctionCode;
};
typedef SMARTP<lpsrSchemeFunction> S_lpsrSchemeFunction;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrSchemeFunction& schemeFunction);


}


#endif
