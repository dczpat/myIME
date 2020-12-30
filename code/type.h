#include "Header.h"

struct my_element
{
	wstring phrase;
	int freq;
	my_element(wstring p = L"", int f = 0) : phrase(p), freq(f) {}
};

extern map<wchar_t, vector<string>> pronunc;//∫∫◊÷->∆¥“Ù;

void makedir(string path_win);
bool qp_translate(vector<string>& pinyin, vector<my_element>& res, int left, int right, int cur, int& end);
bool jp_translate(vector<string>& pinyin, vector<my_element>& res, int left, int right, int cur, int& end);
void type_main(string& input, bool qp, bool jp);
void qp_choose(vector<string>& pinyin, string& full_pinyin);
void jp_choose(vector<string>& pinyin);