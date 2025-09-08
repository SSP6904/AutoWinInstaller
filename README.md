# AutoWinInstaller
A simple automated windows installer in C++

## Information
- Made in C++
- Is a console based application
- Uses the Windows API to automate the installation
- Uses system-based utilities for the partitioning and writing of the image parts
- Can only be run on Windows and 64-bit architectures
	- Releases for the 32-bit architectures will come soon!
- Tested on a Windows PE Recovery Image

## Building the application
1. Clone the repository
2. Open the project in Visual Studio
	- Ensure to have the Windows SDK installed
	- Ensure to have the needed components installed
	- Ensure that you have the latest version of Visual Studio
3. Build the project
4. And you are done!

## FAQ

> Q: I'm getting the VCRUNTIME140_1.dll error. What do I do?
>
> A: Install the Visual C++ Redistributable for Visual Studio 2015-2019 before running the application. You can download it [here](https://www.microsoft.com/en-us/download/details.aspx?id=52685) if you don't have it installed.

> Q: Does this need to be run with admin privileges
>
> A: Yes, this application needs to be run with admin privileges to work properly

> Q: Can I run this on a 32-bit architecture?
>
> A: No, this application can only be run on 64-bit architectures, but support for it will come soon.

> Q: Can I use the WIM file from my Windows ISO?
>
> A: Yes, you can use the WIM file from your Windows ISO, just make sure you copy the location of the WIM file and paste it into the application when prompted.

> Q: Can this be run on a Windows PE?
>
> A: Yes, this application can be run on a Windows PE, just make sure you have the needed components installed.

> Q: Should I download the latest release?
>
> A: Yes, you should download the latest release to ensure that you have the latest version of the application and that you don't have to build it yourself.

> Q: Can I use this to install Windows 7 with?
>
> A: Yes you can, just make sure that you have CSM enabled in your BIOS and that you have the needed drivers ready after install, and that you're using the MBR partitioning scheme.

> Q: How do I set up my hard disk?
>
> A: This [link](https://www.elevenforum.com/t/universal-alternate-windows-11-install-method.2402/) from the ElevenFourms might be able to help you out with that.