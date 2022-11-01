#ifndef ___msrGlissandos___
#define ___msrGlissandos___

#include "msrElements.h"
#include "msrMeasures.h"


namespace MusicFormats
{

//______________________________________________________________________________
class msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

//______________________________________________________________________________
class EXP msrGlissando : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrGlissandoTypeKind {
      kGlissandoTypeNone,
      kGlissandoTypeStart, kGlissandoTypeStop
    };

    static string glissandoTypeKindAsString (
      msrGlissandoTypeKind glissandoTypeKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGlissando> create (
                            int                  inputLineNumber,
                            int                  glissandoNumber,
                            msrGlissandoTypeKind glissandoTypeKind,
                            msrLineTypeKind      glissandoLineTypeKind,
                            const string&        glissandoTextValue);

    SMARTP<msrGlissando> createGlissandoNewbornClone ();

    SMARTP<msrGlissando> createGlissandoDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrGlissando (
                            int                  inputLineNumber,
                            int                  glissandoNumber,
                            msrGlissandoTypeKind glissandoTypeKind,
                            msrLineTypeKind      glissandoLineTypeKind,
                            const string&        glissandoTextValue);

    virtual               ~msrGlissando ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getGlissandoNumber () const
                              { return fGlissandoNumber; }

    msrGlissandoTypeKind  getGlissandoTypeKind () const
                              { return fGlissandoTypeKind; }

    msrLineTypeKind       getGlissandoLineTypeKind () const
                              { return fGlissandoLineTypeKind; }

    string                getGlissandoTextValue () const
                              { return fGlissandoTextValue; }

  public:

    // public services
    // ------------------------------------------------------

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

    int                   fGlissandoNumber;

    msrGlissandoTypeKind  fGlissandoTypeKind;

    msrLineTypeKind       fGlissandoLineTypeKind;

    string                fGlissandoTextValue;
};
typedef SMARTP<msrGlissando> S_msrGlissando;
EXP ostream& operator << (ostream& os, const S_msrGlissando& elt);


}


#endif
