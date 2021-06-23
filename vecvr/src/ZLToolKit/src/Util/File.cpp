/*
 * File.cpp
 *
 *  Created on: 2016年9月22日
 *      Author: xzl
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include "File.h"
#include "logger.h"

using namespace std;

namespace ZL {
namespace Util {

// 可读普通文件 //일반 파일 읽기
bool File::isrfile(const char *path) {
	struct stat buf;
	int cc = stat(path, &buf);
	if (cc == -1) {
		//获取文件信息失败 //파일 정보를 가져 오지 못했습니다.
		WarnL << strerror(errno);
		return false;
	}
	//获取文件信息成功 //성공적으로 파일 정보 가져 오기
	if ((buf.st_mode & S_IFMT) != S_IFREG) {
		//S_IFMT   0170000    文件类型的位遮罩 파일 유형 비트 마스크
		//S_IFREG 0100000     一般文件 일반 문서
		//如果不是一般普通文件 일반 문서가 아닌 경우
		return false;
	}
	if (buf.st_mode & S_IROTH) {
		//S_IROTH 其他用户的读权限 //다른 사용자의 읽기 권한
		//如果其他用户可读该文件,那所有用户都可以读该文件 //다른 사용자가 파일을 읽을 수 있으면 모든 사용자가 파일을 읽을 수 있습니다.
		return true;
	}
	if (buf.st_mode & S_IRGRP) {
		//S_IRGRP 文件所有者同组用户的读权限 //파일 소유자와 동일한 그룹의 사용자에 대한 읽기 권한
		//如果该文件创建者规定同组用户可以读该文件 //파일 작성자가 동일한 그룹의 사용자가 파일을 읽을 수 있도록 지정하는 경우
		/*用来取得执行目前进程有效组识别码。//현재 프로세스의 유효 그룹 ID를 얻는 데 사용됩니다.
		 *有效的组识别码用来决定进程执行时组的权限。返回有效的组识别码。 //유효 그룹 ID는 프로세스가 실행될 때 그룹의 권한을 결정하는 데 사용됩니다. 유효한 그룹 ID를 반환하십시오.
		 */
		unsigned int egid = getegid(); //get-group-id
		if (egid == buf.st_gid) {
			//如果程序启动者与文件创建者是同一组用户 //프로그램 실행기와 파일 작성자가 동일한 사용자 그룹 인 경우
			return true;
		}
	}
	if (buf.st_mode & S_IRUSR) {
		//S_IRUSR 文件所有者的读权限 //파일 소유자에 대한 읽기 권한
		//如果该文件创建者规定创建者可以读该文件 //파일 작성자가 작성자가 파일을 읽을 수 있도록 지정하는 경우
		/*	
		 * 用来取得执行目前进程有效的用户识别码。
		 * 有效的用户识别码用来决定进程执行的权限，
		 * 借由此改变此值，进程可以获得额外的权限。
		 * 倘若执行文件的setID位已被设置，该文件执行时，
		 * 其进程的euid值便会设成该文件所有者的uid。
		 * 例如，执行文件/usr/bin/passwd的权限为-r-s--x--x，
		 * 其s位即为setID（SUID）位，
		 * 而当任何用户在执行passwd时其有效的用户识别码会被设成passwd所有者的uid值，
		 * 即root的uid值（0）。返回有效的用户识别码。
* 현재 프로세스를 실행하기위한 유효한 사용자 식별 코드를 얻는 데 사용됩니다.
* 유효한 사용자 ID는 실행할 프로세스의 권한을 결정하는 데 사용됩니다.
*이 값을 변경하면 프로세스가 추가 권한을 얻을 수 있습니다.
* 실행 파일의 setID 비트가 설정되어있는 경우 파일 실행시
* 프로세스의 euid 값은 파일 소유자의 uid로 설정됩니다.
* 예를 들어 / usr / bin / passwd 파일 실행 권한은 -r-s--x--x,
* s 비트는 setID (SUID) 비트입니다.
* 사용자가 passwd를 실행하면 유효한 사용자 ID는 passwd 소유자의 uid 값으로 설정됩니다.
* 루트 (0)의 uid 값. 유효한 사용자 ID를 반환하십시오.
		 */
		unsigned int euid = geteuid(); //get-group-id
		if (euid == buf.st_uid) {
			//如果程序启动者与文件创建者是同一组用户 //프로그램 실행기와 파일 작성자가 동일한 사용자 그룹 인 경우
			return true;
		}
	}
	return false;
}

FILE *File::createfile_file(const char *file, const char *mode) {
	std::string path = file;
	std::string dir;
	int index = 1;
	FILE *ret = NULL;
	while (1) {
		index = path.find('/', index) + 1;
		dir = path.substr(0, index);
		if (dir.length() == 0) {
			break;
		}
		if (access(dir.c_str(), 0) == -1) { //access函数是查看是不是存在 //액세스 기능은 존재 여부를 확인하는 것입니다.
			if (mkdir(dir.c_str(), 0777) == -1) {  //如果不存在就用mkdir函数来创建 //존재하지 않는 경우 mkdir 함수를 사용하여
				WarnL << dir << ":" << strerror(errno);
				return NULL;
			}
		}
	}
	if (path[path.size() - 1] != '/') {
		ret = fopen(file, mode);
	}
	return ret;
}
bool File::createfile_path(const char *file, unsigned int mod) {
	std::string path = file;
	std::string dir;
	int index = 1;
	while (1) {
		index = path.find('/', index) + 1;
		dir = path.substr(0, index);
		if (dir.length() == 0) {
			break;
		}
		if (access(dir.c_str(), 0) == -1) { //access函数是查看是不是存在 //액세스 기능은 존재 여부를 확인하는 것입니다.
			if (mkdir(dir.c_str(), mod) == -1) {  //如果不存在就用mkdir函数来创建 //존재하지 않는 경우 mkdir 함수를 사용하여
				WarnL << dir << ":" << strerror(errno);
				return false;
			}
		}
	}
	return true;
}

//判断是否为目录 //디렉토리인지 확인
bool File::is_dir(const char *path) {
	struct stat statbuf;
	if (lstat(path, &statbuf) == 0) {
		//lstat返回文件的信息，文件信息存放在stat结构中 //lstat는 stat 구조에 저장된 파일 정보를 반환합니다.
		if (S_ISDIR(statbuf.st_mode)) {
			return true;
		}
#ifdef __WIN32__
		return false;
#else //__WIN32__
		if (S_ISLNK(statbuf.st_mode)) {
			char realFile[256]={0};
			readlink(path,realFile,sizeof(realFile));
			return File::is_dir(realFile);
		}
#endif //__WIN32__
	}
	return false;
}

//判断是否为常规文件 //일반 파일인지 확인
bool File::is_file(const char *path) {
	struct stat statbuf;
	if (lstat(path, &statbuf) == 0)
		return S_ISREG(statbuf.st_mode) != 0; //判断文件是否为常规文件 //파일이 일반 파일인지 확인
	return false;
}

//判断是否是特殊目录 //특수 디렉토리인지 확인
bool File::is_special_dir(const char *path) {
	return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

//生成完整的文件路径 //완전한 파일 경로 생성
void get_file_path(const char *path, const char *file_name, char *file_path) {
	strcpy(file_path, path);
	if (file_path[strlen(path) - 1] != '/')
		strcat(file_path, "/");
	strcat(file_path, file_name);
}
bool  File::rm_empty_dir(const char *path){
	if(!is_dir(path)){
		string superDir = path;
		superDir = superDir.substr(0, superDir.find_last_of('/') + 1);
		return rm_empty_dir(superDir.data());
	}
	return rmdir(path) == 0;
}
void File::delete_file(const char *path) {
	DIR *dir;
	dirent *dir_info;
	char file_path[PATH_MAX];
	if (is_file(path)) {
		remove(path);
		return;
	}
	if (is_dir(path)) {
		if ((dir = opendir(path)) == NULL) {
			rmdir(path);
			closedir(dir);
			return;
		}
		while ((dir_info = readdir(dir)) != NULL) {
			if (is_special_dir(dir_info->d_name)) {
				continue;
			}
			get_file_path(path, dir_info->d_name, file_path);
			delete_file(file_path);
		}
		rmdir(path);
		closedir(dir);
		return;
	}
	unlink(path);
}

} /* namespace Util */
} /* namespace ZL */
