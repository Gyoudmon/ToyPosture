#lang info

(define collection 'use-pkg-name)
(define pkg-desc "Probability and Statistics for Engineering and the Science")

(define deps '("digimon"))
(define build-deps '("digimon" "scribble-lib" "racket-doc"))

(define version "1.0")
(define pkg-authors '(wargrey))
(define test-omit-paths 'all)

(define scribblings '(["tamer/PnSfEnS.scrbl" (main-doc multi-page) ("Gyoudmon")]))
