#lang scribble/manual

@(require digimon/tamer)

@handbook-title/pkg-desc[]

@tamer-smart-summary[]

@handbook-smart-table[]

@include-section[(submod "modeling.change.rkt" doc)]

@handbook-appendix[#:index? #true
 (bib-entry #:key      "AFCiMM"
            #:title    @(#%info 'pkg-desc)
            #:author   (authors "Frank R. Giordano" "William P. Fox" "Steven B.Horton" "Maurice D. Weir")
            #:location (book-location #:edition "Fourth" #:publisher "CENGAGE Learning")
            #:date     "2009"
            #:is-book? #true)]
