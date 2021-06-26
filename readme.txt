There's not much gameplay, other than you could control the rocky monster with WASD, and try dodge that human flying right at you

Some new stuff added:
A custom profiling system, you can find that in the folder Engine / PerformanceProfiling
It's quite easy to use, just add a macro 'PERFORMANCE_MARKER' in a function or code section, the sytem will know it needs to evaluate that part. It outputs some info such as:
1. function name
2. file name
3. line number
4. How many cpu cycles was spent 
5. How much percent is that, compared to total cycles we profiled

All the results will be outputted in the debug window in Visual Studio

If you wish to ignore some library function calls during profiling, you can put 'PERFORMANCE_MARKER_WITH_NAME(given_name)' and 'END_PERFORMANCE_MARKER_WITH_NAME(given_name)'
before and after the section you wanna ignore, and the system would evaluate that as an individual profiling section, something like:

void someFunction() {
	PERFORMANCE_MARKER
	
	std::array<int, 10> s = {5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
    	auto print = [&s](std::string_view const rem) {
        		for (auto a : s) {
            			std::cout << a << ' ';
        		}
        		std::cout << ": " << rem << '\n';
    	};
	
	PERFORMANCE_MARKER_WITH_NAME(STD_SORT);
	std::sort(s.begin(), s.end());
	END_PERFORMANCE_MARKER_WITH_NAME(STD_SORT);

	print("sorted:");
}
