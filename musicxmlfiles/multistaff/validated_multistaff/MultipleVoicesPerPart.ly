\version "2.24.4"

\header {
  workCreditTypeTitle = "Multiple Voices Per Part"
  encodingDate        = "2011-08-08"
  software            = "Finale 2011 for Windows"
  software            = "Dolet 6.0 for Finale"
  right               = "Copyright © 2002 Recordare LLC"
  title               = "Multiple Voices Per Part"
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
  \key ees \major
  \numericTimeSignature \time 3/4
  s4 \stemUp bes'4. c''8
  \bar "|."
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key ees \major
  \numericTimeSignature \time 3/4
  \stemUp d'4 \stemDown f' g'
  \bar "|."
}

Part_POne_Staff_Two_Voice_Three = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key ees \major
  \numericTimeSignature \time 3/4
  \stemUp bes,,4 \stemDown d ees
  \bar "|."
}

\book {

  \score {
    <<
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
        }
        
        <<
        
          \new Staff  = "Part_POne_Staff_One"
          \with {
          }
          <<
            \context Voice = "Part_POne_Staff_One_Voice_One" <<
              \voiceOne % out of 2 regular voices
              \Part_POne_Staff_One_Voice_One
            >>
            \context Voice = "Part_POne_Staff_One_Voice_Two" <<
              \voiceTwo % out of 2 regular voices
              \Part_POne_Staff_One_Voice_Two
            >>
          >>
          
          \new Staff  = "Part_POne_Staff_Two"
          \with {
          }
          <<
            \context Voice = "Part_POne_Staff_Two_Voice_Three" <<
              \Part_POne_Staff_Two_Voice_Three
            >>
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
