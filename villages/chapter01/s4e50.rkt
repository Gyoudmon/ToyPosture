#lang racket

(define sigma^2 24657511.0)
(define sigma 20179.0)
(define N 27.0)

(define median 750.0)

(define variance (/ (- sigma^2 (/ (expt sigma 2) N)) (sub1 N)))
(define deviation (sqrt variance))
(define 2deviation (* deviation 2))

(define maximum (+ median 2deviation))
maximum

;; TODO: do I understand the question?
