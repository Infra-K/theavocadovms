/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
/** <!--
 *
 *  Copyright (C) 2021 infrak infrak@naver.com
 *  <http://infra-k.net>
 *
 * -->
 */
#include "vsccalculate.hpp"

#include <iostream>
#include <QTimer>
#ifdef WIN32
#include <windows.h>
#endif
#include <process.h>

#ifndef WIN32
#ifndef __APPLE__
/* Linux */
class cpuOccupy 
{
public:
	cpuOccupy()
	{
		memset(name, 0, 20);
		user = 0;
		nice = 0;
		system = 0;
		idle = 0;
		iowait = 0;
		irq = 0;
		softirq = 0;
	}
	char name[20];
	unsigned int user;
	unsigned int nice;
	unsigned int system;
	unsigned int idle;
	unsigned int iowait;
	unsigned int irq;
	unsigned int softirq;
} ;

class memOccupy {
public:
	memOccupy()
	{
		memset(name, 0, 20);
		total = 0;
		free = 0;
	}
	char name[20];
	unsigned long total;
	unsigned long free;
};

void getCpuOccupy(cpuOccupy *cpust) {
    FILE *fp;
    char buf[256];
    cpuOccupy *cpuOcc;
    cpuOcc = cpust;

    fp = fopen("/proc/stat", "r");
    if(!fp) {
        fprintf(stderr, "open /proc/stat error!\n");
        return;
    }
    fgets(buf, sizeof(buf), fp);
    sscanf(buf, "%s %u %u %u %u %u %u %u", cpuOcc->name, &cpuOcc->user, &cpuOcc->nice, &cpuOcc->system, &cpuOcc->idle, &cpuOcc->iowait
            ,&cpuOcc->irq, &cpuOcc->softirq);
    fclose(fp);
}

void getMemOccupy(memOccupy *memst) {
    FILE *fp;
    char buf[256];
    memOccupy *memOcc;
    memOcc = memst;

    fp = fopen("/proc/meminfo", "r");
    if(!fp) {
        fprintf(stderr, "open /proc/meminfo!\n");
        return;
    }
    fgets(buf, sizeof(buf), fp);
    sscanf(buf, "%s %lu %s", memOcc->name, &memOcc->total, memOcc->name); 
    fgets(buf, sizeof(buf), fp);
    sscanf(buf, "%s %lu %s", memOcc->name, &memOcc->free, memOcc->name);
}

int calCpuOccupy(cpuOccupy *c1, cpuOccupy *c2) {
    unsigned long total1, total2 , id, sd, xd;
    int cpuUsage = 0;

    total1 = c1->user + c1->nice + c1->system + c1->idle + c1->iowait + c1->irq + c1->softirq;
    total2 = c2->user + c2->nice + c2->system + c2->idle + c2->iowait + c2->irq + c2->softirq;

    id = c2->user - c1->user;
    sd = c2->system - c1->system;
    xd = c2->nice - c1->nice;

    if((total2 - total1) != 0)
        cpuUsage = ((sd + id + xd) *100) / (total2 - total1);
    else
        cpuUsage = 0;
    return cpuUsage;
}

#else /* macOS */
static double ParseMemValue(const char * b)
{
   while((*b)&&(isdigit(*b) == false)) b++;
   return isdigit(*b) ? atof(b) : -1.0;
}

// Returns a number between 0.0f and 1.0f, with 0.0f meaning all RAM is available, and 1.0f meaning all RAM is currently in use
float GetSystemMemoryUsagePercentage()
{
   FILE * fpIn = popen("/usr/bin/vm_stat", "r");
   if (fpIn)
   {
      double pagesUsed = 0.0, totalPages = 0.0;
      char buf[512];
      while(fgets(buf, sizeof(buf), fpIn) != NULL)
      {
         if (strncmp(buf, "Pages", 5) == 0)
         {
            double val = ParseMemValue(buf);
            if (val >= 0.0)
            {
               if ((strncmp(buf, "Pages wired", 11) == 0)||(strncmp(buf, "Pages active", 12) == 0)) pagesUsed += val;
               totalPages += val;
            }
         }
         else if (strncmp(buf, "Mach Virtual Memory Statistics", 30) != 0) break;  // Stop at "Translation Faults", we don't care about anything at or below that
      }
      pclose(fpIn);

      if (totalPages > 0.0) return (float) (pagesUsed/totalPages);
   }
   return -1.0f;  // indicate failure
}

#include <mach/mach_init.h>
#include <mach/mach_error.h>
#include <mach/mach_host.h>
#include <mach/vm_map.h>

static unsigned long long _previousTotalTicks = 0;
static unsigned long long _previousIdleTicks = 0;

float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
	unsigned long long totalTicksSinceLastTime = totalTicks-_previousTotalTicks;
	unsigned long long idleTicksSinceLastTime  = idleTicks-_previousIdleTicks;
	float ret = 1.0f-((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime)/totalTicksSinceLastTime : 0);
	_previousTotalTicks = totalTicks;
	_previousIdleTicks  = idleTicks;
	return ret;
}


// Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
// You'll need to call this at regular intervals, since it measures the load between
// the previous call and the current one.
float GetCPULoad()
{
   host_cpu_load_info_data_t cpuinfo;
   mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
   if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuinfo, &count) == KERN_SUCCESS)
   {
      unsigned long long totalTicks = 0;
      for(int i=0; i<CPU_STATE_MAX; i++) totalTicks += cpuinfo.cpu_ticks[i];
      return CalculateCPULoad(cpuinfo.cpu_ticks[CPU_STATE_IDLE], totalTicks);
   }
   else return -1.0f;
}

#endif
#endif

VSCCalculate::VSCCalculate(QLabel* label):m_label(label)
{
#ifndef WIN32
#ifndef __APPLE__
	m_LastCPU = new cpuOccupy;
	getCpuOccupy(m_LastCPU);
#endif
#endif
	m_Timer = new QTimer(this);
	connect(m_Timer, SIGNAL(timeout()), this, SLOT(showCpuMemoryState()));
	m_Timer->start(3000);
}

VSCCalculate::~VSCCalculate()
{
	m_Timer->stop();
	delete m_Timer;
	m_Timer = NULL;
#ifndef WIN32
#ifndef __APPLE__
	delete m_LastCPU;
	m_LastCPU = NULL;
#endif
#endif
}

#ifdef WIN32

static __int64 file_time_2_utc(const FILETIME* ftime)
{
	LARGE_INTEGER li;

	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;

	return li.QuadPart;
}

static int get_processor_number()
{
	SYSTEM_INFO info;
	
	GetSystemInfo(&info);

	return (int)info.dwNumberOfProcessors;
}

int get_cpu_usage(int pid)
{
	static int processor_count_ = -1;

	static __int64 last_time_ = 0;
	static __int64 last_system_time_ = 0;

	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;
	__int64 system_time;
	__int64 time;
	__int64 system_time_delta;
	__int64 time_delta;

	int cpu = -1;

	if (processor_count_ == -1)
	{
		processor_count_ = get_processor_number();
	}

	GetSystemTimeAsFileTime(&now);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
	{
		return -1;
	}
	system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / processor_count_;
	time = file_time_2_utc(&now);

	if ((last_system_time_ == 0) || (last_time_ == 0))
	{
		last_system_time_ = system_time;
		last_time_ = time;
		return -1;
	}

	system_time_delta = system_time - last_system_time_;
	time_delta = time - last_time_;

	if (time_delta == 0)
		return -1;

	cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
	last_system_time_ = system_time;
	last_time_ = time;

	return cpu;
}

__int64 CompareFileTime(FILETIME time1, FILETIME time2)
{
	__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime;
	__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime;

	return (b - a);
}

//infrak cpu 오차 수정
void VSCCalculate::showCpuMemoryState()
{
	HANDLE hEvent;
	BOOL res ;
	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;
	__int64 idle;
	__int64 kernel;
	__int64 user;
	__int64 sys;
	__int64 cpu;
	__int64 totalcpu;
	int nCnt, i;
	MEMORYSTATUSEX statex;

	totalcpu = nCnt = i = 0;
	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 500);

	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

	idle = CompareFileTime(preidleTime, idleTime);
	kernel = CompareFileTime(prekernelTime, kernelTime);
	user = CompareFileTime(preuserTime, userTime);

	sys = kernel + user;
	cpu = (sys - idle) * 100 / sys;	
	if (cpu <= 100)
	{
		totalcpu = totalcpu + cpu;
		nCnt++;
	}
	for (; i < 5; i++)
	{
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;

		WaitForSingleObject(hEvent, 100);

		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

		idle = CompareFileTime(preidleTime, idleTime);
		kernel = CompareFileTime(prekernelTime, kernelTime);
		user = CompareFileTime(preuserTime, userTime);

		sys = kernel + user;
		cpu = (sys - idle) * 100 / sys;
		if (cpu <= 100)
		{
			totalcpu = totalcpu + cpu;
			nCnt++;
		}
	}
	cpu = totalcpu / nCnt;	// cpu = get_cpu_usage(getpid());
	
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	
	m_label->setText(tr("  CPU: %1%  RAM: %2%").arg(cpu).arg(statex.dwMemoryLoad));

	return;
}

#if 0
void VSCCalculate::showCpuMemoryState()
{
	HANDLE hEvent;
	BOOL res ;
	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;

	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime ;

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL); 

	WaitForSingleObject( hEvent, 1000 ); 

	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

	int idle = CompareFileTime( preidleTime,idleTime);
	int kernel = CompareFileTime( prekernelTime, kernelTime);
	int user = CompareFileTime(preuserTime, userTime);

	int cpu = (kernel + user - idle)*100/(kernel + user);

	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime ;

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	m_label->setText(tr("  CPU: %1%  RAM: %2%").arg(cpu).arg(statex.dwMemoryLoad));
}
#endif

#else
#ifndef __APPLE__
/* Linux */
void VSCCalculate::showCpuMemoryState()
{
    int cpuUsage;
    int memUsage;
	
    cpuOccupy cpuStat;
    memOccupy memstat;
	
    getMemOccupy(&memstat);
    if(memstat.total != 0)
        memUsage = ((memstat.total - memstat.free) * 100) / memstat.total;
    else
        memUsage = 0;
	
	getCpuOccupy(&cpuStat);
	cpuUsage = calCpuOccupy(m_LastCPU, &cpuStat);

	memcpy(m_LastCPU, &cpuStat, sizeof(cpuOccupy));
	
	
	m_label->setText(tr("  CPU: %1%  RAM: %2%").arg(cpuUsage).arg(memUsage));
}
#else
/* macOS */
void VSCCalculate::showCpuMemoryState()
{
	int cpuUsage;
	int memUsage;

	float fCPU = GetCPULoad();
	float fMEM = GetSystemMemoryUsagePercentage();

	if (fCPU < 0)
	{
		fCPU = 0.0;
	}

	if (fMEM < 0)
	{
		fMEM = 0.0;
	}
	cpuUsage = fCPU * 100;
	memUsage = fMEM * 100;

	m_label->setText(tr("  CPU: %1%  RAM: %2%").arg(cpuUsage).arg(memUsage));
}
#endif

#endif
