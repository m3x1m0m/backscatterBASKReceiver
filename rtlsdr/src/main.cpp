//============================================================================
// Name        : rtlsdr.cpp
// Author      : Elmar van Rijnswou
// Version     :
// Copyright   : Innoseis
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "CDemodulator.h"

using namespace std;

int main() {
	CDemodulator MyDemodulator(1.8e6,626e6,20);
	MyDemodulator.continuousReadout();
	return 0;
}
