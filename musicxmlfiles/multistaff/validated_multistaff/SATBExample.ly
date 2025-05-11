\version "2.24.4"

\header {
  movementTitle       = "SATB Example"
  encodingDate        = "2024-01-05"
  software            = "MuseScore 4.1.1"
  title               = "SATB Example"
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
  \partial 4
  
  \clef "treble"
  \key c \major
  \time 4/4
  \stemUp c''4 \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      80
    } % concat
  }
   | % 2
  \barNumberCheck #1
  e''4. e''8 g''4 g''  | % 3
  \barNumberCheck #2
  a''4 a'' g''
  \bar "|."
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  \partial 4
  
  \clef "treble"
  \key c \major
  \time 4/4
  \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      80
    } % concat
  }
  \stemDown c''4  | % 1
  \barNumberCheck #1
  c''4. c''8 e''4 e''  | % 2
  \barNumberCheck #2
  f''4 f'' e''
  \bar "|."
}

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \partial 4
  
  \clef "bass"
  \key c \major
  \time 4/4
  \stemUp e4  | % 2
  \barNumberCheck #1
  g4. g8 c'4. b8  | % 3
  \barNumberCheck #2
  a8 [
  b c' d' ]
  e'4
  \bar "|."
}

Part_PTwo_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  \partial 4
  
  \clef "bass"
  \key c \major
  \time 4/4
  \stemDown c4  | % 1
  \barNumberCheck #1
  c4. c8 c4 c  | % 2
  \barNumberCheck #2
  f8 [
  g a b ]
  c'4
  \bar "|."
}

\book {

  \score {
    <<
      
      
      
      <<
      
        
        \new StaffGroup
        
        
        <<
        
          \new Staff  = "Part_POne_Staff_One"
          \with {
            instrumentName = "Part_POne"
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
          \new Staff  = "Part_PTwo_Staff_One"
          \with {
            instrumentName = "Part_PTwo"
          }
          <<
            \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
              \voiceOne % out of 2 regular voices
              \Part_PTwo_Staff_One_Voice_One
            >>
            \context Voice = "Part_PTwo_Staff_One_Voice_Two" <<
              \voiceTwo % out of 2 regular voices
              \Part_PTwo_Staff_One_Voice_Two
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
