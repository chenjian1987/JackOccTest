Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$docsDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$diagramDir = Join-Path $docsDir "diagrams"
$docxPath = Join-Path $docsDir "offline-welding-architecture-dev-spec.docx"

$diagramNames = @(
    "core-framework-class",
    "module-dependency",
    "motion-control-class",
    "project-create-sequence",
    "project-open-sequence",
    "project-save-sequence",
    "project-export-sequence"
)

foreach ($name in $diagramNames) {
    $sourcePath = Join-Path $diagramDir ($name + ".mmd")
    $targetPath = Join-Path $diagramDir ($name + ".png")
    $body = Get-Content -Path $sourcePath -Raw -Encoding UTF8
    Invoke-WebRequest `
        -Uri "https://kroki.io/mermaid/png" `
        -Method Post `
        -Body $body `
        -ContentType "text/plain; charset=utf-8" `
        -OutFile $targetPath
}

function Set-TextStyle {
    param(
        [Parameter(Mandatory = $true)] $Selection,
        [double] $Size = 10.5,
        [int] $Bold = 0,
        [int] $Alignment = 0
    )

    $Selection.Font.Name = "Microsoft YaHei"
    $Selection.Font.Size = $Size
    $Selection.Font.Bold = $Bold
    $Selection.ParagraphFormat.Alignment = $Alignment
    $Selection.ParagraphFormat.SpaceAfter = 6
    $Selection.ParagraphFormat.SpaceBefore = 0
    $Selection.ParagraphFormat.LineSpacingRule = 0
}

function Add-Paragraph {
    param(
        [Parameter(Mandatory = $true)] $Selection,
        [Parameter(Mandatory = $true)] [string] $Text,
        [double] $Size = 10.5,
        [int] $Bold = 0,
        [int] $Alignment = 0
    )

    Set-TextStyle -Selection $Selection -Size $Size -Bold $Bold -Alignment $Alignment
    $Selection.TypeText($Text)
    $Selection.TypeParagraph()
}

function Add-Bullets {
    param(
        [Parameter(Mandatory = $true)] $Selection,
        [Parameter(Mandatory = $true)] [string[]] $Items
    )

    foreach ($item in $Items) {
        Add-Paragraph -Selection $Selection -Text ("• " + $item) -Size 10.5 -Bold 0 -Alignment 0
    }
}

function Add-PageBreak {
    param([Parameter(Mandatory = $true)] $Selection)
    $wdPageBreak = 7
    $Selection.InsertBreak($wdPageBreak)
}

function Add-ImageFigure {
    param(
        [Parameter(Mandatory = $true)] $Selection,
        [Parameter(Mandatory = $true)] [string] $ImagePath,
        [Parameter(Mandatory = $true)] [string] $Caption
    )

    if (-not (Test-Path $ImagePath)) {
        throw "Image not found: $ImagePath"
    }

    $shape = $Selection.InlineShapes.AddPicture($ImagePath)
    $shape.LockAspectRatio = $true
    if ($shape.Width -gt 460) {
        $shape.Width = 460
    }
    $Selection.TypeParagraph()
    Add-Paragraph -Selection $Selection -Text $Caption -Size 9 -Bold 0 -Alignment 1
}

function Add-Table {
    param(
        [Parameter(Mandatory = $true)] $Document,
        [Parameter(Mandatory = $true)] $Selection,
        [Parameter(Mandatory = $true)] [string[]] $Headers,
        [Parameter(Mandatory = $true)] [object[]] $Rows
    )

    $table = $Document.Tables.Add($Selection.Range, $Rows.Count + 1, $Headers.Count)
    $table.Borders.Enable = 1
    $table.Range.Font.Name = "Microsoft YaHei"
    $table.Range.Font.Size = 9.5
    $table.Range.ParagraphFormat.SpaceAfter = 0

    for ($c = 0; $c -lt $Headers.Count; $c++) {
        $cell = $table.Cell(1, $c + 1)
        $cell.Range.Text = $Headers[$c]
        $cell.Range.Font.Bold = 1
    }

    for ($r = 0; $r -lt $Rows.Count; $r++) {
        for ($c = 0; $c -lt $Headers.Count; $c++) {
            $table.Cell($r + 2, $c + 1).Range.Text = [string]$Rows[$r][$c]
        }
    }

    $table.Rows.Alignment = 0
    $table.AllowAutoFit = $true

    $Selection.SetRange($table.Range.End, $table.Range.End)
    $Selection.TypeParagraph()
}

$coreTaskRows = @(
    @("P0", "IApplication", "Include/App/IApplication.h", "定义应用启动、关闭、当前项目访问、业务管理器访问、命令管理器访问等统一能力。", "现有接口，建议扩充项目相关接口。"),
    @("P0", "IAppFactory", "Include/App/IAppFactory.h", "定义应用工厂，负责创建应用实例和屏蔽具体实现。", "现有接口，可直接沿用。"),
    @("P0", "ApplicationImpl", "sources/App/AppImpl/Application.h", "完成主窗口、文档视图、项目对象、命令管理器和业务编排器的装配，是运行时主入口。", "现有私有实现，建议继续扩充。"),
    @("P1", "AppManager", "sources/App/AppImpl/AppManager.h", "管理应用单例、全局上下文、启动顺序和退出资源释放。", "现有私有实现。"),
    @("P0", "ProjectContext", "Include/App/ProjectContext.h", "封装项目根目录、项目文件路径、运行模式、输出目录、用户配置等上下文信息。", "建议新增。"),
    @("P0", "Project", "Include/Model/Element/Project.h", "项目聚合根，统一实现 Create、Initialize、Open、Save、Export、Import、Close。", "建议新增。"),
    @("P0", "ProjectStorage", "Include/Infrastructure/Common/ProjectStorage.h", "负责项目目录骨架创建、项目文件读写、临时文件刷盘和项目路径规范化。", "建议新增。"),
    @("P0", "ProjectArchiveService", "Include/Infrastructure/Common/ProjectArchiveService.h", "负责项目导出打包和导入解包，统一管理 .myrsp 格式。", "建议新增。"),
    @("P0", "ISerializable", "Include/Infrastructure/Common/ISerializable.h", "统一定义对象的序列化、反序列化、文件持久化契约。", "建议新增。"),
    @("P0", "IPreference", "Include/Infrastructure/Common/IPreference.h", "统一定义配置对象的加载、保存、校验、重置和单项配置读写接口。", "建议新增。"),
    @("P1", "PreferenceFactory", "Include/Infrastructure/Common/PreferenceFactory.h", "负责配置类型注册、实例创建和默认配置对象生成。", "建议新增。"),
    @("P0", "PreferenceManager", "Include/Infrastructure/Common/PreferenceManager.h", "管理项目级和业务级配置对象，统一执行 Create、Load、Save、Validate、Watch。", "建议新增。"),
    @("P0", "IBusiness", "Include/Business/IBusiness.h", "定义各业务模块公共生命周期和异常处理接口。", "建议新增。"),
    @("P0", "BusinessFactory", "Include/Business/BusinessFactory.h", "负责业务模块注册、实例创建和业务类型映射。", "建议新增。"),
    @("P0", "BusinessManager", "Include/Business/BusinessManager.h", "统一创建、初始化、加载和保存所有业务模块，并提供统一查询入口。", "建议新增。"),
    @("P1", "EventBus", "Include/Infrastructure/Common/EventBus.h", "提供发布订阅模型，用于模块解耦和跨模块状态广播。", "建议新增。"),
    @("P1", "AppEvent", "Include/Infrastructure/Common/AppEvent.h", "定义统一事件对象、事件参数和事件命名规范。", "建议新增。"),
    @("P0", "CommandActionManager", "Include/Interaction/CommandActionManager.h", "负责命令注册、启用状态判断、执行、撤销和重做，是 UI 与应用的命令桥梁。", "建议新增，需与现有 GUI 命令定义对齐。"),
    @("P0", "BusinessInteract", "sources/App/AppImpl/BusinessInteract.h", "负责工作站列表、属性面板、控制面板、三维系统和设备通信系统之间的事件连接。", "建议新增私有实现类。")
)

$motionTaskRows = @(
    @("P0", "MotionControlBusiness", "Include/Business/MotionControl/MotionControlBusiness.h", "运动控制总入口，对外提供工作站切换、设备切换、设备移动、轨迹仿真、碰撞检测和数据持久化能力。", "建议新增。"),
    @("P0", "WorkStationManager", "Include/Business/MotionControl/WorkStationManager.h", "管理工作站集合、激活工作站切换和工作站查询。", "建议新增。"),
    @("P0", "WorkStation", "Include/Model/Element/WorkStation.h", "工作站聚合根，包含设备组集合、用途、名称、默认坐标系等。", "建议新增领域对象。"),
    @("P0", "DeviceGroup", "Include/Model/Element/DeviceGroup.h", "设备组聚合，维护设备集合和当前激活设备。", "建议新增领域对象。"),
    @("P0", "Device", "Include/Model/Element/Device.h", "统一设备抽象基类，封装标识、属性、状态和移动接口。", "建议新增抽象基类。"),
    @("P0", "RobotDevice", "Include/Model/Element/RobotDevice.h", "机器人设备对象，扩展轴组、工具坐标、基坐标和机器人专属参数。", "建议新增。"),
    @("P1", "ExternalAxisDevice", "Include/Model/Element/ExternalAxisDevice.h", "外部轴设备对象，描述外部轴的运动范围、联动方式和状态。", "建议新增。"),
    @("P1", "PositionerDevice", "Include/Model/Element/PositionerDevice.h", "变位机设备对象，封装工件姿态变化和联动约束。", "建议新增。"),
    @("P2", "WorkbenchDevice", "Include/Model/Element/WorkbenchDevice.h", "工作台设备对象，用于表达台面、工位和固定装置的空间关系。", "建议新增。"),
    @("P2", "FixtureDevice", "Include/Model/Element/FixtureDevice.h", "工装设备对象，描述夹具、定位块等静态设备。", "建议新增。"),
    @("P0", "Trajectory", "Include/Model/Element/Trajectory.h", "统一描述轨迹点、姿态、速度、工艺参数绑定和轨迹分段。", "建议新增。"),
    @("P0", "MotionCommand", "Include/Model/Element/MotionCommand.h", "统一描述单次移动指令，包含目标设备、目标位姿、速度和加速度信息。", "建议新增。"),
    @("P0", "SimulationController", "Include/Business/MotionControl/SimulationController.h", "负责轨迹仿真执行状态机，包括播放、暂停、停止、进度和回放控制。", "建议新增。"),
    @("P0", "CollideChecker", "Include/Business/MotionControl/CollideChecker.h", "负责设备姿态碰撞检测、轨迹碰撞检测和干涉分析。", "建议新增。"),
    @("P0", "RobotBaseAdapter", "Include/Business/MotionControl/RobotBaseAdapter.h", "封装外部机器人算法 DLL 的访问接口，隔离正逆解和运动学验证细节。", "建议新增。"),
    @("P1", "KinematicsService", "Include/Business/MotionControl/KinematicsService.h", "基于设备对象和算法适配器统一提供 FK、IK、可达性验证和姿态校验。", "建议新增。"),
    @("P2", "MotionAuditService", "Include/Business/MotionControl/MotionAuditService.h", "记录设备切换、设备移动、碰撞结果和仿真结果等关键操作，服务于可追溯性。", "建议新增。")
)

$otherTaskRows = @(
    @("P1", "ThreeDBusiness", "Include/Business/ThreeD/ThreeDBusiness.h", "三维系统总入口，负责模型导入、场景刷新、显示状态和选择状态对外服务。", "建议新增。"),
    @("P1", "SceneManager", "Include/Business/ThreeD/SceneManager.h", "管理场景树、节点可见性、选中集和视图刷新。", "建议新增。"),
    @("P1", "ModelRepository", "Include/Business/ThreeD/ModelRepository.h", "负责工件模型、设备模型加载、缓存和索引。", "建议新增。"),
    @("P1", "DeviceCommBusiness", "Include/Business/DeviceComm/DeviceCommBusiness.h", "设备通信总入口，负责连接状态、参数下发、设备反馈和虚实同步。", "建议新增。"),
    @("P2", "ProtocolAdapter", "Include/Business/DeviceComm/ProtocolAdapter.h", "抽象不同品牌控制器协议，实现协议层适配。", "建议新增。"),
    @("P1", "ProcessBusiness", "Include/Business/Process/ProcessBusiness.h", "工艺系统总入口，负责工艺配置、规则匹配和工艺求解调用。", "建议新增。"),
    @("P1", "TechParamManager", "Include/Business/Process/TechParamManager.h", "管理焊接工艺参数、材料参数和工艺模板集合。", "建议新增。"),
    @("P1", "TechSolver", "Include/Business/Process/TechSolver.h", "根据焊缝类型、板厚、材料等输入求解焊接参数。", "建议新增。"),
    @("P1", "WeldingAppBusiness", "Include/Business/Welding/WeldingAppBusiness.h", "焊接应用总编排器，组织焊缝提取、工艺匹配、路径生成与结果发布。", "建议新增。"),
    @("P1", "SeamExtractor", "Include/Business/Welding/SeamExtractor.h", "从三维模型中提取焊缝、坡口和接头语义信息。", "建议新增。"),
    @("P1", "PathPlanner", "Include/Business/Welding/PathPlanner.h", "根据焊缝对象和工艺参数生成焊接路径、姿态和节拍结果。", "建议新增。"),
    @("P1", "PostProcessBusiness", "Include/Business/PostProcess/PostProcessBusiness.h", "后处理总入口，负责程序生成、模板套用和结果导出。", "建议新增。"),
    @("P1", "ProgramGenerator", "Include/Business/PostProcess/ProgramGenerator.h", "将轨迹和设备坐标转换为控制器可执行程序。", "建议新增。")
)

$appendixRows = @(
    @("核心框架 UML 类图", "docs/diagrams/core-framework-class.mmd", "docs/diagrams/core-framework-class.png"),
    @("模块依赖图", "docs/diagrams/module-dependency.mmd", "docs/diagrams/module-dependency.png"),
    @("运动控制系统 UML 细化类图", "docs/diagrams/motion-control-class.mmd", "docs/diagrams/motion-control-class.png"),
    @("Project.Create 时序图", "docs/diagrams/project-create-sequence.mmd", "docs/diagrams/project-create-sequence.png"),
    @("Project.Open 时序图", "docs/diagrams/project-open-sequence.mmd", "docs/diagrams/project-open-sequence.png"),
    @("Project.Save 时序图", "docs/diagrams/project-save-sequence.mmd", "docs/diagrams/project-save-sequence.png"),
    @("Project.Export 时序图", "docs/diagrams/project-export-sequence.mmd", "docs/diagrams/project-export-sequence.png")
)

$word = $null
$document = $null

try {
    $word = New-Object -ComObject Word.Application
    $word.Visible = $false
    $word.DisplayAlerts = 0
    $document = $word.Documents.Add()
    $selection = $word.Selection

    Add-Paragraph -Selection $selection -Text "三维免示教离线焊接软件研发设计文档" -Size 18 -Bold 1 -Alignment 1
    Add-Paragraph -Selection $selection -Text "版本：V1.0    用途：研发设计 / 汇报附件 / 开发拆任务基线" -Size 10 -Bold 0 -Alignment 1
    Add-Paragraph -Selection $selection -Text "说明：当前仓库实际源码目录为 /sources，文档中接口头文件仍以 /Include 为公开边界，私有实现以 /sources 为准；如后续重构，可再统一为 /Source 命名。" -Size 10 -Bold 0 -Alignment 0

    Add-Paragraph -Selection $selection -Text "1. 文档目标与适用范围" -Size 14 -Bold 1 -Alignment 0
    Add-Paragraph -Selection $selection -Text "本文档面向研发落地，重点补齐核心框架 UML 类图、模块依赖图、运动控制系统 UML 细化类图、项目生命周期时序图和接口拆任务表，用于把当前方案直接收敛成可执行的研发设计输入。" -Size 10.5 -Bold 0 -Alignment 0
    Add-Bullets -Selection $selection -Items @(
        "明确核心框架对象之间的关系、持有关系和编排关系。",
        "明确 UI、交互层、应用层、业务层、领域层和基础设施层的依赖边界。",
        "明确运动控制系统内部对象的聚合关系、算法入口和运行时组件。",
        "明确项目 Create、Open、Save、Export 四类关键流程的时序。",
        "提供可直接下发给开发人员的接口任务拆分清单。"
    )

    Add-PageBreak -Selection $selection
    Add-Paragraph -Selection $selection -Text "2. 核心框架 UML 类图" -Size 14 -Bold 1 -Alignment 0
    Add-Paragraph -Selection $selection -Text "核心框架负责完成应用生命周期管理、项目装配、命令调度和业务编排。该图的重点不是业务算法，而是回答谁持有谁、谁编排谁、谁是跨层协调入口的问题。" -Size 10.5 -Bold 0 -Alignment 0
    Add-ImageFigure -Selection $selection -ImagePath (Join-Path $diagramDir "core-framework-class.png") -Caption "图 2-1 核心框架 UML 类图"
    Add-Bullets -Selection $selection -Items @(
        "IApplication 是应用统一入口，向 UI 和外部模块暴露项目、命令和业务访问能力。",
        "ApplicationImpl 负责装配 MainWindow、DocumentView、Project、BusinessManager 和 CommandActionManager。",
        "Project 是项目聚合根，统一负责项目目录、项目文件、配置和业务模块生命周期。",
        "BusinessManager 用于屏蔽多个业务模块的创建、初始化、加载和保存细节。",
        "BusinessInteract 不承担业务计算，专门负责跨业务模块和 UI 的事件连接。"
    )

    Add-Paragraph -Selection $selection -Text "3. 模块依赖图" -Size 14 -Bold 1 -Alignment 0
    Add-Paragraph -Selection $selection -Text "模块依赖图定义了编译期和设计期边界，目的是防止 UI 直接下沉到底层业务或底层库，避免后续形成难以维护的网状依赖。" -Size 10.5 -Bold 0 -Alignment 0
    Add-ImageFigure -Selection $selection -ImagePath (Join-Path $diagramDir "module-dependency.png") -Caption "图 3-1 模块依赖图"
    Add-Bullets -Selection $selection -Items @(
        "UI 层只能依赖 Interaction 和 AppInterface，不得直接引用具体业务实现。",
        "Interaction 承接用户意图和场景输入，只访问应用接口、领域对象和基础设施。",
        "Application 作为编排层，可依赖 UI、交互、业务、领域和基础设施，但不承载底层算法实现。",
        "业务子系统之间的协作尽量通过 BusinessInteract 或事件总线完成，减少直接双向耦合。",
        "基础设施层只向上提供通用能力，不反向依赖任何上层业务模块。"
    )

    Add-PageBreak -Selection $selection
    Add-Paragraph -Selection $selection -Text "4. 运动控制系统 UML 细化类图" -Size 14 -Bold 1 -Alignment 0
    Add-Paragraph -Selection $selection -Text "运动控制系统是离线焊接软件的核心能力之一，需要同时承载工作站管理、设备管理、运动学求解、仿真控制和碰撞检测。该类图用于确定运动控制模块内部的主要对象、聚合关系和算法入口。" -Size 10.5 -Bold 0 -Alignment 0
    Add-ImageFigure -Selection $selection -ImagePath (Join-Path $diagramDir "motion-control-class.png") -Caption "图 4-1 运动控制系统 UML 细化类图"
    Add-Bullets -Selection $selection -Items @(
        "MotionControlBusiness 是运动控制总入口，对外提供工作站切换、设备激活、移动控制、轨迹仿真和保存加载能力。",
        "WorkStationManager 管理多个工作站对象，并维护当前激活工作站。",
        "WorkStation 聚合多个 DeviceGroup，DeviceGroup 再聚合多个 Device。",
        "Device 采用抽象基类设计，面向机器人、外部轴、变位机、工作台和工装等具体设备派生扩展。",
        "SimulationController 和 CollideChecker 是运动控制运行时的重要执行组件，应避免分散在 UI 或其他模块中。",
        "RobotBaseAdapter 用于隔离第三方运动学 DLL 调用，避免业务层直接依赖算法库细节。"
    )

    Add-PageBreak -Selection $selection
    Add-Paragraph -Selection $selection -Text "5. 项目生命周期时序图" -Size 14 -Bold 1 -Alignment 0
    Add-Paragraph -Selection $selection -Text "项目生命周期是系统的主业务骨架，决定了项目数据如何创建、如何恢复、如何持久化以及如何导出交付。以下四张时序图分别对应项目的 Create、Open、Save、Export 四个关键操作。" -Size 10.5 -Bold 0 -Alignment 0

    Add-Paragraph -Selection $selection -Text "5.1 Project.Create" -Size 12 -Bold 1 -Alignment 0
    Add-ImageFigure -Selection $selection -ImagePath (Join-Path $diagramDir "project-create-sequence.png") -Caption "图 5-1 Project.Create 时序图"
    Add-Paragraph -Selection $selection -Text "核心步骤：创建目录骨架，生成默认项目配置，依次创建各业务模块，再执行统一初始化和界面绑定。" -Size 10.5 -Bold 0 -Alignment 0

    Add-Paragraph -Selection $selection -Text "5.2 Project.Open" -Size 12 -Bold 1 -Alignment 0
    Add-ImageFigure -Selection $selection -ImagePath (Join-Path $diagramDir "project-open-sequence.png") -Caption "图 5-2 Project.Open 时序图"
    Add-Paragraph -Selection $selection -Text "核心步骤：读取项目文件，加载配置与业务数据，统一初始化各业务模块，恢复联动关系和视图状态。" -Size 10.5 -Bold 0 -Alignment 0

    Add-Paragraph -Selection $selection -Text "5.3 Project.Save" -Size 12 -Bold 1 -Alignment 0
    Add-ImageFigure -Selection $selection -ImagePath (Join-Path $diagramDir "project-save-sequence.png") -Caption "图 5-3 Project.Save 时序图"
    Add-Paragraph -Selection $selection -Text "核心步骤：先保存项目级配置，再保存各业务模块数据，最后落盘项目文件并刷新界面状态。" -Size 10.5 -Bold 0 -Alignment 0

    Add-Paragraph -Selection $selection -Text "5.4 Project.Export" -Size 12 -Bold 1 -Alignment 0
    Add-ImageFigure -Selection $selection -ImagePath (Join-Path $diagramDir "project-export-sequence.png") -Caption "图 5-4 Project.Export 时序图"
    Add-Paragraph -Selection $selection -Text "核心步骤：导出前先执行一次保存，随后由归档服务将项目目录打包成 .myrsp，默认排除临时缓存和过程生成目录。" -Size 10.5 -Bold 0 -Alignment 0

    Add-PageBreak -Selection $selection
    Add-Paragraph -Selection $selection -Text "6. 可直接给开发拆任务的接口表" -Size 14 -Bold 1 -Alignment 0
    Add-Paragraph -Selection $selection -Text "说明：下表按优先级和模块分组，用于直接拆分研发任务。标记为建议新增的头文件表示当前仓库中尚未看到对应公开接口，建议在本轮架构收敛中新增。" -Size 10.5 -Bold 0 -Alignment 0

    Add-Paragraph -Selection $selection -Text "6.1 核心框架与项目层接口任务表" -Size 12 -Bold 1 -Alignment 0
    Add-Table -Document $document -Selection $selection -Headers @("优先级", "类名/接口名", "建议头文件名", "责任说明", "备注") -Rows $coreTaskRows

    Add-Paragraph -Selection $selection -Text "6.2 运动控制系统接口任务表" -Size 12 -Bold 1 -Alignment 0
    Add-Table -Document $document -Selection $selection -Headers @("优先级", "类名/接口名", "建议头文件名", "责任说明", "备注") -Rows $motionTaskRows

    Add-Paragraph -Selection $selection -Text "6.3 三维、工艺、焊接应用、后处理与设备通信接口任务表" -Size 12 -Bold 1 -Alignment 0
    Add-Table -Document $document -Selection $selection -Headers @("优先级", "类名/接口名", "建议头文件名", "责任说明", "备注") -Rows $otherTaskRows

    Add-PageBreak -Selection $selection
    Add-Paragraph -Selection $selection -Text "7. 附录：图源文件清单" -Size 14 -Bold 1 -Alignment 0
    Add-Table -Document $document -Selection $selection -Headers @("图示", "Mermaid 源文件", "输出图片") -Rows $appendixRows
    Add-Paragraph -Selection $selection -Text "文档说明：本文件为正式 Word 交付件，对应图源保存在 docs/diagrams 目录；若后续需要维护图示，请优先编辑 Mermaid 源文件并重新执行导出脚本。" -Size 9.5 -Bold 0 -Alignment 0

    if (Test-Path $docxPath) {
        Remove-Item $docxPath -Force
    }

    $document.SaveAs2($docxPath, 16)
    $document.Close()
    $word.Quit()
}
finally {
    if ($document -ne $null) {
        try { $document.Close() } catch { }
    }
    if ($word -ne $null) {
        try { $word.Quit() } catch { }
    }
}

Write-Output "Generated:"
Write-Output $docxPath
