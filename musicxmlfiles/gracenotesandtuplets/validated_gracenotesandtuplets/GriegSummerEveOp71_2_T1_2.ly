\version "2.24.4"

\header {
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
  \key des \major
  \numericTimeSignature \time 3/4
  \stemUp f''4 \p \tempo \markup {
    "Allegretto tranquillamente. M.M."\concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      69
    } % concat
  }
  bes'2  % noteIsFollowedByGraceNotesGroup
  
  \once \omit TupletBracket
  \tuplet 3/2 {  f''8 [
  \< ( \grace { \stemDown ees''16  f''16 } \stemUp ees''8 \stemDown \stemUp des'' ]
  ) } bes'2
  \bar "|."
  
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key des \major
  \numericTimeSignature \time 3/4
  \tempo \markup {
    "Allegretto tranquillamente. M.M."\concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      69
    } % concat
  }
  \stemDown f'4 f'2 f'4 f'2 \!
  \bar "|."
   | % 1
  \barNumberCheck #3
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key des \major
  \numericTimeSignature \time 3/4
  \tempo \markup {
    "Allegretto tranquillamente. M.M."\concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      69
    } % concat
  }
  \stemDown  < des aes > 4  < ges des' > 2  < des aes > 4  < ges des' > 2
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
          shortInstrumentName = "Pno."
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
            \context Voice = "Part_POne_Staff_Two_Voice_Five" <<
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
