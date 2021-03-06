#include "SessionInspector.h"
#include "FakeWindowService.h"
#include <iostream>

using namespace SI;

void main()
{
	FakeWindowService fws;
	std::cout << "Is inside VirtualBox: " << std::boolalpha << SessionInspector::IsInsideVBox() << std::endl;
	std::cout << "Is Current Session Remoteable: " << std::boolalpha << SessionInspector::IsCurrentSessionRemoteable() << std::endl;
	fws.Start();
	std::this_thread::sleep_for(std::chrono::seconds(15));
	fws.Stop();
	system("pause");
}