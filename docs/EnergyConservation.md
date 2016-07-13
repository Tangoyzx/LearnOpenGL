ENERGY CONSERVATION IN GAMES

http://www.rorydriscoll.com/2009/01/25/energy-conservation-in-games/

最近我在工作中与一个同事聊天，聊到了镜面反射的能量守恒。这让我记起了我有一篇写了一段时间的关于这个话题的博客，所以我觉得是时候完成它了。

首先，我想要先从标准漫反射模型开始。在游戏中，用于计算特定光的漫反射的经典公式是：

![energy_conservation_1](https://github.com/Tangoyzx/LearnOpenGL/blob/master/docs/imgs/EnergyConservation_1.png)

Cd是材质的漫反射颜色，Li是灯光颜色，N是法线，L是指向光源的归一化向量。这个公式有什么问题？它并非能量守恒的。对于公式本身，由于我们在游戏中并不会计算光之间的多重弹射，也不会往场景添加光弹射的额外的能量（而ray tracer中会这么做），所以这并不是一个问题。但是这是一个好的讨论起点。

# 能量守恒

像名字所说，能量守恒是反射模型的一个限制，指的是反射光的总量不能超过入射光。这听起来很合理，但经常没有实现。说明此限制的更正式的方式是：

