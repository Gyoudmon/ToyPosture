#lang racket/base

(provide (all-defined-out))

(require scribble/base)
(require scribble/latex-render)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define chinese
  (lambda [#:font [font "SimSun"] . contents]
    (apply elem #:style (not (format "JFS~a" font)) contents)))
