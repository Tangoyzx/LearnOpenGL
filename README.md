# LearnOpenGL

 OpenGL练手代码
 主要参考http://bullteacher.com/

试验各种效果

## ExampleDeferred （2016/6/7）

简单的延迟渲染实例，用简单的Phong

## ExamplePBR （2016/6/8）

观察各个PBR模型的不同，暂时只有Klay和Black OP里面的两种，都只有diffuse和direct spec

## ExamplePBR更新 + ExampleMipmap （2016/6/12）

修改了一下PBR，测试了一下mipmap准备把环境贴图应用到PBR上

## ExampleIBL

尝试实现一下UE4的IBL的效果，不太理想，roughness和NdotV的LUT好像0,0点有点问题

## ExampleDeferred 更新 + ExampleSSAO

Deferred的Depth保存1-深度，保证Clear的时候没有被绘制的点是最远端。

增加一个SSAO的例子，为了更明显使用的时候改成了ao的平方。