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
// PRE-declarations

class   mfslDriver;

//______________________________________________________________________________
class EXP mfslOptionsBlock : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslOptionsBlock> create (const string& optionsBlockName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslOptionsBlock (const string& optionsBlockName);

    virtual               ~mfslOptionsBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getOptionsBlockName () const
                              { return fOptionsBlockName; }

    size_t                getOptionsBlockSize () const
                              { return fOptionsBlockOptionsVector.size (); }

    const vector<S_oahOption>&
                          getOptionsBlockOptionsVector () const
                              { return fOptionsBlockOptionsVector; }

    vector<S_oahOption>&
                          getOptionsBlockOptionsVectorNonConst ()
                              { return fOptionsBlockOptionsVector; }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerOptionsInOptionsBlock (
                            S_oahOption option,
                            mfslDriver& drv);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    string                asOptionsString () const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    string                fOptionsBlockName;

    vector<S_oahOption>   fOptionsBlockOptionsVector;
};
typedef SMARTP<mfslOptionsBlock> S_mfslOptionsBlock;
EXP ostream& operator << (ostream& os, const S_mfslOptionsBlock& elt);

//______________________________________________________________________________
enum class mfslInputNameKind {
  kInputNameNone,
  kInputNameOptionSupplied,
  kInputNameSetInScript
};

string mfslInputNameKindAsString (
  mfslInputNameKind inputNameKind);

ostream& operator << (ostream& os, const mfslInputNameKind& elt);

//______________________________________________________________________________
class EXP mfslInput : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslInput> create (
                            const string& inputName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslInput (
                            const string& inputName);

    virtual               ~mfslInput ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getInputName () const
                              { return fInputName; }

    void                  setInputNameSuppliedByAnOption (
                            const string& name,
                            mfslDriver&   drv);

    void                  selectInputName (
                            const string& name,
                            mfslDriver&   drv);

    string                getInputName (
                            mfslDriver& drv) const;

    string                getInputNameWithoutTrace ( // JMI
                            mfslDriver& drv) const;

    mfslInputNameKind     getInputNameKind () const
                              { return fInputNameKind; }

    const set<string>&    getNamesSet () const
                              { return fNamesSet; }

    S_mfslOptionsBlock    getInputOptionsBlockForName (
                            const string& name,
                            mfslDriver&   drv) const;

    void                  setInputIsUsedInCaseInputStatements ()
                              { fInputIsUsedInCaseInputStatements = true; }

    Bool                  getInputIsUsedInCaseInputStatements () const
                              { return fInputIsUsedInCaseInputStatements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addName (
                            const string& name,
                            mfslDriver&   drv);

    void                  enrichNameOptionsBlock (
                            const string&      name,
                            S_mfslOptionsBlock optionsBlock,
                            mfslDriver&        drv);

  public:

    // print
    // ------------------------------------------------------

    string                namesSetAsString () const;

    string                asString () const;

    void                  displayInputNamesToOptionsBlocksMap (
                            ostream& os) const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    string                fInputName;

    mfslInputNameKind     fInputNameKind;

    set<string>           fNamesSet;

    Bool                  fInputIsUsedInCaseInputStatements;

    map<string, S_mfslOptionsBlock>
                          fInputNamesToOptionsBlocksMap;
};
typedef SMARTP<mfslInput> S_mfslInput;
EXP ostream& operator << (ostream& os, const S_mfslInput& elt);

//______________________________________________________________________________
class EXP mfslInputsTable : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslInputsTable> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslInputsTable ();

    virtual               ~mfslInputsTable ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, S_mfslInput>&
                          getInputsMap () const
                              { return fInputsMap; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addInput (
                            S_mfslInput input,
                            mfslDriver& drv);

//     S_mfslInput          lookupInputByName (
//                             const string& name);

    S_mfslInput           fetchInputByName (
                            const string&     name,
                            const mfslDriver& drv);

    S_mfslInput           fetchInputByNameNonConst (
                            const string& name,
                            mfslDriver&   drv);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  displayInputsMap (ostream& os) const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    map<string, S_mfslInput>
                          fInputsMap;
};
typedef SMARTP<mfslInputsTable> S_mfslInputsTable;
EXP ostream& operator << (ostream& os, const S_mfslInputsTable& elt);

//______________________________________________________________________________
class EXP mfslCaseInputStatement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslCaseInputStatement> create (
                            S_mfslInput caseInput,
                            mfslDriver& drv);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslCaseInputStatement (
                            S_mfslInput caseInput,
                            mfslDriver& drv);

    virtual               ~mfslCaseInputStatement ();

  public:

    // set and get
    // ------------------------------------------------------

    S_mfslInput           getCaseInputInput () const
                              { return fCaseInputInput; }

    const list<string>&   getCaseInputCurrentNamesList () const
                              { return fCaseInputCurrentNamesList; }

    void                  clearCaseInputCurrentNamesList ()
                              { fCaseInputCurrentNamesList.clear (); }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerCaseInputName (
                            const string& name,
                            mfslDriver&   drv);

    void                  checkThatAllNamesHaveBeenUsed (
                            mfslDriver& drv);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    string                currentNamesListAsString () const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    S_mfslInput           fCaseInputInput;

    set<string>           fCaseInputNamesSet;
    list<string>          fCaseInputCurrentNamesList;

    // checking unused names
    set<string>           fUsedNames;
    set<string>           fCaseInputUnusedNames;
};
typedef SMARTP<mfslCaseInputStatement> S_mfslCaseInputStatement;
EXP ostream& operator << (ostream& os, const S_mfslCaseInputStatement& elt);

//______________________________________________________________________________
enum class mfslChoiceLabelKind {
  kChoiceLabelNone,
  kChoiceLabelOptionSupplied,
  kChoiceLabelSetInScript
};

string mfslChoiceLabelKindAsString (
  mfslChoiceLabelKind choiceLabelKind);

ostream& operator << (ostream& os, const mfslChoiceLabelKind& elt);

//______________________________________________________________________________
class EXP mfslChoice : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslChoice> create (
                            const string& choiceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslChoice (
                            const string& choiceName);

    virtual               ~mfslChoice ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getChoiceName () const
                              { return fChoiceName; }

    void                  setChoiceLabelSuppliedByAnOption (
                            const string& label,
                            mfslDriver&   drv);

    void                  selectChoiceLabel (
                            const string& label,
                            mfslDriver&   drv);

    string                getChoiceLabel (
                            mfslDriver& drv) const;

    string                getChoiceLabelWithoutTrace ( // JMI
                            mfslDriver& drv) const;

    mfslChoiceLabelKind   getChoiceLabelKind () const
                              { return fChoiceLabelKind; }

    const set<string>&    getLabelsSet () const
                              { return fLabelsSet; }

    string                getChoiceDefaultLabel () const
                              { return fChoiceDefaultLabel; }

    S_mfslOptionsBlock    getChoiceOptionsBlockForLabel (
                            const string& label,
                            mfslDriver&   drv) const;

    void                  setChoiceIsUsedInCaseChoiceStatements ()
                              { fChoiceIsUsedInCaseChoiceStatements = true; }

    Bool                  getChoiceIsUsedInCaseChoiceStatements () const
                              { return fChoiceIsUsedInCaseChoiceStatements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addLabel (
                            const string& label,
                            mfslDriver&   drv);

    void                  enrichLabelOptionsBlock (
                            const string&      label,
                            S_mfslOptionsBlock optionsBlock,
                            mfslDriver&        drv);

    void                  registerChoiceDefaultLabel (
                            const string& label,
                            mfslDriver&   drv);

  public:

    // print
    // ------------------------------------------------------

    string                labelsSetAsString () const;

    string                asString () const;

    void                  displayChoiceLabelsToOptionsBlocksMap (
                            ostream& os) const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    string                fChoiceName;

    string                fChoiceLabel;
    mfslChoiceLabelKind   fChoiceLabelKind;

    set<string>           fLabelsSet;

    string                fChoiceDefaultLabel;

    Bool                  fChoiceIsUsedInCaseChoiceStatements;

    map<string, S_mfslOptionsBlock>
                          fChoiceLabelsToOptionsBlocksMap;
};
typedef SMARTP<mfslChoice> S_mfslChoice;
EXP ostream& operator << (ostream& os, const S_mfslChoice& elt);

//______________________________________________________________________________
class EXP mfslChoicesTable : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslChoicesTable> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslChoicesTable ();

    virtual               ~mfslChoicesTable ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, S_mfslChoice>&
                          getChoicesMap () const
                              { return fChoicesMap; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addChoice (
                            S_mfslChoice choice,
                            mfslDriver&  drv);

//     S_mfslChoice          lookupChoiceByName (
//                             const string& name);

    S_mfslChoice          fetchChoiceByName (
                            const string&     name,
                            const mfslDriver& drv);

    S_mfslChoice          fetchChoiceByNameNonConst (
                            const string& name,
                            mfslDriver&   drv);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  displayChoicesMap (ostream& os) const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    map<string, S_mfslChoice>
                          fChoicesMap;
};
typedef SMARTP<mfslChoicesTable> S_mfslChoicesTable;
EXP ostream& operator << (ostream& os, const S_mfslChoicesTable& elt);

//______________________________________________________________________________
class EXP mfslCaseChoiceStatement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslCaseChoiceStatement> create (
                            S_mfslChoice caseChoice,
                            mfslDriver&  drv);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslCaseChoiceStatement (
                            S_mfslChoice caseChoice,
                            mfslDriver&  drv);

    virtual               ~mfslCaseChoiceStatement ();

  public:

    // set and get
    // ------------------------------------------------------

    S_mfslChoice          getCaseChoice () const
                              { return fCaseChoice; }

    const list<string>&   getCaseCurrentLabelsList () const
                              { return fCaseCurrentLabelsList; }

    void                  clearCaseCurrentLabelsList ()
                              { fCaseCurrentLabelsList.clear (); }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerCaseChoiceLabel (
                            const string& label,
                            mfslDriver&   drv);

    void                  checkThatAllLabelsHaveBeenUsed (
                            mfslDriver& drv);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    string                currentLabelsListAsString () const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    S_mfslChoice          fCaseChoice;

    set<string>           fCaseLabelsSet;
    list<string>          fCaseCurrentLabelsList;

    // checking unused labels
    set<string>           fUsedLabels;
    set<string>           fCaseUnusedLabels;
};
typedef SMARTP<mfslCaseChoiceStatement> S_mfslCaseChoiceStatement;
EXP ostream& operator << (ostream& os, const S_mfslCaseChoiceStatement& elt);

// // initialization
// //______________________________________________________________________________
// void initializeMFSLBasicTypes ();


#endif
