\version "2.24.4"

\header {
  workCreditTypeTitle = "Simple Staff Changes"
  encodingDate        = "2024-11-24"
  software            = "MuseScore 4.4.3"
  title               = "Simple Staff Changes"
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
  \numericTimeSignature \time 2/4
  s2 \stemDown c''8 [
  bes'
  \change Staff = "Part_POne_Staff_Two"
  \stemUp aes b ]
  
  \bar "|."
   | % 1
  \barNumberCheck #3
}

Part_POne_Staff_Two_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key c \major
  \numericTimeSignature \time 2/4
  \stemUp a8 [
  
  \change Staff = "Part_POne_Staff_One"
  \stemDown e'
  \change Staff = "Part_POne_Staff_Two"
  \stemUp f
  \change Staff = "Part_POne_Staff_One"
  \stemDown g' ]
  
  \bar "|."
  s2  | % 1
  \barNumberCheck #3
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key c \major
  \numericTimeSignature \time 2/4
  \stemUp a,2 ~  a,2
  \bar "|."
   | % 3
  \barNumberCheck #3
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
              \Part_POne_Staff_One_Voice_One
            >>
          >>
          
          \new Staff  = "Part_POne_Staff_Two"
          \with {
          }
          <<
            \context Voice = "Part_POne_Staff_Two_Voice_One" <<
              \voiceOne % out of 2 regular voices
              \Part_POne_Staff_Two_Voice_One
            >>
            \context Voice = "Part_POne_Staff_Two_Voice_Five" <<
              \voiceTwo % out of 2 regular voices
              \Part_POne_Staff_Two_Voice_Five
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
