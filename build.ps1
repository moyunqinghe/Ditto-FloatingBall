# Ditto 悬浮圆球版本编译脚本
# 需要安装 Visual Studio 2019 或更高版本

param(
    [string]$Configuration = "Release",
    [string]$Platform = "x64",
    [switch]$Clean
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Ditto 悬浮圆球版本编译脚本" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# 检查 Visual Studio
$vsPaths = @(
    "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"
)

$msbuild = $null
foreach ($path in $vsPaths) {
    if (Test-Path $path) {
        $msbuild = $path
        break
    }
}

if (-not $msbuild) {
    Write-Error "找不到 MSBuild.exe，请确保已安装 Visual Studio 2019 或更高版本"
    Write-Host "可从 https://visualstudio.microsoft.com/downloads/ 下载" -ForegroundColor Yellow
    exit 1
}

Write-Host "找到 MSBuild: $msbuild" -ForegroundColor Green

# 检查 NuGet
$nuget = Get-Command nuget -ErrorAction SilentlyContinue
if (-not $nuget) {
    Write-Warning "找不到 nuget.exe，尝试下载..."
    $nugetUrl = "https://dist.nuget.org/win-x86-commandline/latest/nuget.exe"
    $nugetPath = "$PSScriptRoot\nuget.exe"
    
    try {
        Invoke-WebRequest -Uri $nugetUrl -OutFile $nugetPath
        $nuget = Get-Command $nugetPath
        Write-Host "NuGet 下载成功" -ForegroundColor Green
    } catch {
        Write-Error "下载 NuGet 失败: $_"
        exit 1
    }
}

Write-Host "找到 NuGet: $($nuget.Source)" -ForegroundColor Green

# 清理
if ($Clean) {
    Write-Host "清理项目..." -ForegroundColor Yellow
    if (Test-Path "Release64") {
        Remove-Item -Recurse -Force "Release64"
    }
    if (Test-Path "Release") {
        Remove-Item -Recurse -Force "Release"
    }
    if (Test-Path "Debug") {
        Remove-Item -Recurse -Force "Debug"
    }
    if (Test-Path "Debug64") {
        Remove-Item -Recurse -Force "Debug64"
    }
    & $msbuild "CP_Main_10.sln" /t:Clean /p:Configuration=$Configuration /p:Platform=$Platform
}

# 还原 NuGet 包
Write-Host "还原 NuGet 包..." -ForegroundColor Yellow
& $nuget restore "CP_Main_10.sln"
if ($LASTEXITCODE -ne 0) {
    Write-Error "还原 NuGet 包失败"
    exit 1
}

# 编译
Write-Host "开始编译..." -ForegroundColor Yellow
Write-Host "配置: $Configuration, 平台: $Platform" -ForegroundColor Gray

& $msbuild "CP_Main_10.sln" `
    /p:Configuration=$Configuration `
    /p:Platform=$Platform `
    /m `
    /verbosity:minimal

if ($LASTEXITCODE -eq 0) {
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "编译成功!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    $outputPath = if ($Platform -eq "x64") { "Release64\Ditto.exe" } else { "Release\Ditto.exe" }
    
    if (Test-Path $outputPath) {
        Write-Host "输出文件: $outputPath" -ForegroundColor Cyan
        Write-Host "文件大小: $([math]::Round((Get-Item $outputPath).Length / 1MB, 2)) MB" -ForegroundColor Gray
        
        # 复制到根目录方便使用
        Copy-Item $outputPath "Ditto_FloatingBall.exe" -Force
        Write-Host "已复制到: Ditto_FloatingBall.exe" -ForegroundColor Cyan
    }
} else {
    Write-Error "编译失败!"
    exit 1
}
