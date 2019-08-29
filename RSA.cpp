#include"RSA.h"
#include<time.h>
#include<iostream>
#include<fstream>

void RSA::ecrept(const char* plain_file_in, const char* ecrept_file_out,
        bm::int1024_t ekey, bm::int1024_t pkey)//����Ϣ���м���
{
    std::ifstream file_in(plain_file_in);
    std::ofstream file_out(ecrept_file_out, std::ofstream::app);
    if (!file_in.is_open())
    {
        std::cout << "open file error!!!" << std::endl;
        return;
    }
    //��һ������,�����ȼ���
    const int num =  1024;
    char buffer[num];
    bm::int1024_t buffer_out[num];
    int cur_num;
    while (!file_in.eof())
    {
        file_in.read(buffer, num);
        cur_num = file_in.gcount();//���ض�ȡ���ֽ���
        for (int i = 0; i < cur_num; ++i)
        {
            buffer_out[i] = ecrept((bm::int1024_t)buffer[i], ekey, pkey);
        }
        file_out.write((char*)buffer_out, cur_num * sizeof(bm::int1024_t));
    }
    file_in.close();
    file_out.close();
}

void RSA::decrept(const char* ecrept_file_in, const char* plain_file_out,
        bm::int1024_t dkey, bm::int1024_t pkey)//����Ϣ���н���
{
    std::ifstream file_in(ecrept_file_in);
    std::ofstream file_out(plain_file_out);
    if (!file_in.is_open())
    {
        std::cout << "open file error!!!" << std::endl;
        return;
    }

    const int num = 50;
    bm::int1024_t buffer[num];
    char buffer_out[num];
    int cur_num;
    while (!file_in.eof())
    {
        //�򿪽����ļ��������ȡ����
        file_in.read((char*)buffer, num * sizeof(bm::int1024_t));
        cur_num = file_in.gcount();//���ض�ȡ�ֽ���
        cur_num /= sizeof(bm::int1024_t);
        for (int i = 0; i < cur_num; ++i)
        {
            buffer_out[i] =(char)ecrept(buffer[i], dkey, pkey);
        }
        file_out.write(buffer_out, cur_num);
    }
    file_in.close();
    file_out.close();
}

std::vector<bm::int1024_t> RSA::ecrept(std::string& str_in, bm::int1024_t ekey, bm::int1024_t pkey)//���ַ�������
{
    std::vector<bm::int1024_t> vec_out;
    for (const auto& e : str_in)
    {
        vec_out.push_back(ecrept(e, ekey, pkey));//�����ܵ���Ϣ���뵽vec_out��
    }
    return vec_out;
}

std::string RSA::decrept(std::vector<bm::int1024_t>& ecrept_str, bm::int1024_t dkey, bm::int1024_t pkey)//�Լ��ܵ��ַ�������
{
    std::string str_out;
    for (const auto& e : ecrept_str)
    {
        str_out.push_back((char)ecrept(e, dkey, pkey));//�����ܵ��ַ����뵽str_out��
    }
    return str_out;
}

bm::int1024_t RSA::ecrept(bm::int1024_t mg, bm::int1024_t ekey, bm::int1024_t pkey)//����   mg����Ҫ���ܵ���Ϣ
{
    bm::int1024_t mg_out = 1;
    bm::int1024_t a = mg;
    bm::int1024_t b = ekey;
    bm::int1024_t c = pkey;
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

bm::int1024_t RSA::produce_prime()//��������
{
    srand(time(nullptr));
    bm::int1024_t prime = 0;
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

bool RSA::is_prime(bm::int1024_t prime)//�ж��Ƿ�Ϊ����
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
    bm::int1024_t prime1 = produce_prime();
    bm::int1024_t prime2 = produce_prime();
    while (prime1 == prime2)//��������������ͬ
    {
        prime2 = produce_prime();
    }

    _key.pkey = produce_pkey(prime1, prime2);//p*q
    bm::int1024_t orla = produce_orla(prime1, prime2);//(p-1)*(q-1)
    _key.ekey = produce_ekey(orla);//��Կ
    _key.dkey = produce_dkey(_key.ekey, orla);//��Կ

}

bm::int1024_t RSA::produce_pkey(bm::int1024_t prime1, bm::int1024_t prime2)//pq->n
{
    return prime1 * prime2;
}

bm::int1024_t RSA::produce_orla(bm::int1024_t prime1, bm::int1024_t prime2)//(p-1)(q-1)
{
    return (prime1 - 1) * (prime2 - 1);

}

bm::int1024_t RSA::produce_ekey(bm::int1024_t orla)//������Կ
{
    bm::int1024_t ekey;
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

bm::int1024_t RSA::produce_gcd(bm::int1024_t ekey, bm::int1024_t orla)//��ԿҪ��oral����
{
    bm::int1024_t residual;           //residual:����
    while (residual = ekey % orla)
    {
        ekey = orla;
        orla = residual;
    }
    return orla;
}

bm::int1024_t RSA::produce_dkey(bm::int1024_t ekey, bm::int1024_t orla)//������Կ
{
    bm::int1024_t dkey = orla / ekey;
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

void RSA::printInfo(std::vector<bm::int1024_t>& ecrept_str)
{
    for (const auto& e : ecrept_str)
    {
        std::cout << e << "@";
    }
    std::cout << std::endl;
}


