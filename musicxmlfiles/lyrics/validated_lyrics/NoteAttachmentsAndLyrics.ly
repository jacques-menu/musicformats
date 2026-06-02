\version "2.24.4"

% Generated from "NoteAttachmentsAndLyrics.xml"
% by xml2ly v2026.2 (built on June 02, 2026)
% on Tuesday 2026-06-02 @ 07:32:40 CEST

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name NoteAttachmentsAndLyrics.ly NoteAttachmentsAndLyrics.xml -lilypond-lyrics-durations-mode explicit
% or, with short option names:
%     NoteAttachmentsAndLyrics.ly NoteAttachmentsAndLyrics.xml explicit


% Scheme function(s): "after"
% A function to create events after given music.
% Thanks to David Kastrup for the inspiration!

after =
#(define-music-function (t e m) (ly:duration? ly:music? ly:music?)
   #{
     \context Bottom <<
       #m
       { \skip $t <> -\tweak extra-spacing-width #empty-interval $e }
     >>
   #})



% Scheme function(s): "date & time"
% A set of functions to obtain the LilyPond file creation or modification time.

#(define commandLine                  (object->string (command-line)))
#(define loc                          (+ (string-rindex commandLine #\space ) 2))
#(define commandLineLength            (- (string-length commandLine) 2))
#(define lilypondFileName             (substring commandLine loc commandLineLength))

#(define lilypondFileDirName          (dirname lilypondFileName))
#(define lilypondFileBaseName         (basename lilypondFileName))
#(define lilypondFileSuffixlessName   (basename lilypondFileBaseName ".ly"))

#(define pdfFileName                  (string-append lilypondFileSuffixlessName ".pdf"))
#(define pdfFileFullName              (string-append lilypondFileDirName file-name-separator-string pdfFileName))

#(define lilypondVersion              (object->string (lilypond-version)))
#(define currentDate                  (strftime "%d/%m/%Y" (localtime (current-time))))
#(define currentTime                  (strftime "%H:%M:%S" (localtime (current-time))))

#(define lilypondFileModificationTime (stat:mtime (stat lilypondFileName)))

#(define lilypondFileModificationTimeAsString (strftime "%A %d/%m/%Y, %H:%M:%S" (localtime lilypondFileModificationTime)))

#(use-modules (srfi srfi-19))
% https://www.gnu.org/software/guile/manual/html_node/SRFI_002d19-Date-to-string.html
%#(define pdfFileCreationTime (date->string (current-date) "~A, ~B ~e ~Y ~H:~M:~S"))
#(define pdfFileCreationTime (date->string (current-date) "~A ~d/~m/~Y, ~H:~M:~S"))



% Scheme function(s): "otherDynamic"
% \\otherDynamic to handle any string as a dynamic.

#(use-modules (ice-9 regex))

otherDynamic =
#(define-event-function (parser location text) (markup?)
   (if (string? text)
       (let* ((underscores-replaced
               (string-map
                (lambda (x) (if (eq? x #\_) #\space x))
                text))
              (split-text (string-split underscores-replaced #\space))
              (formatted (map
                          (lambda (word)
                            (if (string-match "^[mrzfps]*$" word)
                                (markup #:dynamic word)
                                (markup #:normal-text #:italic word)))
                          split-text)))
         #{
           #(make-dynamic-script (make-line-markup formatted))
         #})
       ; user provided a full-blown markup, so we don't mess with it:
       #{
         #(make-dynamic-script (markup #:normal-text text))
       #}))



% Scheme function(s): "schleifer"
% \\schleifer.

%% http://lsr.di.unimi.it/LSR/Item?id=720
%% see also http://lilypond.org/doc/v2.18/Documentation/notation/graphic

%LSR contributed by Valentin as per request http://code.google.com/p/lilypond/issues/detail?id=503

% schleifer: slide, coulé

schleifer =
#(define-music-function (parser location start) (ly:music?)
   #{\once \override Slur.direction = #UP
     \once \override Slur.stencil = $(lambda (grob)
                                         (let* ((slur-stencil (ly:slur::print grob))
                                                (Y-ext (ly:stencil-extent slur-stencil Y))
                                                (text-stencil (ly:text-interface::print grob))
                                                (prall-stencil (ly:stencil-translate text-stencil
                                                                                     (cons 2.5 (car Y-ext))))
                                                (combo-stencil (ly:stencil-combine-at-edge
                                                                prall-stencil X RIGHT
                                                                slur-stencil 0))
                                                (combo-X-ext (ly:stencil-extent combo-stencil X)))
                                           (ly:stencil-translate combo-stencil (cons 0 -1))))
     \once \override Slur.positions = #'(-1 . 2)
     \once \override Slur.text = \markup {
       \musicglyph #"scripts.prall"
     }
     \once \override Slur.control-points = $(lambda (grob)
                                                (let* ((coords (ly:slur::calc-control-points grob))
                                                       (point-0 (list-ref coords 0))
                                                       (point-1 (list-ref coords 1))
                                                       (point-2 (list-ref coords 2))
                                                       (point-3 (list-ref coords 3)))
                                                  (set-car! point-0 (+ (car point-0) 2))
                                                  (set-car! point-1 (+ (car point-1) 1.5))
                                                  (set-car! point-2 (+ (car point-2) -1))
                                                  (set-car! point-3 (+ (car point-3) -1))
                                                  coords))
     s1*0( $start
     \once \override Rest.transparent = ##t
     \once \override Rest.X-extent = #'(0 . 5)
     \grace r1
     s1*0)
   #}
 )



% Scheme function(s): "scoop_stencils"
% \\scoopAbove, \\scoopBelow.

% scoop stencils, thanks to vgay@vintherine.org

#(define (scoop-above-stencil grob)
   (ly:stencil-add
    (ly:note-head::print grob)
    (grob-interpret-markup grob
                           (markup #:with-dimensions '(0 . 0) '(0 . 0)
                                   #:translate '(-0.2 . -0.5)
                                   #:path 0.25 '((moveto -0.2 0.5)
                                                 (curveto 0.2 2 -1.2 2.5 -1.7 2.5))))))
scoopAbove = \once \override NoteHead.stencil = #scoop-above-stencil

#(define (scoop-below-stencil grob)
   (ly:stencil-add
    (ly:note-head::print grob)
    (grob-interpret-markup grob
                           (markup #:with-dimensions '(0 . 0) '(0 . 0)
                                   #:translate '(-0.2 . -0.5)
                                   #:path 0.25 '((moveto 0 0)
                                                 (curveto 0.2 -1 -1.2 -1.5 -1.7 -1.5))))))
scoopBelow = \once \override NoteHead.stencil = #scoop-below-stencil

%\relative c'' {
%  \scoopAbove c2 \scoopBelow c2
%}



% Scheme function(s): "tongue"
% Creates multiple tongue technicals, argument is a number.
% Example: 'c4 -\tongue #3' creates a triple tongue.

tongue =
#(define-music-function (parser location dots) (integer?)
   (let ((script (make-music 'ArticulationEvent
                   'articulation-type 'staccato)))
     (set! (ly:music-property script 'tweaks)
           (acons 'stencil
             (lambda (grob)
               (let ((stil (ly:script-interface::print grob)))
                 (let loop ((count (1- dots)) (new-stil stil))
                   (if (> count 0)
                       (loop (1- count)
                         (ly:stencil-combine-at-edge new-stil X RIGHT stil 0.2))
                       (ly:stencil-aligned-to new-stil X CENTER)))))
             (ly:music-property script 'tweaks)))
     script))


\header {
  movementTitle       = "MuseScoreNoteAttachments"
  title               = "MuseScoreNoteAttachments"
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
  
  oddHeaderMarkup = \markup {
    \fill-line {
      \unless \on-first-page {
        \fromproperty #'page:page-number-std::string
        ' '
        \fromproperty #'header:title
        ' '
        \fromproperty #'header:subtitle
      }
    }
  }

  evenHeaderMarkup = \markup {
    \fill-line {
      \unless \on-first-page {
        \fromproperty #'page:page-number-std::string
        ' '
        \fromproperty #'header:title
        ' '
        \fromproperty #'header:subtitle
      }
    }
  }

  oddFooterMarkup = \markup {
    \tiny
    \column {
      \fill-line {
        #(string-append
"Score generated from MusicXML data by xml2ly v2026.2 (built on June 02, 2026) and LilyPond " (lilypond-version))
      }
      \fill-line { \column { \italic { \concat { \lilypondFileName " was modified on " \lilypondFileModificationTimeAsString } } } }
      \fill-line { \column { \italic { \concat { \pdfFileName " was created on " \pdfFileCreationTime } } } }
     \fill-line { \column { \italic { \concat { "lilypondFileDirName: " \lilypondFileDirName } } } }
     \fill-line { \column { \italic { \concat { "pdfFileFullName: " \pdfFileFullName } } } }
%      \fill-line { \column { \italic { \concat { "lilypondFileBaseName: " \lilypondFileBaseName } } } }
%      \fill-line { \column { \italic { \concat { "lilypondFileSuffixlessName: " \lilypondFileSuffixlessName } } } }
%      \fill-line { \column { \italic { \concat { "pdfFileName: " \pdfFileName } } } }
    }
  }

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
  c''4 \fermata c'' \fermata c'' \shortfermata c'' \longfermata
  c''4 _\fermata
  \arpeggioNormal < c'' e'' g'' >  -\arpeggio
  \arpeggioBracket %{ \kArticulationNonArpeggiatoTypeBottom??? %}
  < c'' e'' g'' >  -\arpeggio c'' \bar "||"
  
  c''4 -> c'' -^ c'' -. c'' -- c''4 -_ c'' -! c'' -! c'' \scoopAbove c''4  %{ plop??? %}  c'' -\bendAfter #+4 c'' -\bendAfter #-4 c'' \breathe c''4
  \override BreathingSign.text = \markup {\musicglyph #"scripts.caesura.curved"}
  \breathe
   c''  %{ stress??? %}  c''  %{ unstress??? %}  r \bar "||"
  c''4 \trill c'' \turn c''4*1/2 s4*1/2\turn c''4 \reverseturn c''4 %{ \shake??? %} \once \override TextSpanner.style = #'trill
  \textSpannerDown c'' \startTextSpan
  \once \override TextSpanner.style = #'trill
  c'' \startTextSpan
  c'' \stopTextSpan c''4 \mordent c'' \prall c'' \schleifer c'' c''4 \turn -\markup { \natural } c'' \turn ^\markup { \sharp } ^\markup { \sesquiflat } r2 \bar "||"
  c''4 \upbow c'' \downbow c'' \flageolet c'' \flageolet c''4 \flageolet c'' \flageolet c'' \flageolet c'' \flageolet c''4 \open c'' \thumb c'' -0 c'' -1 c''4 -2 c'' -3 c'' -4 c'' -5 c''4 -0 c'' -5 -3 -2 c'' %{ kPluck??? %}  c'' %{ kPluck??? %}  -\markup {"a"} c''4 -\tongue #2 c'' -\tongue #3 c'' \stopped c'' \snappizzicato c''4  c''  c'' \1 c'' \5 \after 8 ^"H" c''4  c''  \after 8 ^"P" c''  c''  c''4 \bendAfter 4 c'' \bendAfter 3 c'' \bendAfter -0.5 c'' \bendAfter 3.5 c''4 %{ \Tap??? %}  c'' %{ \Tap??? %}  c'' \heel c'' \ltoe c''4 %{ \Fingernails??? %}  r r2 \bar "||"
  c''4 \f c'' \ppp c'' \sfp c'' -\otherDynamic "sfffz" g'4 ^^ ^. g' _> _- ^. r2
  \bar "|."
}

Part_POne_Staff_One_Voice_One_Stanza_One = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    "ferm."4 "normal ferm."4 "angled ferm."4 "square ferm."4 | % 5316
    "inv.ferm."4 "arp."4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 128 ???%} "non-arp."4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 159 ???%} "acc.mark"4 | % 5316
    "acc."4 "str.-acc."4 "stacc."4 "ten."4 | % 5316
    "det.-leg."4 "stacc.ss"4 "spicc."4 scoop4 | % 5316
    plop4 doit4 falloff4 breath4 | % 5316
    "caes."4 stress4 "unstr."4 4 | % 5316
    "tr."4 turn4 "del.turn"4 "inv.turn"4 | % 5316
    shake4 wavy4 -- 4 line4 | % 5316
    "mord."4 "inv.mord."4 "schl."4 "trem."4 | % 5316
    "turn+acc."4 "turn+acc.(ab.+bel./rel to turn)"4 2 | % 5316
    "up-b."4 "down-b."4 "harm."4 "nat.harm."4 | % 5316
    "art.harm."4 "nat.h./base"4 "nat.h./touching"4 "nat.h./sounding"4 | % 5316
    "open-str."4 "thumb-pos."4 "empty fing."4 "fing.1"4 | % 5316
    "fing.2"4 "fing.3"4 "fing.4"4 "fing.5"4 | % 5316
    "fing.sth."4 "mult.fing."4 "empty pluck"4 "pluck a"4 | % 5316
    "dbl.tng."4 "trpl.tng."4 stopped4 "snp.pizz."4 | % 5316
    "empty fret"4 "fret0"4 "empty str."4 "str. 5"4 | % 5316
    hammer4 -- on4 pull4 -- off4 | % 5316
    bend4 "b.3 with-bar"4 "pre-b. -0.5"4 "b. release 3.5"4 | % 5316
    tap4 "tap T"4 heel4 toe4 | % 5316
    "fingern."4 2. | % 5316
    f4 ppp4 sfp4 "Oth.dyn."4 | % 5316
    "both above"4 "ab./bel./bel."4 2 | % 5316

}

\book {

  \score {
    <<
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Part_POne"
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \Part_POne_Staff_One_Voice_One
          >>
          \new Lyrics
            \with {
            }
            \Part_POne_Staff_One_Voice_One_Stanza_One
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
