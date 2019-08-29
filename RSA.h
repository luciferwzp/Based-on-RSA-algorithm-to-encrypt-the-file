#pragma once
#include<iostream>
#include <string>
#include <vector>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<fstream>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/random.hpp>
namespace bm = boost::multiprecision;

struct Key
{
	//��Կ(ekey, pkey): (e, n)
	bm::int1024_t pkey;
	bm::int1024_t ekey;
	//˽Կ(dkey, pkey): (d, n)
	bm::int1024_t dkey;
};

class RSA
{
public:
	RSA() {};
	Key getKey()
	{
		return _key;
	}
	void ecrept(const char* plain_file_in, const char* ecrept_file_out,
		bm::int1024_t ekey, bm::int1024_t pkey);//���ļ����н���
	void decrept(const char* ecrept_file_in, const char* plain_file_out,
		bm::int1024_t dkey, bm::int1024_t pkey);//���ļ����м���

	std::vector<bm::int1024_t> ecrept(std::string& str_in, bm::int1024_t ekey, bm::int1024_t pkey);//���ַ������м���
	std::string decrept(std::vector<bm::int1024_t>& ecrept_str, bm::int1024_t dkey, bm::int1024_t pkey);//���ַ������н���	

	void printInfo(std::vector<bm::int1024_t>& ecrept_str);//��ӡ���ܺ����Ϣ
private:
	//���ܵ�����Ϣ
	bm::int1024_t ecrept(bm::int1024_t msg, bm::int1024_t ekey, bm::int1024_t pkey);//����
	bm::int1024_t produce_prime();//��������
	bool is_prime(bm::int1024_t prime);//�ж��Ƿ�Ϊ����
	void produce_keys();//��Ź�Կ��Կ
	bm::int1024_t produce_pkey(bm::int1024_t prime1, bm::int1024_t prime2);//pq->n
	bm::int1024_t produce_orla(bm::int1024_t prime1, bm::int1024_t prime2);//(p-1)(q-1)
	bm::int1024_t produce_ekey(bm::int1024_t orla);//������Կ
	bm::int1024_t produce_gcd(bm::int1024_t ekey, bm::int1024_t orla);//����
	bm::int1024_t produce_dkey(bm::int1024_t ekey, bm::int1024_t orla);//������Կ

private:
	Key _key;
};
