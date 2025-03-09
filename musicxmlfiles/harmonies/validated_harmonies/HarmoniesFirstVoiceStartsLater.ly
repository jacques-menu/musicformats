\version "2.24.4"

\header {
  movementTitle       = "Harmonies First Voice Starts Later"
  encodingDate        = "2018-12-21"
  software            = "soundnotation"
  software            = "Dolet 6.6"
  right               = ""
  title               = "Harmonies First Voice Starts Later"
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

Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven = \chordmode {
  \language "nederlands"
  \key c \major
  \numericTimeSignature \time 4/4
  f8.:5.3 s %{ s222 %}  16*13 \break | % 1333333 \myLineBreak
  
  s %{ s222 %}  1  | % 1
  \barNumberCheck #3
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 4/4
  \stemUp a8. [  %{ beam 1, line 139 %}
  e'16 ]  %{ beam 1, line 151 %}
  g8 [  %{ beam 1, line 171 %}
  g ]  %{ beam 1, line 183 %}
  g [  %{ beam 1, line 202 %}
  g g g ]  %{ beam 1, line 238 %}
  \break | % 1333333 \myLineBreak
  
  \stemDown e'8 [  %{ beam 1, line 403 %}
  e' f' f' ]  %{ beam 1, line 491 %}
  e'4 c'  | % 1
  \barNumberCheck #3
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 4/4
  s %{ s222 %}  1 \break | % 1333333 \myLineBreak
  
  \stemUp g'8 [  %{ beam 1, line 388 %}
  g' a' a' ]  %{ beam 1, line 476 %}
  g'4 e'8 [  %{ beam 1, line 535 %}
  c' ]  %{ beam 1, line 564 %}
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
        
          \context ChordNames = "Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven"
          \Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven
          
          \new Staff  = "Part_POne_Staff_One"
          \with {
          }
          <<
            \context Voice = "Part_POne_Staff_One_Voice_Two" <<
              \voiceOne % out of 2 regular voices
              \Part_POne_Staff_One_Voice_Two
            >>
            \context Voice = "Part_POne_Staff_One_Voice_One" <<
              \voiceTwo % out of 2 regular voices
              \Part_POne_Staff_One_Voice_One
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
