# ZXEngine

这个项目是我个人为了学习游戏引擎技术创建的，目前同时支持Vulkan，DirectX 12和OpenGL。并且支持光线追踪渲染管线(基于Vulkan，暂未实现基于DirectX12的光线追踪)。

I created this project to learn and practice game engine technology, It now supports Vulkan, DirectX 12 and OpenGL.And it supports ray tracing rendering (only based on Vulkan for now, not support DXR).

截图展示(介绍后面有更多展示):

Screenshot display (more display later in the introduction):

![](https://github.com/AshenvaleZX/ZXEngine/blob/master/Documents/Images/Engine%20Show%200.png)

## 引擎简介 (Engine Introduction)

本引擎使用自创的zxshader语言来编写shader，同时支持Vulkan，DirectX 12和OpenGL。

This engine uses the self-created zxshader language to write shaders, and supports Vulkan, DirectX 12 and OpenGL at the same time.

引擎本身用C++开发，GamePlay层使用Lua开发，引擎层封装部分C++接口给GamePlay层的Lua调用。使用方式类似Unity的XLua，通过一个GameLogic组件把Lua代码绑定到GameObject上，接收所挂载对象上来自引擎的Start和Update调用，并通过self访问GameObject对象(具体示例看后面)。

The engine is developed with C++, the GamePlay layer is developed with Lua, and the engine encapsulates part of the C++ interface to the Lua call of the GamePlay layer. The usage is similar to Unity's XLua, you can bind the Lua code to the GameObject through a GameLogic component, receive Start and Update calls from the engine on the mounted object, and access the GameObject object through self (see later for specific examples).

项目目前还比较简单，不过完成了引擎所需的基本的场景，预制体，材质，shader文件系统。有一个类Unity的引擎编辑器页面。场景中的对象都是Unity式的 GameObject - Component 结构。

The project is still relatively simple, but the basic scenes, prefabs, materials, and shader file systems have been completed. There is a Unity-like engine editor. Objects in the scene are Unity-style GameObject - Component structures.

## 细节图片展示 (Detailed picture display)

下面是一些图片展示，引擎页面如图，和Unity编辑器类似。上面是主菜单栏，中间是游戏画面窗口，左边是展示场景中GameObject的Hierarchy面板，下面左边是展示当前打开项目的Assets文件目录的Project面板，中间是展示日志的Console面板，右边是展示当前选中对象详细信息的Inspector面板。

The following are some picture displays, the engine is shown in the figure, which is similar to the Unity editor. The top is the main menu bar, the middle is the game screen window, the left is the Hierarchy panel showing the GameObject in the scene, the lower left is the Project panel showing the Assets file directory of the currently opened project, the middle is the Console panel showing the log, and the right is showing the currently selected Inspector panel for object details.

![](https://github.com/AshenvaleZX/ZXEngine/blob/master/Documents/Images/Engine%20Show%201.png)

当前选中了场景中的Sun对象，Inspector面板就展示这个GameObject所挂载的Component信息。

The Sun object in the scene is currently selected, and the Inspector panel displays the Component information on the GameObject.

点击主菜单栏的开始按钮，游戏开始运行，此时能看到场景中物体动了起来，粒子系统也开始工作。

Click the start button on the main menu bar, and the game starts to run. At this time, you can see the objects in the scene move, and the particle system starts to work. 

接下来展示的是一个基于PBR+IBL场景渲染，同时展示了点击材质文件后Inspector面板展示的材质信息和材质预览。

Next picture is a scene rendering based on PBR+IBL, and also shows the material information and material preview displayed on the Inspector panel after clicking the material file.

![](https://github.com/AshenvaleZX/ZXEngine/blob/master/Documents/Images/Engine%20Show%202.png)

然后通过双击场景文件，切换到一个使用光线追踪渲染管线渲染的场景。同时展示了点击模型文件后Inspector面板展示的模型信息和模型预览。

Then switch to a scene rendered with the ray tracing pipeline by double-clicking on the scene file. At the same time, it shows the model information and model preview displayed on the Inspector panel after clicking the model file.

![](https://github.com/AshenvaleZX/ZXEngine/blob/master/Documents/Images/Engine%20Show%203.png)

下面是点击图像展示图像信息。

Click on the image to display the image information.

![](https://github.com/AshenvaleZX/ZXEngine/blob/master/Documents/Images/Engine%20Show%204.png)

下面是分别点击zxshader和Lua后Inspector上的代码预览。

The following is the code preview on the Inspector after clicking zxshader and Lua respectively.

![](https://github.com/AshenvaleZX/ZXEngine/blob/master/Documents/Images/Engine%20Show%205.png)

![](https://github.com/AshenvaleZX/ZXEngine/blob/master/Documents/Images/Engine%20Show%206.png)

## 引擎文件格式介绍 (Engine File format introduction)

***.zxscene**

场景文件，包含GameObjects，天空盒等。如果是光线追踪场景，还包含了光追管线的Shader。

Scene files, containing GameObjects, skyboxes, etc. If it is a ray tracing scene, it also includes the Shader of the light tracing pipeline.

***.zxshader**

这是本引擎自己的shader语言文件，不过目前zxshader仅支持DirectX 12，Vulkan和OpenGL的光栅化渲染管线。示例代码可以在ExampleProject\Assets\Shaders中找到。

This is ZXEngine's own shader language file, but currently zxshader only supports the rasterization rendering pipeline of DirectX 12, Vulkan and OpenGL. Example code can be found in ExampleProject\Assets\Shaders.

***.zxmat  *.zxrtmat**

分别是光栅化渲染管线和光线追踪渲染管线的材质文件。

They are the material files of the rasterization rendering pipeline and the ray tracing rendering pipeline respectively.

***.zxprefab**

预制体文件，和Unity的prefab差不多。

The prefab file is similar to Unity's prefab.

***.zxprjcfg**

由ZXEngine创建的游戏项目工程的配置文件，可以在ExampleProject中找到参考示例。

This is the configuration file for the game project created by ZXEngine, you can find the example in ExampleProject.

***.rgen  *.rmiss  *.rchit  *.rahit  *.rint**

光线追踪管线的各阶段Shader代码文件。目前暂未向光栅化管线那样，搞一个引擎专有的通用语言格式。

Shader code files for each stage of the ray tracing pipeline. At present, there is no engine-specific universal language format like zxshader in the rasterization pipeline.

**Others**

模型，纹理贴图，字体等就是常见的通用文件格式。

Models, textures, fonts, etc. are common file formats.

###        GamePlay层的Lua代码示例(Lua code example for GamePlay layer)

以控制GameObject旋转移动举例，创建一个Lua代码，用GameLogic组件挂在到一个GameObject对象上：

Take controlling the rotation and movement of GameObject as an example, create a Lua code, and use GameLogic component on a GameObject object:

![](https://github.com/AshenvaleZX/ZXEngine/blob/master/Documents/Images/GameLogic.png)

然后Lua代码大致如下：

Then the Lua code is roughly as follows:

```lua
local ObjectMove = NewGameLogic()

ObjectMove.radius = 20
ObjectMove.angle = 0
ObjectMove.rot = 0

function ObjectMove:Start()
    self.trans = self.gameObject:GetComponent("Transform")
end

function ObjectMove:Update()
    self.angle = self.angle + 2 * Time.GetDeltaTime()
    local x = math.sin(self.angle) * self.radius
    local z = math.cos(self.angle) * self.radius
    self.trans:SetPosition(x, 0, z)

    self.rot = self.rot + 50 * Time.GetDeltaTime()
    if self.rot > 360 then
        self.rot = self.rot - 360
    end
    self.trans:SetEulerAngles(0, 0, self.rot)
end

return ObjectMove
```

### 注意事项 (Precautions)

目前zxshader编写好后，在DirectX 12和OpenGL下直接运行即可。但是在Vulkan下需要先点击引擎菜单栏里的“Assets/Compile All Shader for Vulkan”按钮，将zxshader预编译后才可以运行。

### 一些废话 (Some mumbles)

其实工程在2020年5月就创建了，但是最初其实只是想把2020年初学习OpenGL写的代码保存到GitHub上而已。当时写的代码也只是C语言风格的面向过程式编程，一个1000多行的渲染demo。随后花了1年多时间把《Real-Time Rendering 4th》看了，这期间光看书没再实际写东西了（因为这书实在是太偏理论了，没什么可以立刻实践的东西）。

In fact, the project was created in May 2020, but at first I just wanted to save the code I wrote when I learned OpenGL in early 2020 to GitHub. The code I wrote at that time was only procedure oriented programming in C language style, a rendering demo with less than 2000 lines. Then I spent more than a year reading "Real-Time Rendering 4th". During this period, I just read the book and didn't actually write anything (because this book is too theoretical, and there is nothing that can be practiced immediately).

其实当时那个1000多行的demo写完我还想继续加点东西上去，但是感觉加不动了，面向过程的代码实在是难扩展。就一直想把这个demo改成一个正式一点的，有渲染框架的工程，但是不知道从何改起，就一直没动，先看书去了。书看完了还是不知道怎么开始，就沉寂了几个月。最后等到了2022年我才开始正式搭建这个工程，准备开发成一个自己的简单游戏引擎。其实我当时应该建一个新的GitHub仓库的，而不是用这个学习OpenGL的仓库。但是当时对C++工程还很陌生，感觉各种环境配置和库链接太麻烦了，就偷懒直接用这个已经搭好的OpenGL开发环境开始搭建引擎了。所以之前写的demo代码还遗留在这里面的。

In fact, after the demo was written, I wanted to continue to add something, but I felt that it couldn't be done, because the procedure oriented code was really difficult to expand. I have always wanted to make this demo a more formal project with a rendering framework, but I don't know where to start, so I haven't moved, and I read the book first. After reading the book, I still didn't know how to start, so I fell silent for several months. Finally, in 2022, I started to build this project, preparing to develop a simple game engine of my own. In fact, I should have built a new GitHub repository instead of using this repository for learning OpenGL. But at that time, I was still very unfamiliar with C++ projects. I felt that various environment configurations and library links were too troublesome, so I was slack off and started building the engine directly with this already established OpenGL development environment. So the demo code written before is still left here.

这个工程我准备用自己工作之余的时间长期更新下去，慢慢完善，添加更高级的特性。也把这个工程当作自己一个学习技术的平台，有什么感兴趣的技术可以在自己这个工程里实验一下。也希望同样对引擎技术感兴趣的人可以一起分享。因为我之前沉寂了几个月迟迟没有开始，就是因为不知道从何开始。我想找一个简单的参考项目，但是找不到。要么就是UE5这种过于庞大和成熟的引擎，要么就是一些很老的比如Ogre这样的引擎，反正我没有找到一个合适的项目让我学习。我这个项目有很多实现是自己瞎想的，还比较简单，换句话说也比较简陋。所以有大佬看到感觉写的不好的地方，欢迎提出建议。可以发邮件给我，ashenvalezx@gmail.com

I plan to use my spare time to keep updating this project, gradually improve it, and add more advanced features. I also regard this project as a platform for my own learning. I can experiment with any interesting technologies in my own project. I also hope that people who are also interested in game engine technology can share it together. I waited for months and didn't start the project because I didn't know where to start. I'm trying to find a relatively simple project to learn from, but can't find it. Either it is an overly large and mature engine like UE5, or some very old engines such as Ogre. Anyway, I didn't find a suitable project for me to learn. Many of the implementations of my project are thought by myself, and they are relatively simple, in other words, they are relatively crude. So if someone sees some implementations that are not good, suggestions are welcome. You can email me, ashenvalezx@gmail.com

