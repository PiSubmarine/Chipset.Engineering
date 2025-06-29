#pragma once

#ifdef __cplusplus

#include <cstdint>
#include <array>
#include <memory>
#include <array>
#include <cstring>
#include <vector>

namespace PiSubmarine::Chipset::Engineering
{
	class AppMain
	{
	public:

		static AppMain* GetInstance(){return Instance;}
		AppMain();

		virtual ~AppMain();
		void Run();

	private:
		static AppMain* Instance;

	};
}

extern "C"
{
#endif
void AppMainRun(void *argument);

#ifdef __cplusplus
}
#endif
