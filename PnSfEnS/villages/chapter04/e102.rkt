#lang racket

(require math/distributions)
(require plot/pict)

(define B
  (λ [min-x max-x n p]
    (- (cdf (binomial-dist n p) max-x)
       (cdf (binomial-dist n p) (sub1 min-x)))))

(define ~B
  (λ [x n p]
    (+ (cdf (binomial-dist n p) x)
       (- 1.0 (cdf (binomial-dist n p) (- n x 1))))))

(define (oc-curve n p #:color c)
  (function #:y-min 0.0 #:y-max 1.0 #:color c #:label (format "~~B(X; ~a, ~a)" n p)
            (λ [x] (~B x n p)) 0 (quotient n 2)))

(B 7 18 25 0.5)
(B 7 18 25 0.8)

(~B 7 25 0.5)

(B 8 17 25 0.6)
(B 8 17 25 0.8)

(plot #:title "Error Probability"
      #:x-label "{Number of S | [0, X] U [(n - X), n]}"
      #:y-label "Probability of Rejecting the Claim"
      (list (oc-curve 25 0.5 #:color 5)
            (oc-curve 25 0.6 #:color 6)
            (oc-curve 25 0.8 #:color 8)
            (hrule 0.01 #:style 'long-dash #:color 4)))

