#lang racket

(require math/number-theory)

(define μ (* 500 0.005))

(define poisson-distribution
  (lambda [μ X]
    (/ (* (exp (- μ)) (expt μ X))
       (factorial X))))

μ
(poisson-distribution μ 5)
(- 1.0 (for/sum ([x (in-range 0 5)])
         (poisson-distribution μ x)))
