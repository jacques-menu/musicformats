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
// pre-declaration
class mfslDriver;

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
                          getOptionsBlockOptionsVectorToModify ()
                              { return fOptionsBlockOptionsVector; }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerOptionsInOptionsBlock (S_oahOption option);

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
EXP ostream& operator<< (ostream& os, const S_mfslOptionsBlock& elt);

//______________________________________________________________________________
enum class mfslChoiceLabelKind {
  kChoiceLabelNone,
  kChoiceLabelOptionSupplied,
  kChoiceLabelSetInScript
};

string mfslChoiceLabelKindAsString (
  mfslChoiceLabelKind choiceLabelKind);

ostream& operator<< (ostream& os, const mfslChoiceLabelKind& elt);

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
                            const string& label);

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

    void                  setChoiceIsUsedInCaseStatements ()
                              { fChoiceIsUsedInCaseStatements = true; }

    Bool                  getChoiceIsUsedInCaseStatements () const
                              { return fChoiceIsUsedInCaseStatements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addLabel (
                            const string& label,
                            mfslDriver&   drv);

    void                  enrichLabelOptionsBlock (
                            const string&      label,
                            S_mfslOptionsBlock optionsBlock);

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

    Bool                  fChoiceIsUsedInCaseStatements;

    map<string, S_mfslOptionsBlock>
                          fChoiceLabelsToOptionsBlocksMap;
};
typedef SMARTP<mfslChoice> S_mfslChoice;
EXP ostream& operator<< (ostream& os, const S_mfslChoice& elt);

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

    S_mfslChoice          lookupChoiceByName (
                            const string& name);

    S_mfslChoice          fetchChoiceByName (
                            const string&     name,
                            const mfslDriver& drv);
    S_mfslChoice          fetchChoiceByNameToMofidy (
                            const string&    name,
                            const mfslDriver drv);

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
EXP ostream& operator<< (ostream& os, const S_mfslChoicesTable& elt);

//______________________________________________________________________________
class EXP mfslCaseStatement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslCaseStatement> create (
                            S_mfslChoice caseChoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslCaseStatement (
                            S_mfslChoice caseChoice);

    virtual               ~mfslCaseStatement ();

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

    void                  registerCaseLabel (
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
typedef SMARTP<mfslCaseStatement> S_mfslCaseStatement;
EXP ostream& operator<< (ostream& os, const S_mfslCaseStatement& elt);

// // initialization
// //______________________________________________________________________________
// void initializeMFSLBasicTypes ();


#endif
