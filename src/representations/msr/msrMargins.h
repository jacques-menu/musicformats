/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMargins___
#define ___msrMargins___

#include <string>
#include <map>
#include <ostream>

#include "msrLengths.h"


namespace MusicFormats
{

// margins types
//______________________________________________________________________________

enum class msrMarginTypeKind {
  kMarginOdd, kMarginEven, kMarginBoth
};

std::string msrMarginTypeKindAsString (
  msrMarginTypeKind marginTypeKind);

std::ostream& operator << (std::ostream& os, const msrMarginTypeKind& elt);

std::string existingMsrMarginTypeKinds (size_t namesListMaxLength);

extern std::map<std::string, msrMarginTypeKind>
  gGlobalMsrMarginTypeKindsMap;

void initializeMsrMarginTypeKindsMap ();

// margins
//______________________________________________________________________________

class EXP msrMargin : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrMargin> create (
                            msrMarginTypeKind marginTypeKind,
                            msrLength         marginLength);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMargin (
                            msrMarginTypeKind marginTypeKind,
                            msrLength         marginLength);

                          msrMargin ();

    virtual               ~msrMargin ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMarginTypeKind (msrMarginTypeKind& value)
                              { fMarginTypeKind = value; }

    msrMarginTypeKind     getMarginTypeKind () const
                              { return fMarginTypeKind; }

    void                  setMarginLength (msrLength& value)
                              { fMarginLength = value; }

    msrLength             getMarginLength () const
                              { return fMarginLength; };

  public:

    // public services
    // ------------------------------------------------------

    Bool                  operator== (
                            const msrMargin& other) const
                              {
                                return
                                  fMarginTypeKind == other.fMarginTypeKind
                                   &&
                                  fMarginLength == other.fMarginLength;
                              }

    Bool                  operator!= (
                            const msrMargin& other) const
                              { return ! ((*this) == other); }

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------


    msrMarginTypeKind     fMarginTypeKind;
    msrLength             fMarginLength;
};
typedef SMARTP<msrMargin> S_msrMargin;
EXP std::ostream& operator << (std::ostream& os, const S_msrMargin& elt);

// margins groups
//______________________________________________________________________________

class EXP msrMarginsGroup : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrMarginsGroup> create (
                            msrMarginTypeKind marginTypeKind);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMarginsGroup (
                            msrMarginTypeKind marginTypeKind);

                          msrMarginsGroup ();

    virtual               ~msrMarginsGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // margins type
    msrMarginTypeKind     getMarginsGroupTypeKind () const
                              { return fMarginsGroupTypeKind; }

    // margins
    void                  setLeftMargin (
                            int         inputLineNumber,
                            const S_msrMargin& val);

    S_msrMargin           getLeftMargin () const
                              { return fLeftMargin; }

    void                  setRightMargin (
                            int         inputLineNumber,
                            const S_msrMargin& val);

    S_msrMargin           getRightMargin () const
                              { return fRightMargin; }

    void                  setTopMargin (
                            int         inputLineNumber,
                            const S_msrMargin& val);

    S_msrMargin           getTopMargin () const
                              { return fTopMargin; }

    void                  setBottomMargin (
                            int         inputLineNumber,
                            const S_msrMargin& val);

    S_msrMargin           getBottomMargin () const
                              { return fBottomMargin; }

  public:

    // public services
    // ------------------------------------------------------

  public:

/* JMI
    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // margins type
    msrMarginTypeKind     fMarginsGroupTypeKind;

    // margins
    S_msrMargin           fLeftMargin;
    S_msrMargin           fRightMargin;
    S_msrMargin           fTopMargin;
    S_msrMargin           fBottomMargin;
};
typedef SMARTP<msrMarginsGroup> S_msrMarginsGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msrMarginsGroup& elt);


}

#endif
