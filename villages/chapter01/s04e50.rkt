#lang racket

(define Σx² 24657511.0)
(define Σx 20179.0)
(define N 27.0)

(define mean (/ Σx N))

(define s² (/ (- Σx² (/ (expt Σx 2) N)) (sub1 N)))
(define s (sqrt s²))
(define 2s (* s 2))

(define maximum (+ mean 2s))
maximum

;; TODO: do I understand the question?
