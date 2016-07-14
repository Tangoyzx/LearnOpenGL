ENERGY CONSERVATION IN GAMES

http://www.rorydriscoll.com/2009/01/25/energy-conservation-in-games/

最近我在工作中与一个同事聊天，聊到了镜面反射的能量守恒。这让我记起了我有一篇写了一段时间的关于这个话题的博客，所以我觉得是时候完成它了。

首先，我想要先从标准漫反射模型开始。在游戏中，用于计算特定光的漫反射的经典公式是：

![energy_conservation_1](https://github.com/Tangoyzx/LearnOpenGL/blob/master/docs/imgs/EnergyConservation_1.png)

Cd是材质的漫反射颜色，Li是灯光颜色，N是法线，L是指向光源的归一化向量。这个公式有什么问题？它并非能量守恒的。对于公式本身，由于我们在游戏中并不会计算光之间的多重弹射，也不会往场景添加光弹射的额外的能量（而ray tracer中会这么做），所以这并不是一个问题。但是这是一个好的讨论起点。

# 能量守恒

像名字所说，能量守恒是反射模型的一个限制，指的是反射光的总量不能超过入射光。这听起来很合理，但经常没有实现。说明此限制的更正式的方式是：

![energy_conservation_2](https://github.com/Tangoyzx/LearnOpenGL/blob/master/docs/imgs/EnergyConservation_2.png)

函数ρ代表着BRDF，可以从最简单的Lambertian漫反射模型到复杂的微面模型。不管怎样，能量守恒依然存在。

# 漫反射能量守恒

我将要展示为什么上面的漫反射光照等式不是能量守恒，并且怎样令到它符合。先把BRDF换成漫反射颜色常量开始。

![energy_conservation_3](https://github.com/Tangoyzx/LearnOpenGL/blob/master/docs/imgs/EnergyConservation_3.png)

入射光方向在这里被修正，我们可以对所有出射光方向进行积分。因此，Cd和Li都可以作为常量提到积分外。同样，入射光出现在不等式的两边，所以可以除以Li。

![energy_conservation_4](https://github.com/Tangoyzx/LearnOpenGL/blob/master/docs/imgs/EnergyConservation_4.png)

（中间解开积分过程不详述）

得到的最后的不等式为：

![energy_conservation_5](https://github.com/Tangoyzx/LearnOpenGL/blob/master/docs/imgs/EnergyConservation_5.png)

假设我们想要把我们漫反射材质颜色保持在[0, 1]中，也就是说我们需要把它处以π去保持能量守恒。由于这只是一个常量缩放，所以在游戏中假如灯光模型只用漫反射的话可以不这么做。大多游戏用复杂的反射模型，最少会包括镜面反射。

# 镜面能量守恒

标准的Blinn-Phong镜面模型同样不是能量守恒。事实上，在某些意义上它比漫反射模型更糟糕，因为当增加specular power，你就损失更多的能量。这个问题的表现在美术人员很难获取一个小的镜面高光。

这里有一个用三个不同的specular power渲染的球体例子。注意有多少灯光在左边的图像中，并且有多少消失在右边，尽管它应该更加集中。这时美术人员就会开始叫你增加斜面的镜面反射颜色来补偿。这并不是个好的主意！

![energy_conservation_6](http://www.rorydriscoll.com/wp-content/uploads/2009/01/nonenergyconserving.png)

为了避免提高镜面反射的颜色，你需要一个能量守恒的镜面模型。假如你这么做了，同样specular power的同样的球体应该像这样：


![energy_conservation_7](http://www.rorydriscoll.com/wp-content/uploads/2009/01/energyconserving.png)

左边球体上镜面反射比非能量守恒模型的要暗淡。这是因为服能量守恒的镜面模型反射太多光了。随着specular power增加，我们补偿了能量的损失，所以在右边的球体会获得一个很好的小的亮点。

一般的Blinn-Phong反射模型是：

![energy_conservation_8](https://github.com/Tangoyzx/LearnOpenGL/blob/master/docs/imgs/EnergyConservation_6.png)

有些人知道的为了保持能量守恒，有一个归一化系数是：

(n+2) / 2π

所以Blinn-Phong镜面反射的能量守恒函数是：

![energy_conservation_9](https://github.com/Tangoyzx/LearnOpenGL/blob/master/docs/imgs/EnergyConservation_7.png)

（这里开始作者没有说明这个归一化系数的来历。下面大概说说另外一篇文章http://www.farbrausch.de/~fg/stuff/phong.pdf，给出的解释。）

光照能量如下图不等式的左方
![energy_conservation_10](https://github.com/Tangoyzx/LearnOpenGL/blob/master/docs/imgs/EnergyConservation_2.png)

由于要的是归一化系数，也就是求出最大镜面反射比的时候的光照亮度值，所以可以认为N和L相同，也就是说R也等于N。那么NdotV也就等于RdotV，假设都称为cos(x)，对于Phong来说的光照应该就是integrate(RdotV^n * NdotV, x, 0, Ω)乘以光的颜色，相当于integrate(cos(x)^(n+1), x, 0, Ω)，积分得(2π / n + 2)，所以要除以它以归一化，所以Phong的归一化系数是((n + 2) / 2π)

同理可计算Blinn-Phong的近似系数，由于N=L，所以NdotH就等于cos(x / 2)，所以光照应该是integrate(cos(x/2)^n * cos(x), 0, Ω)，计算出来的答案会很复杂，所以一般用的都是简化版本(n + 8)/8π