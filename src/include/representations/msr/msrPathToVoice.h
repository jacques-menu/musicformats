/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrPathToVoice___
#define ___msrPathToVoice___

#include "msrElements.h"

#include "msrVoices.h"

#include "msrStaves.h"

#include "msrParts.h"

#include "msrPartGroups.h"

#include "msrScores.h"

#include "msrBooks.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrPathToVoice : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPathToVoice> createFromVoice (
                            S_msrVoice voice);

    static SMARTP<msrPathToVoice> createFromStaff (
                            S_msrStaff staff);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPathToVoice ();

    virtual               ~msrPathToVoice ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBook (S_msrBook book)
                              { fBook = book; }

    S_msrBook             getBook () const
                              { return fBook; }

    void                  setScore (S_msrScore score)
                              { fScore = score; }

    S_msrScore            getScore () const
                              { return fScore; }

    const list<S_msrPartGroup>&
                          getPartGroupsList () const
                              { return fPartGroupsList; }

    void                  setPart (S_msrPart part)
                              { fPart = part; }

    S_msrPart             getPart () const
                              { return fPart; }

    void                  setStaff (S_msrStaff staff)
                              { fStaff = staff; }

    S_msrStaff            getStaff () const
                              { return fStaff; }

    void                  setVoice (S_msrVoice voice)
                              { fVoice = voice; }

    S_msrVoice            getVoice () const
                              { return fVoice; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPartGroup (S_msrPartGroup partGroup)
                              {
                                fPartGroupsList.push_back (partGroup);
                              }

    Bool                  pathContainsBook (S_msrBook book)
                              { return fBook == book; }

    Bool                  pathContainsScore (S_msrScore score)
                              { return fScore == score; }

    Bool                  pathContainsPart (S_msrPart part)
                              { return fPart == part; }

    Bool                  pathContainsPartGroup (S_msrPartGroup partGroup);

    Bool                  pathContainsStaff (S_msrStaff staff);

    Bool                  pathContainsVoice (S_msrVoice voice);

//   public:
//
//     // visitors
//     // ------------------------------------------------------
//
//     void                  acceptIn  (basevisitor* v) override;
//     void                  acceptOut (basevisitor* v) override;
//
//     void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private methods
    // ------------------------------------------------------

    void                  initializeFromVoice (S_msrVoice voice);

    void                  initializeFromStaff (S_msrStaff staff);

  private:

    // private fields
    // ------------------------------------------------------

    S_msrBook             fBook;

    S_msrScore            fScore;

    // part groups can be nested
    list<S_msrPartGroup>  fPartGroupsList;

    S_msrPart             fPart;

    S_msrStaff            fStaff;
      // a nullptr fVoice means all the staves in the part

    S_msrVoice            fVoice;
      // a nullptr fVoice means all the voices in the staff
};
typedef SMARTP<msrPathToVoice> S_msrPathToVoice;
EXP ostream& operator<< (ostream& os, const S_msrPathToVoice& elt);


} // namespace MusicFormats


#endif
