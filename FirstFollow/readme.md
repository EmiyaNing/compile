#  First && Follow
## 大连理工大学软件学院，编译原理上机代码
> 该代码在求follow集的时候存在段溢出错误，初步估计应该是代码中某处没有涉及好，导致了无限递归的情况。
> 测试代码:
> S->BA
> A->BS|d
> B->aA|bS|c
> 测试无误。
> 测试结果:
> First A: a b c d 
> First B: a b c
> First S: a b c
> Follow S: $ a b c d 
> Follow A: $ a b c d
> Follow B: a b c d
>
> 测试代码:
> A->BCDE
> B->aBA|~
> C->F|~
> D->b|c|~
> E->e|~
> F->d|~
> 该代码计算出的follow集任然存在大问题。。。
> first A: a b c d e ~
> first B: a ~
> first C: d ~
> first D: b c ~
> first E: e ~
> first F: d ~
> follow A: $ d
> follow B: $ a b c d e
> follow C: $ b c d
> follow D: $ e
> follow E: $ d
> follow F: $ b c d
> 初步估计，应该是在求followA的过程中出现了问题。。导致后续均出现了问题.