#include "preproc.h"

map<wchar_t, vector<string>> pronunc;//����->ƴ��
map<wchar_t, vector<string>> duoyinzi;//������->ƴ��
map<wstring, string> duoyinci;//������->���������ֵķ���������������ֵ��������⴦��
set<wstring> wignore;//������1�������ֵĴ���
//set<string> paths;//�ѽ�����·��
set<wstring> not_added;//δ����Ķ�����
string cmd;

wstring str2wstr(const string& str)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0)
		return std::wstring(L"");

	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst)
		return std::wstring(L"");

	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	std::wstring wstr(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;

	return wstr;
}

string wstr2str(const wstring& wstr)
{
	LPCWSTR pwszSrc = wstr.c_str();
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
		return std::string("");

	char* pszDst = new char[nLen];
	if (!pszDst)
		return std::string("");

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	std::string str(pszDst);
	delete[] pszDst;
	pszDst = NULL;

	return str;
}

void makedir(string path_win)
{
	cmd = "md " + path_win;
	system(cmd.c_str());
}

//from ƴ��->���� to ����->ƴ��
//�洢pinyin.txt�еĺ��ֵ���ӦĿ¼
void trans()
{
	fstream fin;
	string line, pinyin;
	wstring wchars;
	vector<string> tmp;
	//string path_win = "C:\\Users\\dongc\\Source\\Repos\\myIME\\myIME\\corpus\\";
	regex re("[zcs]h.+");

	setlocale(LC_ALL, "chs");
	//wofstream f0;
	//f0.imbue(std::locale("chs"));
	int npos;
	fin.open("data/pinyin.txt", ios::in);
	while (getline(fin, line))
	{
		//����һ��ƴ�����
		npos = line.find(",");
		pinyin = line.substr(0, npos);
		wchars = str2wstr(line.substr(npos + 1));

		//if (regex_match(pinyin,re))
			//path_win += pinyin.substr(0,2);
		//else
			//path_win += pinyin[0];
		//makedir(path_win);
		//f0.open(path_win+'\\'+pinyin + ".txt", ios::out);
		for (auto& c : wchars)
		{
			//if (!f0.fail())
				//f0 << c << ' ' << 1 << endl;
			pronunc[c].push_back(pinyin);
			if (pronunc[c].size() > 1)
				duoyinzi[c] = pronunc[c];
		}
		//f0.close();
		//path_win="C:\\Users\\dongc\\Source\\Repos\\myIME\\myIME\\corpus\\";
		//system("cd ..");
	}
	fin.close();
}


//����data.txt����ȡ�����ִʴ����ӦĿ¼������Ӵ�Ƶ
void store_phrase(wstring phrase)
{
	if (wignore.find(phrase) != wignore.end())
	{
		return;
	}
	if (phrase == L"��ù�ϵ")
	{
		int i = 0;
	}
	wfstream wf1, wf2;
	setlocale(LC_ALL, "chs");
	wf1.imbue(std::locale("chs"));
	wf2.imbue(std::locale("chs"));
	string path_win = "C:\\Users\\dongc\\Source\\Repos\\myIME\\myIME\\corpus\\", pinyin;
	regex re("[zcs]h.+");
	int cnt = 0;//�ô��������������ָ���
	string cur;//��ǰ�ֵ�ƴ��

	for (auto& c : phrase)
	{
		if (pronunc.find(c) == pronunc.end())
		{
			cout << "No such character!" << endl;
			return;
		}
		//������
		if (pronunc[c].size() > 1)
		{
			cnt++;
			if (cnt > 1)//������1�������ֵĴ���
			{
				//�˹���Ԥ
				wfstream wfout;
				wfout.imbue(std::locale("chs"));
				setlocale(LC_ALL, "chs");
				wfout.open("data/ignore.txt", ios::app);
				wfout << phrase << endl;
				wfout.close();
				wignore.insert(phrase);
				return;
			}
			if (duoyinci.find(phrase) != duoyinci.end())
			{
				cur = duoyinci[phrase];
			}
			else if (not_added.find(phrase) == not_added.end())//��û����
			{
				//�˹���Ԥ
				not_added.insert(phrase);
				wfstream wfout;
				wfout.imbue(std::locale("chs"));
				setlocale(LC_ALL, "chs");
				wfout.open("data/dyc_new.txt", ios::app);
				wfout << phrase << endl;
				wfout.close();
				return;
			}
		}
		else
			cur = pronunc[c][0];
		pinyin += cur;
		//��ĸΪzh/ch/sh
		if (regex_match(cur, re))
		{
			path_win += cur.substr(0, 2) + '\\';
		}
		else
		{
			path_win += cur.substr(0, 1) + '\\';
		}
	}
	//if (paths.find(path_win) == paths.end())
	//{
	//	paths.insert(path_win);
	//	makedir(path_win);
	//}
	if(!fs::exists(path_win))
	{
		makedir(path_win);
	}
	wf1.open(path_win + pinyin + ".txt", ios::in);
	wstring tmp_phrase;
	int freq = 0, tmp_freq;
	while (wf1 >> tmp_phrase >> tmp_freq)
	{
		if (tmp_phrase == phrase)
		{
			freq = max(freq, tmp_freq);
		}
	}
	wf1.close();
	wf2.open(path_win + pinyin + ".txt", ios::app);
	wf2 << phrase << ' ' << freq + 1 << endl;
	wf2.close();
}

//��ȡ������data.txt�е���Ϣ
void proc_data()
{
	wfstream wfin;
	wstring phrase;
	wfin.imbue(std::locale("chs"));

	//�ڴ洢���Ͽ�����ȡ���ִ�
	wregex re(L"[\u4e00-\u9fa5]+");//����

	setlocale(LC_ALL, "chs");

	wfin.open("data/data.txt", ios::in);
	while (wfin >> phrase)
	{
		if (regex_match(phrase, re))
		{
			store_phrase(phrase);
		}
	}
	wfin.close();
}

//�����е�һЩ��¼�ļ��ж�ȡ��Ϣ�������ظ�����
void load_info()
{
	wfstream wfin;
	wfin.imbue(std::locale("chs"));
	setlocale(LC_ALL, "chs");
	wfin.open("data/duoyinci.txt", ios::in);
	wstring phrase, w_pinyin;
	string pinyin;
	while (wfin >> phrase >> w_pinyin)
	{
		pinyin = wstr2str(w_pinyin);
		duoyinci[phrase] = pinyin;
	}
	wfin.close();

	//for (auto& p : duoyinci)
	//	store_phrase(p.first);

	wfin.open("data/ignore.txt", ios::in);
	while (wfin >> phrase)
	{
		wignore.insert(phrase);
	}
	wfin.close();
}

//�洢�����ַ���ʹ�ã�ͬ�������ظ�����
void store_duoyinzi()
{
	wfstream wfs;
	wfs.imbue(std::locale("chs"));
	setlocale(LC_ALL, "chs");
	wfs.open("data/duoyinzi.txt", ios::out);
	for (auto& p : duoyinzi)
	{
		wfs << p.first << endl;
	}
	wfs.close();
}