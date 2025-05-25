\version "2.24.4"

\header {
  workCreditTypeTitle = "Too Short Measure"
  encodingDate        = "2022-02-28"
  composer            = "Gentil compositeur"
  arranger            = "Gentil arrangeur"
  software            = "Finale 2008 for Windows"
  software            = "Dolet Light for Finale 2008"
  right               = "2022"
  title               = "Too Short Measure"
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

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key a \major
  \numericTimeSignature \time 3/4
  R2. \pageBreak | % 14444444
  
r4 r }

\book {

  \score {
    <<
      
      <<
      
        \new StaffGroup
        \with {
          instrumentName = "WW"
        }
        
        
        <<
        
          \new Staff  = "Part_PTwo_Staff_One"
          \with {
            instrumentName = "Hautbois"
            shortInstrumentName = "Hb."
          }
          <<
            \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
              \Part_PTwo_Staff_One_Voice_One
            >>
          >>
        
        
        >>
        \new StaffGroup
        \with {
          instrumentName = "Brass"
        }
        
        
        <<
        
        
        
        >>
        \new StaffGroup
        \with {
          instrumentName = "Non-Pitched Percussion"
        }
        
        
        <<
        
        
        
        >>
        \new StaffGroup
        \with {
          instrumentName = "Strings"
        }
        
        
        <<
        
        
        
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
