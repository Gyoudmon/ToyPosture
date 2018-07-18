#lang scribble/lp2

@(require digimon/tamer)

@handbook-story{Overview and Discriptive Statistics}

@tamer-smart-summary[]

@chunk[|<taming start>|
       (require digimon/tamer)
       (tamer-taming-start)

       (module+ tamer |<descriptive.statistics:*>|)]

@handbook-scenario{Pictorial and Tabular Methods}

@chunk[|<testcase: realize>|
       (test-spec "sakuyamon realize"
                  (check-eq? 1 1))]

@handbook-scenario{Measures of Location}

@handbook-reference[]

@chunk[|<descriptive.statistics:*>|
       (module+ story
         (define-tamer-suite realize "Sakuyamon, Realize!" |<testcase: realize>|))]
