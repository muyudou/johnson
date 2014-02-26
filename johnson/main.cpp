#include "johnson.h"

int main()
{
	ALGraph<char> wdgGraph(6);
	wdgGraph.createWDG();
	wdgGraph.Johnson();
	/*wdgGraph.displayGraph();*/

	system("pause");
	return 0;
}