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
	cout << "����quit���˳�����..." << endl;
	cout << "����enable-qp/jp��ǿ��ȫƴ/��ƴ���������ȼ�ƴ��������disable-qp/jp�Գ���..." << endl << endl;
	while (true)
	{
		cout << "������ƴ�����س�������" << endl;
		cin >> input;
		if (input == "quit")
			break;

		if (input == "enable-qp")
		{
			qp = true;
			jp = false;
			cout << "ǿ��ȫƴ�ѿ�����" << endl << endl;
		}
		else if (input == "disable-qp")
		{
			qp = false;
			cout << "ǿ��ȫƴ�ѹرգ�" << endl << endl;
		}
		else if (input == "enable-jp")
		{
			jp = true;
			qp = false;
			cout << "ǿ�Ƽ�ƴ�ѿ�����" << endl << endl;
		}
		else if (input == "disable-jp")
		{
			jp = false;
			cout << "ǿ�Ƽ�ƴ�ѹرգ�" << endl << endl;
		}
		else
			type_main(input, qp, jp);
	}
	return 0;
}