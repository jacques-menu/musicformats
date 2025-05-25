\version "2.24.4"

\header {
  workCreditTypeTitle = "Cross Staff Chord"
  miscellaneousField  = "Staff changes in a piano staff.
          The voice from the second staff has some notes/chords on the first
          staff. The final two chords have some notes on the first, some on
          the second staff."
  title               = "Cross Staff Chord"
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
  r1
  \bar "|."
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key c \major
  \time 4/4
  
  \bar "|."
s1 }

Part_POne_Staff_Two_Voice_Two = \absolute {
  \language "nederlands"
  
  
  \clef "bass"
  \key c \major
  \time 4/4
  < c e g c' > 8 [
  \change Staff = "Part_POne_Staff_One"
  < e g b >  
  \change Staff = "Part_POne_Staff_Two"
  < d f a d' >  
  \change Staff = "Part_POne_Staff_One"
  
  \change Staff = "Part_POne_Staff_Two"
  < g c' e' g' >  ]
  \change Staff = "Part_POne_Staff_One"
  
  \change Staff = "Part_POne_Staff_Two"
  r2
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
            \context Voice = "Part_POne_Staff_Two_Voice_Two" <<
              \Part_POne_Staff_Two_Voice_Two
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
