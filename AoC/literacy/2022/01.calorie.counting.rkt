#lang scribble/lp2

@(require "../literacy.rkt")

@(define-aoc-bib D1CC "Calorie Counting" 1)

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{Day 1: Calorie Counting}

故事原文见 @~cite[D1CC]。

丛林杂草丛生，没法使用交通工具，也没法空降，因此精灵们传统上只能徒步远征。
当小船靠岸时，精灵们开始清点存货，食物是重点清点对象之一。具体来说，
也就是每个精灵携带食物的卡路里含量。

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

从中可以看到五只精灵所携带食物的卡路里：

@itemlist[

 @item{第一个精灵带了三种食物，卡路里含量分别为 @racket[1000], @racket[2000] 和 @racket[3000]，总计 @racket[6000] 卡。}  
 @item{第二个精灵只带了一种食物，卡路里含量为 @racket[4000] 卡。}
 @item{第三个精灵带了两种食物，卡路里含量分别为 @racket[5000] 和 @racket[6000]，总计 @racket[11000] 卡。}
 @item{第四个精灵带了三种食物，卡路里含量分别为 @racket[7000], @racket[8000] 和 @racket[9000]，总计 @racket[24000] 卡。}
 @item{第五个精灵也只带了一种食物，含 @racket[10000] 大卡。}

 ]

当精灵饿了或馋了时，它们需要知道问谁去要零食，即需要知道谁带的卡路里最多。
上面的例子中，是第四只精灵携带的食物卡路里含量最高，达到了 @racket[24000] 大卡。

于是，为伺候好这些精灵，我们需要定义一个函数 @racketid[find-maximum-calorie]
来计算@question{携带食物所提供的卡路里最多的那只精灵到底提供了多少卡}，该函数接受一个参数
@racketvarfont{/dev/datin}，可以从里面读取精灵们写好的清单：

@handbook-chunk[<定义函数find-maximum-calorie>
                (define (find-maximum-calorie /dev/datin)
                  <读取-记录-替换-循环>)]

总的来说，这个任务比较简单。需要两个变量 @racketvalfont{max:cal} 和 @racketvalfont{self:cal}，
分别代表@racketcommentfont{已知最大卡路里数}和@racketcommentfont{当前精灵账目上的卡路里总数}。
检阅清单的过程就是个简单的@racketcommentfont{读取-记录-替换}循环（@racketidfont{rrsl}, read-record-substitute-loop），
即逐行读取清单，直到没有更多内容为止，此时已知的最大卡路里数就是最终的结果，将它作为函数返回值即可。
每读到一行，就尝试将该行内容转化成@bold{正整数}，如果能转化成功，说明读到的是卡路里含量，
应当记在当前精灵的账上；否则，准备换个精灵记账。

@handbook-chunk[<读取-记录-替换-循环>
                (let rrsl ([max:cal 0]
                           [self:cal 0])
                  (define line (read-line /dev/datin))
                  (if (string? line)
                      (let ([?cal (string->number line 10)])
                        (if (exact-positive-integer? ?cal)
                            (rrsl max:cal (+ self:cal ?cal))
                            <换精灵记账>))
                      max:cal))]

换精灵记账的逻辑不复杂，但比较考验细心程度。简单来说，无论当前精灵账目上的卡路里数是否最大，
下次记账时都应该清零；而只有当已知最大的卡路里数比当前卡路里数小时，才需要替换最大值。

@handbook-chunk[<换精灵记账>
                (if (< max:cal self:cal)
                    (rrsl self:cal 0)
                    (rrsl max:cal 0))]

接下来就可以通过传入清单文件来执行了：

@tamer-action[#:requires ["../literacy.rkt"]
              (with-aoc-data-from "2022/calorie.counting.dat"
                  find-maximum-calorie)]

这里多说一句，上面的算法是@bold{纯函数式}风格，虽然定义了两个@bold{变量}，
但实际上它们的值从头到尾都没有变过；每一轮循环都相当于是一次（递归）函数调用，
并且比对着那俩变量创建了新的位置，本该用来修改它们的值也被放到新的位置去了，
而它们原来的位置在哪，里面有什么都已经不重要了。这种思路需要专业引导，
初学者一时半会不能适应是正常现象。

当你在冥思苦想上面那都是些啥玩意的时候，精灵们又开始搞事了：天，这么点卡路里迟早会被消耗完的！
这可不行，你必须再找到另外两个所带食物提供了最大卡路里的精灵。这样，一个精灵携带的食物吃完了，
咱还有俩可以继续腐败。

还是看上面的例子，携带食物提供卡路里最多的三只精灵分别是第四只（@racket[24000] 卡）、
第三只（@racket[11000] 卡）和第五只（@racket[10000] 卡），
总计 @racket[45000] 大卡。

得，继续折腾，这次要计算@question{携带食物所提供的卡路里最多的前三只精灵所能提供的卡路里总和}。
精灵们的这两次问话都比较拗口，但其实不难看出，第一个谜题用第二个谜题的思路来说其实就是
@question{携带食物所提供的卡路里最多的前一只精灵所能提供的卡路里总和}，对不对？也就是说，
这两个谜题其实是一个问题，差别在于数量不同。

本来，你可能会想，不就是前三名吗，我设三个变量不就可以了吗？没错，确实可以，但就是感觉很业余的样子。
你就不怕精灵们又突然一拍脑袋说“我们要前六名的”，“前十名的”，“前n名的”…… 没完没了了。
因此，无论是计算机科学、还是数学，我们都希望能找到一个统一的、优雅的问题求解思路，
以免于被脏活累活所困、从而节省时间思考更有趣的问题。

我们需要关注的仍然是卡路里，只不过数量增加了。于是，自然而然地，@bold{不同数量的同类型数据}
可以构成数组、列表、元组等复合类型。在不同的语言里，这些数据结构都可以是很趁手的选择，
不过，为继续函数式风格，我这里选择用列表来实现。而在别的语言里，
比如 Python，不可变的元组才更符合函数式风格；在 C 里，开箱即用的选项就只有数组了，
也几乎没法难体现函数式风格。

@handbook-chunk[<定义函数find-maximum-calories>
                (define (find-maximum-calories /dev/datin n)
                  (let rrsl (<初始化卡路里列表>
                             [self:cal 0])
                    (define line (read-line /dev/datin))
                    (if (string? line)
                        (let ([?cal (string->number line 10)])
                          (if (exact-positive-integer? ?cal)
                              (rrsl calories (+ self:cal ?cal))
                              <换精灵记账:列表版>))
                        <高卡路里合计>)))]

相比于上面的 @racketid[find-maximum-calorie] 函数，这个复数形式的 @racketid[find-maximum-calories]
多了一个参数 @racketvalfont{n} 用以表示@racketcommentfont{前n名}。算法逻辑也还是大同小异，区别在于主循环里
代表最大值的整型参数变为了列表型参数：

@handbook-chunk[<初始化卡路里列表>
                [calories (make-list n 0)]]

最终结果变为了对列表的求和运算：

@handbook-chunk[<高卡路里合计>
                (apply + calories)]


以及，针对列表设计的换精灵记账逻辑：列表中如果存在比当前卡路里含量低的就替换之。

@handbook-chunk[<换精灵记账:列表版>
                (let* ([smaller (λ [max:cal] (< max:cal self:cal))]
                       [idx (index-where calories smaller)])
                  (if (not idx)
                      (rrsl calories 0)
                      (rrsl (list-set calories idx self:cal) 0)))]

至此，问题解决：

@tamer-action[(with-aoc-data-from "2022/calorie.counting.dat"
                find-maximum-calories 3)]

我们还可以对比验证这两个谜题在逻辑上的一致：

@tamer-action[(with-aoc-data-from "2022/calorie.counting.dat"
                find-maximum-calorie)
              (with-aoc-data-from "2022/calorie.counting.dat"
                find-maximum-calories 1)]

最后，为了让程序能正常运行，我们还需要写点额外代码来将上述代码碎片合并到正确的位置：

@handbook-chunk[<*>
                (module tamer racket/base
                  (require racket/list)
                
                  <定义函数find-maximum-calorie>
                  <定义函数find-maximum-calories>)]

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
