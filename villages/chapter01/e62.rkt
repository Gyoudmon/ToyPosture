#lang racket

(require math/number-theory)

#|
Consider the following information on ultimate tensile
strength(1 lb/in² = 0.007MPa) for a sample of n = 4 hard
zirconium copper wire specimens(from
"Characterization Methods for Fine Copper Wire",
Wire J. Intl., Aug., 1997: 74-80):
|#

(define n 4)
(define-values (mean s) (values 76831 180))
(define-values (x1 x4) (values 76683 77048))

#|
Determine the values of the two middle sample observations
(and don't do it by successive guessing!)
|#

(define n-1 (- n 1))
(define s² (* s s))
(define Σx (* n mean))
(define Σ²x (* Σx Σx))
(define x1²+x4² (+ (* x1 x1) (* x4 x4)))

;;; (n-1)S² = Σx² - (Σ²x)/n
(define x2²+x3² (- (+ (* n-1 s²) (/ Σ²x n)) x1²+x4²))
(define x2+x3 (- Σx x1 x4))

(let ([α x2+x3] [β x2²+x3²])
  ;;; 2x² - 2αx + (α² - β) = 0
  (quadratic-solutions 2 (* -2 α) (- (* α α) β)))
