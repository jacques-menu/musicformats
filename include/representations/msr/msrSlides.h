#ifndef ___msrSlides___
#define ___msrSlides___

#include "msrElements.h"
#include "msrMeasures.h"


namespace MusicFormats
{

//______________________________________________________________________________
class   msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

//______________________________________________________________________________
// data types

enum class msrSlideTypeKind {
  kSlideTypeNone,
  kSlideTypeStart, kSlideTypeStop
};

string slideTypeKindAsString (
  msrSlideTypeKind slideTypeKind);

ostream& operator << (ostream& os, const msrSlideTypeKind& elt);

class EXP msrSlide : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSlide> create (
                            int              inputLineNumber,
                            int              slideNumber,
                            msrSlideTypeKind slideTypeKind,
                            msrLineTypeKind  slideLineTypeKind,
                            const string&    slideTextValue);

    SMARTP<msrSlide> createSlideNewbornClone ();

    SMARTP<msrSlide> createSlideDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSlide (
                            int              inputLineNumber,
                            int              slideNumber,
                            msrSlideTypeKind slideTypeKind,
                            msrLineTypeKind  slideLineTypeKind,
                            const string&    slideTextValue);

    virtual               ~msrSlide ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getSlideNumber () const
                              { return fSlideNumber; }

    msrSlideTypeKind      getSlideTypeKind () const
                              { return fSlideTypeKind; }

    msrLineTypeKind       getSlideLineTypeKind () const
                              { return fSlideLineTypeKind; }

    string                getSlideTextValue () const
                              { return fSlideTextValue; }

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

    int                   fSlideNumber;

    msrSlideTypeKind      fSlideTypeKind;

    msrLineTypeKind       fSlideLineTypeKind;

    string                fSlideTextValue;
};
typedef SMARTP<msrSlide> S_msrSlide;
EXP ostream& operator << (ostream& os, const S_msrSlide& elt);


}


#endif
