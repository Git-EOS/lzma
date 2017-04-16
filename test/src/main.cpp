#include <iostream>
using namespace std;

#include "liblzma/LzmaLib.h"
#include "liblzma/LzmaCoder.h"
#pragma comment(lib,"LZMA.lib")


int main()
{
	char * baseChar = new char[1024 * 5 * 1024];
	for (int i = 0; i < 1024*1024; i++)
	{
		baseChar[i * 5 + 0] = 'a';
		baseChar[i * 5 + 1] = 'b';
		baseChar[i * 5 + 2] = 'c';
		baseChar[i * 5 + 3] = 'd';
		baseChar[i * 5 + 4] = 'e';
	}
	string baseStr(baseChar);
	cout << "bstr len:\t" << baseStr.length() << endl;
	//cout << "bstr :\t" << baseStr.c_str() << endl;
	auto str = LZER->code(baseStr);
	cout << "\nstr len:\t" << str.length() << endl;
	auto str2 = LZER->decode(str);
	cout << "\nstr2 len:\t" << str2.length() << endl;
	//cout << "str2 :\t" << str2.c_str() << endl;

	return 0;
}