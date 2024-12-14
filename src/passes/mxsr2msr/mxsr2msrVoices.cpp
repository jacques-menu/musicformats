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
  const S_msrVoice&  voice)
{
  mxsrVoice* obj = new
    mxsrVoice (
      voice);
  assert (obj != nullptr);
  return obj;
}

mxsrVoice::mxsrVoice (
  const S_msrVoice&  voice)
{
  fMsrVoice = voice;
}

mxsrVoice::~mxsrVoice ()
{}

void mxsrVoice::displayTupletsStack (
  const std::string& context)
{
  size_t tupletsStackSize = fTupletsStack.size ();

  gLog <<
    std::endl <<
    ">>++++++++++++++ The tuplets stack contains " <<
    mfSingularOrPlural (
      tupletsStackSize, "element", "elements") <<
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
      gLog << tuplet->asString (); // JMI v0.9.71
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

// void mxsrVoice::displayLastHandledTupletInVoiceMap (
//   const std::string& header)
// {
//   gLog <<
//     std::endl <<
//     header <<
//     ", fLastHandledTupletInVoiceMap contains:";
//
//   if (! fLastHandledTupletInVoiceMap.size ()) {
//     gLog <<
//       " none" <<
//       std::endl;
//   }
//
//   else {
//     std::map <std::pair <int, int>, S_msrTuplet>::const_iterator
//       iBegin = fLastHandledTupletInVoiceMap.begin (),
//       iEnd   = fLastHandledTupletInVoiceMap.end (),
//       i      = iBegin;
//
//     gLog << std::endl;
//
//     ++gIndenter;
//
//     for ( ; ; ) {
//       gLog <<
//         "staff " << (*i).first.first <<
//         ", voice " <<  (*i).first.second <<
//         std::endl;
// //        "\"" << (*i).first->getVoiceName () <<
// //        "\" ----> " << (*i).second->asString ();
//       if (++i == iEnd) break;
//       gLog << std::endl;
//     } // for
//
//     gLog << std::endl;
//
//     --gIndenter;
//   }
//
//   gLog << std::endl;
// }

void mxsrVoice::finalizeTupletStackTopAndPopItFromTupletsStack (
  int         inputLineNumber,
  std::string context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Finalizing tuplet stack top and popping it from tuplet stack" <<
      ", fTupletsStack.size (): " << fTupletsStack.size ();

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
  if (gTraceOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## Before finalizeTupletStackTopAndPopItFromTupletsStack() 2");
  }
//   if (gTraceOahGroup->getTraceTuplets ()) {
//     displayVoicesTupletsStacksMap (
//       "############## Before finalizeTupletStackTopAndPopItFromTupletsStack() 3");
//   }
#endif // MF_TRACE_IS_ENABLED

  // fetch the current voice
  S_msrVoice
    currentNoteVoice =
      fMsrVoice; // JMI v0.9.70

  // get tuplet from top of tuplet stack
  S_msrTuplet
    tupletStackFront =
      fTupletsStack.front ();

  // pop from the tuplets stack
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Popping tupletStackFront " <<
      tupletStackFront->asString () <<
      " for voice " <<
      fMsrVoice->getVoiceName () <<
      ", context: " << context <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## After finalizeTupletStackTopAndPopItFromTupletsStack() 4");
  }
//   if (gTraceOahGroup->getTraceTupletsDetails ()) {
//     displayVoicesTupletsStacksMap (
//       "############## After finalizeTupletStackTopAndPopItFromTupletsStack() 5");
//   }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check JMI v0.9.70
  mfAssert (
    __FILE__, __LINE__,
    fTupletsStack.size () > 0,
    "fTupletsStack is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fTupletsStack.pop_front (); // JMI v0.9.68

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## After finalizeTupletStackTopAndPopItFromTupletsStack() 6");
  }
//   if (gTraceOahGroup->getTraceTupletsDetails ()) {
//     displayVoicesTupletsStacksMap (
//       "############## After finalizeTupletStackTopAndPopItFromTupletsStack() 7");
//   }
#endif // MF_TRACE_IS_ENABLED

//   abort ();

  switch (fTupletsStack.size ()) {
    case 0:
      // tupletStackFront is a top level tuplet
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceTupletsBasics ()) {
        gLog <<
          "Appending top level tuplet " <<
        tupletStackFront->asString () <<
        " to voice \"" <<
        currentNoteVoice->getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        std::endl;
      }
#endif // MF_TRACE_IS_ENABLED
    break;

    default:
      // tupletStackFront is a nested tuplet
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceTupletsBasics ()) {
        gLog <<
          "=== appending nested tuplet " <<
        tupletStackFront <<
          " to current stack top tuplet " <<
        fTupletsStack.front ()->asString () <<
        ", line " << inputLineNumber <<
        std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      fTupletsStack.front ()->
        appendTupletToTuplet (tupletStackFront);
  } // switch

    // forget about the current outermost tuplet and its first note // JMI v0.9.68 HARMFUL
//     fCurrentOuterMostTupletFirstNote = nullptr;
//     fCurrentOuterMostTuplet = nullptr;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## After finalizeTupletStackTopAndPopItFromTupletsStack() 8");
  }
//   if (gTraceOahGroup->getTraceTupletsDetails ()) {
//     displayVoicesTupletsStacksMap (
//       "############## After finalizeTupletStackTopAndPopItFromTupletsStack() 9");
//   }
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

  const int fieldWidth = 33;

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

