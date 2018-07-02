#lang racket

(require plot)

(define xts (list 47 54 53 50 46 46 47 50 51 50 46 52 50 50))

(define metrics
  (for/list ([xt (in-list xts)]
             [t (in-naturals)])
    (vector t xt)))

(plot #:y-min 0 #:y-max 60
      (lines metrics #:color 6 #:label "T(°F)"))
