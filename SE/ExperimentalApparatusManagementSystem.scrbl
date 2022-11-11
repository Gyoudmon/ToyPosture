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
@include-section{literacy/component.scrbl}
@include-section{literacy/schema.scrbl}

@section{结语}

客观的说，本系统的典型特征是，简单的部分超级简单，复杂的部分又极其复杂。
因此开发任务能很快步入轨道，然后真要完全达到我们预设的目标和设计的方案，还有很多技术难题需要攻克。

比如说，如何实时获知实验物品的位置？完成此目标所依赖的技术已经超出软件系统的范畴，而达到计算机系统的层面。
增加一些硬件设备有助于解决这个问题，但也不能完全解决。比如，登记入库时给所有物品都贴上智能标签，
这对于非耗材类物品有帮助，对其他物品就比较麻烦，关键不划算。

软件系统得以有效发挥其功能的前提是软件的目标用户愿意更新使用习惯，
这个对于该行业的从业人员来说客观上是个不小的问题，
行业尚不成熟、工作事务杂乱是主要原因之一。

就本系统自身而言，其方向是可以的，相信随着 STEAM 和计算机科学的融合逐渐深入，
本系统和相关教育系统应当会有更紧密的联动。

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-appendix[#:index-section? #true #:numbered? #true
                   (book-bib-entry "SEAPA" "Software Engineering: A Practitioner's Approach"
                                   (authors "Roger S. Pressman" "Bruce R. Maxim") "McGraw Hill Higher Education"
                                   #:date "2014")]
