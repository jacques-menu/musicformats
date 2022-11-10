/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___ischemeBasicTypes___
#define ___ischemeBasicTypes___

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

class   ischemeDriver;

//______________________________________________________________________________
class EXP ischemeOptionsBlock : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<ischemeOptionsBlock> create (const string& optionsBlockName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeOptionsBlock (const string& optionsBlockName);

    virtual               ~ischemeOptionsBlock ();

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
                            ischemeDriver& drv);

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
typedef SMARTP<ischemeOptionsBlock> S_ischemeOptionsBlock;
EXP ostream& operator << (ostream& os, const S_ischemeOptionsBlock& elt);

//______________________________________________________________________________
enum class ischemeInputNameKind {
  kInputNameNone,
  kInputNameOptionSupplied,
  kInputNameSetInScript
};

string ischemeInputNameKindAsString (
  ischemeInputNameKind inputNameKind);

ostream& operator << (ostream& os, const ischemeInputNameKind& elt);

//______________________________________________________________________________
class EXP ischemeInput : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<ischemeInput> create (
                            const string& inputName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeInput (
                            const string& inputName);

    virtual               ~ischemeInput ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getInputName () const
                              { return fInputName; }

    void                  setInputNameSuppliedByAnOption (
                            const string& name,
                            ischemeDriver&   drv);

    void                  selectInputName (
                            const string& name,
                            ischemeDriver&   drv);

    string                getInputName (
                            ischemeDriver& drv) const;

    string                getInputNameWithoutTrace ( // JMI
                            ischemeDriver& drv) const;

    ischemeInputNameKind     getInputNameKind () const
                              { return fInputNameKind; }

    const set<string>&    getNamesSet () const
                              { return fNamesSet; }

    S_ischemeOptionsBlock    getInputOptionsBlockForName (
                            const string& name,
                            ischemeDriver&   drv) const;

    void                  setInputIsUsedInCaseInputStatements ()
                              { fInputIsUsedInCaseInputStatements = true; }

    Bool                  getInputIsUsedInCaseInputStatements () const
                              { return fInputIsUsedInCaseInputStatements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addName (
                            const string& name,
                            ischemeDriver&   drv);

    void                  enrichNameOptionsBlock (
                            const string&      name,
                            S_ischemeOptionsBlock optionsBlock,
                            ischemeDriver&        drv);

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

    ischemeInputNameKind     fInputNameKind;

    set<string>           fNamesSet;

    Bool                  fInputIsUsedInCaseInputStatements;

    map<string, S_ischemeOptionsBlock>
                          fInputNamesToOptionsBlocksMap;
};
typedef SMARTP<ischemeInput> S_ischemeInput;
EXP ostream& operator << (ostream& os, const S_ischemeInput& elt);

//______________________________________________________________________________
class EXP ischemeInputsTable : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<ischemeInputsTable> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeInputsTable ();

    virtual               ~ischemeInputsTable ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, S_ischemeInput>&
                          getInputsMap () const
                              { return fInputsMap; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addInput (
                            S_ischemeInput input,
                            ischemeDriver& drv);

//     S_ischemeInput          lookupInputByName (
//                             const string& name);

    S_ischemeInput           fetchInputByName (
                            const string&     name,
                            const ischemeDriver& drv);

    S_ischemeInput           fetchInputByNameNonConst (
                            const string& name,
                            ischemeDriver&   drv);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  displayInputsMap (ostream& os) const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    map<string, S_ischemeInput>
                          fInputsMap;
};
typedef SMARTP<ischemeInputsTable> S_ischemeInputsTable;
EXP ostream& operator << (ostream& os, const S_ischemeInputsTable& elt);

//______________________________________________________________________________
class EXP ischemeCaseInputStatement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<ischemeCaseInputStatement> create (
                            S_ischemeInput caseInput,
                            ischemeDriver& drv);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeCaseInputStatement (
                            S_ischemeInput caseInput,
                            ischemeDriver& drv);

    virtual               ~ischemeCaseInputStatement ();

  public:

    // set and get
    // ------------------------------------------------------

    S_ischemeInput           getCaseInputInput () const
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
                            ischemeDriver&   drv);

    void                  checkThatAllNamesHaveBeenUsed (
                            ischemeDriver& drv);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    string                currentNamesListAsString () const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    S_ischemeInput           fCaseInputInput;

    set<string>           fCaseInputNamesSet;
    list<string>          fCaseInputCurrentNamesList;

    // checking unused names
    set<string>           fUsedNames;
    set<string>           fCaseInputUnusedNames;
};
typedef SMARTP<ischemeCaseInputStatement> S_ischemeCaseInputStatement;
EXP ostream& operator << (ostream& os, const S_ischemeCaseInputStatement& elt);

//______________________________________________________________________________
enum class ischemeChoiceLabelKind {
  kChoiceLabelNone,
  kChoiceLabelOptionSupplied,
  kChoiceLabelSetInScript
};

string ischemeChoiceLabelKindAsString (
  ischemeChoiceLabelKind choiceLabelKind);

ostream& operator << (ostream& os, const ischemeChoiceLabelKind& elt);

//______________________________________________________________________________
class EXP ischemeChoice : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<ischemeChoice> create (
                            const string& choiceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeChoice (
                            const string& choiceName);

    virtual               ~ischemeChoice ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getChoiceName () const
                              { return fChoiceName; }

    void                  setChoiceLabelSuppliedByAnOption (
                            const string& label,
                            ischemeDriver&   drv);

    void                  selectChoiceLabel (
                            const string& label,
                            ischemeDriver&   drv);

    string                getChoiceLabel (
                            ischemeDriver& drv) const;

    string                getChoiceLabelWithoutTrace ( // JMI
                            ischemeDriver& drv) const;

    ischemeChoiceLabelKind   getChoiceLabelKind () const
                              { return fChoiceLabelKind; }

    const set<string>&    getLabelsSet () const
                              { return fLabelsSet; }

    string                getChoiceDefaultLabel () const
                              { return fChoiceDefaultLabel; }

    S_ischemeOptionsBlock    getChoiceOptionsBlockForLabel (
                            const string& label,
                            ischemeDriver&   drv) const;

    void                  setChoiceIsUsedInCaseChoiceStatements ()
                              { fChoiceIsUsedInCaseChoiceStatements = true; }

    Bool                  getChoiceIsUsedInCaseChoiceStatements () const
                              { return fChoiceIsUsedInCaseChoiceStatements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addLabel (
                            const string& label,
                            ischemeDriver&   drv);

    void                  enrichLabelOptionsBlock (
                            const string&      label,
                            S_ischemeOptionsBlock optionsBlock,
                            ischemeDriver&        drv);

    void                  registerChoiceDefaultLabel (
                            const string& label,
                            ischemeDriver&   drv);

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
    ischemeChoiceLabelKind   fChoiceLabelKind;

    set<string>           fLabelsSet;

    string                fChoiceDefaultLabel;

    Bool                  fChoiceIsUsedInCaseChoiceStatements;

    map<string, S_ischemeOptionsBlock>
                          fChoiceLabelsToOptionsBlocksMap;
};
typedef SMARTP<ischemeChoice> S_ischemeChoice;
EXP ostream& operator << (ostream& os, const S_ischemeChoice& elt);

//______________________________________________________________________________
class EXP ischemeChoicesTable : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<ischemeChoicesTable> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeChoicesTable ();

    virtual               ~ischemeChoicesTable ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, S_ischemeChoice>&
                          getChoicesMap () const
                              { return fChoicesMap; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addChoice (
                            S_ischemeChoice choice,
                            ischemeDriver&  drv);

//     S_ischemeChoice          lookupChoiceByName (
//                             const string& name);

    S_ischemeChoice          fetchChoiceByName (
                            const string&     name,
                            const ischemeDriver& drv);

    S_ischemeChoice          fetchChoiceByNameNonConst (
                            const string& name,
                            ischemeDriver&   drv);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  displayChoicesMap (ostream& os) const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    map<string, S_ischemeChoice>
                          fChoicesMap;
};
typedef SMARTP<ischemeChoicesTable> S_ischemeChoicesTable;
EXP ostream& operator << (ostream& os, const S_ischemeChoicesTable& elt);

//______________________________________________________________________________
class EXP ischemeCaseChoiceStatement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<ischemeCaseChoiceStatement> create (
                            S_ischemeChoice caseChoice,
                            ischemeDriver&  drv);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeCaseChoiceStatement (
                            S_ischemeChoice caseChoice,
                            ischemeDriver&  drv);

    virtual               ~ischemeCaseChoiceStatement ();

  public:

    // set and get
    // ------------------------------------------------------

    S_ischemeChoice          getCaseChoice () const
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
                            ischemeDriver&   drv);

    void                  checkThatAllLabelsHaveBeenUsed (
                            ischemeDriver& drv);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    string                currentLabelsListAsString () const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    S_ischemeChoice          fCaseChoice;

    set<string>           fCaseLabelsSet;
    list<string>          fCaseCurrentLabelsList;

    // checking unused labels
    set<string>           fUsedLabels;
    set<string>           fCaseUnusedLabels;
};
typedef SMARTP<ischemeCaseChoiceStatement> S_ischemeCaseChoiceStatement;
EXP ostream& operator << (ostream& os, const S_ischemeCaseChoiceStatement& elt);

// // initialization
// //______________________________________________________________________________
// void initializeiSchemeBasicTypes ();


#endif
