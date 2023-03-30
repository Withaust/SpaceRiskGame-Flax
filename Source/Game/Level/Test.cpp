#include "Test.h"

Test::Test(const SpawnParams& params)
    : Script(params),
	Example(1.0f)
{
    _tickUpdate = true;
}

void Test::OnEnable()
{
	
}

void Test::OnDisable()
{
	
}

void Test::OnUpdate()
{
	USLEEP(Example)
	{
		//ULOG_DEBUG_STR("EXAMPLE");
	}
}
