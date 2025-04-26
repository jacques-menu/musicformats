/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mnx2mnxsrBasicTypes___
#define ___mnx2mnxsrBasicTypes___

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
// pre-declaration
class mnx2mnxsrDriver;

//______________________________________________________________________________
class EXP mnx2mnxsrOptionsBlock : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mnx2mnxsrOptionsBlock> create (const std::string& optionsBlockName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mnx2mnxsrOptionsBlock (const std::string& optionsBlockName);

    virtual               ~mnx2mnxsrOptionsBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getOptionsBlockName () const
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
                            mnx2mnxsrDriver& drv);

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

    vector<S_oahOption>   fOptionsBlockOptionsVector;
};
typedef SMARTP<mnx2mnxsrOptionsBlock> S_mnx2mnxsrOptionsBlock;
EXP std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrOptionsBlock& elt);

//______________________________________________________________________________
enum class mnx2mnxsrInputNameKind {
  kInputNameNone,
  kInputNameOptionSupplied,
  kInputNameSetInScript
};

std::string mnx2mnxsrInputNameKindAsString (
  mnx2mnxsrInputNameKind inputNameKind);

std::ostream& operator<< (std::ostream& os, const mnx2mnxsrInputNameKind& elt);

//______________________________________________________________________________
class EXP mnx2mnxsrInput : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mnx2mnxsrInput> create (
                            const std::string& inputName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mnx2mnxsrInput (
                            const std::string& inputName);

    virtual               ~mnx2mnxsrInput ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getInputName () const
                              { return fInputName; }

    void                  setInputNameSuppliedByAnOption (
                            const std::string& name,
                            mnx2mnxsrDriver&        drv);

    void                  selectInputName (
                            const std::string& name,
                            mnx2mnxsrDriver&        drv);

    std::string           getInputName (
                            mnx2mnxsrDriver& drv) const;

    std::string           getInputNameWithoutTrace ( // JMI
                            mnx2mnxsrDriver& drv) const;

    mnx2mnxsrInputNameKind     getInputNameKind () const
                              { return fInputNameKind; }

    const std::set <std::string>&
                          getNamesSet () const
                              { return fNamesSet; }

    S_mnx2mnxsrOptionsBlock    getInputOptionsBlockForName (
                            const std::string& name,
                            mnx2mnxsrDriver&        drv) const;

    void                  setInputIsUsedInCaseInputStatements ()
                              { fInputIsUsedInCaseInputStatements = true; }

    Bool                  getInputIsUsedInCaseInputStatements () const
                              { return fInputIsUsedInCaseInputStatements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addName (
                            const std::string& name,
                            mnx2mnxsrDriver&        drv);

    void                  enrichNameOptionsBlock (
                            const std::string& name,
                            S_mnx2mnxsrOptionsBlock optionsBlock,
                            mnx2mnxsrDriver&        drv);

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

    mnx2mnxsrInputNameKind     fInputNameKind;

    std::set <std::string> fNamesSet;

    Bool                  fInputIsUsedInCaseInputStatements;

    map<std::string, S_mnx2mnxsrOptionsBlock>
                          fInputNamesToOptionsBlocksMap;
};
typedef SMARTP<mnx2mnxsrInput> S_mnx2mnxsrInput;
EXP std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrInput& elt);

//______________________________________________________________________________
class EXP mnx2mnxsrInputsTable : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mnx2mnxsrInputsTable> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mnx2mnxsrInputsTable ();

    virtual               ~mnx2mnxsrInputsTable ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<std::string, S_mnx2mnxsrInput>&
                          getInputsMap () const
                              { return fInputsMap; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addInput (
                            S_mnx2mnxsrInput input,
                            mnx2mnxsrDriver& drv);

//     S_mnx2mnxsrInput          lookupInputByName (
//                             const std::string& name);

    S_mnx2mnxsrInput           fetchInputByName (
                            const std::string& name,
                            const mnx2mnxsrDriver&  drv);

    S_mnx2mnxsrInput           fetchInputByNameNonConst (
                            const std::string& name,
                            mnx2mnxsrDriver&        drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  displayInputsMap (std::ostream& os) const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    map<std::string, S_mnx2mnxsrInput>
                          fInputsMap;
};
typedef SMARTP<mnx2mnxsrInputsTable> S_mnx2mnxsrInputsTable;
EXP std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrInputsTable& elt);

//______________________________________________________________________________
class EXP mnx2mnxsrCaseInputStatement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mnx2mnxsrCaseInputStatement> create (
                            S_mnx2mnxsrInput caseInput,
                            mnx2mnxsrDriver& drv);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mnx2mnxsrCaseInputStatement (
                            S_mnx2mnxsrInput caseInput,
                            mnx2mnxsrDriver& drv);

    virtual               ~mnx2mnxsrCaseInputStatement ();

  public:

    // set and get
    // ------------------------------------------------------

    S_mnx2mnxsrInput           getCaseInputInput () const
                              { return fCaseInputInput; }

    const list<std::string>&
                          getCaseInputCurrentNamesList () const
                              { return fCaseInputCurrentNamesList; }

    void                  clearCaseInputCurrentNamesList ()
                              { fCaseInputCurrentNamesList.clear (); }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerCaseInputName (
                            const std::string& name,
                            mnx2mnxsrDriver&        drv);

    void                  checkThatAllNamesHaveBeenUsed (
                            mnx2mnxsrDriver& drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    std::string           currentNamesListAsString () const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    S_mnx2mnxsrInput           fCaseInputInput;

    std::set <std::string> fCaseInputNamesSet;
    list<std::string>     fCaseInputCurrentNamesList;

    // checking unused names
    std::set <std::string> fUsedNames;
    std::set <std::string> fCaseInputUnusedNames;
};
typedef SMARTP<mnx2mnxsrCaseInputStatement> S_mnx2mnxsrCaseInputStatement;
EXP std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrCaseInputStatement& elt);

//______________________________________________________________________________
enum class mnx2mnxsrChoiceLabelKind {
  kChoiceLabelNone,
  kChoiceLabelOptionSupplied,
  kChoiceLabelSetInScript
};

std::string mnx2mnxsrChoiceLabelKindAsString (
  mnx2mnxsrChoiceLabelKind choiceLabelKind);

std::ostream& operator<< (std::ostream& os, const mnx2mnxsrChoiceLabelKind& elt);

//______________________________________________________________________________
class EXP mnx2mnxsrChoice : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mnx2mnxsrChoice> create (
                            const std::string& choiceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mnx2mnxsrChoice (
                            const std::string& choiceName);

    virtual               ~mnx2mnxsrChoice ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getChoiceName () const
                              { return fChoiceName; }

    void                  setChoiceLabelSuppliedByAnOption (
                            const std::string& label,
                            mnx2mnxsrDriver&   drv);

    void                  selectChoiceLabel (
                            const std::string& label,
                            mnx2mnxsrDriver&   drv);

    std::string           getChoiceLabel (
                            mnx2mnxsrDriver& drv) const;

    std::string           getChoiceLabelWithoutTrace ( // JMI
                            mnx2mnxsrDriver& drv) const;

    mnx2mnxsrChoiceLabelKind   getChoiceLabelKind () const
                              { return fChoiceLabelKind; }

    const std::set <std::string>&
                          getLabelsSet () const
                              { return fLabelsSet; }

    std::string           getChoiceDefaultLabel () const
                              { return fChoiceDefaultLabel; }

    S_mnx2mnxsrOptionsBlock    getChoiceOptionsBlockForLabel (
                            const std::string& label,
                            mnx2mnxsrDriver&        drv) const;

    void                  setChoiceIsUsedInCaseChoiceStatements ()
                              { fChoiceIsUsedInCaseChoiceStatements = true; }

    Bool                  getChoiceIsUsedInCaseChoiceStatements () const
                              { return fChoiceIsUsedInCaseChoiceStatements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addLabel (
                            const std::string& label,
                            mnx2mnxsrDriver&        drv);

    void                  enrichLabelOptionsBlock (
                            const std::string& label,
                            S_mnx2mnxsrOptionsBlock optionsBlock,
                            mnx2mnxsrDriver&        drv);

    void                  registerChoiceDefaultLabel (
                            const std::string& label,
                            mnx2mnxsrDriver&        drv);

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
    mnx2mnxsrChoiceLabelKind   fChoiceLabelKind;

    std::set <std::string> fLabelsSet;

    std::string           fChoiceDefaultLabel;

    Bool                  fChoiceIsUsedInCaseChoiceStatements;

    map<std::string, S_mnx2mnxsrOptionsBlock>
                          fChoiceLabelsToOptionsBlocksMap;
};
typedef SMARTP<mnx2mnxsrChoice> S_mnx2mnxsrChoice;
EXP std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrChoice& elt);

//______________________________________________________________________________
class EXP mnx2mnxsrChoicesTable : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mnx2mnxsrChoicesTable> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mnx2mnxsrChoicesTable ();

    virtual               ~mnx2mnxsrChoicesTable ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<std::string, S_mnx2mnxsrChoice>&
                          getChoicesMap () const
                              { return fChoicesMap; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addChoice (
                            S_mnx2mnxsrChoice choice,
                            mnx2mnxsrDriver&  drv);

//     S_mnx2mnxsrChoice          lookupChoiceByName (
//                             const std::string& name);

    S_mnx2mnxsrChoice          fetchChoiceByName (
                            const std::string& name,
                            const mnx2mnxsrDriver&  drv);

    S_mnx2mnxsrChoice          fetchChoiceByNameNonConst (
                            const std::string& name,
                            mnx2mnxsrDriver&        drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  displayChoicesMap (std::ostream& os) const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    map<std::string, S_mnx2mnxsrChoice>
                          fChoicesMap;
};
typedef SMARTP<mnx2mnxsrChoicesTable> S_mnx2mnxsrChoicesTable;
EXP std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrChoicesTable& elt);

//______________________________________________________________________________
class EXP mnx2mnxsrCaseChoiceStatement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mnx2mnxsrCaseChoiceStatement> create (
                            S_mnx2mnxsrChoice caseChoice,
                            mnx2mnxsrDriver&  drv);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mnx2mnxsrCaseChoiceStatement (
                            S_mnx2mnxsrChoice caseChoice,
                            mnx2mnxsrDriver&  drv);

    virtual               ~mnx2mnxsrCaseChoiceStatement ();

  public:

    // set and get
    // ------------------------------------------------------

    S_mnx2mnxsrChoice          getCaseChoice () const
                              { return fCaseChoice; }

    const list<std::string>&   getCaseCurrentLabelsList () const
                              { return fCaseCurrentLabelsList; }

    void                  clearCaseCurrentLabelsList ()
                              { fCaseCurrentLabelsList.clear (); }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerCaseChoiceLabel (
                            const std::string& label,
                            mnx2mnxsrDriver&        drv);

    void                  checkThatAllLabelsHaveBeenUsed (
                            mnx2mnxsrDriver& drv);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    std::string           currentLabelsListAsString () const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    S_mnx2mnxsrChoice          fCaseChoice;

    std::set <std::string> fCaseLabelsSet;
    list<std::string>     fCaseCurrentLabelsList;

    // checking unused labels
    std::set <std::string> fUsedLabels;
    std::set <std::string> fCaseUnusedLabels;
};
typedef SMARTP<mnx2mnxsrCaseChoiceStatement> S_mnx2mnxsrCaseChoiceStatement;
EXP std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrCaseChoiceStatement& elt);

// // initialization
// //______________________________________________________________________________
// void initializeMNX2MNXSRBasicTypes ();


#endif // ___mnx2mnxsrBasicTypes___
