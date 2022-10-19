# Taiyou Config
TaiyouConfig is a simple and easy to use file format for storing data

Structure
* Namespace
  * KeyName
    * Value

This project contains the compiler and runtime library (soon)
 
## Source File
Syntax for a `.tcfg` file:
```
string:DataDir=./Source

$namespace Graphics
	bool:Fullscreen=True
	screenResolution:FullscreenResolution=1366x768
$end

$namespace Graphics.PostProcessing
	bool:Bloom=True
	bool:AutoExposure=True
$end
```