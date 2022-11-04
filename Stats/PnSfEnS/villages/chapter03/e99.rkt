#lang racket

(require math/number-theory)

(define k-out-of-n-system
  (lambda [k n p]
    (for/sum ([i (in-range k (add1 n))])
      (* (binomial n i) (expt p i) (expt (- 1.0 p) (- n i))))))

(k-out-of-n-system 3 5 0.9)
