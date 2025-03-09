\version "2.24.4"

\header {
  movementTitle       = "Backup Inbetween Tuplet Members With Four Voices"
  title               = "Backup Inbetween Tuplet Members With Four Voices"
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

Part_POne_Staff_One_Voice_Three = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key f \major
  \numericTimeSignature \time 3/4
  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 53 %}  \stemDown bes,8 [  %{ beam 1, line 68 %}
  d ees ]  %{ beam 1, line 121 %}
  }  | % 0
  \barNumberCheck #1
}

Part_POne_Staff_One_Voice_Four = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key f \major
  \numericTimeSignature \time 3/4
  \stemDown f,2  | % 0
  \barNumberCheck #1
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key f \major
  \numericTimeSignature \time 3/4
  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 133 %}  \stemUp c'8 [  %{ beam 1, line 147 %}
  ees' f' ]  %{ beam 1, line 200 %}
  }  | % 0
  \barNumberCheck #1
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key f \major
  \numericTimeSignature \time 3/4
  \stemUp g2  | % 1
  \barNumberCheck #1
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Klavier"
          shortInstrumentName = "Pno."
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_Three" <<
            \voiceOne % out of 4 regular voices
            \Part_POne_Staff_One_Voice_Three
          >>
          \context Voice = "Part_POne_Staff_One_Voice_Four" <<
            \voiceTwo % out of 4 regular voices
            \Part_POne_Staff_One_Voice_Four
          >>
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \voiceThree % out of 4 regular voices
            \Part_POne_Staff_One_Voice_One
          >>
          \context Voice = "Part_POne_Staff_One_Voice_Two" <<
            \voiceFour % out of 4 regular voices
            \Part_POne_Staff_One_Voice_Two
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
