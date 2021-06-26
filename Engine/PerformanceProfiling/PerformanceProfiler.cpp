#include <PerformanceProfiling/PerformanceProfiler.h>
namespace Profiler {


	PerformanceProfiler& PerformanceProfiler::GetInstance() {
		static PerformanceProfiler m_instance;
		return m_instance;

	}

	void PerformanceProfiler::ResetInstance() {
		for (FunctionCallStack_t::iterator it = GetInstance().m_callStack.begin(); it != GetInstance().m_callStack.end(); ++it) {
			delete (*it);
		}

		GetInstance().m_callStack.clear();
		GetInstance().m_performanceLog.clear();
	}

	__int64 PerformanceProfiler::GetCPUCycleCount() {
		return __rdtsc();
	}

	PerformanceProfiler::PerformanceProfiler() :
		m_getCPUCycleOverhead(0),
		m_totalCPUCycles(0),
		m_lastCPUCycle(0)
	{
		// put a root function on the callstack so it's never empty
		m_performanceLog.insert(PerformanceData(__FILE__, "ROOT_function", 0));
		m_callStack.push_back(new PerformanceData(__FILE__, "ROOT_function", 0));

		m_lastCPUCycle = GetCPUCycleCount();
		CalibrateOverhead();

	}

	void PerformanceProfiler::CalibrateOverhead() {
		// TODO
	}

	void PerformanceProfiler::EnterFunction(const char* i_fileName, const char* i_functionName, const int i_lineNumber) {
		// mark time stamp when we end processing the previous function
		__int64 endCycle = GetCPUCycleCount();

		if (m_callStack.size() > 0) {
			// get the last item in vector (front item in stack)
			FunctionCallStack_t::reverse_iterator previousMarker = m_callStack.rbegin();

			// calculate total cycle spent on previous function
			__int64 cyclesSpent = (endCycle - m_lastCPUCycle) - m_getCPUCycleOverhead;
			(**previousMarker).cycles += cyclesSpent;

			m_totalCPUCycles += cyclesSpent;
		}


		// insert new item to performance log
		auto insertLogResult = m_performanceLog.insert(PerformanceData(i_fileName, i_functionName, i_lineNumber));

		// if there's duplicate log just ignore

		// update call count on the new function marker
		auto& referenceToIterator = *(insertLogResult.first);
		auto* ptrToIterator = &referenceToIterator;
		PerformanceData* newMarker = const_cast<PerformanceData*>(ptrToIterator);
		newMarker->functionCalls++;

		// push this performance marker onto stack
		m_callStack.push_back(newMarker);

		// update last cpu cycle
		m_lastCPUCycle = GetCPUCycleCount();

	}

	void PerformanceProfiler::ExitFunction() {
		// mark time stamp when we exit a function
		__int64 endCycle = GetCPUCycleCount();

		// update current function run time
		FunctionCallStack_t::reverse_iterator currentMarker = m_callStack.rbegin();
		__int64 cycleSpent = endCycle - m_lastCPUCycle;
		(**currentMarker).cycles += cycleSpent;
		m_totalCPUCycles += cycleSpent;
		
		// pop the current function on stack
		m_callStack.pop_back();

		// update last cpu cycle
		m_lastCPUCycle = GetCPUCycleCount();

	}

	void PerformanceProfiler::Log() {
		if (m_totalCPUCycles) {
			DEBUG_PRINT("Performance Log Marker Count: %d", m_performanceLog.size());
			DEBUG_PRINT("\t\t============ START:\tPerformance Profiling Log ============");

			for (PERFORMANCE_LOG_t::iterator it = m_performanceLog.begin(); it != m_performanceLog.end(); ++it) {
				double cyclePercent = static_cast<double>(it->cycles) / static_cast<double>(m_totalCPUCycles);
				DEBUG_PRINT("[\t%lld\tcycles]\t(%.5lf%%)\tused in\t[\t%d]\tcalls by\t[%s: %s]\tat line %d", it->cycles, cyclePercent * 100.0f, it->functionCalls, it->fileName, it->functionName, it->lineNumber);
			}
			DEBUG_PRINT("\t\t============ END:\tPerformance Profiling Log ============");
		}
	}

	PerformanceMarker::PerformanceMarker(const char* i_fileName, const char* i_functionName, const int i_lineNumber) :
		m_isOnStack(true) // initialize IsOnStack upon instantiating performance marker
	{
		PerformanceProfiler::GetInstance().EnterFunction(i_fileName, i_functionName, i_lineNumber);
	}

	PerformanceMarker::~PerformanceMarker() {
		// when the marker goes out of scope, invoke endMarker
		PerformanceMarker::EndMarker();
	}

	void PerformanceMarker::EndMarker() {
		if (m_isOnStack) {
			PerformanceProfiler::GetInstance().ExitFunction();
			m_isOnStack = false;
		}
	}




}