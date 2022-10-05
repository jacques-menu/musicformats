/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___brailleGenerationOah___
#define ___brailleGenerationOah___


#include "oahBasicTypes.h"
#include "oahAtomsCollection.h"

#include "bsrBasicTypes.h"

#include "brailleGeneration.h"


namespace MusicFormats
{

//______________________________________________________________________________
// constants
EXP extern const string K_BRAILLE_USE_ENCODING_IN_FILE_NAME_LONG_NAME;
EXP extern const string K_BRAILLE_USE_ENCODING_IN_FILE_NAME_SHORT_NAME;

EXP extern const string K_BRAILLE_OUTPUT_KIND_LONG_NAME;
EXP extern const string K_BRAILLE_OUTPUT_KIND_SHORT_NAME;

EXP extern const string K_BRAILLE_UTF_KIND_LONG_NAME;
EXP extern const string K_BRAILLE_UTF_KIND_SHORT_NAME;

EXP extern const string K_BRAILLE_BYTE_ORDERING_KIND_LONG_NAME;
EXP extern const string K_BRAILLE_BYTE_ORDERING_KIND_SHORT_NAME;

//______________________________________________________________________________
class EXP brailleOutputKindAtom : public oahAtomImplicitlyStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<brailleOutputKindAtom> create (
                            const string&         longName,
                            const string&         shortName,
                            const string&         description,
                            const string&         variableName,
                            bsrBrailleOutputKind& brailleOutputKindVariable,
                            bsrBrailleOutputKind  brailleOutputKindValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          brailleOutputKindAtom (
                            const string&         longName,
                            const string&         shortName,
                            const string&         description,
                            const string&         variableName,
                            bsrBrailleOutputKind& brailleOutputKindVariable,
                            bsrBrailleOutputKind  brailleOutputKindValue);

    virtual               ~brailleOutputKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    bsrBrailleOutputKind& fBrailleOutputKindVariable;
    bsrBrailleOutputKind  fBrailleOutputKindValue;
};
typedef SMARTP<brailleOutputKindAtom> S_brailleOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_brailleOutputKindAtom& elt);

//______________________________________________________________________________
class EXP brailleUTFKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<brailleUTFKindAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            bsrUTFKind&   brailleUTFKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          brailleUTFKindAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            bsrUTFKind&   brailleUTFKindVariable);

    virtual               ~brailleUTFKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    bsrUTFKind&           fBsrUTFKindVariable;
};
typedef SMARTP<brailleUTFKindAtom> S_brailleUTFKindAtom;
EXP ostream& operator<< (ostream& os, const S_brailleUTFKindAtom& elt);

//______________________________________________________________________________
class EXP brailleByteOrderingKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<brailleByteOrderingKindAtom> create (
                            const string&        longName,
                            const string&        shortName,
                            const string&        description,
                            const string&        valueSpecification,
                            const string&        variableName,
                            bsrByteOrderingKind& brailleByteOrderingKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          brailleByteOrderingKindAtom (
                            const string&        longName,
                            const string&        shortName,
                            const string&        description,
                            const string&        valueSpecification,
                            const string&        variableName,
                            bsrByteOrderingKind& brailleByteOrderingKindVariable);

    virtual               ~brailleByteOrderingKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    bsrByteOrderingKind&  fBsrByteOrderingKindVariable;
};
typedef SMARTP<brailleByteOrderingKindAtom> S_brailleByteOrderingKindAtom;
EXP ostream& operator<< (ostream& os, const S_brailleByteOrderingKindAtom& elt);

//______________________________________________________________________________
class EXP brailleGenerationOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<brailleGenerationOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeBrailleGenerationOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          brailleGenerationOahGroup ();

    virtual               ~brailleGenerationOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    // fields
    // ------------------------------------------------------

    // braille output kind

    void                  setBrailleOutputKind (bsrBrailleOutputKind value)
                              { fBrailleOutputKind = value; }
    bsrBrailleOutputKind  getBrailleOutputKind () const
                              { return fBrailleOutputKind;  }

    // UTF encoding

    void                  setUTFKind (bsrUTFKind value)
                              { fBrailleUTFKind = value; }
    bsrUTFKind            getUTFKind () const
                              { return fBrailleUTFKind;  }

    // byte ordering

    void                  setByteOrderingKind (bsrByteOrderingKind value)
                              { fByteOrderingKind = value; }
    bsrByteOrderingKind   getByteOrderingKind () const
                              { return fByteOrderingKind;  }

    // Braille file name

    void                  setUseEncodingInFileName ()
                              { fUseEncodingInFileName = true; }
    Bool                  getUseEncodingInFileName () const
                              { return fUseEncodingInFileName;  }

    // page parameters

    void                  setCellsPerLine (int value)
                              { fCellsPerLine = value; }
    int                   getCellsPerLine () const
                              { return fCellsPerLine;  }

    void                  setMeasuresPerLine (int value)
                              { fMeasuresPerLine = value; }
    int                   getMeasuresPerLine () const
                              { return fMeasuresPerLine;  }

    void                  setLinesPerPage (int value)
                              { fLinesPerPage = value; }
    int                   getLinesPerPage () const
                              { return fLinesPerPage;  }

    // code generation

    void                  setXml2brlInfos ()
                              { fXml2brlInfos = true; }
    Bool                  getXml2brlInfos () const
                              { return fXml2brlInfos;  }

    void                  setNoBrailleCode ()
                              { fNoBrailleCode = true; }
    Bool                  getNoBrailleCode () const
                              { return fNoBrailleCode;  }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
// JMI    void                  initializeBsr2brailletracingOah ();
#endif

    void                  initializeBrailleFileNameOptions ();

    void                  initializeBrailleOutputOptions ();

    void                  initializePageParametersOptions ();

    void                  initializeCodeGenerationOptions ();

    void                  initializeMacroOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printBrailleGenerationOahHelp ();

    void                  printBrailleGenerationOahValues (int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // braille file name

    Bool                  fUseEncodingInFileName;
    S_oahBooleanAtom      fUseEncodingInFileNameAtom;

    // braille output

    bsrBrailleOutputKind  fBrailleOutputKind;
    S_brailleOutputKindAtom
                          fBrailleOutputKindAtom;

    bsrUTFKind            fBrailleUTFKind;
    S_brailleUTFKindAtom  fBrailleUTFKindAtom;

    bsrByteOrderingKind   fByteOrderingKind;
    S_brailleByteOrderingKindAtom
                          fByteOrderingKindAtom;

    // page parameters

    int                   fCellsPerLine;
    int                   fMeasuresPerLine;
    int                   fLinesPerPage;

    // code generation

    Bool                  fXml2brlInfos;

    Bool                  fNoBrailleCode;
};
typedef SMARTP<brailleGenerationOahGroup> S_brailleGenerationOahGroup;
EXP ostream& operator<< (ostream& os, const S_brailleGenerationOahGroup& elt);

EXP extern S_brailleGenerationOahGroup gGlobalBrailleGenerationOahGroup;

//______________________________________________________________________________
EXP S_brailleGenerationOahGroup createGlobalBrailleGenerationOahGroup ();


}


#endif
