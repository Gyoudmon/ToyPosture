#lang info

(define collection "Advent of Code")
(define pkg-desc "代码降临节")

(define deps '("digimon" "pict"))
(define build-deps '("digimon" "scribble-lib" "racket-doc"))

(define version "1.0")
(define pkg-authors '("WarGrey Gyoudmon Ju"))
(define test-omit-paths 'all)

(define typesettings '(["literacy/AoC.scrbl" xelatex #:always-make]))
