\version "2.24.4"

\header {
  workCreditTypeTitle = "Two Tuplets In A Row"
  movementTitle       = "Two Tuplets In A Row"
  encodingDate        = "2025-02-10"
  composer            = "Lars P. Opfermann"
  software            = "Dorico 5.1.81.2225"
  right               = "©"
  title               = "Two Tuplets In A Row"
}

\paper {
  % horizontal-shift = 0.0\mm
  % indent = 0.0\mm
  % short-indent = 0.0\mm
  
  % markup-system-spacing-padding = 0.0\mm
  % between-system-space = 0.0\mm
  % page-top-space = 0.0\mm
  
  % page-count = -1
  % system-count = -1
  
  % oddHeaderMarkup = ""
  % evenHeaderMarkup = ""
  % oddFooterMarkup = ""
  % evenFooterMarkup = ""
}

\layout {
  \context { \Score
    autoBeaming = ##f % to display tuplets brackets
  }
  \context { \Voice
  }
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 3/4
  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  r8 \stemUp g'8. \f [
  _- a'16 ]
  _- } \once \omit TupletBracket
  \tuplet 3/2 {  a'8 [
  bis'!16 _. b'! _. cis''! _. d'' ]
  _. } r4
  \bar "|."
  
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Flöte"
          shortInstrumentName = "Fl."
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \Part_POne_Staff_One_Voice_One
          >>
        >>
      
      
      >>
    
    >>
    
    \layout {
      \context { \Score
        autoBeaming = ##f % to display tuplets brackets
      }
      \context { \Voice
      }
    }
    
    \midi {
      \tempo 16 = 360
    }
  }
  
}
