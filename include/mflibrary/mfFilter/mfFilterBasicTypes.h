/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfFilterBasicTypes___
#define ___mfFilterBasicTypes___

#include <string>
#include <list>

#include "smartpointer.h"
#include "exports.h"

#include "mfBool.h"
#include "mfList.h"

#include "oahBasicTypes.h"


using namespace std;

using namespace MusicXML2;

using namespace MusicFormats;

//______________________________________________________________________________
// pre-declaration
class mfFilterDriver;

class mfFilterExpression;
typedef SMARTP<mfFilterExpression> S_mfFilterExpression;

class mfFilterTerm;
typedef SMARTP<mfFilterTerm> S_mfFilterTerm;

class mfFilterFactor;
typedef SMARTP<mfFilterFactor> S_mfFilterFactor;

//______________________________________________________________________________
class EXP mfFilterExpression : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfFilterExpression> create ();

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFilterExpression ();

    virtual               ~mfFilterExpression ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  appendTerm (
                            S_mfFilterTerm& term);

    Bool                  stringIsFoundInExpression (
                            std::string theString) const;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;
    std::string           asStringWithFullParentheses () const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    std::list<S_mfFilterTerm>
                          fORedTermsList;
};
typedef SMARTP<mfFilterExpression> S_mfFilterExpression;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFilterExpression& elt);

//______________________________________________________________________________
class EXP mfFilterTerm : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfFilterTerm> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFilterTerm ();

    virtual               ~mfFilterTerm ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  appendFactor (
                            S_mfFilterFactor& factor);

    Bool                  stringIsFoundInTerm (
                            std::string theString) const;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;
    std::string           asStringWithFullParentheses () const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    std::list<S_mfFilterFactor>
                          fANDedFactorsList;
};
typedef SMARTP<mfFilterTerm> S_mfFilterTerm;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFilterTerm& elt);

//______________________________________________________________________________
/*
  purely virtual base class for mfFilterSimpleFactor and mfFilterSubExpression
*/
class EXP mfFilterFactor : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

//     static SMARTP<mfFilterFactor> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFilterFactor ();

    virtual               ~mfFilterFactor ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    virtual Bool          stringIsFoundInFactor (
                            std::string theString) const = 0;

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asString () const = 0;
    virtual std::string   asStringWithFullParentheses () const = 0;

    virtual void          print (std::ostream& os) const = 0;


  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<mfFilterFactor> S_mfFilterFactor;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFilterFactor& elt);

//______________________________________________________________________________
class EXP mfFilterSimpleFactor : public mfFilterFactor
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfFilterSimpleFactor> create (
                            std::string theString,
                            Bool        negated);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFilterSimpleFactor (
                            std::string theString,
                            Bool        negated);

    virtual               ~mfFilterSimpleFactor ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    Bool                  stringIsFoundInFactor (
                            std::string theString) const override
                              {
                                return stringIsFoundInSimpleFactor (theString);
                              }

    Bool                  stringIsFoundInSimpleFactor (
                            std::string theString) const;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;
    std::string           asStringWithFullParentheses () const override;

    void                  print (std::ostream& os) const override;


  private:

    // private fields
    // ------------------------------------------------------

    std::string           fString;
    Bool                  fNegated;
};
typedef SMARTP<mfFilterSimpleFactor> S_mfFilterSimpleFactor;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFilterSimpleFactor& elt);

//______________________________________________________________________________
class EXP mfFilterSubExpression : public mfFilterFactor
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfFilterSubExpression> create (
                            S_mfFilterExpression expression);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFilterSubExpression (
                            S_mfFilterExpression expression);

    virtual               ~mfFilterSubExpression ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    Bool                  stringIsFoundInFactor (
                            std::string theString) const override
                              {
                                return stringIsFoundInSubExpression (theString);
                              }

    Bool                  stringIsFoundInSubExpression (
                            std::string theString) const;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;
    std::string           asStringWithFullParentheses () const override;

    void                  print (std::ostream& os) const override;


  private:

    // private fields
    // ------------------------------------------------------

  S_mfFilterExpression      fExpression;
};
typedef SMARTP<mfFilterSubExpression> S_mfFilterSubExpression;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFilterSubExpression& elt);

//______________________________________________________________________________
void testMfFind (std::ostream& os);

// // initialization
// //______________________________________________________________________________
// void initializemfFilterBasicTypes ();


#endif // ___mfFilterBasicTypes___
