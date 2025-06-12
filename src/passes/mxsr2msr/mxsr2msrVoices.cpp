/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "mxsr2msrVoices.h"
#include "mxsr2msrWae.h"

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
      gLog << tuplet; // JMI 0.9.71
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
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Pushing new innermost tuplet " <<
      tuplet->asString () <<
      " onto the tuplets stack in voice " <<
      fMsrVoice->getVoiceName () <<
      ", context: " << context <<
      ", fTupletsStack.size (): " <<
      fTupletsStack.size () <<
      ", line " << tuplet->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  mfTupletNumber
    tupletNumber =
      tuplet->getTupletNumber ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check JMI 0.9.72
  if (fTupletsStack.empty ()) {
//     mfAssert (
//       __FILE__, mfInputLineNumber (__LINE__),
//       tupletNumber == 1,
//       "tupletNumber is not equal to 1");
  }

  else {
    mfTupletNumber
      innerMostTupletNumber =
        fTupletsStack.back ()->
          getTupletNumber ();

    if (tupletNumber != innerMostTupletNumber + 1) {
      std::stringstream ss;

      ss <<
        "tuplet number " <<
        tupletNumber <<
        " is not equal to innerMostTupletNumber " <<
        innerMostTupletNumber <<
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
//         __FILE__, mfInputLineNumber (__LINE__),
//         ss.str ());
    }
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // the tuplets starts are implicitly supposed to be in tuplet number order
  // sort them in to be sure? JMI ??? 0.9.72
  fTupletsStack.push_front (tuplet);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    displayTupletsStack (
      "############## pushTupletOntoTupletsStack(): after push_front()");
  }
#endif // MF_TRACE_IS_ENABLED
}

S_msrTuplet mxsrVoice::popInnerMostTuplet (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check JMI 0.9.70
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    ! fTupletsStack.empty (),
    "fTupletsStack is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // get current tuplet from the top of the tuplet stack
  S_msrTuplet
    currentInnerMostTuplet =
      fTupletsStack.front ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Popping current innermost tuplet from the stack " <<
      currentInnerMostTuplet->asString () <<
      " in voice " <<
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
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());

    displayTupletsStack (
      "############## popInnerMostTuplet(): before pop_front()");
  }
#endif // MF_TRACE_IS_ENABLED

  // pop from the tuplets stack
  fTupletsStack.pop_front (); // JMI 0.9.68

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    displayTupletsStack (
      "############## popInnerMostTuplet(): after pop_front()");
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about the current outermost tuplet and its first note // JMI 0.9.68 HARMFUL
//     fCurrentOuterMostTupletFirstNote = nullptr; // VITAL
  fCurrentOuterMostTuplet = nullptr;

  return currentInnerMostTuplet;
}

void mxsrVoice::appendNoteToInnerMostTuplet (
  const mfInputLineNumber& inputLineNumber,
  const S_msrNote&   note,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    gLog <<
      "Appending note " <<
    note->asString () <<
    " to tuplets stack top in voice \"" <<
    fMsrVoice->getVoiceName () <<
    "\"" <<
    ", context: " << context <<
    ", line " << inputLineNumber <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletsStack.front ()->
    appendNoteToTuplet (
      note);
}

void mxsrVoice::appendTupletToMsrVoice (
  const mfInputLineNumber& inputLineNumber,
  const S_msrTuplet& tuplet,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    gLog <<
      "Appending tuplet " <<
    tuplet->asString () <<
    " to MSR voice \"" <<
    fMsrVoice->getVoiceName () <<
    "\"" <<
    ", context: " << context <<
    ", line " << inputLineNumber <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  fMsrVoice->
    appendTupletToVoice (
      tuplet);
}

void mxsrVoice::appendTupletWhereItBelongs (
  const mfInputLineNumber& inputLineNumber,
  const S_msrTuplet& tuplet,
  const std::string& context)
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
      __FILE__, mfInputLineNumber (__LINE__),
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
    // no current on-going tuplet, tuplet is a top level tuplet
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      gLog <<
        "Appending top-level tuplet " <<
      tuplet->asString () <<
      " in voice \"" <<
      fMsrVoice->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

    // append tuplet to the MSR voice
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

    // append tuplet to the tuplets stack top, i.e. the current on-going tuplet
    fTupletsStack.front ()->
      appendTupletToTuplet (
        tuplet);
  } // switch

  // push tuplet upon the tuplets stack in all cases
  // since it becomes the mew current on-going tuplet
  pushTupletOntoTupletsStack (
    tuplet,
    "appendTupletWhereItBelongs()");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    displayTupletsStack (
      "############## appendTupletWhereItBelongs() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsrVoice::handleTupletBegin (
  const S_msrVoice& currentNoteVoice,
  S_msrTuplet       tuplet)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a tuplet begin with number " <<
      tuplet->getTupletNumber () <<
      " in voice" <<
      currentNoteVoice->getVoiceName () <<
      ", line " << tuplet->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle tuplet
  if (fetchTupletsStackIsEmpty ()) {
    // tuplet is an outermost tuplet
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      std::stringstream ss;

      ss <<
        "An outermost tuplet begins at line " <<
         tuplet->getInputLineNumber () <<
        " in voice " <<
        currentNoteVoice->getVoiceName ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentOuterMostTuplet = tuplet;
  }

  else {
    // tuplet is a nested tuplet
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      std::stringstream ss;

      ss <<
        "A nested tuplet begins at line " <<
         tuplet->getInputLineNumber () <<
        " in voice " <<
        currentNoteVoice->getVoiceName ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // register tuplet by pushing it onto the tuplet stack
//       pushTupletOntoTupletsStack (
//         tuplet,
//         "handleTupletBegin()");

//     fetchInnerMostTuplet () = tuplet;
  }

  // append endingTuplet where it belongs in fCurrentRecipientMxsrVoice->
  appendTupletWhereItBelongs (
    currentNoteVoice->getInputLineNumber (), // JMI 0.9.75
    tuplet,
    "handleTupletBegin()");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## handleTupletBegin()");
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsrVoice::handleTupletContinue (
  const S_msrNote&  note,
  const S_msrVoice& currentNoteVoice)
{
  if (fetchTupletsStackIsEmpty ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      std::stringstream ss;

      ss <<
        "Handling a tuplet continue of tuplet stack top:" <<
        std::endl;
      ++gIndenter;
      ss <<
        fetchInnerMostTuplet ()->asString ();
      --gIndenter;
      ss <<
        " in voice " <<
        currentNoteVoice->getVoiceName ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // populate fetchInnerMostTuplet ()
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      gLog <<
        "--> handleTupletContinue(): appending tuplet member note " <<
        note->asShortString () <<
        " to stack top tuplet " <<
        fetchInnerMostTuplet ()->asString () <<
        " in voice " <<
        currentNoteVoice->getVoiceName () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

    // append note to fetchInnerMostTuplet ()
    fetchInnerMostTuplet ()->
      appendNoteToTuplet (
        note);
  }

  else {
    std::stringstream ss;

    ss <<
      "handleTupletContinue():" <<
      std::endl <<
      "tuplet member note " <<
      note->
        asShortString () <<
      " cannot be handled, tuplets stack is empty" <<
      ", line " << note->getInputLineNumber ();

    mxsr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      note->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
}

void mxsrVoice::handleTupletEnd (
  const S_msrNote&  note,
  const S_msrVoice& currentNoteVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a tuplet end at line " <<
      note->getInputLineNumber () <<
      " in voice " <<
      currentNoteVoice->getVoiceName ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check JMI 0.9.70
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    ! fTupletsStack.empty (),
    "fTupletsStack is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrTuplet
    endingTuplet =
      popInnerMostTuplet (
        note->getInputLineNumber (),
        "handleTupletEnd()");

  // handle endingTuplet
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    gLog <<
      "End of tuplet:" <<
      std::endl;
    ++gIndenter;
    gLog <<
      endingTuplet <<
      std::endl;
    --gIndenter;

    gLog <<
      "at line " <<
      note->getInputLineNumber () <<
      " in voice " <<
      currentNoteVoice->getVoiceName () <<
      std::endl;

    gLog <<
      "note:" <<
      std::endl;
    ++gIndenter;
    gLog <<
      note->asShortString () <<
      std::endl;
    --gIndenter;

    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

//     // append endingTuplet where it belongs in fCurrentRecipientMxsrVoice->
//       appendTupletWhereItBelongs (
//         currentNoteVoice->getVoiceNumber (),
//         endingTuplet,
//         "handleTupletEnd()");

//   else {
//     std::stringstream ss;
//
//     ss <<
//       "handleTupletEnd(): tuplet member note " <<
//       note-> asShortString () <<
//       " cannot be handled, tuplets stack is empty" <<
//       ", line " << note->getInputLineNumber ();
//
//     mxsr2msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       note->getInputLineNumber (),
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## handleTupletEnd()");
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
    "fTupletsStack" << ": ";

  if (! fTupletsStack.empty ()) {
    os << std::endl;

    ++gIndenter;
    for (S_msrTuplet tuplet : fTupletsStack) {
      os <<
        tuplet <<
        std::endl;
    } // for
    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

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

