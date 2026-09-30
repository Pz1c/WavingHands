# Builds the Android debug APK the way Qt Creator's "Qt 6.11.0 for Android arm64-v8a" kit does
# (qmake, the NDK's make, androiddeployqt) and puts it on the phone over adb. Run by the
# "Android: ..." tasks in tasks.json, or by hand:
#   powershell -ExecutionPolicy Bypass -File .vscode\android.ps1 -Action Deploy
#
#   Build   builds build\vscode-android-arm64-debug\...\android-build-debug.apk
#   Deploy  Build, then installs the APK on the phone (app data is kept) and starts the app
#   Run     installs the last built APK and starts the app
#   Logcat  follows the running app's log (console.log, qDebug)
#
# With more than one device connected, set ANDROID_SERIAL to the one to use (see `adb devices`).
param(
    [ValidateSet('Build', 'Deploy', 'Run', 'Logcat')]
    [string]$Action = 'Deploy'
)

$ErrorActionPreference = 'Stop'

# This machine's toolchain, the one Qt Creator is set up with
$QtHost    = 'C:\Qt\6.11.0\mingw_64'
$QtAndroid = 'C:\Qt\6.11.0\android_arm64_v8a'
$Sdk       = "$env:LOCALAPPDATA\Android\Sdk"
$Ndk       = "$Sdk\ndk\27.2.12479018"
$Jdk       = 'C:\Program Files\Eclipse Adoptium\jdk-17.0.16.8-hotspot'
$Platform  = 'android-36'

$Package  = 'net.is.games.WarlocksDuel'
$Activity = 'com.kdab.training.MainActivity'

$Root     = Split-Path -Parent $PSScriptRoot
$BuildDir = "$Root\build\vscode-android-arm64-debug"
$Apk      = "$BuildDir\android-build\build\outputs\apk\debug\android-build-debug.apk"
$Adb      = "$Sdk\platform-tools\adb.exe"
$Make     = "$Ndk\prebuilt\windows-x86_64\bin\make.exe"

$env:ANDROID_SDK_ROOT = $Sdk
$env:ANDROID_NDK_ROOT = $Ndk
$env:JAVA_HOME = $Jdk
# GNU make runs recipes with sh.exe when it finds one on PATH (Git's usr\bin), but qmake writes
# cmd.exe recipes (del, copy) into this Makefile
$env:PATH = (($env:PATH -split ';') | Where-Object { $_ -and -not [IO.File]::Exists($_.TrimEnd('\') + '\sh.exe') }) -join ';'

function Invoke-Tool([string]$Exe, [string[]]$Arguments) {
    Write-Host "> $(Split-Path -Leaf $Exe) $($Arguments -join ' ')" -ForegroundColor Cyan
    # Windows PowerShell turns native stderr into errors when its own stderr is redirected, and
    # androiddeployqt and gradle warn on stderr: only the exit code decides
    $ErrorActionPreference = 'Continue'
    & $Exe @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "$(Split-Path -Leaf $Exe) failed (exit code $LASTEXITCODE)"
    }
}

function Build-Apk {
    New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null
    Push-Location $BuildDir
    try {
        if (-not (Test-Path Makefile)) {
            # later on the Makefile re-runs qmake by itself when WarlocksDuel.pro changes
            Invoke-Tool "$QtHost\bin\qmake6.exe" @('-qtconf', "$QtAndroid\bin\target_qt.conf", "$Root\WarlocksDuel.pro", '-spec', 'android-clang', 'CONFIG+=debug')
        }
        Invoke-Tool $Make @("-j$([Environment]::ProcessorCount)")
        Invoke-Tool $Make @("INSTALL_ROOT=$BuildDir\android-build", 'install')
        Invoke-Tool "$QtHost\bin\androiddeployqt.exe" @('--input', "$BuildDir\android-WarlocksDuel-deployment-settings.json", '--output', "$BuildDir\android-build", '--android-platform', $Platform, '--jdk', $Jdk, '--gradle')
    } finally {
        Pop-Location
    }
    Write-Host "APK: $Apk" -ForegroundColor Green
}

function Get-Device {
    if ($env:ANDROID_SERIAL) {
        return $env:ANDROID_SERIAL
    }
    $devices = @(& $Adb devices | ForEach-Object { if ($_ -match '^(\S+)\s+device$') { $Matches[1] } })
    if ($devices.Count -eq 1) {
        return $devices[0]
    }
    & $Adb devices -l
    if ($devices.Count -eq 0) {
        throw 'No phone ready. Plug it in, unlock it and accept "Allow USB debugging?" (an "unauthorized" line above is waiting for that).'
    }
    throw 'More than one device: set ANDROID_SERIAL to the one to use.'
}

function Install-Apk {
    if (-not (Test-Path $Apk)) {
        throw "No APK yet, build it first: $Apk"
    }
    $device = Get-Device
    # -r replaces the app and keeps its data (both builds are signed with the debug keystore).
    # Don't uninstall to get past an install error: that wipes the saved accounts.
    Invoke-Tool $Adb @('-s', $device, 'install', '-r', $Apk)
    Invoke-Tool $Adb @('-s', $device, 'shell', 'am', 'start', '-S', '-n', "$Package/$Activity")
}

function Show-Log {
    $device = Get-Device
    $appPid = ''
    for ($i = 0; ($i -lt 20) -and -not $appPid; ++$i) {
        $appPid = "$(& $Adb -s $device shell pidof -s $Package)".Trim()
        if (-not $appPid) {
            Start-Sleep -Milliseconds 500
        }
    }
    if (-not $appPid) {
        throw "$Package isn't running on the phone"
    }
    Invoke-Tool $Adb @('-s', $device, 'logcat', "--pid=$appPid")
}

switch ($Action) {
    'Build'  { Build-Apk }
    'Deploy' { Build-Apk; Install-Apk }
    'Run'    { Install-Apk }
    'Logcat' { Show-Log }
}
