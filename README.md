# ACTGame

ACTGame 是一个基于 Unreal Engine 5.4 的第三人称动作游戏项目，当前代码和资源已经整理为以 `ACTGame` 模块为核心的工程结构。项目主体由纯 C++ 玩法框架和蓝图资源协同构成，当前重点覆盖玩家战斗、状态机、输入缓存、生命值 UI、敌人 AI 以及 Wwise 音频集成。

## 项目概览

- 引擎版本：`UE 5.4`
- 主模块：`Source/ACTGame`
- 默认启动地图：`/Game/Level/Test`
- 编辑器启动地图：`/Game/Level/Test`
- 默认游戏模式：`/Game/Blueprint/GamePlay/BP_GameMode`
- 主要插件与依赖：
  - `Enhanced Input`
  - `UMG`
  - `AIModule`
  - `NavigationSystem`
  - `AkAudio / Wwise`
  - `CommonUI`

## 当前玩法结构

项目当前已经形成以下几类核心玩法模块：

- 玩家角色系统
  - `APlayerCharacter` 负责相机、武器、生命值、动作数据和状态机组件的挂载
  - `AACTPlayerController` 负责增强输入绑定、输入分发和输入缓存驱动
- 战斗状态机
  - `UPlayerStateMachine` 统一管理待机、移动、转身、闪避、普攻链、特殊攻击等状态
  - 状态实现集中在 `Player/StateMachine/States`
- 动画驱动战斗
  - `PlayerAnimInstance` 负责表现层动画参数同步
  - `ANS_AttackCheck`、`ANS_AttackCombo`、`ANS_InputCache`、`ANS_AttackExit` 用于命中窗口、连招窗口和输入缓存窗口控制
- 战斗数据驱动
  - `ActionData` 和 `ActionDataContainer` 负责把动作状态与数据资产映射起来
- 生命值与 UI
  - `PlayerHealth` 提供 `AddHealth`、`ReduceHealth`、`OnHealthChange` 和 `OnPlayerDeath`
  - `UIManager` 负责主 HUD 的创建与更新
- 敌人 AI
  - `EnemyBaseCharacter` 作为敌人基类
  - `EnemyAIController` 配合行为树、黑板和感知系统处理索敌、攻击和眩晕恢复
  - AI 任务与服务节点集中在 `Enemy/AI/BT`

## 目录结构

### 顶层目录

```text
ACTGame/
├── Config/                    # 工程配置、默认地图、碰撞和打包设置
├── Content/                   # 游戏资源与蓝图
├── Plugins/                   # 本地插件，包含 Wwise 和 UnrealMCP
├── README/                    # 补充说明文档
├── Source/                    # C++ 源码
├── ACTGame_WwiseProject/      # Wwise 工程文件
├── ACTGame.uproject           # Unreal 工程入口
└── README.md                  # 项目说明
```

### C++ 代码结构

```text
Source/ACTGame/
├── ACTGame.Build.cs
├── ACTGame.cpp
├── Enemy/
│   ├── EnemyBaseCharacter.h/cpp
│   └── AI/
│       ├── EnemyAIController.h/cpp
│       └── BT/
├── Player/
│   ├── Animation/
│   │   ├── PlayerAnimInstance.h/cpp
│   │   └── AnimNotify/State/
│   ├── Base/
│   │   ├── CharacterBase.h/cpp
│   │   └── GamePlay/
│   │       ├── ACTGameMode.h/cpp
│   │       └── ACTPlayerState.h/cpp
│   ├── Character/
│   │   ├── ACTPlayerController.h/cpp
│   │   └── PlayerCharacter.h/cpp
│   ├── Data/Action/
│   ├── Health/
│   ├── Input/
│   ├── StateMachine/
│   ├── UI/
│   └── Weapon/
└── Tools/Log/
```

### 主要资源目录

```text
Content/
├── Blueprint/
│   ├── Character/             # 玩家角色与动画蓝图
│   ├── GamePlay/
│   │   ├── Game/              # GameMode、UIManager、PlayerController、PlayerState 蓝图
│   │   └── MainMenu/          # 主菜单相关蓝图
│   └── Widget/                # HUD 和菜单 UI
├── Data/Attack/               # 动作数据资产
├── Enemy/AI/                  # 敌人 AI 蓝图、行为树、黑板
├── Input/                     # InputAction 与 IMC
├── Level/                     # 关卡地图
├── Model/Character/           # 角色模型、骨骼、材质、动画
├── Resources/                 # 字体、图片、UI 样式
└── WwiseAudio/                # Wwise 导入资源
```

## 当前默认流程

项目目前的默认运行链路如下：

1. 打开 `ACTGame.uproject`
2. 编辑器默认进入 `Level/Test`
3. `BP_GameMode` 作为默认 GameMode 启动
4. C++ 默认类链路为：
   - `AACTGameMode`
   - `AACTPlayerController`
   - `APlayerCharacter`
   - `AACTPlayerState`
   - `AUIManager`

## 开发与维护建议

### 本地打开项目

1. 使用 `Unreal Engine 5.4` 打开 `ACTGame.uproject`
2. 若提示编译 C++ 模块，使用 Visual Studio 工具链重新生成并编译
3. 若音频相关模块报错，优先检查 `Plugins/Wwise` 和 `ACTGame_WwiseProject`

### 编译前注意事项

项目资源中存在中文命名文件。若 UnrealBuildTool 因 `git status` 路径转义导致异常，可先在仓库内执行：

```powershell
git config core.quotepath false
```

然后重新生成工程文件或重新编译。

### 阅读顺序建议

如果你要快速理解当前项目，建议按下面顺序阅读：

1. `Source/ACTGame/Player/Character/PlayerCharacter.h`
2. `Source/ACTGame/Player/StateMachine/PlayerStateMachine.h`
3. `Source/ACTGame/Player/StateMachine/States`
4. `Source/ACTGame/Player/Weapon/WeaponCollider.h`
5. `Source/ACTGame/Enemy/AI/EnemyAIController.h`
6. `README/C++代码结构说明.md`

## 补充说明

- `README/C++代码结构说明.md` 提供了更细的 C++ 模块拆分与关系说明
- `Content/Level` 当前包含 `MainMenu` 和 `Test` 两张主要地图
- `Plugins/UnrealMCP` 目录已在仓库内保留，便于后续编辑器联动与扩展
- `ACTGame_WwiseProject` 为独立 Wwise 工程，请与 `Content/WwiseAudio` 配合维护
