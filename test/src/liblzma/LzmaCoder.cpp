#include "LzmaCoder.h"
#include <iostream>
#include <assert.h>

#define  S_KEY_LEN 5
static const Byte S_KEY_UPD[S_KEY_LEN] = {0x17,0xe1, 0xfb, 0x25, 0x47};//ÃÜÔ¿ÔöÇ¿
LzmaCoder::LzmaCoder()
{
	src_cache = nullptr;
	code_cache = nullptr;
}

LzmaCoder::~LzmaCoder()
{
	if (src_cache)
	{
		delete[] src_cache;
		src_cache = nullptr;
	}

	if (code_cache)
	{
		delete[] code_cache;
		code_cache = nullptr;
	}
}

static LzmaCoder* sp_LzmaCoder = nullptr;
LzmaCoder* LzmaCoder::getInstance()
{
	if (sp_LzmaCoder == nullptr)
	{
		sp_LzmaCoder = new LzmaCoder;
	}
	return sp_LzmaCoder;
}

bool LzmaCoder::resetCahe(Byte*& cache, size_t len)
{
	bool ret = true;
	if (len > CACHE_LEN)
	{
		if (cache)
		{
			delete[] cache;
		}
		cache = new Byte[len];
		ret = false;
	}
	else
	{
		if (cache == nullptr)
		{
			cache = new Byte[CACHE_LEN];
		}
		len = CACHE_LEN;
	}
	
	memset(cache, 0, len);
	return ret;
}

string LzmaCoder::code(const string& srcStr)
{
	size_t src_len = srcStr.length();

	bool isDefaut = resetCahe(src_cache, src_len);
	memcpy(src_cache, (Byte*)srcStr.c_str(), src_len);

	size_t code_len = code(src_cache, src_len);
	string ret_str((char*)code_cache, code_len);

	if (!isDefaut)
	{
		delete[] src_cache;
		src_cache = nullptr;
	}
	if (code_len > CACHE_LEN)
	{
		delete[] code_cache;
		code_cache = nullptr;
	}
	return ret_str;
}

size_t LzmaCoder::code(const Byte* src_buff, size_t src_len)
{
	bool isDefaut = resetCahe(code_cache, src_len+9);

	Byte skey[S_KEY_LEN] = "eos.";
	size_t skey_len = S_KEY_LEN;

	size_t code_len = CACHE_LEN > src_len ? CACHE_LEN : src_len;
	size_t ret = LzmaCompress(code_cache, &code_len, src_buff, src_len, skey, &skey_len, 9, (1 << 24), 8, 0, 2, 273, 1);
	if (ret != SZ_OK)
	{
		cout << "some error in func LzmaCompress";
	}
	for (int i = 0; i < S_KEY_LEN; i++)
	{
		code_cache[code_len] = skey[i] + S_KEY_UPD[i];
		code_len++;
	}
	Byte fix = 0xff;
	for (int i = 0; i < 4; i++)
	{
		code_cache[code_len] = src_len & fix;
		code_len++;
		src_len >>= 8;
	}
	return code_len;
}

string LzmaCoder::decode(const string& codeStr)
{
	size_t code_len = codeStr.length();

	bool isDefaut = resetCahe(code_cache, code_len);
	memcpy(code_cache, (Byte*)codeStr.c_str(), code_len);

	size_t src_len = decode(code_cache, code_len);
	string ret_str((char*)src_cache, src_len);

	if (!isDefaut)
	{
		delete[] code_cache;
		code_cache = nullptr;
	}
	if (src_len > CACHE_LEN)
	{
		delete[] src_cache;
		src_cache = nullptr;
	}
	return ret_str;
}

size_t LzmaCoder::decode(const Byte* code_buff, size_t src_len_param)
{
	size_t code_len = src_len_param - 9;

	Byte skey[S_KEY_LEN] = {};
	for (int i = code_len, j = 0; j < S_KEY_LEN; i++, j++)
	{
		skey[j] = (Byte)code_buff[i] - S_KEY_UPD[j];
	}

	size_t src_len = 0;
	for (int i = 1; i < 5; i++)
	{
		src_len <<= 8;
		src_len |= code_buff[src_len_param - i];
	}

	bool isDefaut = resetCahe(src_cache, src_len);

	size_t ret = LzmaUncompress(src_cache, &src_len, code_buff, &code_len, skey, S_KEY_LEN);
	if (ret != SZ_OK && code_len != src_len_param - 9)
	{
		cout << "some error in func LzmaCompress";
	}

	return src_len;
}

void LzmaCoder::copySrcBuff(Byte* src_buff, size_t len)
{
	memcpy(src_buff, src_cache, len);

	if (len > CACHE_LEN)
	{
		delete[] src_cache;
		src_cache = nullptr;
	}
}

void LzmaCoder::copyCodeBuff(Byte* dest_buff, size_t len)
{
	memcpy(dest_buff, code_cache, len);

	if (len > CACHE_LEN)
	{
		delete[] code_cache;
		code_cache = nullptr;
	}
}