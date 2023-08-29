\version "2.25.0"

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"

  \clef "treble"
  \key c \major
  \numericTimeSignature \time 4/4
  <c' e' >4 ( <b g' >4 ) <c' e' >4 -\staccato \markup { "cresc." } <b g' >4 | % 2
  \barNumberCheck #2
  <c' e' >4 -\f <b \> g' >4 \> <c' e' >4 <b g' >4 | % 3
  \barNumberCheck #3
  <c' \! e' >1 -- \markup { "ten." } \!
  \bar "|."
  | % 4
  \barNumberCheck #4
}

{ \Part_POne_Staff_One_Voice_One }
