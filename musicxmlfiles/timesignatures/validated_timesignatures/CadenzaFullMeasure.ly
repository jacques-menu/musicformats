\version "2.24.4"

\header {
  movementTitle       = "Excerpt from Magnificat secundi toni"
  encodingDate        = "2018-05-20"
  composer            = "Gilles Binchois"
  software            = "MuseScore 2.1.0"
  right               = "Copyright © 2008 Recordare LLC"
  title               = "Excerpt from Magnificat secundi toni"
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
  
  
  \clef "bass"
  \key f \major
  c'4 \once\omit Rest r1
  \bar "|."
  
}

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  
  \clef "bass"
  \key f \major
  r4*5 \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      60
    } % concat
  }
  
  \bar "|."
  
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Cantus"
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \Part_POne_Staff_One_Voice_One
          >>
        >>
        \new Staff  = "Part_PTwo_Staff_One"
        \with {
          instrumentName = "Cantus 2 and Tenor"
        }
        <<
          \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
            \Part_PTwo_Staff_One_Voice_One
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
