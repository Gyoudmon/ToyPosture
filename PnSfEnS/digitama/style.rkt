#lang racket

(provide (all-defined-out))

(require scribble/manual)

(define racket%
  (lambda [v]
    (racketvalfont (format "~a%" v))))
