#lang scribble/lp2

@(require digimon/tamer)

@handbook-story{Overview and Discriptive Statistics}

@tamer-smart-summary[]

@handbook-scenario{Pictorial and Tabular Methods}

@tamer-note['realize]
@chunk[|<testcase: realize>|
       (test-spec "sakuyamon realize"
                  (check-eq? 1 1))]

@handbook-scenario{Measures of Location}

@handbook-reference[]

@; Chunks after `handbook-reference[]` will never be rendered in documents
@; <*> is the main chunk by convention.

@chunk[|<*>|
       (require digimon/tamer)
       (tamer-taming-start!)

       (module+ tamer |<descriptive.statistics:*>|)]

@chunk[|<descriptive.statistics:*>|
       (module+ story
         (define-tamer-suite realize "Sakuyamon, Realize!" |<testcase: realize>|))]
