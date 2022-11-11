#lang scribble/sigplan @nocopyright

@(require (except-in "literacy/literacy.rkt" author))

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-title/pkg-desc[#:hide-author? #true]

@authorinfo[@unsafe-author-info["WHOAMI"] @unsafe-author-info["DEPARTMENT"] ""]
 
@abstract{近年来，随着STEAM教育在世界范围内的推广，各类STEAM校外教育机构也逐渐在各地兴起。
          这是好事，但是该行业的从业人员大多不具备专业的计算机工程技术，因此日常工作流程比较粗放；
          再加上行业刚刚起步，尚未发现特别成熟的信息系统可供从业人员选择。
          因此本项目尝试根据作者的实际工作经验给出一个初步的信息化解决方案，
          以解决实验物品的管理难题。}

@handbook-smart-table[]

@include-section{literacy/requirements.scrbl}
@include-section{literacy/architecture.scrbl}

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-appendix[#:index-section? #true #:numbered? #true
                   (book-bib-entry "SEAPA" "Software Engineering: A Practitioner's Approach"
                                   (authors "Roger S. Pressman" "Bruce R. Maxim") "McGraw Hill Higher Education"
                                   #:date "2014")]
