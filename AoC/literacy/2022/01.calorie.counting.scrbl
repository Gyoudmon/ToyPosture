#lang scribble/manual

@(require "../literacy.rkt")

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{Day 1: Calorie Counting}

丛林杂草丛生，没法使用交通工具，也没法空降，因此精灵们传统上只能徒步远征。
当小船靠岸时，精灵们开始清点存货，食物是重点清点对象之一。具体来说，
也就是每个精灵携带的卡路里数量。

于是，精灵们把各自携带的各种肉类、点心、口粮等食物的卡路里数写在纸上，
一行一项，不同精灵之间用空行隔开。比如，假如精灵们出了如下一份清单：

@tabular[#:style 'boxed
         (list (list @racket[1000]) (list @racket[2000]) (list @racket[3000])
               (list @hspace[1])
               (list @racket[4000])
               (list @hspace[1])
               (list @racket[5000]) (list @racket[6000])
               (list @hspace[1])
               (list @racket[7000]) (list @racket[8000]) (list @racket[9000])
               (list @hspace[1])
               (list @racket[10000]))]

从中可以看到五只精灵所携带的卡路里：

@itemlist[

 @item{第一个精灵带了含 @racket[1000], @racket[2000] 和 @racket[3000] 卡路里的食物，总计 @racket[6000] 卡路里。}  
 @item{第二个精灵只带了一种含 @racket[4000] 卡路里的食物。}
 @item{第三个精灵带了含 @racket[5000] 和 @racket[6000] 卡路里的食物，总计 @racket[11000] 卡路里。}
 @item{第四个精灵带了含 @racket[7000], @racket[8000] 和 @racket[9000] 卡路里的食物，总计 @racket[24000] 卡路里。}
 @item{第五个精灵也只带了一种含 @racket[10000] 卡路里的食物。}

 ]

当精灵饿了或馋了时，它们需要知道问谁去要零食，即需要知道谁带的卡路里最多。
上面的例子中，是带了 @racket[24000] 卡路里第四只精灵。

于是，第一题，找到携带卡路里最多的精灵，@question{它一共带了多少卡路里}？

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
