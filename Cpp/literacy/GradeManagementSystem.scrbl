#lang scribble/acmart @manuscript @nonacm @screen @timestamp

@(require (except-in "literacy.rkt" author))

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-title[#:author @author{@unsafe-author-info["WHOAMI"]}]{学生成绩管理系统}
@authorsaddresses{@unsafe-author-info["ADDRESS"]}
 
@abstract{目前的义务教育领域，STEM 教育存在的主要问题包括靠谱的老师太少，
 因此少部分重视 STEM 教育的私立学校会将整课外包给校外STEM教育机构。
 而对于 STEM 教师来说，以外包合作的方式参与义务教育需要解决的问题包括如何有效地了解学生情况。
 具体来说，一是如何快速记忆学生的姓名；一是如何从多个维度了解学生对STEM的学习情况。
 因此，本学生管理系统将为解决这两个问题提供一个初步的方案。}

@handbook-smart-table[]

@include-section{build.scrbl}
@include-section{schema.scrbl}
@include-section{design.scrbl}

@section{结语}

因时间有限，且我的游戏引擎还不够完善，本系统还有很大的改进空间。
比如可视化班级成绩分布曲线、学生能力雷达、学生能力成长曲线等。

不过，相对于这些工具性的东西，如何做STEM教育本身才更值得思考和研究。
但愿这些工具真能发挥作用。

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-appendix[#:index-section? #false #:numbered? #false]
