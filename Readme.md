# TheAvocadoVMS include AvocadoStorage(server) and AvocadoMonitor(client) #

# Build #
### Linux ###
	centos 7: sudo yum install nasm xorg-x11-server-devel zlib-devel gcc gcc-c++ perl-version libxcb libxcb-devel xcb-util xcb-util-devel xcb-util-*-devel libX11-devel libXrender-devel libXi-devel redhat-lsb-core libxslt-devel cmake libuuid-devel
	
	ubuntu: sudo apt-get install  "^libxcb.*" build-essential shtool libtool automake autoconf libbsd-dev libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev cmake libx11-dev yasm libxext-dev libgl1-mesa-dev  zlib1g-dev freeglut3-dev uuid-dev
	
	Change VE_PATH in the rules.mk
	$source ./rules.mk
	$make

### macOS ###
	Install xcode
	Change VE_PATH in the rules.mk
	$source ./rules-macos.mk
	$make 

### Windows ###
	1. Install VS2013
	2. Install Qt 5.x.x in the http://download.qt-project.org/official_releases/qt/
	3. http://download.qt-project.org/official_releases/vsaddin/
	4. Add Qt version to VS2013
	5. Change scripts\dllprepare* QT_WIN32 and QT_WIN64
	6. run the dllprepare.bat
	7. veuilib\prj_win32\veuilib.sln build
	8. vecvr\prj_win32\vecvr.sln build
	9. client\prj_win32\veclient.sln build
	
### Raspberry pi 3 ###
	Change VE_PATH & VE_CROSS_COMPILER_PATH in the rules-pi3.mk
	$source ./rules-pi3.mk
	$make 

# Wiki #
*  [Manual](http://infra-k.net/main/main.html)

*  [Manual PDF](http://infra-k.net/main/main.html)

#### Release Version ####
[https://github.com/infrak/version/tree/master/avocadovms](https://github.com/infrak/version/tree/master/avocadovms)

### Features list ###
	ONVIF profile S support.
	Unlimited timeline playback.
	Anti file system fragment recording system.
	Multi Display screen support.
	Alarm Support
	PTZ Support
	Emap
	HW accel Decoding
	MJPEG/MPEG4/H264/H265 support
	Linux(ARM X64 X86) Windows MacOS(Coming sonn)support
	RTSP over SSL(HTTPS)
	RTSP Server/RTMP Server/HLS Server

#### License ####
* Licensing: dual licensed as open source Affero GPL and commercial-use license (available for purchase).

[Learn More for License](http://infra-k.net/main/main.html)

# [Buy](http://infra-k.net/main/main.html) #


For more guide
[http://infra-k.net/](http://infra-k.net/)

Mail  : [infrak@naver.com](infrak@naver.com)

Fax    : 064-724-1042
