#pragma once

#include <assert.h>
#include <vector>
#include <iostream>
#include <thread>

namespace Stass {
	constexpr bool enabledWindows =
#if defined(_WIN32) || defined(WIN32)
		true;

	#define STASS_ENABLED_WINDOWS
#else
		false;
#endif

	constexpr bool enabledDebug =
#if defined(_DEBUG) || defined(DEBUG) || !defined(NDEBUG)
		true;

	#define STASS_ENABLED_DEBUG
#else
		false;

	#define STASS_ENABLED_RELEASE
#endif

	enum CpuArch {
		X_86,
		X_64,
		ARM,
		ARM_64
	};

	constexpr CpuArch cpuArch =
#if defined(_M_IX86) || defined(__i386__)
		CpuArch::X_86;

	#ifndef STASS_CPU_X86
		#define STASS_CPU_X86
	#endif
#elif defined(_M_X64) || defined(__x86_64__)
		CpuArch::X_64;

	#ifndef STASS_CPU_X64
		#define STASS_CPU_X64
	#endif
#elif defined(_M_ARM) || defined(__arm__)
		CpuArch::ARM;

	#ifndef STASS_CPU_ARM
		#define STASS_CPU_ARM
	#endif
#elif defined(_M_ARM64) || defined(__aarch64__)
		CpuArch::ARM_64;

	#ifndef STASS_CPU_ARM64
		#define STASS_CPU_ARM64
	#endif
#endif
}