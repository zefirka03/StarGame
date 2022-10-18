#pragma once
#include <iostream>
#include <cassert>
#include <chrono>
#define WA(x) (std::cout<<"WA: "<<x<<'\n');
#define AIR_STATIC_ASSERT(x, a) static_assert(x, a); 

namespace air {
	class Timer {
	public:
		Timer(const char* name) {
			func = name;
			start = std::chrono::high_resolution_clock::now();
		}
		~Timer() {
			stop();
		}
		void stop() {
			auto end = std::chrono::high_resolution_clock::now();

			auto s = std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count();
			auto e = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();

			auto duration = e - s;
			double ms = duration * 0.001;
			
			std::cout << "duration of " << func <<": " << ms << " ms" << "\n";
		}
	private:
		std::string func;
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
	};

	class TimeStep {
	public:
		TimeStep() { restart(); };
		~TimeStep() {};
		void restart() {
			start = std::chrono::high_resolution_clock::now();
		}
		double elapsed_ms() {
			auto end = std::chrono::high_resolution_clock::now();

			auto s = std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count();
			auto e = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();

			auto duration = e - s;
			double ms = duration * 0.001;

			return ms;
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
	};
}