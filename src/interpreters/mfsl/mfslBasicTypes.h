/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfslBasicTypes___
#define ___mfslBasicTypes___

#include <string>
#include <set>
#include <map>

#include "smartpointer.h"
#include "exports.h"

#include "mfBool.h"

#include "oahBasicTypes.h"


using namespace std;

using namespace MusicXML2;

using namespace MusicFormats;

//______________________________________________________________________________
class EXP mfslElement : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------

  protected:

                          mfslElement (
                            int inputLineNumber);

    virtual               ~mfslElement ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getInputLineNumber () const
                              { return fInputLineNumber; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    virtual string        asString () const;

    virtual void          print (ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    int                   fInputLineNumber;
};
typedef SMARTP<mfslElement> S_mfslElement;
EXP ostream& operator<< (ostream& os, const S_mfslElement& elt);

//______________________________________________________________________________
class EXP mfslScope : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslScope> create (const string& scopeName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslScope (const string& scopeName);

    virtual               ~mfslScope ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getScopeName () const
                              { return fScopeName; }

//     const list<S_mfslElement>&
//                           getScopeElementsList () const
//                               { return fScopeElementsList; }

    const vector<S_oahOption>&
                          getScopeOptionsVector () const
                              { return fScopeOptionsVector; }

    vector<S_oahOption>&
                          getScopeOptionsVectorToModify ()
                              { return fScopeOptionsVector; }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerOptionsInScope (
                            S_oahOption
                              option);

//     void                  appendElementToScope (S_mfslElement element);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    string                fScopeName;

    vector<S_oahOption>   fScopeOptionsVector;

//     list<S_mfslElement>   fScopeElementsList;
};
typedef SMARTP<mfslScope> S_mfslScope;
EXP ostream& operator<< (ostream& os, const S_mfslScope& elt);

//______________________________________________________________________________
enum class mfslVariableKind {
  kVariableChoice,
  kVariablePath
};

string mfslVariableKindAsString (
  mfslVariableKind variableKind);

ostream& operator<< (ostream& os, const mfslVariableKind& elt);

enum class mfslVariableValueKind {
  kVariableValueNone,
  kVariableValueSuppliedToScript,
  kVariableValueSetInScript
};

string mfslVariableValueKindAsString (
  mfslVariableValueKind variableValueKind);

ostream& operator<< (ostream& os, const mfslVariableValueKind& elt);

//______________________________________________________________________________
class EXP mfslVariable : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslVariable> create (
                            const string&    variableName,
                            mfslVariableKind variableKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslVariable (
                            const string&    variableName,
                            mfslVariableKind variableKind);

    virtual               ~mfslVariable ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setVariableName (const string& value)
                              { fVariableName = value; }

    string                getVariableName () const
                              { return fVariableName; }

    mfslVariableKind      getVariableKind () const
                              { return fVariableKind; }

    void                  setVariableValue (const string& value);

    string                getVariableValue () const;
    string                getVariableValueWithoutTrace () const;

    mfslVariableValueKind getVariableValueKind () const
                              { return fVariableValueKind; }

    const set<string>&    getPossibleValuesSet () const
                              { return fPossibleValuesSet; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addPossibleValue (const string& value);

    Bool                  isValuePossible (const string& value);

  public:

    // print
    // ------------------------------------------------------

    string                possibleValuesAsString () const;

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    string                fVariableName;
    mfslVariableKind      fVariableKind;

    string                fVariableValue;

    set<string>           fPossibleValuesSet;

    mfslVariableValueKind fVariableValueKind;
};
typedef SMARTP<mfslVariable> S_mfslVariable;
EXP ostream& operator<< (ostream& os, const S_mfslVariable& elt);

//______________________________________________________________________________
class EXP mfslVariablesTable : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslVariablesTable> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslVariablesTable ();

    virtual               ~mfslVariablesTable ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    S_mfslVariable        createAndInsertVariable (
                            const string& variableName);

    S_mfslVariable        lookupVariableByName (
                            const string& variableName);

    S_mfslVariable        checkCaseVariableByName (
                            const string& variableName);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    map<string, S_mfslVariable>
                          fVariablesMap;

};
typedef SMARTP<mfslVariablesTable> S_mfslVariablesTable;
EXP ostream& operator<< (ostream& os, const S_mfslVariablesTable& elt);

//______________________________________________________________________________
class EXP mfslCaseStatement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslCaseStatement> create (
                            S_mfslVariable caseVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslCaseStatement (
                            S_mfslVariable caseVariable);

    virtual               ~mfslCaseStatement ();

  public:

    // set and get
    // ------------------------------------------------------

    S_mfslVariable        getCaseVariable () const
                              { return fCaseVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerCaseLabelValue (const string& value);

    void                  checkThatAllPossibleValuesHaveBeenUsed ();

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    S_mfslVariable        fCaseVariable;

    // checking unused possible values
    set<string>           fCaseUnusedLabels;
};
typedef SMARTP<mfslCaseStatement> S_mfslCaseStatement;
EXP ostream& operator<< (ostream& os, const S_mfslCaseStatement& elt);

//______________________________________________________________________________
class EXP mfslAllStatement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslAllStatement> create (
                            S_mfslVariable allVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslAllStatement (
                            S_mfslVariable allVariable);

    virtual               ~mfslAllStatement ();

  public:

    // set and get
    // ------------------------------------------------------

    S_mfslVariable        getAllVariable () const
                              { return fAllVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerAllLabelValue (const string& value);

    void                  checkThatAllPossibleValuesHaveBeenUsed ();

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    S_mfslVariable        fAllVariable;

    // checking unused possible values
    set<string>           fAllUnusedLabels;
};
typedef SMARTP<mfslAllStatement> S_mfslAllStatement;
EXP ostream& operator<< (ostream& os, const S_mfslAllStatement& elt);

// // initialization
// //______________________________________________________________________________
// void initializeMFSLBasicTypes ();


#endif
