#lang scribble/manual

@(require (except-in "literacy.rkt" author))

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-title[#:author @author{@unsafe-author-info["WHOAMI"]}]{学生成绩管理系统}
 
@handbook-smart-table[]

@include-section{build.scrbl}
@include-section{schema.scrbl}
@include-section{design.scrbl}

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-appendix[#:index-section? #false #:numbered? #false]
