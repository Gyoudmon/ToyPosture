#lang racket

(require math/distributions)
(require plot/pict)

(define B (λ [x n p] (cdf (binomial-dist n p) x)))

(define (oc-curve x n #:color c)
    (function #:y-min 0.0 #:y-max 1.0 #:color c #:label (format "B(~a; ~a, p)" x n)
              (λ [p] (- 1.0 (B (sub1 x) n p))) 0.0 1.0))

(- 1.0 (B 4 25 0.05))
(- 1.0 (B 4 25 0.10))
(- 1.0 (B 4 25 0.20))

(- 1.0 (B 5 25 0.05))
(- 1.0 (B 6 25 0.10))
(- 1.0 (B 6 25 0.20))

(plot #:title "Error Probability"
      #:x-label "Proportion of Defective Chips"
      #:y-label "Probability of Rejecting the Batch"
      (list (oc-curve 5 25 #:color 5)
            (oc-curve 6 25 #:color 6)))

