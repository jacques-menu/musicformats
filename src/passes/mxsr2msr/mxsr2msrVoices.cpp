/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfAssert.h"
#include "mfStringsHandling.h"

#include "mxsr2msrVoices.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_mxsrVoice mxsrVoice::create (
  const S_msrVoice& voice)
{
  mxsrVoice* obj = new
    mxsrVoice (
      voice);
  assert (obj != nullptr);
  return obj;
}

mxsrVoice::mxsrVoice (
  const S_msrVoice& voice)
{
  fMsrVoice = voice;
}

mxsrVoice::~mxsrVoice ()
{}

void mxsrVoice::displayTupletsStack (
  const std::string& context) const
{
  size_t tupletsStackSize = fTupletsStack.size ();

  gLog <<
    std::endl <<
    ">>++++++++++++++ The tuplets stack of voice " <<
    fMsrVoice->getVoiceName () <<
    " contains " <<
    mfSingularOrPlural (
      tupletsStackSize, "element", "elements") <<
    " - context: " << context <<
    ':' <<
    std::endl;

  if (tupletsStackSize) {
    std::list <S_msrTuplet>::const_iterator
      iBegin = fTupletsStack.begin (),
      iEnd   = fTupletsStack.end (),
      i      = iBegin;

    S_msrTuplet tuplet = (*i);

    ++gIndenter;

    int n = tupletsStackSize;
    for ( ; ; ) {
      gLog <<
        "v (" << n << ")" <<
        std::endl;

      ++gIndenter;
      gLog << tuplet; // JMI v0.9.71
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLog << std::endl;
    } // for

    --gIndenter;
  }

  gLog <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;
}

void mxsrVoice::pushTupletOntoTupletsStack (
  const S_msrTuplet& tuplet,
  std::string        context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Pushing tuplet " <<
      tuplet->asString () <<
      " onto the tuplets stack in voice " <<
      fMsrVoice->getVoiceName () <<
      ", context: " << context <<
      ", fTupletsStack.size (): " <<
      fTupletsStack.size () <<
      ", line " << tuplet->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int
    tupletNumber =
      tuplet->getTupletNumber ();


#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check JMI v0.9.72
  if (fTupletsStack.empty ()) {
//     mfAssert (
//       __FILE__, __LINE__,
//       tupletNumber == 1,
//       "tupletNumber is not equal to 1");
  }

  else {
    int
      tupletsStackTopStackNumber =
        fTupletsStack.back ()->
          getTupletNumber ();

    if (tupletNumber != tupletsStackTopStackNumber + 1) {
      std::stringstream ss;

      ss <<
        "tuplet number " <<
        tupletNumber <<
        " is not equal to tupletsStackTopStackNumber " <<
        tupletsStackTopStackNumber <<
        " plus 1";

gLog << std::endl << ss.str () << std::endl;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceTupletsBasics ()) {
        displayTupletsStack (
          "############## pushTupletOntoTupletsStack(): tuplet number problem");
      }
#endif // MF_TRACE_IS_ENABLED

      gLog << std::endl << ss.str () << std::endl << std::endl;

//       mfAssertFalse (
//         __FILE__, __LINE__,
//         ss.str ());
    }
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fTupletsStack.push_front (tuplet);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    displayTupletsStack (
      "############## pushTupletOntoTupletsStack(): after push_front()");
  }
#endif // MF_TRACE_IS_ENABLED
}

S_msrTuplet mxsrVoice::popTupletStackTop (
  int         inputLineNumber,
  std::string context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Popping tuplet stack top " <<
      fMsrVoice->getVoiceName () <<
      ", context: " << context <<
      ", fTupletsStack.size (): " <<
      fTupletsStack.size () <<
      ", line " << inputLineNumber;

    ss <<
      ", fCurrentOuterMostTuplet: ";
    if (fCurrentOuterMostTuplet) {
      ss <<
        fCurrentOuterMostTuplet->asString ();
    }
    else {
      ss << "[NULL]";
    }

    ss <<
      ", context: " << context <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // get current tuplet from the top of the tuplet stack
  S_msrTuplet
    currentTupletStackTop =
      fTupletsStack.front ();

  // pop from the tuplets stack
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Popping currentTupletStackTop " <<
      currentTupletStackTop->asString () <<
      " in voice " <<
      fMsrVoice->getVoiceName () <<
      ", context: " << context <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check JMI v0.9.70
  mfAssert (
    __FILE__, __LINE__,
    ! fTupletsStack.empty (),
    "fTupletsStack is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fTupletsStack.pop_front (); // JMI v0.9.68

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    displayTupletsStack (
      "############## popTupletStackTop(): after pop_front()");
  }
#endif // MF_TRACE_IS_ENABLED

    // forget about the current outermost tuplet and its first note // JMI v0.9.68 HARMFUL
//     fCurrentOuterMostTupletFirstNote = nullptr;
//     fCurrentOuterMostTuplet = nullptr;

  return currentTupletStackTop;
}

void mxsrVoice::appendTupletWhereItBelongs (
  int         inputLineNumber,
  S_msrTuplet tuplet,
  std::string context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending tuplet " <<
      tuplet->asString () <<
      " where it belongs in voice " <<
      fMsrVoice->getVoiceName () <<
      ", context: " << context <<
      ", fTupletsStack.size (): " <<
      fTupletsStack.size () <<
      ", line " << inputLineNumber;

    ss <<
      ", fCurrentOuterMostTuplet: ";
    if (fCurrentOuterMostTuplet) {
      ss <<
        fCurrentOuterMostTuplet->asString ();
    }
    else {
      ss << "[NULL]";
    }

    ss <<
      ", context: " << context <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    displayTupletsStack (
      "############## appendTupletWhereItBelongs() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  // analyze the new tuplets stack size
  if (fTupletsStack.empty ()) {
    // no open tuplet left, tuplet is a top level tuplet
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      gLog <<
        "Appending tuplet " <<
      tuplet->asString () <<
      " at the top level of voice \"" <<
      fMsrVoice->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

    fMsrVoice->
      appendTupletToVoice (
        tuplet);
  }

  else {
      // tuplet is nested in the tuplet stack top
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceTupletsBasics ()) {
        gLog <<
          "Appending tuplet " <<
        tuplet <<
          " as nested in current stack top tuplet " <<
        fTupletsStack.front ()->asString () <<
        " in voice \"" <<
        fMsrVoice->getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      fTupletsStack.front ()->
        appendTupletToTuplet (
          tuplet);
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    displayTupletsStack (
      "############## appendTupletWhereItBelongs() 3");
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string mxsrVoice::asString () const
{
  std::stringstream ss;

  ss <<
		"[mxsrVoice" <<
    ", fMsrVoice: " << fMsrVoice->getVoiceName () <<
    ']' <<
    std::endl;

  return ss.str ();
}

void mxsrVoice::print (std::ostream& os) const
{
	os <<
		"[mxsrVoice" <<
		std::endl;

	++gIndenter;

  constexpr int fieldWidth = 33;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMsrVoice" << ": " <<
    fMsrVoice->getVoiceName () <<
    std::endl << std::endl <<

//     std::setw (fieldWidth) <<
//     "fLastMetNoteInVoice" << ": " <<
//     fLastMetNoteInVoice <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "fCurrentOuterMostTupletFirstNote" << ": " <<
    fCurrentOuterMostTupletFirstNote <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fCurrentOuterMostTuplet" << ": " <<
    fCurrentOuterMostTuplet <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fCurrentOuterMostTuplet" << ": " <<
    fCurrentOuterMostTuplet <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletsStack" << ": " <<
    std::endl;

  ++gIndenter;
  for (S_msrTuplet tuplet : fTupletsStack) {
    os <<
      tuplet <<
      std::endl;
  } // for
  --gIndenter;

  --gIndenter;

	os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const mxsrVoice& elt)
{
  elt.print (os);
  return os;
}

std::ostream& operator << (std::ostream& os, const S_mxsrVoice& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


} // namespace

