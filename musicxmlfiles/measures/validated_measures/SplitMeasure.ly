\version "2.24.4"

\header {
  workCreditTypeTitle = "CHACONNE DES SCARAMOUCHES"
  composer            = ""
  poet                = "CA"
  software            = "Neuratron PhotoScore"
  right               = ""
  title               = "CHACONNE DES SCARAMOUCHES"
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
  \numericTimeSignature \time 3/4
  \stemUp c'2. d'2 \bar "||"
  \stemDown e'8 [
  f' ]
  g'2.  | % 4
  \barNumberCheck #4
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
