\version "2.24.4"

\header {
  workCreditTypeTitle = "Harmony With Backup"
  encodingDate        = "2018-10-19"
  software            = "Sibelius 18.5"
  software            = "Dolet 6.6 for Sibelius"
  title               = "Harmony With Backup"
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
  
  \clef "treble"
  \key a \minor
  \numericTimeSignature \time 4/4
  \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      130
    } % concat
  }
  a1:sus2 c8:maj7 s2..  | % 3
  \barNumberCheck #3
  s2 f4:5.3^5/e s  | % 4
  \barNumberCheck #4
  s1  | % 5
  \barNumberCheck #5
  \break | % 1333333 \myLineBreak
  
  d4:sus4 s2.  | % 1
  \barNumberCheck #6
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key a \minor
  \numericTimeSignature \time 4/4
  R1 \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      130
    } % concat
  }
  \stemUp e'8 [
  d' ]
  c'4 r2  | % 3
  \barNumberCheck #3
  r4 g' a' a'8 [
  g' ]
   | % 4
  \barNumberCheck #4
  r2 \slashedGrace { g''8 } \stemDown a''4 \stemUp \stemDown a''8 [
  b'' ]
   | % 5
  \barNumberCheck #5
  \break | % 1333333 \myLineBreak
  
  \stemUp a'4 g'8 [
  e' ]
  d'4 c'  | % 1
  \barNumberCheck #6
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key a \minor
  \numericTimeSignature \time 4/4
  \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      130
    } % concat
  }
  s1 s1  | % 3
  \barNumberCheck #3
  s1  | % 4
  \barNumberCheck #4
  \stemUp  < a,,, a,, > 2 \stemDown  < e' f' a' >   | % 5
  \barNumberCheck #5
  \break | % 1333333 \myLineBreak
  
  s1  | % 6
  \barNumberCheck #6
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
          shortInstrumentName = "Voice"
        }
        
        <<
        
          \context ChordNames = "Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven"
          \Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven
          
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
