#pragma once

#include <assert.h>
#include <vector>
#include <iostream>
#include <thread>
#include <type_traits>
#include <utility>
#include <tuple>

namespace Stass {
	// Is true if enabled Windows platform
	constexpr bool enabledWindows =
#if defined(_WIN32) || defined(WIN32)
		true;

	// Defined if the platform is Windows
	#define STASS_ENABLED_WINDOWS
#else
		false;
#endif

	// Is true if enabled Debug configuration
	constexpr bool enabledDebug =
#if defined(_DEBUG) || defined(DEBUG) || !defined(NDEBUG)
		true;

	// Defined if the configuration is Debug
	#define STASS_ENABLED_DEBUG
#else
		false;

	// Defined if the configuration is Release
	#define STASS_ENABLED_RELEASE
#endif

	// CPU archirecture
	enum CpuArch {
		UNDEFINED, // Undefined CPU architecture
		X_86,      // 32-bit x86 architecture
		X_64,      // 64-bit x86 architecture
		ARM,       // 32-bit ARM architecture
		ARM_64     // 64-bit ARM architecture
	};

	// Contains the enabled CPU architecture
	constexpr CpuArch cpuArch =
#if defined(_M_IX86) || defined(__i386__)
		CpuArch::X_86;

	#define STASS_CPU_X86
#elif defined(_M_X64) || defined(__x86_64__)
		CpuArch::X_64;

	#define STASS_CPU_X64
#elif defined(_M_ARM) || defined(__arm__)
		CpuArch::ARM;

	#define STASS_CPU_ARM
#elif defined(_M_ARM64) || defined(__aarch64__)
		CpuArch::ARM_64;

	#define STASS_CPU_ARM64
#else
		CpuArch::UNDEFINED;
#endif
}