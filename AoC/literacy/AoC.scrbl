#lang scribble/report

@(require "literacy.rkt")
@(require "graphviz.rkt")

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-title/pkg-desc[]

@handbook-statistics[#:ignore '(#px"Cpp/digitama") #:filter '(#px"^AoC/")
                     #:gitstat-width 420 #:gitstat-radius 75
                     #:altcolors '(["Racket" . "Green"]
                                   ["Python" . "Khaki"])]

@handbook-smart-table[]

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-appendix[#:index-section? #true #:numbered? #false]
