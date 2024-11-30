//______________________________________________________________________________
template <typename T, std::string printPrefix>
class EXP mfValueWrapper
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfValueWrapper (T value);

    virtual               ~mfValueWrapper ();

  public:

    // set and get
    // ------------------------------------------------------

    T                     getValue () const
                              { return fValue; }

    std::string           getPrintPrefix () const
                              { return fPrintPrefix; }

  public:

    // operators
    // ------------------------------------------------------

    explicit operator     T () const;

    Bool                  operator == (
                            const mfValueWrapper& otherValueWrapper) const;

  private:

    T                     fValue;
    std::string           fPrintPrefix;
};

template <typename T, std::string printPrefix>
EXP std::ostream& operator << (std::ostream& os, const mfValueWrapper& elt);

template <typename T, std::string printPrefix>
mfValueWrapper::mfValueWrapper (T value)
{
  fValue = value;
  fPrintPrefix = printPrefix;
}

template <typename T, std::string printPrefix>
mfValueWrapper::~mfValueWrapper ()
{}

template <typename T, std::string printPrefix>
mfValueWrapper::operator T () const
{
  return fValue;
}

template <typename T, std::string printPrefix>
Bool mfValueWrapper::operator == (
  const mfValueWrapper& otherInputLineNumber) const
{
  return fValue == otherInputLineNumber.fValue;
}

template <typename T, std::string printPrefix>
EXP std::ostream& operator << (std::ostream& os, const mfValueWrapper& elt)
{
  os << elt.getValue ();
  return os;
}

