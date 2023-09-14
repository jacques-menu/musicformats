/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___stringFilterBasicTypes___
#define ___stringFilterBasicTypes___

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
// class stringFilterDriver;

// class stringFilterExpression;
// typedef SMARTP<stringFilterExpression> S_stringFilterExpression;
//
// class stringFilterTerm;
// typedef SMARTP<stringFilterTerm> S_stringFilterTerm;
//
// class stringFilterFactor;
// typedef SMARTP<stringFilterFactor> S_stringFilterFactor;

//______________________________________________________________________________
class EXP stringFilterNode : public smartable
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringFilterOr> create ();
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringFilterNode ();

    virtual               ~stringFilterNode ();

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
typedef SMARTP<stringFilterNode> S_stringFilterNode;
EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterNode& elt);

//______________________________________________________________________________
class EXP stringFilterMonadicOperator : public stringFilterNode
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringFilterMonadicOperator> create (
                            const S_stringFilterNode&  operand);
*/

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringFilterMonadicOperator (
                            const S_stringFilterNode&  operand);

    virtual               ~stringFilterMonadicOperator ();

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

    S_stringFilterNode    fOperand;
};
typedef SMARTP<stringFilterMonadicOperator> S_stringFilterMonadicOperator;
EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterMonadicOperator& elt);

//______________________________________________________________________________
class EXP stringFilterDyadicOperator : public stringFilterNode
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringFilterDyadicOperator> create (
                            const S_stringFilterNode& leftOperand,
                            const S_stringFilterNode& rightOperand);
*/

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringFilterDyadicOperator (
                            const S_stringFilterNode& leftOperand,
                            const S_stringFilterNode& rightOperand);

    virtual               ~stringFilterDyadicOperator ();

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

    S_stringFilterNode    fLeftOperand;
    S_stringFilterNode    fRightOperand;
};
typedef SMARTP<stringFilterDyadicOperator> S_stringFilterDyadicOperator;
EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterDyadicOperator& elt);

//______________________________________________________________________________
class EXP stringFilterNot : public stringFilterMonadicOperator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringFilterNot> create (
                            const S_stringFilterNode& operand);

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringFilterNot (
                            const S_stringFilterNode& operand);

    virtual               ~stringFilterNot ();

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
typedef SMARTP<stringFilterNot> S_stringFilterNot;
EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterNot& elt);

//______________________________________________________________________________
class EXP stringFilterOr : public stringFilterDyadicOperator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringFilterOr> create (
                            const S_stringFilterNode& leftOperand,
                            const S_stringFilterNode& rightOperand);

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringFilterOr (
                            const S_stringFilterNode& leftOperand,
                            const S_stringFilterNode& rightOperand);

    virtual               ~stringFilterOr ();

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
typedef SMARTP<stringFilterOr> S_stringFilterOr;
EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterOr& elt);

//______________________________________________________________________________
class EXP stringFilterXor : public stringFilterDyadicOperator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringFilterXor> create (
                            const S_stringFilterNode& leftOperand,
                            const S_stringFilterNode& rightOperand);

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringFilterXor (
                            const S_stringFilterNode& leftOperand,
                            const S_stringFilterNode& rightOperand);

    virtual               ~stringFilterXor ();

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
typedef SMARTP<stringFilterXor> S_stringFilterXor;
EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterXor& elt);

//______________________________________________________________________________
class EXP stringFilterAnd : public stringFilterDyadicOperator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringFilterAnd> create (
                            const S_stringFilterNode& leftOperand,
                            const S_stringFilterNode& rightOperand);

//   protected:
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          stringFilterAnd (
                            const S_stringFilterNode& leftOperand,
                            const S_stringFilterNode& rightOperand);

    virtual               ~stringFilterAnd ();

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
typedef SMARTP<stringFilterAnd> S_stringFilterAnd;
EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterAnd& elt);

//______________________________________________________________________________
class EXP stringFilterString : public stringFilterNode
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringFilterString> create (
                            const std::string& theString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          stringFilterString (
                            const std::string& theString);

    virtual               ~stringFilterString ();

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
typedef SMARTP<stringFilterString> S_stringFilterString;
EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterString& elt);

// //______________________________________________________________________________
// class EXP stringFilterSubExpression : public stringFilterFactor
// {
//   public:
//
//     // creation
//     // ------------------------------------------------------
//
//     static SMARTP<stringFilterSubExpression> create (
//                             S_stringFilterExpression expression);
//
//   protected:
//
//     // constructors/destructor
//     // ------------------------------------------------------
//
//                           stringFilterSubExpression (
//                             S_stringFilterExpression expression);
//
//     virtual               ~stringFilterSubExpression ();
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
//   S_stringFilterExpression      fExpression;
// };
// typedef SMARTP<stringFilterSubExpression> S_stringFilterSubExpression;
// EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterSubExpression& elt);

// // initialization
// //______________________________________________________________________________
// void initializestringFilterBasicTypes ();


#endif // ___stringFilterBasicTypes___
