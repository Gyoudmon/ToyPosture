#lang scribble/manual

@(current-digimon "PnSfEnS")

@(require digimon/tamer)

@handbook-title[]

@handbook-smart-table[]

@include-section[(submod "sakuyamon.rkt" doc)]

@handbook-appendix[#:index? #true
 (bib-entry #:key      "PnSfEnS"
            #:title    "Probability and Statistics for Engineering and the Sciences"
            #:author   (authors "JAY L. DEVORE")
            #:location (book-location #:edition "ninth" #:publisher "CENGAGE Learning")
            #:date     "2017"
            #:is-book? #true)]
