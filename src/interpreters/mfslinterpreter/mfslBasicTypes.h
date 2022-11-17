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

    static SMARTP<mfslOptionsBlock> create (const std::string& optionsBlockName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslOptionsBlock (const std::string& optionsBlockName);

    virtual               ~mfslOptionsBlock ();

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
                            mfslDriver& drv);

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
typedef SMARTP<mfslOptionsBlock> S_mfslOptionsBlock;
EXP std::ostream& operator << (std::ostream& os, const S_mfslOptionsBlock& elt);

//______________________________________________________________________________
enum class mfslInputNameKind {
  kInputNameNone,
  kInputNameOptionSupplied,
  kInputNameSetInScript
};

std::string mfslInputNameKindAsString (
  mfslInputNameKind inputNameKind);

std::ostream& operator << (std::ostream& os, const mfslInputNameKind& elt);

//______________________________________________________________________________
class EXP mfslInput : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslInput> create (
                            const std::string& inputName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslInput (
                            const std::string& inputName);

    virtual               ~mfslInput ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getInputName () const
                              { return fInputName; }

    void                  setInputNameSuppliedByAnOption (
                            const std::string& name,
                            mfslDriver&   drv);

    void                  selectInputName (
                            const std::string& name,
                            mfslDriver&   drv);

    std::string           getInputName (
                            mfslDriver& drv) const;

    std::string           getInputNameWithoutTrace ( // JMI
                            mfslDriver& drv) const;

    mfslInputNameKind     getInputNameKind () const
                              { return fInputNameKind; }

    const std::set<std::string>&    getNamesSet () const
                              { return fNamesSet; }

    S_mfslOptionsBlock    getInputOptionsBlockForName (
                            const std::string& name,
                            mfslDriver&   drv) const;

    void                  setInputIsUsedInCaseInputStatements ()
                              { fInputIsUsedInCaseInputStatements = true; }

    Bool                  getInputIsUsedInCaseInputStatements () const
                              { return fInputIsUsedInCaseInputStatements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addName (
                            const std::string& name,
                            mfslDriver&   drv);

    void                  enrichNameOptionsBlock (
                            const std::string&           name,
                            S_mfslOptionsBlock optionsBlock,
                            mfslDriver&        drv);

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

    mfslInputNameKind     fInputNameKind;

    std::set<std::string>           fNamesSet;

    Bool                  fInputIsUsedInCaseInputStatements;

    std::map<std::string, S_mfslOptionsBlock>
                          fInputNamesToOptionsBlocksMap;
};
typedef SMARTP<mfslInput> S_mfslInput;
EXP std::ostream& operator << (std::ostream& os, const S_mfslInput& elt);

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

    const std::map<std::string, S_mfslInput>&
                          getInputsMap () const
                              { return fInputsMap; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addInput (
                            S_mfslInput input,
                            mfslDriver& drv);

//     S_mfslInput          lookupInputByName (
//                             const std::string& name);

    S_mfslInput           fetchInputByName (
                            const std::string&          name,
                            const mfslDriver& drv);

    S_mfslInput           fetchInputByNameNonConst (
                            const std::string& name,
                            mfslDriver&   drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  displayInputsMap (std::ostream& os) const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    std::map<std::string, S_mfslInput>
                          fInputsMap;
};
typedef SMARTP<mfslInputsTable> S_mfslInputsTable;
EXP std::ostream& operator << (std::ostream& os, const S_mfslInputsTable& elt);

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
                            mfslDriver&   drv);

    void                  checkThatAllNamesHaveBeenUsed (
                            mfslDriver& drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    std::string           currentNamesListAsString () const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    S_mfslInput           fCaseInputInput;

    std::set<std::string>           fCaseInputNamesSet;
    std::list<std::string>          fCaseInputCurrentNamesList;

    // checking unused names
    std::set<std::string>           fUsedNames;
    std::set<std::string>           fCaseInputUnusedNames;
};
typedef SMARTP<mfslCaseInputStatement> S_mfslCaseInputStatement;
EXP std::ostream& operator << (std::ostream& os, const S_mfslCaseInputStatement& elt);

//______________________________________________________________________________
enum class mfslChoiceLabelKind {
  kChoiceLabelNone,
  kChoiceLabelOptionSupplied,
  kChoiceLabelSetInScript
};

std::string mfslChoiceLabelKindAsString (
  mfslChoiceLabelKind choiceLabelKind);

std::ostream& operator << (std::ostream& os, const mfslChoiceLabelKind& elt);

//______________________________________________________________________________
class EXP mfslChoice : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslChoice> create (
                            const std::string& choiceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslChoice (
                            const std::string& choiceName);

    virtual               ~mfslChoice ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getChoiceName () const
                              { return fChoiceName; }

    void                  setChoiceLabelSuppliedByAnOption (
                            const std::string& label,
                            mfslDriver&   drv);

    void                  selectChoiceLabel (
                            const std::string& label,
                            mfslDriver&   drv);

    std::string           getChoiceLabel (
                            mfslDriver& drv) const;

    std::string           getChoiceLabelWithoutTrace ( // JMI
                            mfslDriver& drv) const;

    mfslChoiceLabelKind   getChoiceLabelKind () const
                              { return fChoiceLabelKind; }

    const std::set<std::string>&    getLabelsSet () const
                              { return fLabelsSet; }

    std::string           getChoiceDefaultLabel () const
                              { return fChoiceDefaultLabel; }

    S_mfslOptionsBlock    getChoiceOptionsBlockForLabel (
                            const std::string& label,
                            mfslDriver&   drv) const;

    void                  setChoiceIsUsedInCaseChoiceStatements ()
                              { fChoiceIsUsedInCaseChoiceStatements = true; }

    Bool                  getChoiceIsUsedInCaseChoiceStatements () const
                              { return fChoiceIsUsedInCaseChoiceStatements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addLabel (
                            const std::string& label,
                            mfslDriver&   drv);

    void                  enrichLabelOptionsBlock (
                            const std::string&           label,
                            S_mfslOptionsBlock optionsBlock,
                            mfslDriver&        drv);

    void                  registerChoiceDefaultLabel (
                            const std::string& label,
                            mfslDriver&   drv);

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
    mfslChoiceLabelKind   fChoiceLabelKind;

    std::set<std::string>           fLabelsSet;

    std::string           fChoiceDefaultLabel;

    Bool                  fChoiceIsUsedInCaseChoiceStatements;

    std::map<std::string, S_mfslOptionsBlock>
                          fChoiceLabelsToOptionsBlocksMap;
};
typedef SMARTP<mfslChoice> S_mfslChoice;
EXP std::ostream& operator << (std::ostream& os, const S_mfslChoice& elt);

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

    const std::map<std::string, S_mfslChoice>&
                          getChoicesMap () const
                              { return fChoicesMap; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addChoice (
                            S_mfslChoice choice,
                            mfslDriver&  drv);

//     S_mfslChoice          lookupChoiceByName (
//                             const std::string& name);

    S_mfslChoice          fetchChoiceByName (
                            const std::string&          name,
                            const mfslDriver& drv);

    S_mfslChoice          fetchChoiceByNameNonConst (
                            const std::string& name,
                            mfslDriver&   drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  displayChoicesMap (std::ostream& os) const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    std::map<std::string, S_mfslChoice>
                          fChoicesMap;
};
typedef SMARTP<mfslChoicesTable> S_mfslChoicesTable;
EXP std::ostream& operator << (std::ostream& os, const S_mfslChoicesTable& elt);

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
                            mfslDriver&   drv);

    void                  checkThatAllLabelsHaveBeenUsed (
                            mfslDriver& drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    std::string           currentLabelsListAsString () const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    S_mfslChoice          fCaseChoice;

    std::set<std::string>           fCaseLabelsSet;
    std::list<std::string>          fCaseCurrentLabelsList;

    // checking unused labels
    std::set<std::string>           fUsedLabels;
    std::set<std::string>           fCaseUnusedLabels;
};
typedef SMARTP<mfslCaseChoiceStatement> S_mfslCaseChoiceStatement;
EXP std::ostream& operator << (std::ostream& os, const S_mfslCaseChoiceStatement& elt);

// // initialization
// //______________________________________________________________________________
// void initializeMFSLBasicTypes ();


#endif
