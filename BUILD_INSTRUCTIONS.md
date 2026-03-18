# 悬浮圆球功能编译说明

## 修改内容

本次修改将Ditto剪贴板管理器的悬浮窗改为一个圆形悬浮球，点击后显示主面板。

### 新增文件

1. **src/FloatingBall.h** - 悬浮圆球窗口类头文件
2. **src/FloatingBall.cpp** - 悬浮圆球窗口类实现

### 修改文件

1. **src/MainFrm.h** - 添加悬浮球成员和消息处理
2. **src/MainFrm.cpp** - 添加悬浮球创建和消息处理
3. **CP_Main.vcxproj** - 添加新文件到项目
4. **CP_Main.vcxproj.filters** - 添加新文件到过滤器

## 编译步骤

### 前提条件

- Visual Studio 2019 或更高版本（支持C++）
- 安装了"使用C++的桌面开发"工作负载

### 编译命令

#### 方法1: 使用Visual Studio IDE

1. 打开 `CP_Main_10.sln`
2. 选择配置为 `Release` 和平台为 `x64`
3. 点击 "生成" -> "生成解决方案"

#### 方法2: 使用命令行

1. 打开 "Developer Command Prompt for VS 2022"（或VS 2019）
2. 切换到项目目录:
   ```
   cd D:\person_project\Ditto-3.25.113.0
   ```
3. 还原NuGet包:
   ```
   nuget restore CP_Main_10.sln
   ```
4. 编译项目:
   ```
   msbuild CP_Main_10.sln /p:Configuration=Release /p:Platform=x64
   ```

## 输出文件

编译成功后，可执行文件将位于:
- **Release64\Ditto.exe** - 主程序

## 功能说明

### 悬浮圆球特性

1. **外观**: 60x60像素的圆形蓝色渐变球体
2. **位置**: 默认显示在屏幕右下角
3. **拖拽**: 可以用鼠标拖拽移动到任意位置
4. **点击**: 单击显示/隐藏主窗口
5. **置顶**: 始终保持在最上层显示

### 使用方法

1. 运行Ditto.exe后，悬浮球会自动显示
2. 单击悬浮球打开主面板
3. 再次单击悬浮球关闭主面板
4. 拖拽悬浮球可以移动位置

## 故障排除

### 编译错误

1. **找不到MSBuild**: 确保安装了Visual Studio并配置了环境变量
2. **缺少NuGet包**: 运行 `nuget restore CP_Main_10.sln`
3. **头文件找不到**: 确保所有文件路径正确

### 运行时问题

1. **悬浮球不显示**: 检查是否有其他窗口覆盖
2. **点击无响应**: 确保主窗口已正确创建
3. **无法拖拽**: 检查鼠标事件是否被其他程序拦截

## 自定义修改

### 修改圆球大小
编辑 `src/FloatingBall.h` 中的:
```cpp
static const int BALL_SIZE = 60;  // 修改这个值
```

### 修改圆球颜色
编辑 `src/FloatingBall.h` 中的:
```cpp
static const int BALL_COLOR_R = 0;      // 红色分量 (0-255)
static const int BALL_COLOR_G = 150;    // 绿色分量 (0-255)
static const int BALL_COLOR_B = 255;    // 蓝色分量 (0-255)
```

### 修改初始位置
编辑 `src/FloatingBall.cpp` 中的 `CreateBall` 函数:
```cpp
int x = screenWidth - BALL_SIZE - 20;   // 修改X坐标
int y = screenHeight - BALL_SIZE - 100; // 修改Y坐标
```
