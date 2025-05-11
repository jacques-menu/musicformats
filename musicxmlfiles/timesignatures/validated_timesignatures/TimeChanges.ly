\version "2.24.4"

\header {
  encodingDate        = "2017-02-20"
  software            = "MuseScore 2.0.3.1"
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
  R2. R2.  | % 3
  \barNumberCheck #3
  R2.  | % 4
  \barNumberCheck #4
  \numericTimeSignature \time 2/4
  R2  | % 5
  \barNumberCheck #5
  \time 4/4
  R1  | % 6
  \barNumberCheck #6
  R1  | % 7
  \barNumberCheck #7
  R1  | % 8
  \barNumberCheck #8
  \numericTimeSignature \time 3/4
  R2.  | % 9
  \barNumberCheck #9
  R2.
  \bar "|."
   | % 10
  \barNumberCheck #10
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
