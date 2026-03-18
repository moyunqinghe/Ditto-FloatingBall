# GitHub 仓库设置指南

## 步骤1: 创建GitHub仓库

1. 打开 https://github.com/new
2. 填写仓库信息:
   - **Repository name**: `Ditto-FloatingBall` (或其他你喜欢的名称)
   - **Description**: `Ditto clipboard manager with floating ball feature`
   - **Visibility**: Public 或 Private
   - **Initialize**: 不要勾选任何初始化选项
3. 点击 "Create repository"

## 步骤2: 推送代码到GitHub

创建仓库后，GitHub会显示推送命令，请执行以下命令：

```powershell
# 进入项目目录
cd D:\person_project\Ditto-3.25.113.0

# 添加远程仓库（替换YOUR_USERNAME为你的GitHub用户名）
git remote add origin https://github.com/YOUR_USERNAME/Ditto-FloatingBall.git

# 推送代码
git push -u origin master
```

## 步骤3: 触发自动编译

推送成功后，GitHub Actions会自动触发编译。你可以在以下位置查看：

1. 打开GitHub仓库页面
2. 点击 "Actions" 标签
3. 选择 "Build Floating Ball Version" 工作流
4. 等待编译完成（约5-10分钟）

## 步骤4: 下载编译结果

编译成功后：

1. 在Actions页面点击最新的运行记录
2. 向下滚动到 "Artifacts" 部分
3. 下载 `Ditto-FloatingBall-EXE` 压缩包
4. 解压后得到 `Ditto.exe`，即可使用

## 手动触发编译

如果需要重新编译：

1. 进入仓库的 Actions 页面
2. 选择 "Build Floating Ball Version"
3. 点击 "Run workflow" 按钮
4. 选择分支并点击 "Run workflow"

## 故障排除

### 推送失败

如果遇到权限问题，可以使用GitHub CLI或Personal Access Token:

```powershell
# 使用GitHub CLI登录
gh auth login

# 然后重新推送
git push -u origin master
```

### 编译失败

1. 检查Actions日志中的错误信息
2. 确保所有文件已正确提交
3. 检查 `.github/workflows/build-floating-ball.yml` 是否存在

## 获取编译文件的其他方式

如果你没有GitHub账户，可以使用以下替代方案：

### 方案1: 使用GitHub Codespaces
1. 在GitHub仓库页面点击 "Code" -> "Codespaces"
2. 创建一个新的Codespace
3. 在Codespace终端中运行编译命令

### 方案2: 找朋友帮忙
将这个项目分享给你的朋友，让他们帮你Fork仓库并触发编译。

### 方案3: 使用其他CI服务
可以使用以下免费的CI服务：
- Azure DevOps (https://dev.azure.com)
- AppVeyor (https://www.appveyor.com)
- GitLab CI (https://gitlab.com)
