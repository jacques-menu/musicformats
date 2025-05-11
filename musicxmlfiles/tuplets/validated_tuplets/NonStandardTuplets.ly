\version "2.24.4"

\header {
  movementTitle       = "Non-standard Tuplets"
  miscellaneousField  = "Displaying tuplet note types,
          that might not coincide with the displayed note. The first two tuplets
          take the type from the note, the second two from the
          <time-modification> element, the remaining pair of tuplets from the
          <tuplet> notation element. The tuplets in measure 3 specify both
          a number of notes and a type inside the <tuplet-actual> and
          <tuplet-normal> elements, the ones in measure 4 specify only a
          note type (but no number), and the ones in measure 5 specify only a
          number of tuplet-notes (but no type, which is deduced from the
          note's type). The first tuplet of measures 3-5 uses
          'display-type=\"actual\"', the second one 'display-type=\"both\"'.
          FIXME: The tuplet-normal should coincide with the real notes!"
  title               = "Non-standard Tuplets"
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
  
  \once\override TupletBracket.bracket-visibility = ##t
  \once \override TupletNumber.text = #tuplet-number::calc-fraction-text
  \tuplet 7/5 {  c''8 c'' c'' }
  \once\override TupletBracket.bracket-visibility = ##t
  \once \override TupletNumber.text = #tuplet-number::calc-fraction-text
  \tuplet 7/5 {  c''4. c'' c'' }
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "MusicXML Part"
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \Part_POne_Staff_One_Voice_One
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
