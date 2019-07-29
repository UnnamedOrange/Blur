#include "pch.h"
#include "resource.h"
#include "main.h"

Demo app;

int APIENTRY wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
	if (!app.SingleInstance())
	{
		app.host.ActivateHost();
		return 0;
	}
	return app.Execute();
}

INT Demo::Execute()
{
	host.Create();
	return host.MsgLoop();
}