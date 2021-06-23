project(ZLToolKit)
cmake_minimum_required(VERSION 2.8)

#加载自定义模块 # 맞춤 모듈 불러 오기
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
#设置库文件路径 # 라이브러리 파일 경로 설정
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
#设置可执行程序路径 # 실행 가능한 프로그램 경로 설정
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#设置子目录 # 하위 디렉토리 설정
set(SUB_DIR_LIST "Network" "Poller" "Thread" "Util")

foreach(SUB_DIR ${SUB_DIR_LIST})
    #遍历源文件 #Traverse 소스 파일
    aux_source_directory(src/${SUB_DIR} SRC_LIST)
    #创建头文件安装文件夹 # 헤더 파일 설치 폴더 생성
    execute_process(COMMAND mkdir -p ${PROJECT_BINARY_DIR}/include/${CMAKE_PROJECT_NAME}/${SUB_DIR}/ )
    #遍历头文件 #Traverse 헤더 파일
    file(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/src/${SUB_DIR}/*.h")
    #拷贝头文件至安装文件夹 # 설치 폴더에 헤더 파일 복사
    execute_process(COMMAND cp ${HEADER_FILES} ${PROJECT_BINARY_DIR}/include/${CMAKE_PROJECT_NAME}/${SUB_DIR}/ )
endforeach(SUB_DIR ${SUB_DIR_LIST})

#非苹果平台移除.mm类型的文件 # 비 Apple 플랫폼에서 .mm 파일 제거
if(NOT APPLE)
list(REMOVE_ITEM SRC_LIST "src/Network/Socket_ios.mm")
endif(NOT APPLE)

set(LINK_LIB_LIST)

#查找openssl是否安装 #openssl이 설치되어 있는지 확인
find_package(OpenSSL QUIET)
if(OPENSSL_FOUND)
message(STATUS "找到openssl库:\"${OPENSSL_INCLUDE_DIR}\",ENABLE_OPENSSL宏已打开")#message (STATUS "opensl 라이브러리 발견 : \"$ {OPENSSL_INCLUDE_DIR} \ ", ENABLE_OPENSSL 매크로가 열렸습니다.")
include_directories(${OPENSSL_INCLUDE_DIR})
add_definitions(-DENABLE_OPENSSL)
list(APPEND  LINK_LIB_LIST ${OPENSSL_LIBRARIES})
endif(OPENSSL_FOUND)

#查找mysql是否安装 #mysql이 설치되어 있는지 확인
find_package(MYSQL QUIET)
if(MYSQL_FOUND)
message(STATUS "找到mysqlclient库:\"${MYSQL_INCLUDE_DIR}\",ENABLE_MYSQL宏已打开")#message (STATUS "Mysqlclient 라이브러리 발견 : \"$ {MYSQL_INCLUDE_DIR} \ ", ENABLE_MYSQL 매크로가 열렸습니다.")
include_directories(${MYSQL_INCLUDE_DIR})
add_definitions(-DENABLE_MYSQL)
list(APPEND  LINK_LIB_LIST ${MYSQL_LIBRARIES})
endif(MYSQL_FOUND)

#打印库文件 # 라이브러리 파일 인쇄
message(STATUS "将链接依赖库:${LINK_LIB_LIST}")
#引用头文件路径 # 참조 헤더 파일 경로
include_directories(${PROJECT_SOURCE_DIR}/src)
#使能c++11 # C ++ 11 활성화
add_compile_options(-std=c++11)
#add_compile_options(-Wno-deprecated-declarations)
#add_compile_options(-Wno-predefined-identifier-outside-function)


#编译动态库 # 동적 라이브러리 컴파일
if(NOT IOS AND NOT ANDROID)
add_library(${CMAKE_PROJECT_NAME}_shared SHARED ${SRC_LIST})
target_link_libraries(${CMAKE_PROJECT_NAME}_shared ${LINK_LIB_LIST})
set_target_properties(${CMAKE_PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME "${CMAKE_PROJECT_NAME}")
install(TARGETS ${CMAKE_PROJECT_NAME}_shared LIBRARY DESTINATION lib)
endif()

#编译静态库 # 정적 라이브러리 컴파일
add_library(${CMAKE_PROJECT_NAME}_static STATIC ${SRC_LIST})
set_target_properties(${CMAKE_PROJECT_NAME}_static PROPERTIES OUTPUT_NAME "${CMAKE_PROJECT_NAME}")
install(TARGETS ${CMAKE_PROJECT_NAME}_static ARCHIVE DESTINATION lib)

#安装头文件至系统目录 # 시스템 디렉토리에 헤더 파일 설치
install(DIRECTORY ${PROJECT_BINARY_DIR}/include/${CMAKE_PROJECT_NAME} DESTINATION include)

#测试程序 # 테스트 프로그램
if(NOT IOS)
add_subdirectory(tests)
endif(NOT IOS)
