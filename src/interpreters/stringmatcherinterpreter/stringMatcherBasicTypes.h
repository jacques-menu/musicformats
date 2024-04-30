/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___stringMatcherBasicTypes___
#define ___stringMatcherBasicTypes___

#include <string>

#include "smartpointer.h"
#include "exports.h"

#include "mfBool.h"

#include "oahBasicTypes.h"


using namespace std;

using namespace MusicXML2;

using namespace MusicFormats;

//______________________________________________________________________________
// pre-declaration
// class stringMatcherDriver;

// class stringMatcherExpression;
// typedef SMARTP<stringMatcherExpression> S_stringMatcherExpression;
//
// class stringMatcherTerm;
// typedef SMARTP<stringMatcherTerm> S_stringMatcherTerm;
//
// class stringMatcherFactor;
// typedef SMARTP<stringMatcherFactor> S_stringMatcherFactor;

//______________________________________________________________________________
class EXP stringMatcherNode : public smartable
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringMatcherOr> create ();
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringMatcherNode ();

    virtual               ~stringMatcherNode ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    virtual Bool          stringMatches (
                            const std::string& theString) const = 0;

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
typedef SMARTP<stringMatcherNode> S_stringMatcherNode;
EXP std::ostream& operator<< (std::ostream& os, const S_stringMatcherNode& elt);

//______________________________________________________________________________
class EXP stringMatcherMonadicOperator : public stringMatcherNode
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringMatcherMonadicOperator> create (
                            const S_stringMatcherNode&  operand);
*/

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringMatcherMonadicOperator (
                            const S_stringMatcherNode&  operand);

    virtual               ~stringMatcherMonadicOperator ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

//     Bool                  stringMatches (
//                             const std::string& theString) const override;

  public:

    // print
    // ------------------------------------------------------

//     std::string           asString () const override;
//     std::string           asStringWithFullParentheses () const override;
//
//     void                  print (std::ostream& os) const override;


  protected:

    // protected fields
    // ------------------------------------------------------

    S_stringMatcherNode    fOperand;
};
typedef SMARTP<stringMatcherMonadicOperator> S_stringMatcherMonadicOperator;
EXP std::ostream& operator<< (std::ostream& os, const S_stringMatcherMonadicOperator& elt);

//______________________________________________________________________________
class EXP stringMatcherDyadicOperator : public stringMatcherNode
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringMatcherDyadicOperator> create (
                            const S_stringMatcherNode& leftOperand,
                            const S_stringMatcherNode& rightOperand);
*/

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringMatcherDyadicOperator (
                            const S_stringMatcherNode& leftOperand,
                            const S_stringMatcherNode& rightOperand);

    virtual               ~stringMatcherDyadicOperator ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

//     Bool                  stringMatches (
//                             const std::string& theString) const override;

  public:

    // print
    // ------------------------------------------------------

//     std::string           asString () const override;;
//     std::string           asStringWithFullParentheses () const override;

//     void                  print (std::ostream& os) const override;


  protected:

    // protected fields
    // ------------------------------------------------------

    S_stringMatcherNode    fLeftOperand;
    S_stringMatcherNode    fRightOperand;
};
typedef SMARTP<stringMatcherDyadicOperator> S_stringMatcherDyadicOperator;
EXP std::ostream& operator<< (std::ostream& os, const S_stringMatcherDyadicOperator& elt);

//______________________________________________________________________________
class EXP stringMatcherNot : public stringMatcherMonadicOperator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringMatcherNot> create (
                            const S_stringMatcherNode& operand);

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringMatcherNot (
                            const S_stringMatcherNode& operand);

    virtual               ~stringMatcherNot ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    Bool                  stringMatches (
                            const std::string& theString) const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;
    std::string           asStringWithFullParentheses () const override;

    void                  print (std::ostream& os) const override;


  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<stringMatcherNot> S_stringMatcherNot;
EXP std::ostream& operator<< (std::ostream& os, const S_stringMatcherNot& elt);

//______________________________________________________________________________
class EXP stringMatcherOr : public stringMatcherDyadicOperator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringMatcherOr> create (
                            const S_stringMatcherNode& leftOperand,
                            const S_stringMatcherNode& rightOperand);

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringMatcherOr (
                            const S_stringMatcherNode& leftOperand,
                            const S_stringMatcherNode& rightOperand);

    virtual               ~stringMatcherOr ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    Bool                  stringMatches (
                            const std::string& theString) const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;;
    std::string           asStringWithFullParentheses () const override;

    void                  print (std::ostream& os) const override;


  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<stringMatcherOr> S_stringMatcherOr;
EXP std::ostream& operator<< (std::ostream& os, const S_stringMatcherOr& elt);

//______________________________________________________________________________
class EXP stringMatcherXor : public stringMatcherDyadicOperator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringMatcherXor> create (
                            const S_stringMatcherNode& leftOperand,
                            const S_stringMatcherNode& rightOperand);

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringMatcherXor (
                            const S_stringMatcherNode& leftOperand,
                            const S_stringMatcherNode& rightOperand);

    virtual               ~stringMatcherXor ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    Bool                  stringMatches (
                            const std::string& theString) const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;
    std::string           asStringWithFullParentheses () const override;

    void                  print (std::ostream& os) const override;


  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<stringMatcherXor> S_stringMatcherXor;
EXP std::ostream& operator<< (std::ostream& os, const S_stringMatcherXor& elt);

//______________________________________________________________________________
class EXP stringMatcherAnd : public stringMatcherDyadicOperator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringMatcherAnd> create (
                            const S_stringMatcherNode& leftOperand,
                            const S_stringMatcherNode& rightOperand);

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringMatcherAnd (
                            const S_stringMatcherNode& leftOperand,
                            const S_stringMatcherNode& rightOperand);

    virtual               ~stringMatcherAnd ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    Bool                  stringMatches (
                            const std::string& theString) const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;
    std::string           asStringWithFullParentheses () const override;

    void                  print (std::ostream& os) const override;


  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<stringMatcherAnd> S_stringMatcherAnd;
EXP std::ostream& operator<< (std::ostream& os, const S_stringMatcherAnd& elt);

//______________________________________________________________________________
class EXP stringMatcherString : public stringMatcherNode
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringMatcherString> create (
                            const std::string& theString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          stringMatcherString (
                            const std::string& theString);

    virtual               ~stringMatcherString ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    Bool                  stringMatches (
                            const std::string& theString) const override;

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
};
typedef SMARTP<stringMatcherString> S_stringMatcherString;
EXP std::ostream& operator<< (std::ostream& os, const S_stringMatcherString& elt);

// //______________________________________________________________________________
// class EXP stringMatcherSubExpression : public stringMatcherFactor
// {
//   public:
//
//     // creation
//     // ------------------------------------------------------
//
//     static SMARTP<stringMatcherSubExpression> create (
//                             S_stringMatcherExpression expression);
//
//   protected:
//
//     // constructors/destructor
//     // ------------------------------------------------------
//
//                           stringMatcherSubExpression (
//                             S_stringMatcherExpression expression);
//
//     virtual               ~stringMatcherSubExpression ();
//
//   public:
//
//     // set and get
//     // ------------------------------------------------------
//
//   public:
//
//     // public services
//     // ------------------------------------------------------
//
//     Bool                  stringMatches (
//                             const std::string& theString) const override;
//
//   public:
//
//     // print
//     // ------------------------------------------------------
//
//     std::string           asString () const override;
//     std::string           asStringWithFullParentheses () const override;
//
//     void                  print (std::ostream& os) const override;
//
//
//   private:
//
//     // private fields
//     // ------------------------------------------------------
//
//   S_stringMatcherExpression      fExpression;
// };
// typedef SMARTP<stringMatcherSubExpression> S_stringMatcherSubExpression;
// EXP std::ostream& operator<< (std::ostream& os, const S_stringMatcherSubExpression& elt);

// // initialization
// //______________________________________________________________________________
// void initializestringMatcherBasicTypes ();


#endif // ___stringMatcherBasicTypes___
