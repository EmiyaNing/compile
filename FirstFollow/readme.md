#  First && Follow
## 大连理工大学软件学院，编译原理上机代码
> 不知道第几个的version，现在该代码应该不存在大的bug了,现在的代码已经能完美跑通第二份测试代码。
> 测试代码:
> S->BA
> A->BS|d
> B->aA|bS|c
> 测试无误。
> 测试结果:

>> First A: a b c d 

>> First B: a b c

>> First S: a b c

>> Follow S: $ a b c d

>> Follow A: $ a b c d

>> Follow B: a b c d


> 测试代码:

>> A->BCDE

>> B->aBA|~

>> C->F|~

>> D->b|c|~

>> E->e|~

>> F->d|~

> 测试结果:

>> first A: a b c d e ~

>> first B: a ~

>> first C: d ~

>> first D: b c ~

>> first E: e ~

>> first F: d ~

>> follow A: $ a b c d e

>> follow B: $ a b c d e

>> follow C: $ a b c d e

>> follow D: $ a b c d e

>> follow E: $ a b c d e

>> follow F: $ a b c d e

> 上个版本的follow集求解函数中，如果出现: 程序求解follow A发现需要先求解follow B，然后在求解follow B的过程中发现求解follow A.按照老师介绍给我们的思路，我们在进行求解这种情况的时候我们需要令follow A = follow B。在之前的bug版本中，我直接让程序在进入: follow A --> follow B --> follow A情况下，直接在第二个follow A 处让该递归返回。

> 然后我发现，采用上面的逻辑会出现，在求解follow A的时候求出的follow B 并不是完整的，在后面程序再对follow B 进行求解的时候，添加的新的结果无法同时更新follow A。

> 应此在这个版本的代码中，在应对follow A == follow B的情况，我直接令指向followA 的follow缓冲区指针指向 follow B的follow 缓冲区。这样，follow A follow B 共用同一片缓冲区，从而实现了物理意义上的真正相等，同时节省了空间。。。
