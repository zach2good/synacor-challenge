#include <iostream>
#include <fstream>

#include "vm.h"

using namespace std;

int main() {

	VM vm = VM();
	vm.isRunning = true;
	vm.debug = false;
	vm.printOutput = true;

	//vm.restoreState("dump_695700.bin");
	//vm.isRunning = true;

	//vm.step();
	vm.run();

	return 0;
}

