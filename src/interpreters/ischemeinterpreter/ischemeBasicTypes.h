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

    static SMARTP<ischemeOptionsBlock> create (const std::string& optionsBlockName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeOptionsBlock (const std::string& optionsBlockName);

    virtual               ~ischemeOptionsBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getOptionsBlockName () const
                              { return fOptionsBlockName; }

    size_t                getOptionsBlockSize () const
                              { return fOptionsBlockOptionsVector.size (); }

    const std::vector<S_oahOption>&
                          getOptionsBlockOptionsVector () const
                              { return fOptionsBlockOptionsVector; }

    std::vector<S_oahOption>&
                          getOptionsBlockOptionsVectorNonConst ()
                              { return fOptionsBlockOptionsVector; }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerOptionsInOptionsBlock (
                            const S_oahOption& option,
                            ischemeDriver& drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    std::string           asOptionsString () const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    std::string           fOptionsBlockName;

    std::vector<S_oahOption>   fOptionsBlockOptionsVector;
};
typedef SMARTP<ischemeOptionsBlock> S_ischemeOptionsBlock;
EXP std::ostream& operator << (std::ostream& os, const S_ischemeOptionsBlock& elt);

//______________________________________________________________________________
enum class ischemeInputNameKind {
  kInputNameNone,
  kInputNameOptionSupplied,
  kInputNameSetInScript
};

std::string ischemeInputNameKindAsString (
  ischemeInputNameKind inputNameKind);

std::ostream& operator << (std::ostream& os, const ischemeInputNameKind& elt);

//______________________________________________________________________________
class EXP ischemeInput : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<ischemeInput> create (
                            const std::string& inputName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeInput (
                            const std::string& inputName);

    virtual               ~ischemeInput ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getInputName () const
                              { return fInputName; }

    void                  setInputNameSuppliedByAnOption (
                            const std::string& name,
                            ischemeDriver&   drv);

    void                  selectInputName (
                            const std::string& name,
                            ischemeDriver&   drv);

    std::string           getInputName (
                            ischemeDriver& drv) const;

    std::string           getInputNameWithoutTrace ( // JMI
                            ischemeDriver& drv) const;

    ischemeInputNameKind     getInputNameKind () const
                              { return fInputNameKind; }

    const std::set<std::string>&    getNamesSet () const
                              { return fNamesSet; }

    S_ischemeOptionsBlock    getInputOptionsBlockForName (
                            const std::string& name,
                            ischemeDriver&   drv) const;

    void                  setInputIsUsedInCaseInputStatements ()
                              { fInputIsUsedInCaseInputStatements = true; }

    Bool                  getInputIsUsedInCaseInputStatements () const
                              { return fInputIsUsedInCaseInputStatements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addName (
                            const std::string& name,
                            ischemeDriver&   drv);

    void                  enrichNameOptionsBlock (
                            const std::string&           name,
                            S_ischemeOptionsBlock optionsBlock,
                            ischemeDriver&        drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           namesSetAsString () const;

    std::string           asString () const;

    void                  displayInputNamesToOptionsBlocksMap (
                            std::ostream& os) const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fInputName;

    ischemeInputNameKind     fInputNameKind;

    std::set<std::string>           fNamesSet;

    Bool                  fInputIsUsedInCaseInputStatements;

    std::map<std::string, S_ischemeOptionsBlock>
                          fInputNamesToOptionsBlocksMap;
};
typedef SMARTP<ischemeInput> S_ischemeInput;
EXP std::ostream& operator << (std::ostream& os, const S_ischemeInput& elt);

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

    const std::map<std::string, S_ischemeInput>&
                          getInputsMap () const
                              { return fInputsMap; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addInput (
                            S_ischemeInput input,
                            ischemeDriver& drv);

//     S_ischemeInput          lookupInputByName (
//                             const std::string& name);

    S_ischemeInput           fetchInputByName (
                            const std::string&          name,
                            const ischemeDriver& drv);

    S_ischemeInput           fetchInputByNameNonConst (
                            const std::string& name,
                            ischemeDriver&   drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  displayInputsMap (std::ostream& os) const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    std::map<std::string, S_ischemeInput>
                          fInputsMap;
};
typedef SMARTP<ischemeInputsTable> S_ischemeInputsTable;
EXP std::ostream& operator << (std::ostream& os, const S_ischemeInputsTable& elt);

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

    const std::list<std::string>&
                          getCaseInputCurrentNamesList () const
                              { return fCaseInputCurrentNamesList; }

    void                  clearCaseInputCurrentNamesList ()
                              { fCaseInputCurrentNamesList.clear (); }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerCaseInputName (
                            const std::string& name,
                            ischemeDriver&   drv);

    void                  checkThatAllNamesHaveBeenUsed (
                            ischemeDriver& drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    std::string           currentNamesListAsString () const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    S_ischemeInput           fCaseInputInput;

    std::set<std::string>           fCaseInputNamesSet;
    std::list<std::string>          fCaseInputCurrentNamesList;

    // checking unused names
    std::set<std::string>           fUsedNames;
    std::set<std::string>           fCaseInputUnusedNames;
};
typedef SMARTP<ischemeCaseInputStatement> S_ischemeCaseInputStatement;
EXP std::ostream& operator << (std::ostream& os, const S_ischemeCaseInputStatement& elt);

//______________________________________________________________________________
enum class ischemeChoiceLabelKind {
  kChoiceLabelNone,
  kChoiceLabelOptionSupplied,
  kChoiceLabelSetInScript
};

std::string ischemeChoiceLabelKindAsString (
  ischemeChoiceLabelKind choiceLabelKind);

std::ostream& operator << (std::ostream& os, const ischemeChoiceLabelKind& elt);

//______________________________________________________________________________
class EXP ischemeChoice : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<ischemeChoice> create (
                            const std::string& choiceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          ischemeChoice (
                            const std::string& choiceName);

    virtual               ~ischemeChoice ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getChoiceName () const
                              { return fChoiceName; }

    void                  setChoiceLabelSuppliedByAnOption (
                            const std::string& label,
                            ischemeDriver&   drv);

    void                  selectChoiceLabel (
                            const std::string& label,
                            ischemeDriver&   drv);

    std::string           getChoiceLabel (
                            ischemeDriver& drv) const;

    std::string           getChoiceLabelWithoutTrace ( // JMI
                            ischemeDriver& drv) const;

    ischemeChoiceLabelKind   getChoiceLabelKind () const
                              { return fChoiceLabelKind; }

    const std::set<std::string>&    getLabelsSet () const
                              { return fLabelsSet; }

    std::string           getChoiceDefaultLabel () const
                              { return fChoiceDefaultLabel; }

    S_ischemeOptionsBlock    getChoiceOptionsBlockForLabel (
                            const std::string& label,
                            ischemeDriver&   drv) const;

    void                  setChoiceIsUsedInCaseChoiceStatements ()
                              { fChoiceIsUsedInCaseChoiceStatements = true; }

    Bool                  getChoiceIsUsedInCaseChoiceStatements () const
                              { return fChoiceIsUsedInCaseChoiceStatements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addLabel (
                            const std::string& label,
                            ischemeDriver&   drv);

    void                  enrichLabelOptionsBlock (
                            const std::string&           label,
                            S_ischemeOptionsBlock optionsBlock,
                            ischemeDriver&        drv);

    void                  registerChoiceDefaultLabel (
                            const std::string& label,
                            ischemeDriver&   drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           labelsSetAsString () const;

    std::string           asString () const;

    void                  displayChoiceLabelsToOptionsBlocksMap (
                            std::ostream& os) const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fChoiceName;

    std::string           fChoiceLabel;
    ischemeChoiceLabelKind   fChoiceLabelKind;

    std::set<std::string>           fLabelsSet;

    std::string           fChoiceDefaultLabel;

    Bool                  fChoiceIsUsedInCaseChoiceStatements;

    std::map<std::string, S_ischemeOptionsBlock>
                          fChoiceLabelsToOptionsBlocksMap;
};
typedef SMARTP<ischemeChoice> S_ischemeChoice;
EXP std::ostream& operator << (std::ostream& os, const S_ischemeChoice& elt);

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

    const std::map<std::string, S_ischemeChoice>&
                          getChoicesMap () const
                              { return fChoicesMap; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addChoice (
                            S_ischemeChoice choice,
                            ischemeDriver&  drv);

//     S_ischemeChoice          lookupChoiceByName (
//                             const std::string& name);

    S_ischemeChoice          fetchChoiceByName (
                            const std::string&          name,
                            const ischemeDriver& drv);

    S_ischemeChoice          fetchChoiceByNameNonConst (
                            const std::string& name,
                            ischemeDriver&   drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  displayChoicesMap (std::ostream& os) const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    std::map<std::string, S_ischemeChoice>
                          fChoicesMap;
};
typedef SMARTP<ischemeChoicesTable> S_ischemeChoicesTable;
EXP std::ostream& operator << (std::ostream& os, const S_ischemeChoicesTable& elt);

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

    const std::list<std::string>&
                          getCaseCurrentLabelsList () const
                              { return fCaseCurrentLabelsList; }

    void                  clearCaseCurrentLabelsList ()
                              { fCaseCurrentLabelsList.clear (); }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerCaseChoiceLabel (
                            const std::string& label,
                            ischemeDriver&   drv);

    void                  checkThatAllLabelsHaveBeenUsed (
                            ischemeDriver& drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    std::string           currentLabelsListAsString () const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    S_ischemeChoice          fCaseChoice;

    std::set<std::string>           fCaseLabelsSet;
    std::list<std::string>          fCaseCurrentLabelsList;

    // checking unused labels
    std::set<std::string>           fUsedLabels;
    std::set<std::string>           fCaseUnusedLabels;
};
typedef SMARTP<ischemeCaseChoiceStatement> S_ischemeCaseChoiceStatement;
EXP std::ostream& operator << (std::ostream& os, const S_ischemeCaseChoiceStatement& elt);

// // initialization
// //______________________________________________________________________________
// void initializeiSchemeBasicTypes ();


#endif
