/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrPartGroups___
#define ___mxsrPartGroups___

#include "msrPartGroups.h"


namespace MusicFormats
{

/*
  The part groups are described for the neded of this pass by class mxsrPartGroup,
  which is distinct from msrPartGroup, an instance of which they contain.

  The identifiers for msrPartGroups instances contain 'Msr'
  wherever needed to make things clear
*/

//________________________________________________________________________
// pre-declaration
class   mxsrPartGroup;
typedef SMARTP<mxsrPartGroup> S_mxsrPartGroup;

//________________________________________________________________________
class mxsrPartGroup : public smartable
{
/*
  positions represent the order in which the parts appear in <part-list />,
  starting at 0 since std::vectors are used
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxsrPartGroup> create (
                            int                   inputLineNumber,
                            int                   partGroupNumber,
                            const S_msrPartGroup& theMsrPartGroup,
                            int                   partGroupSequentialNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrPartGroup (
                            int                   inputLineNumber,
                            int                   partGroupNumber,
                            const S_msrPartGroup& theMsrPartGroup,
                            int                   partGroupSequentialNumber);

    virtual               ~mxsrPartGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getPartGroupNumber () const
                              { return fPartGroupNumber; }

    void                  setPartGroupSequentialNumber (int partGroupSequentialNumber)
                              { fPartGroupSequentialNumber = partGroupSequentialNumber; }

    int                   getPartGroupSequentialNumber () const
                              { return fPartGroupSequentialNumber; }

    int                   getInputLineNumber () const
                              { return fInputLineNumber; }

    int                   getStopInputLineNumber () const
                              { return fStopInputLineNumber; }

    S_msrPartGroup        getMsrPartGroup () const
                              { return fMsrPartGroup; }

  public:

    // public services
    // ------------------------------------------------------

    std::string           fetchMsrPartGroupNameForTrace () const
                              {
                                return
                                  fMsrPartGroup->fetchPartGroupInformationForTrace ();
                              }

    static bool           comparePartGroupsByIncreasingIdentity (
                            const S_mxsrPartGroup& first,
                            const S_mxsrPartGroup& second);

    static bool           comparePartGroupsByDecreasingIdentity (
                            const S_mxsrPartGroup& first,
                            const S_mxsrPartGroup& second);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fPartGroupNumber; // may be reused later

    int                   fPartGroupSequentialNumber; // set upon part group start

    int                   fInputLineNumber;
    int                   fStopInputLineNumber;


    S_msrPartGroup        fMsrPartGroup;
};
EXP std::ostream& operator << (std::ostream& os, const S_mxsrPartGroup& elt);

//______________________________________________________________________________
class EXP mxsrPartGroupsList : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxsrPartGroupsList> create ();

    static SMARTP<mxsrPartGroupsList> create (
                            const S_mxsrPartGroup& partGroup);

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrPartGroupsList ();

                          mxsrPartGroupsList (
                            const S_mxsrPartGroup& partGroup);

    virtual               ~mxsrPartGroupsList ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::list <S_mxsrPartGroup>&
                          getMxsrPartGroupsStdList () const
                              { return fMxsrPartGroupsStdList;}

  public:

    // public services
    // ------------------------------------------------------

    // basic list stuff
    size_t                size () const
                              { return fMxsrPartGroupsStdList.size (); }

    S_mxsrPartGroup&      front ()
                              { return fMxsrPartGroupsStdList.front (); }

    S_mxsrPartGroup&      back ()
                              { return fMxsrPartGroupsStdList.back (); }

    void                  pop_front ()
                              { fMxsrPartGroupsStdList.pop_front (); }

    void                  pop_back ()
                              { fMxsrPartGroupsStdList.pop_back (); }

    void                  push_front (
                            const S_mxsrPartGroup& partGroup)
                              { fMxsrPartGroupsStdList.push_front (partGroup); }

    void                  push_back (
                            const S_mxsrPartGroup& partGroup)
                              { fMxsrPartGroupsStdList.push_back (partGroup); }

    // sort
    void                  sortByDecreasingIdentity();

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const;

    void                  printWithContext (
                            const std::string& context,
                            char               evidencer,
                            std::ostream&      os) const;

  private:

    // private services
    // ------------------------------------------------------


    // add an mxsrPartGroup
    void                  prependPartGroup (
                            const S_mxsrPartGroup& thePartGroup);

    void                  appendPartGroup (
                            const S_mxsrPartGroup& thePartGroup);

    // private fields
    // ------------------------------------------------------

    std::string           fMxsrPartGroupListName;

    std::list <S_mxsrPartGroup>
                          fMxsrPartGroupsStdList;
};
typedef SMARTP<mxsrPartGroupsList> S_mxsrPartGroupsList;
EXP std::ostream& operator << (std::ostream& os, const mxsrPartGroupsList& elt);
EXP std::ostream& operator << (std::ostream& os, const S_mxsrPartGroupsList& elt);

//______________________________________________________________________________
class EXP mxsrPartGroupsStack : public smartable
/*
  C++ std::stack cannot be traversed, so we use std::list instead
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxsrPartGroupsStack> create ();

    static SMARTP<mxsrPartGroupsStack> create (
                            const S_mxsrPartGroup& partGroup);

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrPartGroupsStack ();

                          mxsrPartGroupsStack (
                            const S_mxsrPartGroup& partGroup);

    virtual               ~mxsrPartGroupsStack ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::list <S_mxsrPartGroup>&
                          getMxsrPartGroupsStdList () const
                              { return fMxsrPartGroupsStdList;}

  public:

    // public services
    // ------------------------------------------------------

    // basic list stuff
    size_t                size () const
                              { return fMxsrPartGroupsStdList.size (); }

    Bool                  empty () const
                              { return fMxsrPartGroupsStdList.size () == 0; }

    void                  push (
                            const S_mxsrPartGroup& partGroup)
                              { fMxsrPartGroupsStdList.push_front (partGroup); }

    S_mxsrPartGroup&      top ()
                              { return fMxsrPartGroupsStdList.front (); }

    void                  pop ()
                              { fMxsrPartGroupsStdList.pop_front (); }

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const;

    void                  printWithContext (
                            const std::string& context,
                            char               evidencer,
                            std::ostream&      os) const;

  private:

    // private services
    // ------------------------------------------------------

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fMxsrPartGroupStackName;

    std::list <S_mxsrPartGroup>
                          fMxsrPartGroupsStdList;
};
typedef SMARTP<mxsrPartGroupsStack> S_mxsrPartGroupsStack;
EXP std::ostream& operator << (std::ostream& os, const mxsrPartGroupsStack& elt);
EXP std::ostream& operator << (std::ostream& os, const S_mxsrPartGroupsStack& elt);

}

#endif // ___mxsrPartGroups___
