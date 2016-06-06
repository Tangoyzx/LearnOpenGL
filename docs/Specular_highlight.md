https://en.wikipedia.org/wiki/Specular_highlight

# 镜面高光【译】
镜面高光指的是闪亮物体被照亮时出现的光斑（例子如右图）。镜面高光对于3DCG而言很重要，因为他们对一个物体的形状提供了强烈的视觉效果，并且它反应了光源在场景中的位置。

## 微面 Microfacets

specular参数代表光以镜面的方式从光源被完美反射到观察者中。高光反射只有在平面法线正好是光源向量和视线向量的半向量的时候才会看到；这被称为半角（half-angle）向量因为他平分了入射光和观察者向量的角度。所以，镜面反射平面会以一个完美的光源反射形状显示一个镜面高光。

这可以被解释为微面。我们假设平面都不是完美平滑的，并且由很多微小的面组成，每个面都是一个完美的高光反射。这些微面的法线被分布在平面近似法线附近。微面法线在光滑平面法线附近分布的程度不同是由平面的粗糙程度决定的。物体上平滑法线接近半角向量的位置上，很多微面法线都是半角向量，所以高光很亮。越远离高光的中心，平滑法线和半角向量会分得越开；朝向半角向量的微面数量就会减少，所以高光强度就会减少到0.

镜面高光反射大部分的光源颜色，而不是物体的颜色。这是因为很多材质都有一层薄薄的清晰的材质在有颜色材质的平面上。例如，塑料是由有颜色的小珠在透明的聚合物内，而人类的皮肤择优一层薄薄的有或者汗在有颜色的细胞上。这种材质会显示镜面高光，在光谱的所有部分，都是同样的反射。在金属材质类似金，高光的颜色会反射材质的颜色。

## 高光的模型 Models of specular highlight

有很多不同的模型来预测微面的分布。大多假定微面法线均匀地分布在平面法线上；这些模型称为各向同性。假如微面分布依赖于平面的某个特定方向，那么这个分布就是各项异性的。

NOTE：对于大部分等式，当说![spec_highlight_1](https://en.wikipedia.org/api/rest_v1/media/math/render/svg/6600c2cda03eb3caf59db71966c6e7e11209c202)的时候代表着![spec_highlight_2](https://en.wikipedia.org/api/rest_v1/media/math/render/svg/c5afc755f4924c2f943f0fe2859de967b97d74b6)

### 庞氏分布 Phong distribution
在庞氏反射模型，高光强度是这么计算的：

![spec_highlight_3](https://en.wikipedia.org/api/rest_v1/media/math/render/svg/d97b296ed81f8e1ea176b84e6f8e5087e1ff6dc2)

R是光线向量对于平面的反射向量，而V是视线向量

在Blinn-Phong着色模型中，镜面高光强度是这么几岁按的

![spec_highlight_4](https://en.wikipedia.org/api/rest_v1/media/math/render/svg/94089fcf07095e47a10f2ae61888eace279b9373)

N是光滑平面的法线，H是半角向量

n被称为Phong指数，这是个用户选择的参数去控制平面的光滑度。这些等式意味着微面向量分布对于相应的角度是高斯分布近似的（Gaussian distribution），或者是Pearson type II distribution近似。这是个很有用的启发，并且可以产生可信的结果，但是它不是基于物理的模型。

对于另外一些类似的共识，只有计算上的不同：

![spec_highlight_5](https://en.wikipedia.org/api/rest_v1/media/math/render/svg/671b3f44e70be98ca58cd89dae8e257565d9ce3c)

R是眼睛反射向量，E是眼睛向量（视线向量），N是平面法线向量，所有向量都归一化，L是光线向量。例如
…………
…………
（各种计算就不贴图了）


### 高斯分布 Gaussian distribution

一个稍微好点的微面分布模型可以用高斯分布建模。

常用的计算镜面高光强度公式如下

![spec_highlight_6](https://en.wikipedia.org/api/rest_v1/media/math/render/svg/b2d0148237f67503a718bd6a47a01a2fe2acba73)

m是个0到1 的常数控制平面的光滑度。

### Beckmann distribution

一个基于物理的微面分布模型是Beckmann distribution

![spec_highlight_7](https://en.wikipedia.org/api/rest_v1/media/math/render/svg/673ecae72403fa8e61f0f4c8807cc8abc742f422)

m是微面的均方根斜率。与上面的经验模型比较，这个函数“给出了纯粹的反射率而又没引入任意常数；缺点是需要更多的计算量”。但是，这个模型可以被简化为![spec_highlight_8](https://en.wikipedia.org/api/rest_v1/media/math/render/svg/e4b7b7ceeecf0dadb7f91ced877ce78ea0e996a3)。同样需要注意cos(a)和平面分布函数的点积是在半球上归一化来满足这个公式。

### Heidrich-Seidel anisotropic distribution

### Ward anisotropic distribution

（各向异性分布先不翻译）

### Cook-Torrance model

Cook-Torrance模型使用下面这种形式的高光参数

![spec_highlight_9](https://en.wikipedia.org/api/rest_v1/media/math/render/svg/70b610bf99b3f0c30bf64e5e09a0b3ab58c5db9e)

这里D是上面提到的Beckmann分布因子，而F是Fresnel参数。

![spec_highlight_10](https://en.wikipedia.org/api/rest_v1/media/math/render/svg/4cda2a8e19014c1677e4d3381044251cd7e5d765)

因为性能的原因，在3D实时渲染图形中经常用Schlick近似来近似Fresnel

G是几何衰减项，描述微面理论中的自阴影，以下面的形式出现

![spec_highlight_11](https://en.wikipedia.org/api/rest_v1/media/math/render/svg/8185f28d2c83bd637a87e498f4246cd158ea7490)

在这等式中，V是视线向量，H是半角向量，L是光线向量，N是法线向量，a是H和N的夹角。

## 使用多个分布

在期望中，不同的分布（通常，使用同样的分布函数切改变m和n的值）可以通过权重平均来组合。这对建模很有用，例如，平面可以有小范围的光滑与粗糙补丁而不是均匀粗糙的表面。
