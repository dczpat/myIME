#include "preproc.h"
#include "type.h"

int main()
{
	load_info();
	trans();
	//store_duoyinzi();
	//proc_data();
	string input;
	bool qp = false;
	bool jp = false;
	cout << "输入quit以退出程序..." << endl;
	cout << "输入enable-qp/jp以强制全拼/简拼，否则优先简拼；再输入disable-qp/jp以撤销..." << endl << endl;
	while (true)
	{
		cout << "请输入拼音，回车结束：" << endl;
		cin >> input;
		if (input == "quit")
			break;

		if (input == "enable-qp")
		{
			qp = true;
			jp = false;
			cout << "强制全拼已开启！" << endl << endl;
		}
		else if (input == "disable-qp")
		{
			qp = false;
			cout << "强制全拼已关闭！" << endl << endl;
		}
		else if (input == "enable-jp")
		{
			jp = true;
			qp = false;
			cout << "强制简拼已开启！" << endl << endl;
		}
		else if (input == "disable-jp")
		{
			jp = false;
			cout << "强制简拼已关闭！" << endl << endl;
		}
		else
			type_main(input, qp, jp);
	}
	return 0;
}