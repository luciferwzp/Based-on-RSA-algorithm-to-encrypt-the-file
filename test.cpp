#include"RSA.h"

int main()
{
	RSA rsa;
	Key key = rsa.getKey();
	std::string str;
	std::cout << "��������Ҫ���ܵ���Ϣ��" << std::endl;
	getline(std::cin, str);
	std::vector<long> e_in = rsa.ecrept(str, key.ekey, key.pkey);
	std::string de_out = rsa.decrept(e_in, key.dkey, key.pkey);
	std::cout << "���ܺ����Ϣ��" << std::endl;
	rsa.printInfo(e_in);
	std::cout << "���ܺ����Ϣ��" << std::endl;
	std::cout << de_out << std::endl;
	return 0;
}
