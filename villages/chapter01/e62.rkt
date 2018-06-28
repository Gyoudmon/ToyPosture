#lang racket

(require math/number-theory)

(define n 4)
(define mean 76831)
(define s 180)
(define x1 76683)
(define x4 77048)

(define n-1 (- n 1))
(define s² (* s s))
(define Σx (* n mean))
(define Σx² (* Σx Σx))
(define x1²+x4² (+ (* x1 x1) (* x4 x4)))

(define x2+x3 (- Σx x1 x4))
(define x2²+x3² (- (+ (* n-1 s²) (/ Σx² n)) x1²+x4²))

(define α x2+x3)
(define β x2²+x3²)

;;; 2x² - 2αx + (α² - β) = 0

(define a 2)
(define b (* -2 α))
(define c (- (* α α) β))

(quadratic-solutions a b c)
