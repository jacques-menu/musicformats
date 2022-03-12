/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfBool___
#define ___mfBool___

#include <string>
#include <ostream>

#include "exports.h"

using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
/*
  This class to ensure all values are initialized (to false as it is) JMI
*/
/*
Matrix::Matrix(const Matrix& rMatrix) :
    _iRows(rMatrix._iRows), _iColumns(rMatrix._iColumns), _pVector(0)
{
    _pVector = new Vector[_iRows];
    for (int i = 0; i < _iRows; i++) { _pVector[i] = rMatrix._pVector[i]; }
}

Matrix& Matrix::operator=(const Matrix& rMatrix)
{
    if (this != &rMatrix)
    {
        if (0 != _pVector) { delete[] _pVector; pVector = 0; }
        _iRows = rMatrix._iRows;
        _iColumns = rMatrix._iColumns;
        _pVector = new Vector[_iRows];
        for (int i = 0; i < _iRows; i++) { _pVector[i] = rMatrix._pVector[i]; }
    }
    return *this;
}
Matrix& Matrix::operator+=(const Matrix& rMatrix)
{
    *this = *this + rMatrix;
    return *this;
}

Matrix Matrix::operator+(const Matrix& rMatrix) const
{
    Matrix matrix(_iRows, _iColumns);
    ValidateSizes(rMatrix);
    for (int i = 0; i < _iRows; i++) { matrix._pVector[i] = _pVector[i] + rMatrix._pVector[i]; }
    return matrix;
}

Matrix operator+(const Matrix& rMatrix, double dNum)
{
    Matrix matrix(rMatrix._iRows, rMatrix._iColumns);
    matrix.ValidateSizes(rMatrix);
    for (int i = 0; i < matrix._iRows; i++) { matrix._pVector[i] = dNum + rMatrix._pVector[i]; }
    return matrix;
}

Matrix operator+(double dNum, const Matrix& rMatrix)
{
    return operator+(rMatrix, dNum);
}

bool Matrix::ValidateSizes(const Matrix& rMatrix) const
{
    if (_iRows != rMatrix._iRows) { / * THROW EXCEPTION * / }
    if (_iColumns != rMatrix._iColumns) { / * THROW EXCEPTION * / }
    return true;
}
*/

//______________________________________________________________________________
class EXP Bool
/*
  this class encapsulates a 'bool' value to guarantee it is initialized
*/
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          Bool ();

                          Bool (bool value);

                          Bool (const string& theString);

    virtual               ~Bool ();

    // copy constructor
    // ------------------------------------------------------

                          Bool (const Bool& obj);

public:

    // set and get
    // ------------------------------------------------------

    // set the value
    void                  setValue (bool value)
                              { fValue = value; }

    // get the value
    bool                  getValue () const
                              { return fValue; }

  public:

    // public services
    // ------------------------------------------------------

    // explicit conversion
    explicit              operator bool () const { return fValue; }

    // copy assignment operator
    // ------------------------------------------------------

    Bool&                 operator= (const Bool& theBool);

/*
    // Declare prefix and postfix decrement operators.
    Point& operator--();       // Prefix decrement operator.
    Point operator--(int);     // Postfix decrement operator.
*/

    // negation, prefix operator
    Bool                  operator! () const;

    // dyadic operators
    Bool                  operator&& (const Bool& otherBool) const;
    Bool                  operator|| (const Bool& otherBool) const;

    // compare value JMI ???
    Bool                  operator == (const Bool &otherBool) const
                              { return fValue == otherBool.fValue; }
    Bool                  operator != (const Bool &otherBool) const
                              { return fValue != otherBool.fValue; }

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    bool                  fValue;
};

//EXP Bool operator&& (const Bool& leftBool, const Bool& rightBool);
EXP Bool operator&& (const Bool& leftBool, const bool& theBool);
EXP Bool operator&& (const bool& theBool, const Bool& rightBool);

//EXP Bool operator|| (const Bool& leftBool, const Bool& rightBool);
EXP Bool operator|| (const Bool& leftBool, const bool& theBool);
EXP Bool operator|| (const bool& theBool, const Bool& rightBool);

EXP ostream& operator<< (ostream& os, const Bool& theBool);

EXP Bool nand (Bool p, Bool q);
EXP Bool nor (Bool p, Bool q);
EXP Bool xorr (Bool p, Bool q);

EXP Bool implies (Bool p, Bool q);

EXP void testBool ();


}


#endif
