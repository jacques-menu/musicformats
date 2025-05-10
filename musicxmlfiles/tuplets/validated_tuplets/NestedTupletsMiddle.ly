\version "2.24.4"

\header {
  workCreditTypeTitle = "Nested Tuplets Middle"
  movementTitle       = "Bloc 1"
  encodingDate        = "2024-11-14"
  software            = "Dorico 5.1.60.2187"
  title               = "Nested Tuplets Middle"
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
  \numericTimeSignature \time 4/4
  \stemUp g'4
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 5/4 {  a'8 [
  b' ]
  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  \stemDown c''8 [
  d'' e'' ]
  } f'' } g''4
  \bar "|."
  
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Voice 1"
          shortInstrumentName = "V. 1"
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
