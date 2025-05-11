\version "2.24.4"

\header {
  workCreditTypeTitle = "TwoChordsInATuplet"
  movementTitle       = "Two Chords In A Tuplet"
  encodingDate        = "2025-01-30"
  software            = "Dorico 5.1.81.2225"
  title               = "TwoChordsInATuplet"
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
  \time 4/4
  \stemDown  < g'' c'' > 4.  < f'' b' >   < g'' c'' >   < f'' b' >  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {   < g'' c'' > 4.  < f'' b' >  }
  \bar "|."
   | % 2
  \barNumberCheck #2
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Part_POne"
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
