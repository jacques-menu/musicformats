\version "2.25.2"
% automatically converted by musicxml2ly from NotesStaffChange_musicxml2ly.xml

\header {
  title =  "Notes Staff Change"
  texidoc =
  "Staff changes in a piano staff.
          The voice from the second staff has some notes/chords on the first
          staff. The final two chords have some notes on the first, some on
          the second staff."
}

\layout {
  \context {
    \Score
    autoBeaming = ##f
  }
}
PartPOneVoiceTwo =  \relative a {
  \clef "bass" \time 4/4 \key c \major | % 1
  a8 [ \change Staff="1" e'8 \change Staff="2" g,8 \change Staff="1"
  f'8 ] c'8 [ b8 \change Staff="2" f,8 d'8 ]
}


% The score definition
\score {
  <<

    \new PianoStaff
    <<

      \context Staff = "1" <<
        \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
        \context Voice = "FOO" { s1 }
      >>

      \context Staff = "2" <<
        \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
        \context Voice = "PartPOneVoiceTwo" {  \PartPOneVoiceTwo }
      >>

    >>

  >>
  \layout {}
  % To create MIDI output, uncomment the following line:
  %  \midi {\tempo 4 = 100 }
}

