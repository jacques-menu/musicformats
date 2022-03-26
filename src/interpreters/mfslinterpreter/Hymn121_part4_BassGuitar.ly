\version "2.22.0"

% Pick your choice from the next two lines as needed
%myBreak = { \break }
myBreak = {}

% Pick your choice from the next two lines as needed
%myPageBreak = { \pageBreak }
myPageBreak = {}

\header {
  title                = "Hymn 121 - Angels From The Realms Of Glory - Harmony band"
  workTitle            = "121. ANGELS FROM THE REALMS OF GLORY"
  composer             = "Flemish"
  poet                 = ""
  software             = "Neuratron PhotoScore"
  right                = ""
  title                = "Hymn 121 - Angels From The Realms Of Glory - Harmony band"
  subtitle             = "Part 4 - Bass guitar"
}

\paper {
  % page-count = -1
  % system-count = -1
  
  
  
  
}

\layout {
  \context {
    \Score
    autoBeaming = ##f % to display tuplets brackets
  }
  \context {
    \Voice
    \consists "Ambitus_engraver"
  }
}

Part_PFour_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key f \major
  \numericTimeSignature \time 4/4
  
  \clef "bass"
  
  \bar "|."
  f4 \mp f f f | % 2
  \barNumberCheck #2
  | % 2
  \barNumberCheck #2
  a4. g8 f4 f | % 3
  \barNumberCheck #3
  | % 3
  \barNumberCheck #3
  d4 d _\< d \! a | % 4
  \barNumberCheck #4
  | % 4
  \barNumberCheck #4
  g4 _\> c \! f2 | % 5
  \barNumberCheck #5
  | % 5
  \barNumberCheck #5
  f4. e8 d4 c | % 6
  \barNumberCheck #6
  | % 6
  \barNumberCheck #6
  d4 e f f | % 7
  \barNumberCheck #7
  | % 7
  \barNumberCheck #7
  f4 c' _\< a \! f | % 8
  \barNumberCheck #8
  | % 8
  \barNumberCheck #8
  g4 _\> c \! f2 | % 9
  \barNumberCheck #9
  | % 9
  \barNumberCheck #9
  \bar "||"
  f4 \f ( ees! d2 ) | % 10
  \barNumberCheck #10
  | % 10
  \barNumberCheck #10
  g8 [ ( f e! d ] ) e4 c | % 11
  \barNumberCheck #11
  | % 11
  \barNumberCheck #11
  \break | % 11\myLineBreak
  
  f8 [ ( e d c ] ) d4 <a, a > | % 12
  \barNumberCheck #12
  | % 12
  \barNumberCheck #12
  <g, g >2 <c c' >4 ( <bes, bes > ) | % 13
  \barNumberCheck #13
  | % 13
  \barNumberCheck #13
  <a, a >4 <g, g > <f, f > bes, | % 14
  \barNumberCheck #14
  | % 14
  \barNumberCheck #14
  c1 | % 15
  \barNumberCheck #15
  | % 15
  \barNumberCheck #15
  f4 ( ees! d2 ) | % 16
  \barNumberCheck #16
  | % 16
  \barNumberCheck #16
  g8 [ ( f e! d ] ) e4 c | % 17
  \barNumberCheck #17
  | % 17
  \barNumberCheck #17
  f8 [ ( e d c ] ) d4 <a, a > | % 18
  \barNumberCheck #18
  | % 18
  \barNumberCheck #18
  <g, g >2 <c _\< c' >4 ( \< <bes, \! bes > ) \! | % 19
  \barNumberCheck #19
  | % 19
  \barNumberCheck #19
  <a, a >4 -\ff <g, g > <f, f > bes, | % 20
  \barNumberCheck #20
  | % 20
  \barNumberCheck #20
  <c c' >2 -> <c c' > -> | % 21
  \barNumberCheck #21
  | % 21
  \barNumberCheck #21
  <f, f >1 -> | % 22
  \barNumberCheck #22
  | % 22
  \barNumberCheck #22
}

\book {
  \score {
    <<
      
      \new Staff = "Part_PFour_Staff_One"
      \with {
        instrumentName = "Voice 4"
      }
      <<
        \context Voice = "Part_PFour_Staff_One_Voice_One" <<
          \Part_PFour_Staff_One_Voice_One
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
        \consists "Ambitus_engraver"
      }
    }
    
    \midi {
      \tempo 16 = 360
    }
  }
  
}
