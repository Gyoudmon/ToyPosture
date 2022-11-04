#lang racket/base

(provide (all-defined-out))

(require scribble/manual)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define racket%
  (lambda [v]
    (racketvalfont (format "~a%" v))))

(define math-expt
  (lambda [v]
    (superscript (math v))))

(define math-index
  (lambda [v]
    (subscript (math v))))
