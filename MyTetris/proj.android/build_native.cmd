@echo off

set DIR=%~dp0
set APP_ROOT=%DIR%/..
set APP_ANDROID_ROOT=%DIR%
set COCOS2DX_ROOT=%COCOS2D_HOME%

echo "NDK_ROOT = %NDK_ROOT%"
echo "COCOS2DX_ROOT = %COCOS2D_HOME%"
echo "APP_ROOT = %APP_ROOT%"
echo "APP_ANDROID_ROOT = %APP_ANDROID_ROOT%"


echo "Using prebuilt externals"
%NDK_ROOT%/ndk-build NDK_MODULE_PATH=%COCOS2DX_ROOT%;%COCOS2DX_ROOT%/cocos2dx/platform/third_party/android/prebuilt

