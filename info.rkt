#lang info

(define collection 'multi)
(define pkg-desc "Self Teaching Projects")

(define deps '("digimon" "base" "math-lib" "plot-gui-lib" "plot-lib" "scribble-lib" "typed-racket-lib"))
(define build-deps '("digimon" "math-lib" "typed-racket-lib" "racket-doc" "math-doc" "plot-doc"))

(define version "1.0")
(define pkg-authors '(wargrey))
(define test-omit-paths 'all)
