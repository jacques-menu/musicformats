#ifndef ___bsrTimeSignatures___
#define ___bsrTimeSignatures___

#include "bsrElements.h"

#include "mfBool.h"

#include "bsrLineContentsElements.h"
#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations

class   bsrTimeSignatureItem;
typedef SMARTP<bsrTimeSignatureItem> S_bsrTimeSignatureItem;

// PRE-declarations

class   bsrTimeSignature;
typedef SMARTP<bsrTimeSignature> S_bsrTimeSignature;

//______________________________________________________________________________
class EXP bsrTimeSignatureItem : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTimeSignatureItem> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrTimeSignatureItem (
                            int inputLineNumber);

    virtual               ~bsrTimeSignatureItem ();

  public:

    // set and get
    // ------------------------------------------------------

    const vector<int>&    getTimeSignatureBeatsNumbersVector ()
                              { return fTimeSignatureBeatsNumbersVector; }

    void                  setTimeSignatureBeatValue (int timeSignatureBeatValue);

    int                   getTimeSignatureBeatValue () const
                              { return fTimeSignatureBeatValue; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (S_bsrTimeSignatureItem otherTimeSignatureItem) const;

    void                  appendBeatsNumber (int beatsNumber);

    int                   getTimeSignatureBeatsNumber () const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    vector<int>           fTimeSignatureBeatsNumbersVector; // 5+3+1 is possible
    int                   fTimeSignatureBeatValue;
};
typedef SMARTP<bsrTimeSignatureItem> S_bsrTimeSignatureItem;
EXP ostream& operator << (ostream& os, const S_bsrTimeSignatureItem& elt);

//______________________________________________________________________________
// data types

enum class bsrTimeSignatureKind {
  kTimeSignatureNone,
  kTimeSignatureCommon,
  kTimeSignatureCut,
  kTimeSignatureNumerical, // JMI
  kTimeSignatureNote,
  kTimeSignatureDottedNote,
  kTimeSignatureSingleNumber,
  kTimeSignatureSenzaMisura
};

string bsrTimeSignatureKindAsString (
  bsrTimeSignatureKind timeKind);

string bsrTimeKindAsDebugString (
  bsrTimeSignatureKind timeKind);

ostream& operator << (ostream& os, const bsrTimeSignatureKind& elt);

class EXP bsrTimeSignature : public bsrLineContentsElement
{
  public:

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTimeSignature> create (
                            int         inputLineNumber,
                            bsrTimeSignatureKind timeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrTimeSignature (
                            int         inputLineNumber,
                            bsrTimeSignatureKind timeKind);

    virtual               ~bsrTimeSignature ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrTimeSignatureKind           getTimeKind () const
                              { return fTimeKind; }

    const vector<S_bsrTimeSignatureItem>&
                          getTimeSignatureItemsVector () const
                              { return fTimeSignatureItemsVector; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendTimeSignatureItem (S_bsrTimeSignatureItem timeSignatureItem);

    S_bsrCellsList        fetchCellsList () const override
                              { return buildCellsList (); }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildCellsList () const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    string                asDebugString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    bsrTimeSignatureKind           fTimeKind;

    vector<S_bsrTimeSignatureItem> fTimeSignatureItemsVector;
};
typedef SMARTP<bsrTimeSignature> S_bsrTimeSignature;
EXP ostream& operator << (ostream& os, const S_bsrTimeSignature& elt);


}


#endif
