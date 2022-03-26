\version "2.22.0"

% Pick your choice from the next two lines as needed
%myBreak = { \break }
myBreak = {}

% Pick your choice from the next two lines as needed
%myPageBreak = { \pageBreak }
myPageBreak = {}

\header {
  title                = "Hymn 121"
  workTitle            = "121. ANGELS FROM THE REALMS OF GLORY"
  composer             = "Flemish"
  poet                 = ""
  software             = "Neuratron PhotoScore"
  right                = ""
  title                = "Hymn 121"
}

\paper {
  % page-count = -1
  % system-count = -1
  ragged-bottom = ##f
  ragged-last-bottom = ##f
}

\layout {
  \context {
    \Score
    autoBeaming = ##f % to display tuplets brackets
  }
  \context {
    \Voice
  }
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key f \major
  \numericTimeSignature \time 4/4
  
  \clef "treble"
  a'4 \mp a' a' a'8 [ ( c'' ] ) | % 2
  \barNumberCheck #2
  | % 2
  \barNumberCheck #2
  c''4. bes'8 a'4 f' | % 3
  \barNumberCheck #3
  | % 3
  \barNumberCheck #3
  a'4 a'8 [ ^\< ( g' ] \! ) a'4 a'8 [ c'' ] | % 4
  \barNumberCheck #4
  | % 4
  \barNumberCheck #4
  c''4. ^\> bes'8 \! a'2 | % 5
  \barNumberCheck #5
  | % 5
  \barNumberCheck #5
  a'4 a' a' a'8 [ ( c'' ] ) | % 6
  \barNumberCheck #6
  | % 6
  \barNumberCheck #6
  c''4. bes'8 a'4 f' | % 7
  \barNumberCheck #7
  | % 7
  \barNumberCheck #7
  a'4 a'8 [ ^\< ( g' ] \! ) a'4 a'8 [ ( c'' ] ) | % 8
  \barNumberCheck #8
  | % 8
  \barNumberCheck #8
  c''4. ^\> ( bes'8 \! a'2 ) | % 9
  \barNumberCheck #9
  | % 9
  \barNumberCheck #9
  \bar "||"
  c''2 \f d''8 [ ( c'' bes' a' ] ) | % 10
  \barNumberCheck #10
  | % 10
  \barNumberCheck #10
  bes'2 c''8 [ ( bes' a' g' ] ) | % 11
  \barNumberCheck #11
  | % 11
  \barNumberCheck #11
  \break | % 11\myLineBreak
  
  a'2 bes'8 [ ( a' g' f' ] ) | % 12
  \barNumberCheck #12
  | % 12
  \barNumberCheck #12
  g'2 c' | % 13
  \barNumberCheck #13
  | % 13
  \barNumberCheck #13
  f'4 g' a' bes' | % 14
  \barNumberCheck #14
  | % 14
  \barNumberCheck #14
  a'2 ( g' ) | % 15
  \barNumberCheck #15
  | % 15
  \barNumberCheck #15
  c''2 d''8 [ ( c'' bes' a' ] ) | % 16
  \barNumberCheck #16
  | % 16
  \barNumberCheck #16
  bes'2 c''8 [ ( bes' a' g' ] ) | % 17
  \barNumberCheck #17
  | % 17
  \barNumberCheck #17
  a'2 bes'8 [ ( a' g' f' ] ) | % 18
  \barNumberCheck #18
  | % 18
  \barNumberCheck #18
  g'2 c' ^\< | % 19
  \barNumberCheck #19
  | % 19
  \barNumberCheck #19
  f'4 \ff \! g' a' bes' | % 20
  \barNumberCheck #20
  | % 20
  \barNumberCheck #20
  a'2 -> g' -> | % 21
  \barNumberCheck #21
  | % 21
  \barNumberCheck #21
  f'1 -> | % 22
  \barNumberCheck #22
  | % 22
  \barNumberCheck #22
}

\book {
  \score {
    <<
      
      \new Staff = "Part_POne_Staff_One"
      \with {
        instrumentName = "Voice"
      }
      <<
        \context Voice = "Part_POne_Staff_One_Voice_One" <<
          \Part_POne_Staff_One_Voice_One
        >>
      >>
      
    >>
    
    \layout {
      \context {
        \Score
        autoBeaming = ##f % to display tuplets brackets
      }
      \context {
        \Voice
      }
    }
    
    \midi {
      \tempo 16 = 360
    }
  }
  
}
