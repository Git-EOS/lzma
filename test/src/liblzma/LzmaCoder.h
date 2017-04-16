#pragma once
#ifndef __LZMA_CODER__
#define __LZMA_CODER__

#include <string>
#include "LzmaLib.h"
#pragma comment(lib,"LZMA.lib")
using namespace std;

#define LZER LzmaCoder::getInstance()
#define CACHE_LEN 1048576 //1M

class LzmaCoder
{
public:
	LzmaCoder();
	~LzmaCoder();

	static LzmaCoder* getInstance();
	string code(const string& src);
	size_t code(const Byte* src_buff, size_t src_len);

	size_t decode(const Byte* code_buff, size_t code_len_param);
	string decode(const string& src);

	void copySrcBuff(Byte* src_buff, size_t len);
	void copyCodeBuff(Byte* code_buff, size_t len);
private:

	bool resetCahe(Byte*& cache, size_t len);

private:
	Byte* src_cache;//Ô´Âë³Ø
	Byte* code_cache;//±àÂë³Ø
	bool isKeep;//ÔÝÊ±±£Áô
};
#endif