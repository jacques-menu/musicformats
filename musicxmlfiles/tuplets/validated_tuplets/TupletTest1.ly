\version "2.24.4"

\header {
  movementTitle       = "TupletTest1"
  title               = "TupletTest1"
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
  \key e \minor
  \numericTimeSignature \time 2/4
  \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  \stemDown fis''8 [
  ( e'' ) c'' ]
  } \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  fis''8 [
  ( e'' ) c'' ]
  }
  
  \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  fis''8 [
  ( e'' ) c'' ]
  } \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  fis''8 [
  ( e'' ) c'' ]
  }
  \bar "|."
  
}

\book {

  \score {
    <<
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Piano"
          shortInstrumentName = "Pno."
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
