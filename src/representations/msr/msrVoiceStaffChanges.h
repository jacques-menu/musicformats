#ifndef ___msrStaffChanges___
#define ___msrStaffChanges___

#include "msrStaves.h"


namespace MusicFormats
{

//______________________________________________________________________________
class msrPartGroup;
typedef SMARTP<msrPartGroup> S_msrPartGroup;

class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

class msrRepeat;
typedef SMARTP<msrRepeat> S_msrRepeat;

class msrRepeatEnding;
typedef SMARTP<msrRepeatEnding> S_msrRepeatEnding;

//______________________________________________________________________________
class EXP msrVoiceStaffChange : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVoiceStaffChange> create (
                            int        inputLineNumber,
                            S_msrStaff staffToChangeTo);

    SMARTP<msrVoiceStaffChange> createStaffChangeNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrVoiceStaffChange (
                            int        inputLineNumber,
                            S_msrStaff staffToChangeTo);

    virtual               ~msrVoiceStaffChange ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaff            getStaffToChangeTo () const
                              { return fStaffToChangeTo; }

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

    S_msrStaff            fStaffToChangeTo;
};
typedef SMARTP<msrVoiceStaffChange> S_msrVoiceStaffChange;
EXP ostream& operator<< (ostream& os, const S_msrVoiceStaffChange& elt);


}


#endif
