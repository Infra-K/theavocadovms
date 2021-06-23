# C ++ 11 기반의 간단하고 사용하기 쉬운 경량 네트워크 프로그래밍 프레임 워크
플랫폼 | 컴파일 상태
# 一个基于C++11简单易用的轻量级网络编程框架
平台|编译状态
----|-------
Linux | [![Build Status](https://travis-ci.org/xiongziliang/ZLToolKit.svg?branch=master)](https://travis-ci.org/xiongziliang/ZLToolKit)
macOS | [![Build Status](https://travis-ci.org/xiongziliang/ZLToolKt-build_for_mac.svg?branch=master)](https://travis-ci.org/xiongziliang/ZLToolKt-build_for_mac)
iOS | [![Build Status](https://travis-ci.org/xiongziliang/ZLToolKt-build_for_ios.svg?branch=master)](https://travis-ci.org/xiongziliang/ZLToolKt-build_for_ios)
Android | [![Build Status](https://travis-ci.org/xiongziliang/ZLToolKt-build_for_android.svg?branch=master)](https://travis-ci.org/xiongziliang/ZLToolKt-build_for_android)

## 项目初衷
多年的编程经历让我接触过多种网络开源库，譬如libevent、libev、libuv、boost.asio等等。这些开源框架有些是用C语言开发的，里面包含了各种难以阅读层层嵌套佶屈聱牙的宏，学习起来非常费力；有些使用起来又不甚方便，代码被切割成碎片零零碎碎；有些虽然使用简单，但是却非常宏大，牵涉各种代码，配置复杂，很难交叉编译。由于作者既从事过linux服务器编程又有jni、ios的编程经历，所以一直以来在寻求既能在服务器端高效运行又能在嵌入式平台方便开发的方法，但是一直没有找到比较合适的方案；于是作者大约在一年前开始整理多年的工作成果积累，抽取经过时间检验证明稳定有效的代码并且参考其他成熟的框架形成了这个项目。后面在我使用该项目（初期版本）用于实际开发，一路林林总总遇到了很多问题，但是在后面几个月不间断的调试、测试、修正、优化等过程中项目代码逐渐沉淀稳定，经过长时高强度的测试之后我把代码提交到github形成了这个项目。
## 프로젝트의 원래 의도
다년간의 프로그래밍 경험을 통해 libevent, libev, libuv, boost.asio 등과 같은 다양한 네트워크 오픈 소스 라이브러리를 접하게되었습니다. 이러한 오픈 소스 프레임 워크 중 일부는 C 언어로 개발되었으며, 중첩되고 어색하며 배우기가 매우 힘든 다양한 읽기 어려운 매크로가 포함되어 있습니다. 일부는 사용하기 불편하고 코드가 조각으로 잘립니다. 깨진; 일부는 사용하기 쉽지만 매우 야심적이며 다양한 코드, 복잡한 구성 및 크로스 컴파일이 어렵습니다. 저자는 jni 및 ios 프로그래밍 경험뿐만 아니라 Linux 서버 프로그래밍에 종사해 왔기 때문에 서버 측에서 효율적으로 실행되고 임베디드 플랫폼에서 개발을 용이하게 할 수있는 방법을 찾고 있었지만 더 적합한 솔루션을 찾지 못했습니다. 저자는 약 1 년 전에 수년간 축적 된 작업을 정리하고 시간 테스트를 거친 후 안정적이고 효과적인 것으로 입증 된 코드를 추출하고 다른 성숙한 프레임 워크를 참조하여이 프로젝트를 구성하기 시작했습니다. 나중에 실제 개발을 위해 프로젝트 (초기 버전)를 사용했을 때 많은 문제가 발생했지만 중단없는 디버깅, 테스트, 수정, 최적화 등 몇 달 동안 프로젝트 코드가 점차 안정되고 안정화되었습니다. 오랜 시간 집중적인 테스트를 거친 후이 프로젝트를 구성하기 위해 코드를 github에 제출했습니다.

## 特性 ## 특징
- 网络库
  - tcp/udp客户端，接口简单易用并且是线程安全的，用户不必关心具体的socket api操作。
  - tcp服务器，使用非常简单，只要实现具体的tcp会话（TcpSession类）逻辑,使用模板的方式可以快速的构建高性能的服务器。
  - 对套接字多种操作的封装。
-네트워크 라이브러리
   -tcp / udp 클라이언트, 인터페이스는 간단하고 사용하기 쉬우 며 스레드로부터 안전합니다. 사용자는 특정 소켓 API 작업에 대해 신경 쓸 필요가 없습니다.
   -TCP 서버는 사용이 매우 간편하며, 특정 TCP 세션 (TcpSession 클래스) 로직이 구현되어있는 한 템플릿을 이용하여 고성능 서버를 빠르게 구축 할 수 있습니다.
   -소켓에 대한 여러 작업의 캡슐화.
- 线程库
  - 使用线程实现的简单易用的定时器（AsyncTaskThread）。
  - 读写锁。
  - 信号量的封装（ios下用条件变量实现）。
  - 自旋锁。
  - 线程组。
  - 简单易用的线程池，可以异步或同步执行任务，支持functional 和 lambad表达式。
-스레드 라이브러리
   -스레드를 사용하여 구현 된 간단하고 사용하기 쉬운 타이머 (AsyncTaskThread).
   -읽기-쓰기 잠금.
   -세마포어 캡슐화 (ios에서 조건 변수로 구현 됨).
   -스핀 잠금.
   -스레드 그룹.
   -간단하고 사용하기 쉬운 스레드 풀로 작업을 비동기식 또는 동기식으로 실행할 수 있으며 기능 및 람 바드 식을 지원합니다.
- 工具库
  - 文件操作。
  - std::cout风格的日志库，支持颜色高亮、代码定位、异步打印。
  - INI配置文件的读写。
  - 监听者模式的消息广播器。
  - 基于智能指针的循环池，不需要显式手动释放。
  - 环形缓冲，支持主动读取和读取事件两种模式。
  - mysql链接池，使用占位符（？）方式生成sql语句，支持同步异步操作。
  - 简单易用的ssl加解密黑盒，支持多线程。
  - 其他一些有用的工具。
-도구 라이브러리
   -파일 작업.
   -std :: cout 스타일 로그 라이브러리, 색상 강조 표시, 코드 위치 지정 및 비동기 인쇄를 지원합니다.
   -INI 구성 파일 읽기 및 쓰기.
   -리스너 모드의 메시지 브로드 캐스터.
   -스마트 포인터를 기반으로하는 순환 풀은 명시 적으로 수동으로 해제 할 필요가 없습니다.
   -링 버퍼, 활성 읽기 및 읽기 이벤트의 두 가지 모드를 지원합니다.
   -mysql 링크 풀, 자리 표시 자 (?)를 사용하여 SQL 문을 생성하고 동기 및 비동기 작업을 지원합니다.
   -SSL 암호화 및 복호화를위한 간단하고 사용하기 쉬운 블랙 박스, 멀티 스레딩 지원.
   -다른 유용한 도구.
   
## 后续任务
- 提供更多的示例代码
## 후속 작업
-더 많은 샘플 코드 제공
## 编译(Linux)
- 我的编译环境
  - Ubuntu16.04 64 bit + gcc5.4(最低gcc4.7)
  - cmake 3.5.1
- 编译
## 컴파일 (Linux)
-내 컴파일 환경
   -Ubuntu16.04 64 비트 + gcc5.4 (최소 gcc4.7)
   -cmake 3.5.1
-엮다
  ```
  cd ZLToolKit
  ./build_for_linux.sh
  ```  
  
## 编译(macOS)
- 我的编译环境
  - macOS Sierra(10.12.1) + xcode8.3.1
  - Homebrew 1.1.3
  - cmake 3.8.0
- 编译
## 컴파일 (macOS)
-내 컴파일 환경
   -macOS Sierra (10.12.1) + xcode8.3.1
   -Homebrew 1.1.3
   -cmake 3.8.0
-엮다  
  ```
  cd ZLToolKit
  ./build_for_mac.sh
  ```
	 
## 编译(iOS)
- 编译环境:`请参考macOS的编译指导。`
- 编译
## 컴파일 (iOS)
-컴파일 환경 :`macOS의 컴파일 가이드를 참조하세요. `
-엮다  
  ```
  cd ZLToolKit
  ./build_for_ios.sh
  ```
- 你也可以生成Xcode工程再编译：

  ```
  cd ZLToolKit
  mkdir -p build
  cd build
  # 生成Xcode工程，工程文件在build目录下 # Xcode 프로젝트 생성, 프로젝트 파일은 빌드 디렉토리에 있습니다.
  cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/iOS.cmake -DIOS_PLATFORM=SIMULATOR64 -G "Xcode"
  ```
## 컴파일 (Android)
-내 컴파일 환경
## 编译(Android)
- 我的编译环境
  - macOS Sierra(10.12.1) + xcode8.3.1
  - Homebrew 1.1.3
  - cmake 3.8.0
  - [android-ndk-r14b](https://dl.google.com/android/repository/android-ndk-r14b-darwin-x86_64.zip)
- 编译
-엮다
  ```
  cd ZLToolKit
  export ANDROID_NDK_ROOT=/path/to/ndk
  ./build_for_android.sh
  ```
	
## 联系方式
- 邮箱：<771730766@qq.com>
- QQ群：542509000
## 연락처 정보
-이메일 : <771730766@qq.com>
-QQ 그룹 : 542509000
