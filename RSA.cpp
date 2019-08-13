#include"RSA.h"

void RSA::ecrept(const char* plain_file_in, const char* ecrept_file_out,
        long ekey, long pkey)//���ļ����м���
{
    std::ifstream file_in(plain_file_in);
    std::ofstream file_out(ecrept_file_out);
    if (!file_in.is_open())
    {
        std::cout << "open file error!!!" << std::endl;
    }
    //��һ������,�����ȼ���
    const int num =  1024;
    char buffer[num];
    long buffer_out[num];
    int cur_num;
    while (!file_in.eof())
    {
        file_in.read(buffer, num);
        cur_num = file_in.gcount();
        for (int i = 0; i < cur_num; ++i)
        {
            buffer_out[i] = ecrept((long)buffer[i], ekey, pkey);
        }
        file_out.write((char*)buffer_out, cur_num * sizeof(long));
    }
    file_in.close();
    file_out.close();
}

void RSA::decrept(const char* ecrept_file_in, const char* plain_file_out,
        long dkey, long pkey)//���ļ����н���
{
    std::ifstream file_in(ecrept_file_in);
    std::ofstream file_out(plain_file_out);
    if (!file_in.is_open())
    {
        std::cout << "open file error!!!" << std::endl;
    }

    const int num = 50;
    long buffer[num];
    char buffer_out[num];
    int cur_num;
    while (!file_in.eof())
    {
        file_in.read((char*)buffer, num * sizeof(long));
        cur_num = file_in.gcount();
        cur_num /= sizeof(long);
        for (int i = 0; i < cur_num; ++i)
        {
            buffer_out[i] =(char)ecrept(buffer[i], dkey, pkey);
        }
        file_out.write(buffer_out, cur_num);
    }
    file_in.close();
    file_out.close();
}

std::vector<long> RSA::ecrept(std::string& str_in, long ekey, long pkey)//���ַ�������
{
    std::vector<long> vec_out;
    for (const auto& e : str_in)
    {
        vec_out.push_back(ecrept(e, ekey, pkey));//�����ܵ���Ϣ���뵽vec_out��
    }
    return vec_out;
}

std::string RSA::decrept(std::vector<long>& ecrept_str, long dkey, long pkey)//�Լ��ܵ��ַ�������
{
    std::string str_out;
    for (const auto& e : ecrept_str)
    {
        str_out.push_back((char)ecrept(e, dkey, pkey));//�����ܵ��ַ����뵽str_out��
    }
    return str_out;
}

long RSA::ecrept(long mg, long ekey, long pkey)//����   mg����Ҫ���ܵ���Ϣ
{
    long mg_out = 1;
    long a = mg;
    long b = ekey;
    int c = pkey;
    while (b)
    {
        if (b & 1)
        {
            mg_out = (mg_out * a) % c;
        }
        b >> 1;
        a = (a * a) % c;
    }
    return mg_out;
}

long RSA::produce_prime()//��������
{
    srand(time(nullptr));
    long prime = 0;
    while (1)
    {
        prime = rand() % 20 + 2;//����һ������2������
        if (is_prime(prime))
        {
            break;
        }
    }
    return prime;
}

bool RSA::is_prime(long prime)//�ж��Ƿ�Ϊ����
{
    if (prime < 2)
    {
        return false;
    }
    for(int i = 2; i <= sqrt(prime); i++)
    {
        if (prime % i == 0)
        {
            return false;
        }
    }
    return true;
}

void RSA::produce_keys()//��Ź�Կ��Կ
{
    long prime1 = produce_prime();
    long prime2 = produce_prime();
    while (prime1 == prime2)//��������������ͬ
    {
        prime2 = produce_prime();
    }

    _key.pkey = produce_pkey(prime1, prime2);//p*q
    long orla = produce_orla(prime1, prime2);//(p-1)*(q-1)
    _key.ekey = produce_ekey(orla);//��Կ
    _key.dkey = produce_dkey(_key.ekey, orla);//��Կ

}

long RSA::produce_pkey(long prime1, long prime2)//pq->n
{
    return prime1 * prime2;
}

long RSA::produce_orla(long prime1, long prime2)//(p-1)(q-1)
{
    return (prime1 - 1) * (prime2 - 1);

}

long RSA::produce_ekey(long orla)//������Կ
{
    long ekey;
    srand(time(nullptr));
    while (1)
    {
        ekey = rand() % orla;
        if (ekey > 1 && produce_gcd(ekey, orla) == 1)
        {
            break;
        }
    }
    return ekey;

}

long RSA::produce_gcd(long ekey, long orla)//��ԿҪ��oral����
{
    long residual;           //residual:����
    while (residual = ekey % orla)
    {
        ekey = orla;
        orla = residual;
    }
    return orla;
}

long RSA::produce_dkey(long ekey, long orla)//������Կ
{
    long dkey = orla / ekey;
    while (1)
    {
        if ((dkey * ekey) % orla == 1)
        {
            break;
        }
        ++dkey;
    }
    return dkey;
}

void RSA::printInfo(std::vector<long>& ecrept_str)
{
    for (const auto& e : ecrept_str)
    {
        std::cout << e << "@";
    }
    std::cout << std::endl;
}


