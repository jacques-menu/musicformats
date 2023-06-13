/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfFindBasicTypes___
#define ___mfFindBasicTypes___

#include <string>
#include <list>

#include "smartpointer.h"
#include "exports.h"

#include "mfBool.h"
#include "mfBool.h"

#include "oahBasicTypes.h"


using namespace std;

using namespace MusicXML2;

using namespace MusicFormats;

//______________________________________________________________________________
// pre-declaration
class mfFindDriver;

class mfFindExpression;
typedef SMARTP<mfFindExpression> S_mfFindExpression;

class mfFindTerm;
typedef SMARTP<mfFindTerm> S_mfFindTerm;

class mfFindFactor;
typedef SMARTP<mfFindFactor> S_mfFindFactor;

//______________________________________________________________________________
class EXP mfFindExpression : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfFindExpression> create ();

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFindExpression ();

    virtual               ~mfFindExpression ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  appendTerm (
                            S_mfFindTerm& term);

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

    std::list<S_mfFindTerm>
                          fORedTermsList;
};
typedef SMARTP<mfFindExpression> S_mfFindExpression;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFindExpression& elt);

//______________________________________________________________________________
class EXP mfFindTerm : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfFindTerm> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFindTerm ();

    virtual               ~mfFindTerm ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  appendFactor (
                            S_mfFindFactor& factor);

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

    std::list<S_mfFindFactor>
                          fANDedFactorsList;
};
typedef SMARTP<mfFindTerm> S_mfFindTerm;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFindTerm& elt);

//______________________________________________________________________________
/*
  purely virtual base class for mfFindSimpleFactor and mfFindSubExpression
*/
class EXP mfFindFactor : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

//     static SMARTP<mfFindFactor> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFindFactor ();

    virtual               ~mfFindFactor ();

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
typedef SMARTP<mfFindFactor> S_mfFindFactor;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFindFactor& elt);

//______________________________________________________________________________
class EXP mfFindSimpleFactor : public mfFindFactor
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfFindSimpleFactor> create (
                            std::string theString,
                            Bool        negated);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFindSimpleFactor (
                            std::string theString,
                            Bool        negated);

    virtual               ~mfFindSimpleFactor ();

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
typedef SMARTP<mfFindSimpleFactor> S_mfFindSimpleFactor;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFindSimpleFactor& elt);

//______________________________________________________________________________
class EXP mfFindSubExpression : public mfFindFactor
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfFindSubExpression> create (
                            S_mfFindExpression expression);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfFindSubExpression (
                            S_mfFindExpression expression);

    virtual               ~mfFindSubExpression ();

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

  S_mfFindExpression
                          fExpression;
};
typedef SMARTP<mfFindSubExpression> S_mfFindSubExpression;
EXP std::ostream& operator<< (std::ostream& os, const S_mfFindSubExpression& elt);

//______________________________________________________________________________
void testMfFind (std::ostream& os);

// // initialization
// //______________________________________________________________________________
// void initializemfFindBasicTypes ();


#endif // ___mfFindBasicTypes___
