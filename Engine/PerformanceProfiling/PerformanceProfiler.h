#pragma once
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <inttypes.h>
#include <Console/ConsolePrint.h>
#include <memory>
#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif


namespace Profiler {
	struct PerformanceData {
		PerformanceData(const char* i_fileName,
			const char* i_functionName,
			const int i_lineNumber,
			__int64 i_cycles = 0,
			__int64 i_functionCalls = 0) :
			fileName(i_fileName),
			functionName(i_functionName),
			lineNumber(i_lineNumber),
			cycles(i_cycles),
			functionCalls(i_functionCalls)
		{

		}

		const char* fileName;
		const char* functionName;
		const int lineNumber;
		__int64 cycles;
		__int64 functionCalls;
	};

	// this should be able to change when we log with different sorting methods
	class PerformanceLogSortByCycles {
	public:
		bool operator() (const PerformanceData& i_lhs, const PerformanceData& i_rhs) const {
			if (i_rhs.lineNumber == i_lhs.lineNumber) {
				return strcmp(i_rhs.fileName, i_lhs.fileName) > 0;
			}
			else {
				return i_rhs.lineNumber > i_lhs.lineNumber;
			}
		}
	};

	typedef std::set<PerformanceData, PerformanceLogSortByCycles> PERFORMANCE_LOG_t;
	typedef std::vector<PerformanceData*> FunctionCallStack_t;


	// the performance profiler class
	class PerformanceProfiler {
	public:
		static PerformanceProfiler& GetInstance();
		void ResetInstance();

		// enter the function and track it 
		void EnterFunction(const char* i_fileName, const char* i_functionName, const int i_lineNumber);
		
		// exit the function we're tracking
		void ExitFunction();

		// log profiling result
		void Log();

	private:
		PerformanceProfiler();
		__int64 GetCPUCycleCount();

		void CalibrateOverhead(); // to calibrate actual spent cycle on a function, cuz GetCPUCycle has overhead too

		PERFORMANCE_LOG_t m_performanceLog;
		FunctionCallStack_t m_callStack;

		// to record last cpu cycle, total cpu cycles spent
		// and cycle spent on calling GetCPUCycle (to calibrate actual spent cycle on a function, cuz GetCPUCycle has overhead too)
		__int64 m_lastCPUCycle;
		__int64 m_totalCPUCycles;
		__int64 m_getCPUCycleOverhead; // update this in 'CalibrateOverhead'
		
		

	};

	// performance marker, for ease of use
	class PerformanceMarker {
	public:
		// invoke EnterFunction in the constructor
		PerformanceMarker(const char* i_fileName, const char* i_functionName, const int i_lineNumber);
		~PerformanceMarker();

		// invoke ExitFunction
		void EndMarker();

	private:
		bool m_isOnStack;
	};

}

#define TO_STRINT_LITERAL(i) #i
#define PERFORMANCE_MARKER_WITH_NAME(i_name) Profiler::PerformanceMarker i_name(__FILE__, TO_STRINT_LITERAL(i_name), __LINE__);
#define END_PERFORMANCE_MARKER_WITH_NAME(i_name) i_name.EndMarker();
#define PERFORMANCE_MARKER Profiler::PerformanceMarker MARKER(__FILE__, __FUNCTION__, __LINE__);



