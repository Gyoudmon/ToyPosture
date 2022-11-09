#lang info

(define collection 'use-pkg-name)
(define pkg-desc "实验物品管理系统")

(define deps '("digimon"))
(define build-deps '("digimon" "scribble-lib" "racket-doc"))

(define version "1.0")
(define pkg-authors '("wargrey"))
(define test-omit-paths 'all)

(define typesettings '(["ExperimentalApparatusManagementSystem.scrbl" xelatex #px#"vsdx/\\w+[.]png$"]))
