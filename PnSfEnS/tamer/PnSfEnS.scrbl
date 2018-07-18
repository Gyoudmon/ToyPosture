#lang scribble/manual

@(require digimon/tamer)

@handbook-title{Probability and Statistics for Engineering and the Sciences}

@tamer-smart-summary[]

@handbook-smart-table[]

@include-section[(submod "descriptive.statistics.rkt" doc)]

@handbook-appendix[#:index? #true
 (bib-entry #:key      "PnSfEnS"
            #:title    "Probability and Statistics for Engineering and the Sciences"
            #:author   (authors "JAY L. DEVORE")
            #:location (book-location #:edition "ninth" #:publisher "CENGAGE Learning")
            #:date     "2017"
            #:is-book? #true)]
