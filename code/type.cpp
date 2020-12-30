#include "type.h"

const string base = "C:\\Users\\dongc\\Source\\Repos\\myIME\\myIME\\corpus\\";

string pattern1 = "(ang?|a[io]?|ou?|e[inr]?|pou|m[io]u|[bmp](ia[no]|[aei]ng?|a[io]?|ei|ie?|o|u)|me|fou|wai|[fw]([ae]ng?|a|ei|o|u)|dei|diu|[dt]([aeio]ng|an|a[io]?|e|ia[no]|ie?|ou|uan|u[ino]?)|lun|[nl](i?ang?|a[io]?|[eio]ng|ei|iao|in|i[eu]|ou|uan|u[eo]?|ve?)|ni|nen?|le|lia?|[ghk]([ae]ng?|a[io]?|ong|ou|uang?|uai|u[aino]?)|ke|[gh]ei?|[jqx](i(ang?|ao?|e|ng?|ong|u)?|uan|u[en]?)|([csz]h?|r)([ae]ng?|ao|e|i|ou|uan|u[ino]?)|[csz](ai?|ong)|[csz]h(ai?|uai|uang)|zei|[sz]hua|([cz]h|r)ong|y([ai]ng?|ao?|e|i|ong|ou|uan|u[en]?))";
string pattern2 = "((([csz]h|[abcdefghjklmnopqrstwxyz])'?)+)";
string pattern3 = "([csz]h|[abcdefghjklmnopqrstwxyz])";
regex re_quanpin = regex(pattern1);//单个全拼
regex re_isjianpin = regex(pattern2);//整个简拼
regex re_jianpin = regex(pattern3);//单个简拼
wfstream wf;
set<wstring> uniq;

string shengmu(string& pinyin)
{
	regex dual("[zcs]h.+");
	//声母为zh/ch/sh
	if (regex_match(pinyin, dual))
		return pinyin.substr(0, 2);
	else
		return pinyin.substr(0, 1);
}

bool cmp(const my_element& e1, const my_element& e2)
{
	return (e1.freq > e2.freq) || (e1.freq == e2.freq && e1.phrase > e2.phrase);
}

void jianpin(string& input, vector<string>& pinyin)
{
	smatch res;
	string cur;
	string::const_iterator iterStart = input.begin();
	string::const_iterator iterEnd = input.end();
	while (regex_search(iterStart, iterEnd, res, re_jianpin))
	{
		cur = res[0];
		pinyin.push_back(cur);
		iterStart = res[0].second;
	}
}

void quanpin(string& input, vector<string>& pinyin, string& full_pinyin)
{
	smatch res;
	string cur;
	string::const_iterator iterStart = input.begin();
	string::const_iterator iterEnd = input.end();
	while (regex_search(iterStart, iterEnd, res, re_quanpin))
	{
		cur = res[0];
		pinyin.push_back(cur);
		full_pinyin += cur;
		iterStart = res[0].second;
	}
}

void type_main(string& input, bool qp, bool jp)
{
	smatch res;
	string cur;
	vector<string> pinyin;
	string full_pinyin;
	setlocale(LC_ALL, "chs");
	wf.imbue(std::locale("chs"));
	if (jp || (!qp && regex_match(input, re_isjianpin)))
	{
		jianpin(input, pinyin);
		jp_choose(pinyin);
	}
	else
	{
		quanpin(input, pinyin, full_pinyin);
		qp_choose(pinyin, full_pinyin);
	}
}

void qp_choose(vector<string>& pinyin, string& full_pinyin)
{
	vector<my_element> result;
	wstring ret;
	vector<wstring> res_uniq;
	int option = 0;
	int cur_end = 0;
	int left = 0, right = pinyin.size() - 1;
	while (true)
	{
		qp_translate(pinyin, result, left, right, 0, cur_end);
		//按频率降序排列
		sort(result.begin(), result.end(), cmp);
		//res_uniq.push_back(result[0].phrase);
		//去除重复
		for (auto it = result.begin(); it < result.end(); ++it)
		{
			if (it->phrase.length() == cur_end - left + 1 && uniq.find(it->phrase) == uniq.end())
			{
				uniq.insert(it->phrase);
				res_uniq.push_back(it->phrase);
			}
		}
		uniq.clear();

		for (int i = 0; i < res_uniq.size(); ++i)
		{
			wcout << i + 1 << L". " + res_uniq[i] << L"  ";
		}

		cout << endl
			<< "请输入对应标号：" << endl
			<< "（输入0表示没有符合的选项）" << endl;
		cin >> option;
		if (option < 0 || option > res_uniq.size())
		{
			cout << "输入有误！" << endl << endl;
			return; //TODO
		}
		if (option == 0)
		{
			if (cur_end > 0)
				right = cur_end - 1;
			else //TODO
			{
				cout << "没有可供选择的选项！" << endl << endl;
				return;
			}
		}
		else
		{
			ret += res_uniq[option - 1];
			//跳出循环
			if (cur_end == pinyin.size() - 1)
				break;
			left = cur_end + 1;
			right = pinyin.size() - 1;
		}
		result.clear();
		res_uniq.clear();
	}
	string final_path = base;
	for (auto& str : pinyin)
	{
		final_path += shengmu(str) + "\\";
	}
	if (!fs::exists(final_path))
		makedir(final_path);
	final_path += full_pinyin + ".txt";
	wf.open(final_path.c_str(), ios::app);
	//被选中的优先级最高
	wf << ret << " " << INT_MAX << endl;
	wf.close();
	//输出结果
	wcout << endl << ret << endl << endl;
}

void jp_choose(vector<string>& pinyin)
{
	vector<my_element> result;
	wstring ret;
	vector<wstring> res_uniq;
	int option = 0;
	int cur_end = 0;
	int left = 0, right = pinyin.size() - 1;
	while (true)
	{
		jp_translate(pinyin, result, left, right, 0, cur_end);
		//按频率降序排列
		sort(result.begin(), result.end(), cmp);
		//去除重复
		for (auto it = result.begin(); it < result.end(); ++it)
		{
			if (it->phrase.length() == cur_end - left + 1 && uniq.find(it->phrase) == uniq.end())
			{
				uniq.insert(it->phrase);
				res_uniq.push_back(it->phrase);
			}
		}
		uniq.clear();
		for (int i = 0; i < res_uniq.size(); ++i)
		{
			wcout << i + 1 << L". " + res_uniq[i] << L"  ";
		}

		cout << endl
			<< "请输入对应标号：" << endl
			<< "（输入0表示没有符合的选项）" << endl;
		cin >> option;
		if (option < 0 || option > res_uniq.size())
		{
			cout << "输入有误！" << endl << endl;
			return; //TODO
		}
		if (option == 0)
		{
			if (cur_end > 0)
				right = cur_end - 1;
			else //TODO
			{
				cout << "没有可供选择的选项！" << endl << endl;
				return;
			}
		}
		else
		{
			ret += res_uniq[option - 1];
			//跳出循环
			if (cur_end == pinyin.size() - 1)
				break;
			left = cur_end + 1;
			right = pinyin.size() - 1;
		}
		result.clear();
		res_uniq.clear();
	}
	string final_path = base;
	for (auto& prefix : pinyin)
	{
		final_path += prefix + "\\";
	}
	if (!fs::exists(final_path))
		makedir(final_path);
	for (auto& c : ret)
	{
		//if(pronunc[c].size()==1)
		final_path += pronunc[c][0];
	}
	final_path += ".txt";
	wf.open(final_path.c_str(), ios::app);
	//被选中的优先级最高
	wf << ret << " " << INT_MAX << endl;
	wf.close();
	//输出结果
	wcout << endl << ret << endl << endl;
}

//end 当前完成的 如gu'xiang'meng'huan，识别出gu'xiang，则end=1，还剩下meng'huan仍需继续
bool qp_translate(vector<string>& pinyin, vector<my_element>& res, int left, int right, int cur, int& end)
{
	string path_win = base;
	string full_pinyin;
	wstring phrase;
	int freq;
	for (int i = left; i <= cur; ++i)
	{
		path_win += shengmu(pinyin[i]) + '\\';
		full_pinyin += pinyin[i];
	}

	if (cur < right && fs::exists(path_win + shengmu(pinyin[cur + 1])))
	{
		if (qp_translate(pinyin, res, left, right, cur + 1, end))
			return true;
	}

	string tmp_path = path_win + full_pinyin + ".txt";
	wf.open(tmp_path.c_str(), ios::in);
	if (wf.fail())
		return false;
	while (wf >> phrase >> freq)
	{
		my_element e(phrase, freq);
		res.push_back(e);
	}
	wf.close();
	end = cur;
	return true;
}

//end 当前完成的 如gu'xiang'meng'huan，识别出gu'xiang，则end=1，还剩下meng'huan仍需继续
bool jp_translate(vector<string>& pinyin, vector<my_element>& res, int left, int right, int cur, int& end)
{
	string path_win = base;
	wstring phrase;
	int freq;
	for (int i = left; i <= cur; ++i)
	{
		path_win += pinyin[i] + '\\';
	}

	if (cur < right && fs::exists(path_win + pinyin[cur + 1]))
	{
		if (jp_translate(pinyin, res, left, right, cur + 1, end))
			return true;
	}

	bool ret = false;
	fs::directory_iterator files(path_win);
	for (auto& it : files)
	{
		if (it.status().type() == fs::file_type::regular)
		{
			ret = true;
			wf.open(it.path().c_str(), ios::in);
			while (wf >> phrase >> freq)
			{
				my_element e(phrase, freq);
				res.push_back(e);
			}
			wf.close();
		}
	}
	if (ret)
		end = cur;
	return ret;
}