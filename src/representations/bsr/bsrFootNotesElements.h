#ifndef ___bsrFootNotesElements___
#define ___bsrFootNotesElements___

#include "bsrElements.h"

#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrFootNotesElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrFootNotesElement> create (
                            int           inputLineNumber,
                            const std::string& footNoteText);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrFootNotesElement (
                            int           inputLineNumber,
                            const std::string& footNoteText);

    virtual               ~bsrFootNotesElement ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getFootNoteText () const
                              { return fFootNoteText; }

    S_bsrCellsList        getFootNotesElementCellsList () const
                              { return fFootNotesElementCellsList ;}

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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fFootNoteText;

    S_bsrCellsList        fFootNotesElementCellsList; // UNUSED JMI v0.9.66
};
typedef SMARTP<bsrFootNotesElement> S_bsrFootNotesElement;
EXP std::ostream& operator << (std::ostream& os, const S_bsrFootNotesElement& elt);


}


#endif
