

**腾讯小尺度景观簇生成 **

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.001.png)


# 目录
[**一、**	**背景和需求分析**	4](#_Toc106061004)

[（一）标签管理系统设计思路	5](#_Toc106061005)

[（二）现有植物簇生成工具和算法	5](#_Toc106061006)

[**二、创新点说明**	4](#_Toc106061007)

[**三、设计目标**	4](#_Toc106061008)

[（一）标签管理系统	5](#_Toc106061009)

[（二）植物簇生成插件	5](#_Toc106061010)

[**四、设计思路**	4](#_Toc106061011)

[（一）标签管理系统设计思路	5](#_Toc106061012)

[（二） 植物簇生成设计思路	5](#_Toc106061013)

[1、生成流程	5](#_Toc106061014)

[2、生成算法研究	5](#_Toc106061015)

[**五、技术方案**	5](#_Toc106061016)

[（一）总体技术方案	5](#_Toc106061017)

[（二）标签管理系统	5](#_Toc106061018)

[1、存储	6](#_Toc106061019)

[2、GUI界面	6](#_Toc106061020)

[3、IPC	6](#_Toc106061021)

[4、Server	6](#_Toc106061022)

[（三） 植物簇生成插件	5](#_Toc106061023)

[1、Plugin开发	6](#_Toc106061024)

[2、植物推荐算法	6](#_Toc106061025)

[3、植物簇生成算法	6](#_Toc106061026)

[4、自定义植物簇DataAsset 和PlantBunchManager	6](#_Toc106061027)

[5、植物簇美学比例优化	6](#_Toc106061028)

[6、针对不同生态系统的自适应参数调优	6](#_Toc106061029)

[7、对地形高度进行适配	6](#_Toc106061030)

[（四） 过滤器系统	5](#_Toc106061031)

[1、UI扩展实现	5](#_Toc106061032)

[2、过滤条件	6](#_Toc106061033)

[3、配置过滤器	6](#_Toc106061034)

[**六、实验验证**	6](#_Toc106061035)

[**七、环境配置**	9](#_Toc106061036)

[**八、参考文献**	9](#_Toc106061037)

[**附录1、组会记录**	11](#_Toc106061038)


**


1. **背景和需求分析**

（一）标签管理系统设计思路

随着科技的进步，数字技术的发展也越来越完善。数字技术的应用已经广泛渗透到人类社会生产、生活的各个领域。随着互联网的壮大，很大一部分社会活动都已在网络世界上开展。 在这个发展趋势下，无论是从建立元宇宙的角度，或是单纯的从开发游戏的角度, 研究如何简单快速地在数字世界里构建类真实世界的植被如森林和草原等是有重要意义的。

游戏里的植被是由大大小小不同的景观簇组成的。景观簇需要有一定的层级结构；同一层级植物会因为竞争关系而保持一定的疏密距离，不同层级、种类的植物则相互共生，如下图。在摆放景观簇的时候，不但要考虑到植物在现实世界里的相互依赖关系，还要考虑在游戏视角里画面的和谐和比例。

![A picture containing text, porcelain

Description automatically generated](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.002.png)

图1		景观簇概念图

植被作为美术流水线的重要一环，在整体项目进度中占据着比较重要的地位。在传统的植被制作过程中，美工需要根据游戏要求的生态环境，花费大量的精力来制作植被的模型（如图2），并对植被的摆放进行调整（图3）。在不同的生态区域，如雪山、雨林和草原，植物的多样性和分布都各有不同。 如果使用传统意义上的人工搭建，从搜集资料到植物建模，再把植物模型手工放置在地形上面，从对人力的需求和所需的时间来看是不现实的。这种传统的制作方式在小规模的地图制作中还能发挥应有的作用，但随着游戏品质的不断上升和地图的不断增大，纯人工的植被生成所需要的人力物力呈几何级数增长。

![See the source image](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.003.jpeg)

图2		Speed Tree 树木建模软件

![See the source image](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.004.jpeg)

图3		Unreal Engine 模型摆放界面

因此，程序化的植被生成一直是游戏业界所追求的，在近些年的游戏中，地形场景的程序化生成越来越普遍高效，这是游戏工业化进步的必然产物。自动化生成工具的优势在于能够极大地减少手工设计摆放植物的工作量，并且能够将一些生态学的知识，如植被共生关系、环境因素等，融入其中，进行一些较为复杂的计算，从而生成出比一般手工摆放更加契合现实植被的画面。

根据现有的计算机技术，结合植物学的知识，可以使用自动生成来构建需要的生态区域景观簇，如雪山、森林、雨林、草原、热带沙漠和戈壁等。首先，根据生态区域和层级对植物模型进行标签分类，再对应不同生态区域植物的分布多样性和气候变化，在地形图上自动生成相应的植物群落。

基于此，腾讯的魔方工作室提出了小尺度景观簇生成的需求，经过反复的讨论后，对于需求的整理如下：

- 根据地形上的锚点标签（tag）自动生成主干树，并设计推荐算法自动生成对应的植物簇
- 支持植物簇的单独编辑，如增删或者替换其中的某株植物
- 植物簇的生成需符合自然规律且具有一定的随机性。自然规律包括层级伴生关系、生态类型、环境变化引起的物种变化等。
- 在生成时还需要考虑地形的适配与剔除、构图控制、景观簇蔓延细节、纵向细节填充、颜色等美学特征等。
- 建立独立的标签管理系统，支持对UE项目中的植物模型进行标签的增删改查

（二）现有植物簇生成工具和算法

**基于houdini的植被生成**

Houdini是一款三维计算机图形软件，由加拿大Side Effects Software Inc.（简称SESI）公司开发。它是基于节点模式设计的产物，常用于影视行业，也被用来程序化地生成地形植被等游戏美术资源。Houdini 提供了一些宏观参数供用户设置，可以在一定范围内自动生成散点图，散点图中包含了模型的位置和方向，之后用户根据需要在 UE4 中给每个散点图配置对应的植被模型。

但不足之处在于不同植被之间在生态上的关联性没有很好体现在制作流程中，主要依靠用户进行设置。

**利用ue4已有的grass type 和procedural foliage进行生成**

Ue4提供了procedural foliage 模块供游戏制作者进行过程化的植被生成。其主要的参 数在每个植物模型对应的静态网格体植被中进行设置，每个静态网格体植被各自拥有一套参数，通过创建程序化植被体积和程序化植被阻挡体积，可以控制一定范围内生成或不生成植被，该模块会根据用户设置过程化地生成植被。UE4 程序化植被工具为每一种植被提供了较为丰富的参数可以设置，并能根据参数生成一定范围内的植被。

不足之处在于用户不能直接修改生成结果的局部细节。用户需要指定场景中包含的植物类型，并为其设置详细的参数以实现预期的效果。此外，UE4的程序化植被工具不支持读取用户布置好的植物簇来自动生成植被。

**游戏厂商内部使用的自研基于gpu的植被生成工具**

例如《地平线：零之曙光》中的基于mask的植被生成。

**较新的论文中的方法**

植被群的自动生成目前主要基于地形数据、非生物数据（包括阳光、湿度等自然环境数据）和用户自定义数据（通常为植被分布的草图）结合植被数据（包括植物生长年限、植物冠状半径等）生成对应的植物分布。

在植被的生成过程中，通常将植被分为三个层次：上层林木，下层林木和底层植被。

在生成的过程中，一般按照从上自下的顺序进行生成。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.005.png)

图4		植被层示意图

在上层林木生成的过程通过image2image 的gan网络，使用用户绘制的草图mask作为输入，增强用户输入信息，生成更为详细真实的上层林木分布图，让植被的分布更加合理。同时，在上层林木的生成过程中，也会通过迭代方法让树冠的覆盖面积趋于最大化。[1-2]

在完成上层林木的生成后，我们可以根据该分布计算下层的阳光接受率进而根据自然数据计算温度湿度等信息。通过这些数据，我们可以对于下层的植被覆盖进行模拟，这样，我们就可以将完整的植被覆盖情况通过相应的算法生成出来。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.006.png)

图5		分布和效果图



**二、创新点说明**

- 本项目的工具具有一些生态学的考虑，比如植物的共生关系，不同的光照、温湿度等环境下植被等不同的分布等，这样生成的植被能分布能够更加逼真，更不容易让玩家产生违和感。
- 本项目的工具在完成生成之后支持建模师对于生成的植被进行一定程度的调整和修改，这样很大程度上增加了我们工具的可用性。

**三、设计目标**

（一）标签管理系统

因为我们的算法是基于给植物资源打相应的标签，标签管理系统是用户同系统之间进行交互，对于植物资源的标签进行管理的界面，同时还需要提供能够和植物簇生成的主模块进行交互的接口。

标签管理系统这一部分的设计目标包括以下几点。

1. 能够很好地配合植物簇生成模块的运行，提供方便的接口使得植物簇生成模块能够获取已经纳入管理的植物资源的标签。
1. 提供方便的交互界面让用户对于植物资源进行纳入和管理，并且对于植物资源的标签能够方便地管理。在这一步需要着重考虑以人为本的人机交互体验，基于专业用户的用户定位，在体验方面尤其需要考虑的是效率问题。最开始建立项目时会有大量的资源需要导入系统，对于这一部分的工作效率是需要重点考虑的问题。
1. 为了系统的可扩展性，支持算法后续可能的优化，这一部分的设计尽可能地与植物簇生成算法解耦。

（二）植物簇生成插件

基于腾讯的魔方工作室提出的需求，植物簇生成插件有以下几个设计目标：

1. 根据地形上的锚点标签（tag）自动生成主干树，并设计推荐算法自动生成对应的植物簇
1. 支持植物簇的单独编辑，如增删或者替换其中的某株植物
1. 植物簇的生成需符合自然规律且具有一定的随机性。自然规律包括层级伴生关系、生态类型、环境变化引起的物种变化等。
1. 在生成时还需要考虑地形的适配与剔除、构图控制、景观簇蔓延细节、纵向细节填充、颜色等美学特征等。

**四、设计思路**

（一）标签管理系统设计思路

基于建模师在相关领域的专业性，我们将tag的设定全部交由用户管理，以便获取更高的工作效率。为了便于用户对于植物资源进行管理，我们单独开发了一套植物模型资源管理系统，方便用户在使用资源之外能够对资源进行统一的管理。用户在使用某一个植物模型之前，只需要将该资源导入我们的系统中，然后设定相应的tag即可。

为了方便用户统一管理，我们将所有项目的资源放置在了同一套系统中，这样用户可以从同一个入口进入系统，仅需要选择项目名就可以在同一个界面管理不同项目中的植物模型资源，降低操作成本。在界面的设计上，我们希望界面尽可能地简单明了，降低管理模型资源所需要的时间成本，而与之对应的是，作为专业应用，无需过度考虑界面的美观度。

这个界面支持用户对于植物资源进行增删改查，支持用户对于单个植物的标签进行增删改。用户可以选择一个数据库文件进行打开，界面会显示数据库中的所有植物，考虑到植物资源可能会同名，我们在数据库中使用其基于一个设定的母文件夹的相对路径作为资源的标识。在用户选择一个植物之后，就会显示这个植物的全部标签。在植物列表和标签列表的上方都有新建和删除的按钮。在植物列表的上方还有查询过滤的选项。

这个界面的开发需要着重考虑用户操作的效率。我们希望对于植物资源管理的流程对于用户足够友好，恰好我们小组成员在调试主模块的过程中，需要提前准备好已经打好标签的植物资源，因此我们在标签管理模块开发好第一版之后，由小组成员边使用边进行评估，并且做出了一些改进，极大地优化了用户体验。这些改进包括：

1. 对于缩略图的支持。最初的版本的界面中是不显示资源的缩略图的，而由于我们的资源名字采取相对路径的方式，加之大多第三方购买的资源命名较为混乱，用户难以通过名字对不同资源有直观的区分。因此我们加入了缩略图的显示，在数据库中同时存储资源的缩略图，以便用户在管理资源时对资源有直观的识别。
1. 对于批量导入的优化。最初的版本不支持批量导入，而在我们小组成员使用的过程中，发现当最初建立项目时，一个个导入资源的工作非常繁琐，效率低下。基于这点考虑，我们加入了批量导入的功能，能够在选择资源文件时多选，并且全部导入。而经过测试，批量导入需要花费大量的时间，因此需要增添一个进度条，让用户在等待时能够实时获知进度。
1. 有了批量导入之后，便需要考虑对一次导入的大量资源的处理。因为不论我们的算法如何，以及后续是否进行更改和优化，在系统中的资源必然大量拥有同样Key值的标签。而逐个标签添加又是一项非常繁琐的工作，因此我们增添了“植物模板”这一功能，用户可以将一个植物资源导出为模板，然后在导入新的资源之前可以选择模板，这样导入的新资源便会具有和模板植物相同的标签，用户只需要对其Value按需进行修改即可。
1. 图片的批量导入。有了缩略图功能，我们增添了对于图片的批量导入。用户可以通过在Unreal Engine中选取需要导出缩略图，然后对目标文件夹一键导出，这样缩略图就会被导出到目标文件夹中，并且层次结构和Unreal Engine项目中“内容”文件夹下一致。资源管理系统的用户界面能够自动识别这个层次结构，并且和数据库中资源名称的相对路径进行匹配，然后自动地将这些缩略图加入数据库中。
1. 快捷操作的支持。在最初版本中，用户只能通过鼠标点击来对资源进行管理，在资源数目较多时，逐个植物资源、逐个标签点击非常繁琐，因此我们增加了键盘的支持，用户可以通过按键盘上的Enter键快捷切换到下一个标签，如果当前标签是最后一个，就切换到下一个植物资源，并且在切换到下一个之后，输入会自动被定位到当前标签的Key上，用户可以通过按下TAB键切换到Value。这样用户在管理大量资源时，就可以通过“输入Key->TAB->输入Value->Enter”操作序列的不断循环而无需使用鼠标找到相应的按钮进行点击，提高了用户操作的效率，有类似“填写表格”的效率。
1. 对取色的支持，用户可以在缩略图中进行快捷取色，并且显示在界面中，以供用户编辑颜色相关的标签进行参考。

资源的存储方面，为了方便较多资源的快速导入，我们并没有将资源单独存储，而是将模型资源保存在Unreal Engine项目本身中，通过记录资源的文件名的方式，在我们的系统中保存其标签。这样做的好处在于，一是方便用户直接在Unreal Engine中管使用资源，能够对现有的已经在项目中包含较多模型资源文件的项目有较好的支持；二是避免了在导入操作时需要的额外的磁盘I/O操作占用额外的时间以及额外的存储空间。

除此之外，我们的设计中，还将资源管理系统和Unreal Engine完全解耦开来，这样不仅利于我们小组成员的开发，而且还有利于我们的资源管理系统的可扩展性，将来如果在其它开发平台部署我们的工具，这套解耦的资源管理系统便可以直接使用。因此，除了单独的管理界面，我们将自动化生成工具获取植物标签的接口设计为以单独的进程进行运行，并且在Unreal Engine的Plugin中开发相应的接口进行进程间通信以获取需要的信息。

因此，我们有关资源管理模块的总体架构设计如下图所示。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.007.png)

图6		系统架构设计



（二） 植物簇生成设计思路

1、生成流程

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.008.png)

图7		生成流程

自定义了两个类：Plant Bunch (自定义植物簇)和Plant Bunch Manager(自定义植物簇生成器)， 这两个类均实现了自定义面板。

2、生成算法研究

在植物簇的生成中需要考虑生态（层级分布、生态区域分布）和美学（园林景观搭配）的因素进行生成。

1、考虑因素一 : 层级分布

在计算机游戏、电影和虚拟环境等应用的自然场景中，植物是一个重要的视觉元素。在一个集体的植物分布里，需要根据地形和对应的生态区域放置植物，其中涉及到不同物种、高度和树冠密度。环境条件，如温度、阳光照射和水分，都会对生态系统中植物有所影响。

对自然环境的逼真描绘是计算机图形学的一个长期挑战。一个生态系统可以被分成不同结构层，由树冠层、林下层、灌木层、草木层、根基层、林地和藤蔓层组成 （如下图）。

![Permaculture Tip of the Day - The 7 Layers of a Forest – School of  Permaculture](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.009.jpeg)

图8		森林层级示例图

森林生态学的一个核心问题是如何理解和控制物种共存和群落结构的过程和机制，特别是在各种空间尺度上。 树木之間的空间是由环境和物种之间对食物的竞争能力来决定。学术界对在树冠层的主要树种的分布和密度也有分析和研究，如下表描述的是热带雨林中主要树种的分布 【1】。

![A screenshot of a computer

Description automatically generated with low confidence](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.010.png)

表1		热带雨林中主要树种分布

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.011.png)

图9		不同形状的树木

对于不同形状的树木，树底留下的空间大小不同，因此需要搭配不同的伴生植物，例如对于尖塔型植物下方没有多余空间，只可搭配一些草类植物。但是对于圆球型植物下方有较多的剩余空间，因此可以布置灌木，花草等多个层级。

2、考虑因素二：生态区域分布

根据世界自然基金会 (WWF)，生态区域可分为14种不同的类型，如表1所示。

![Table

Description automatically generated](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.012.png)

表2		世界自然基金会对生物群落和生态区域的分类 (Olson et al. 2001)

在不同的生态区域里，植物的分布密度和多样性都各有不同。以热带阔叶树林为例，常見树种有棕榈树、巴西坚果树、可可树和木棉树等，主要分布在树冠层和林下层。比较常見的附生植物（藤蔓层）为兰科植物（Orchidaceae）。除此之外，还有在灌木层和草木层的其他植物如草木植物胡椒科（Piperaceae）和无花果科（Moraceae），森林地里的酢浆草科（Oxalidaceae）等。(Morales, 2003)

又如在草原生态区域里，主要树种为木棉树、橡树和柳树，根据植物的生长高度，分布在树冠层、林下层、灌木层和草木层。灌木层和草木层里主要是菊花、康乃馨、金鸡菊、向日葵、三叶草和迷迭香等。林地里，有紫色针叶草、蓝草和水牛草。根据草原的湿度，在较湿润的地区，林地里的草会更高，土壤深而黑和肥沃。

3、考虑因素三：园林美学

根据树木数量由不同的搭配方式，通过参考风景园林搭配的书籍和资料总结如下：

a) 丛植：常见形式有两株丛植、三株丛植、四株丛植、五株丛植。基本原则是草本花卉配灌木，灌木配乔木，深色配浅色。在植物簇生成过程中在摆放植物的时候需要设计算法使得摆放对应下文的结构。

**两株丛植**

- 两株树的距离应小于小树树冠直径长度
- 按矛盾统一原理，两树必须既调和又对比，二者成为对立统一体。
- 两树须有通相，即采用同一树种（或外形十分相似的不同树种）。
- 须有殊相，即在姿态和体型大小上，两树应有差异，才能有对比。二株一丛，必须一俯一仰，一倚一直，一向左，一向右。

**三株丛植**

- `  `树木的大小、姿态要有对比和差异。
- `  `不要在同一条直线上。
- 忌等边三角形栽植。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.013.png)

图10	三株丛植示意图

**四株丛植**

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.014.png)

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.015.png)

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.016.png)

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.017.png)

图11	四株及五株丛植示意图

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.018.png)

图12	上树丛构图和分组形式

b) 园林植物的色彩搭配

- 单色应用 - 孤植
- 双色配合 - 补色

`	`红与绿: 大面积草坪上配置少量红色的花卉，浅绿色落叶树前栽植大红的花灌木或花卉

`	`黄与紫: 黄色郁金香与紫色郁金香，桔梗与黄波斯菊

- 双色配合 - 邻补色

`	`金黄色与大红色、青色与大红色、橙色与紫色

- 类似色配合

`	`金盏菊的橙色与金黄色、月季的深红色与浅红色搭配

- 多色配合
- ![A picture containing conifer, tree, plant

Description automatically generated](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.019.jpeg)
- ![A drawing of a tree

Description automatically generated with medium confidence](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.020.jpeg)
- 图13		观簇概念图

**

**五、技术方案**

（一）总体技术方案

经过与魔方工作室多次的直接或间接的沟通，经过一次较大的技术方案迭代后，我们发现彼此在对需求的理解上侧重有所不同。经过讨论后我们定下了如下改进版的技术方案。

改进后的方案更侧重于局部植物簇的自动生成。使用基于标签的推荐算法，根据给定的主干树标签生成对应的植物簇。标签的管理决定采用外部数据库，并使用单独的GUI窗口以支持对标签的编辑。整体流程图如下。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.021.png)
**
图14	整体流程图

（二）标签管理系统

1、存储

在存储方面，有两种方案，一种是通过文件保存json文件，这种方案的优点是轻量级，占用系统资源少，读取方法简单，但缺点是当面临大量数据时，由于直接查找效率低下，性能就会变得很差。另一种方式是通过后台运行一个例如MySQL、MongoDB之类的数据库服务，然后将我们的数据保存在数据库中，这种做法的优点是即便面临大量数据，也可以使用数据库的一些特性，做到较好的性能，但缺点是比起直接的文件操作会占用更多的系统资源，也增加了项目部署的复杂性。在权衡两者利弊之后，我们选择了一种折中的方案，即使用SQLite保存数据。SQLite作为一种轻量级的关系型数据库，无需运行任何额外服务，其所有操作逻辑都是集成在调用其API的进程当中，同时又具有关系型数据库的特点，支持建立索引等优化性能的方式，其数据库是以文件的方式放置在磁盘上的。这种方案的资源消耗和性能都介于大型数据库系统和直接的文件读取之间，比较适合我们的项目。

数据库设计的方面，我们将植物的Tag抽象为Key-Value对的形式。因为SQLite是关系型数据库，我们首先建立了植物的表，用于保存植物模型资源的文件名，同时建立两个Tag表，分别对应数值型Tag和自定义字符串型Tag，通过外键与植物表关联，每一行表示一个Tag。此外，我们使用了一张单独的标存储植物的图片，以实现按需加载。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.022.png)

图15	 数据库结构设计

这样，我们实现了下面的“植物”类的抽象

` 				`class Plant{

` 					`string name

`		            `array<byte>image

` 					`array<key,value>number\_tags

` 				    `array<key,value>string\_tags

` 				`}

`	`并且能够通过SQLite相关的接口通过SQL-query对其进行增删改查操作。

2、GUI界面

GUI界面方面，我们采用的是QT，原因主要是基于项目开发工作考虑：一是QT提供了很好的SQLite框架，我们只需要调用QT中框架中的相关类的函数，便可以方便地在目标文件上执行SQL语句；二是QT本身提供了很好的GUI组件，尤其在我们项目定位是专业人士使用，不必过分考虑美观的情况下，QT提供的组件的默认样式便可以直接使用，非常方便。例如，我们对于植物资源和标签的列表，直接使用了QT的WidgestList组件；对于进度条，也可以使用QT自带的ProcessBar组件，对于内容的显示便不需要花费过多精力进行开发。我们将数据库操作抽象成一个类，提供相应的接口，在这些接口中在数据库中应用SQL语句，而在GUI界面中在相应操作后对这些接口进行调用以实现对数据库的查询和更改。

最终我们实现的GUI界面如下图所示。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.023.png)

图16	标签管理GUI界面

3、IPC

常用的进程间通信方法包括命名管道、消息队列、套接字等，而其中大部分都是操作系统相关的，基于这些方法实现的进程间通信程序可移植性很差，因此我们选用了套接字的方式，在实现了本机进程间通信的同时，也为未来多设备扩展的可能性提供了一个接口。

基于Windows系统和包括Mac OS和Linux在内的Unix系系统，我们各自开发了一套套接字客户端接口，以实现对跨平台的支持。

4、Server

为了实现方便，同时为了较好的可移植性，我们选择了使用Python的网络编程模式，实现了一个小型的网络服务器来实现进程间的通信，并且使用python的SQLite接口对数据库进行读取，并且使用json格式进行数据在server和client之间的传递，实现了一个类似远程过程调用的抽象。同时，我们在Unreal Engine的插件中封装了一些通过调用Python提供的服务接口读取数据库相关信息的接口，这样在Plugin的相关开发中，便无需考虑这些实现细节，直接通过调用这些接口便可以获取植物的tag的相关信息。

（三） 植物簇生成插件

1、Plugin开发

`    `植物簇生成通过开发UE4 plugin完成。写Plugin 本质是修改源码，但是移植性强。在编写plugin过程中，每次修改需重新编译引擎。但是通过修改源码，增加了自由性，如可以修改Unreal Engine 的交互面板，自定义流程等。![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.024.png)

图：UE4 Plugin结构

2、植物推荐算法

1) 基础标签设计

为了实现推荐算法，需要给植物打上足以表征其基础外观特征的标签。根据目前推荐算法的设计，除了植物层级以外，我们为数据库中的植物添加了颜色和生态系统的标签。

1. 颜色标签组由三个数值型标签组成，由选取的植物特征颜色的HSV三个分量表示。
1. 生态系统标签为单个字符型标签，若植物可分布于多种生态系统，不同生态系统之间由‘/’分隔。
1) 推荐算法设计

输入：主树名称、目标植物层级、（可选）指定生态系统

输出：匹配植物的名称列表

算法流程：

1. 层级筛选

对于数据库中的植物，选取出层级符合目标植物层级的植物。

1. 颜色匹配

对于大部分层级，匹配HSV三值均与主树在一定偏差范围内的植物，其中色相的范围随主树饱和度的升高而缩小（类似HSV空间的距离计算）。各值的权重均经过实验调整。

对于花层级，取消色相匹配，允许其出现对比色，其余相同。

对于石头层级，由于其与主树在颜色上的关联度较低，取消色相匹配，并增大其他值的匹配范围。

1. 生态系统筛选

筛选出满足指定生态系统中任意一个的植物。若输入未指定生态系统，则使用主树的生态系统。

1. 自定义标签

对于用户自定义的其它标签，不作为筛选的依据，而是按照其与主树对应标签的匹配数量进行推荐结果的排序。

1) 推荐结果映射到蓝图

为了方便推荐模块的使用，对推荐接口进行了额外的封装，将推荐得到的植物名称映射到UE4中的蓝图。

1. 对植物名称做路径解析以生成对应的软对象路径。
1. 使用软对象路径获取蓝图的软引用。
1. 利用资源软引用，动态加载蓝图资源到内存中，并返回。

3、植物簇生成算法

根据不同的树木的形状，温度、湿度、碰撞体的大小（bounding box）、中心树形状(下层空间剩余) 、颜色等因素搭配下层林木

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.025.png)

图17		有代表性的不同形状的树种

根据上文提到的多丛树种结构总结了1-6株及以上植物的生成模式。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.026.jpeg)

图18		园林几何搭配总结

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.027.png)![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.028.jpeg)

图19		园林搭配的具体实现

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.029.png)

图20		单株

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.030.png)![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.031.png)

图21		双株

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.032.png)![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.033.png)

图22		三株

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.034.png)![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.035.png)

图23		四株

4、自定义植物簇DataAsset 和PlantBunchManager

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.036.png)

图24		植物簇界面，所有的参数都可以推荐生成或者手动调整

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.037.png)

图25		推荐获得的伴生植物的模型和数量

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.038.png)

图26		选择PlantBunchManager加入场景![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.039.png)

图27		PlantBunchManager detail 面板

选定好场景中的主干树和植物簇后，点击Recommend函数，系统从数据库中根据主干树的tag推荐伴生植物，用户可以浏览修改推荐结果，点击Generate Button, 进行地形高度匹配后生成植物。

5、植物簇美学比例优化

`	`中期之前的植物比例采用模型库里的模型原比例，但这样导致有些树木过于高大，花草灌木等比例过于失调。经过研究需求文档中给出的植物簇，我们调整了植物簇中树木的比例，减小了树木的高度。在中期答辩中老师指出我们的地面覆盖仍需改进。经过研究需求文档中给出的植物簇，我们增加了地表植被的稠密度。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.040.png)

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.041.png)

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.042.png)![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.043.png)

图28		期中时的植物比例，从整体远观时下层草被不明显，地表较光秃。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.044.png)

图29	经过改进的植物比例，可以看到树木的比例缩小，其他层级存在感加强，地面植被覆盖率提高。符合下图的植物簇配比要求。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.045.png)

图30	腾讯植物簇配比参考图

6、针对不同生态系统的自适应参数调优

我们针对热带、亚热带、沙漠、针叶林等气候进行了参数设计，包括稠密度，伴生植物的数量、种类、中心聚集程度等。例如对于沙漠气候，由于水分的限制，中心的聚集程度就较小，植物的稠密程度也较小。对于亚热带植物，地表的草被覆盖率就较高，中心聚集程度也较高。不同气候的植物簇生成效果如下图。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.046.png)

图31·	热带植物簇

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.047.png)

图32	亚热带植物簇

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.048.png)

图33	针叶林植物簇

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.049.png)

图34	沙漠植物簇

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.050.png)

图35	沙漠植物簇2

7、对地形高度进行适配

在生成植物时，由于地形存在高度，且地形高度会有所起伏，需要调整植物的z轴坐标对地形高度进行适配。UE4提供了SweepMultiByObjectType函数，该函数以类似射线检测的形式，在一定范围内进行碰撞检测，并返回碰撞的结果。使用SweepMultiByObjectType函数，在生成植物所在的位置，沿着x轴方向检测Landscape类型的物体，并获取碰撞点所在高度。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.051.png)如果直接将碰撞点的z轴坐标，作为植物模型的z轴坐标，则会出现植物半浮空的情况，这是由于植物在x和y轴上并非一点，而是存在范围。为了解决这一问题，需要对植物周围的地形数据进行采样，具体的采样点如下所示，其中红色点表示植物所在点，黑色点表示额外添加的采样点：

图36	采样点分布

`	`在对所有点进行采样后，选择其中的最低点的z轴坐标，作为植物模型的z轴坐标，从而能够避免植物浮空的问题。

`	`此外，从美观以及合理性的角度考虑，在地形过于陡峭的地方，如悬崖等，不宜生成植物，需要进行剔除。剔除的算法为在上述采样之后，记录最低点和最高点并求得其高度差，如果高度差过大，则判断该点位置不适宜生成植物。

（四） 过滤器系统

1、UI扩展实现

对过滤器的扩展使用UE4插件的方式实现。单个自定义过滤器的类继承自源码中的FFrontendFilter类，并指定其颜色、名称、提示的内容等。源码中提供了UContentBrowserFrontEndFilterExtension类作为过滤器的接口，通过该类的函数，可以添加新的自定义过滤器的对象，而源码的SFilterList脚本会遍历所有UContentBrowserFrontEndFilterExtension对象，并将过滤器添加到浏览器中。这部分涉及到原代码的ContentBrowser和ContentBrowserData模块，需要在插件的依赖模块进行添加。

目前的标签主要有两种类型，分别为颜色和生态系统。颜色标签使用HSV格式，其中的H值对颜色的影响最有辨识度，方便用户识别，因此过滤器中主要以H值的范围进行过滤。生态系统则以字符串的形式，检测数据库中对应值是否匹配进行过滤。

过滤器在编辑器中还会显示颜色以示区分。为了方便识别，颜色标签过滤器会取H范围的中间值作为显示的颜色值。而生态系统不一定具有明显的标志性颜色，但是纯随机会让用户难以形成记忆，因此设计中生态系统标签过滤器使用过滤器名称作为参数，以固定方式计算出一个数值，这样保证用户每次启动项目时看到的过滤器标签颜色是一致的。

2、过滤条件

每个过滤器在启用时对当前目录下所有的文件执行一次判断函数，检测其是否满足条件，返回一个布尔值表示该文件是否应当在当前过滤器条件下显示。为了减小数据库检索的压力，首先对文件的后缀名进行判断，只有.uasset后缀类型的文件才能通过过滤条件。之后获取当前文件的相对路径，并通过调用数据库和UE4通信的接口，将文件路径和需要检索的标签传递给数据库。最后获取数据库的结果，并判断标签对应值是否满足过滤条件。

3、配置过滤器

`	`考虑到用户需求的变化，本项目中过滤器并不是完全在代码中确定的，而是需要依赖项目中的FilterSetting.ini文件，用户可以在其中对过滤器的数量、类型、过滤条件进行修改。同时，该文件还支持以”// “开头的注释，在读取时会自动忽略。在项目启动时，插件中的代码会对该文件进行读取，并生成对应的过滤器。

**六、实验验证**

我们首先将植物资源纳入了数据库的管理当中。我们准备好植物资源放入Unreal Engine项目文件夹中，并且首先导出了其缩略图。然后我们使用标签管理的GUI界面，首先制作了我们需要的标签的植物模板，然后在GUI界面中应用这个模板，并且选中我们的植物资源文件，将其纳入到标签管理系统中，然后通过批量添加图片的方式，将其缩略图一并导入。然后我们对需要用到的植物资源的标签在模板的基础上进行了修改。

在这一部分的用户体验上，由于批量操作的引入，整个操作过程十分流畅，导入过程虽然等待时间较长，但是需要用户进行操作的时间不过一两分钟，并且在等待时有进度条提示。而在编辑标签的过程中，由于键盘操作的引入，整个过程也十分流畅，小组成员实验下来用户体验良好。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.052.png)

图37	导入植物资源

在植物簇生成上，经过流程的简化，用户只需要选择主干树和植物簇-> 推荐-> 生成这三个步骤，非常简单高效。在连接数据库获取推荐植物上只需要非常短的响应时间（<1s）。同时我们提供了很高的用户自由度，例如用户在得到推荐结果后可以自由更改植物簇配比，在生成植物簇后也可以在editor内修改植物的摆放。相比传统的植物生成工具更具有灵活度。我们也在实际的游戏场景中进行了测试如下：

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.053.png)

图38	游戏场景中选择主干树

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.054.png)

图39	游戏场景中生成植物簇

可以看到生成的热带雨林植物簇和环境融合的很好，该植物簇生成工具为下一步大面积生成植物簇提供了基础。

该植物簇工具针对不同的气候进行了自适应参数调整，也增加了真实感和可用性。

**七、环境配置**

可以使用Windows、OS X、Linux等主流操作系统。

基于Unreal Engine和Python运行项目，并且项目的构建依赖QT。

开发测试版本基于Unreal Engine 4.27，Python 3.9，QT 6.3.0，建议使用这些版本进行运行。

在运行UE4项目前，需要先开启数据库的本地服务器，具体方式为运行server.py，并在参数中给出到数据库文件夹的相对路径，如: python server.py ../db/

**八、参考文献**

【1】Kapp, Konrad, et al. "Data-driven authoring of large-scale ecosystems." ACM Transactions on Graphics (TOG) 39.6 (2020): 1-14.

【2】80.lv. 2022. Procedural Forest Terrain Production with Houdini and UE4. [online] Available at: <https://80.lv/articles/006sdf-procedural-forest-terrain-production-with-houdini-and-ue4/> [Accessed 26 March 2022]. 	 

【3】Cnblogs.com. 2022. Houdini技术体系 过程化地形系统（一）：Far Cry5的植被系统分析. [online] Available at: <https://www.cnblogs.com/traceplus/p/9202567.html> [Accessed 26 March 2022].

【4】Docs.unrealengine.com. 2022. 程序化植被工具快速入门. [online] Available at: <https://docs.unrealengine.com/4.27/zh-CN/BuildingWorlds/OpenWorldTools/ProceduralFoliage/QuickStart/> [Accessed 26 March

【5】Hai, N.H., Wiegand, K. & Getzin, S. Spatial distributions of tropical tree species in northern Vietnam under environmentally variable site conditions. Journal of Forestry Research 25, 257–268 (2014).

【6】 Kapp, K., Gain, J., Guérin, E., Galin, E., & Peytavie, A. (2020). Data-driven authoring of large-scale ecosystems. ACM Transactions on Graphics (TOG), 39(6), 1-14.）

【7】 Zhang, J., Wang, C., Li, C., & Qin, H. (2019). Example-based rapid generation of vegetation on terrain via CNN-based distribution learning. The Visual Computer, 35(6), 1181-1191.

【8】 Morales, K., & Vinicius, T. 2003. Amazon rainforest: biodiversity and biopiracy. STUDENT BMJ, 13, pp 386-7

【9】 Garifullin A., Frolov V.A., Khlupina A.A. (2021) Approximate Instancing for Modeling Plant Ecosystems. GraphiCon 2021: 31st International Conference on Computer Graphics and Vision, September 27-30, 2021, Nizhny Novgorod, Russia

【10】 Miłosz Makowski, Torsten Hädrich, Jan Scheffczyk, Dominik L. Michels, Sören Pirk, and Wojtek Pałubicki. 2019. Synthetic Silviculture: Multi-scale Modeling of Plant Ecosystems. ACM Trans. Graph. 38, 4, Article 131 (July 2019), 14 pages. https://doi.org/10.1145/3306346.3323039

【11】 Tomas Polasek, David Hrusa, Bedrich Benes, and Martin Čadík. 2021. ICTree: Automatic Perceptual Metrics for Tree Models. ACM Trans. Graph. 40, 6, Article 1 (December 2021), 15 pages. https://doi.org/10.1145/3478513.3480519
**


**附录1、组会记录**

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.055.png)

图: 组会记录

重要组会讨论成果整理：因为肖老师说和实验室以及和老师的讨论也作为中间过程是我们的成果，并且中间因为对需求的了解不断加深，方案换了好几个，所以也放在报告中。

![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.056.png) ![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.057.png) ![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.058.png) ![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.059.png) ![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.060.png) ![](Aspose.Words.ce4afd25-1bdc-44f8-92a4-6691ba97ed54.061.png)


