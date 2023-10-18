#include <iostream>
#include <fstream>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <chrono> // �ð� ������ ���� ���
#include <cstdlib> // �޸� ��뷮�� �����ϱ� ���� ���
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

static const int key_bunch = 10;
static const int ing_size = 100;		
static const int ven_size = 300;


//part ���̺� ����ü
struct part {
	int PARTKEY;
	char NAME[100];
	char MFGR[20];
	char BRAND[10];
	char TYPE[30];
	int SIZE;
	char CONTAINER[15];
	float RETAILPRICE;
	char COMMENT[30];
};
//partsupp ���̺� ����ü
struct partsupp {
	int PARTKEY;
	int SUPPKEY;
	int AVAILQTY;
	float SUPPLYCOST;
	char COMMNET[500];
};
//���� �о �����ϴ� �Լ�
void read_driving_file(const char* file_name, struct part* p) {
	ifstream file(file_name);
	
	if (!file.is_open()) {
		cout << "������ �� �� �����ϴ�." << endl;
		return;
	}
	for (int i = 0; i < ing_size; i++) {
		file >> p[i].PARTKEY;
		file.ignore(); // '|' ����

		file.getline(p[i].NAME, 100, '|');
		file.getline(p[i].MFGR, 20, '|');
		file.getline(p[i].BRAND, 10, '|');
		file.getline(p[i].TYPE, 30, '|');

		file >> p[i].SIZE;
		file.ignore(); // '|' ����

		file.getline(p[i].CONTAINER, 15, '|');
		file >> p[i].RETAILPRICE;
		file.ignore(); // '|' ����

		file.getline(p[i].COMMENT, 30);
	}
	file.close();
}
//���� �о �����ϴ� �Լ�
void read_driven_file(const char* file_name, struct partsupp * ps) {
	ifstream file(file_name);
	if (!file.is_open()) {
		cout << "������ �� �� �����ϴ�." << endl;
		return;
	}
	for (int i = 0; i < ven_size; i++) {
		file >> ps[i].PARTKEY;
		file.ignore();

		file >> ps[i].SUPPKEY;
		file.ignore();

		file >> ps[i].AVAILQTY;
		file.ignore();

		file >> ps[i].SUPPLYCOST;
		file.ignore();

		file.getline(ps[i].COMMNET, 500, '|');
		
	}
	file.close();
}

void print_part(struct part* p) {
	for (int i = 0; i < ing_size; i++) {
		cout << "PARTKEY: " << p[i].PARTKEY << endl;
		cout << "NAME: " << p[i].NAME << endl;
		cout << "MFGR: " << p[i].MFGR << endl;
		cout << "BRAND: " << p[i].BRAND << endl;
		cout << "TYPE: " << p[i].TYPE << endl;
		cout << "SIZE: " << p[i].SIZE << endl;
		cout << "CONTAINER: " << p[i].CONTAINER << endl;
		cout << "RETAILPRICE: " << p[i].RETAILPRICE << endl;
		cout << "COMMENT: " << p[i].COMMENT << endl;

		cout << "---------------------------------------" << endl;
	}
}

void print_partsupp(struct partsupp* ps) {
	for (int i = 0; i < ven_size; i++) {
		cout << "PARTKEY: " << ps[i].PARTKEY << endl;
		cout << "SUPPKEY: " << ps[i].SUPPKEY << endl;
		cout << "AVAILQTY: " << ps[i].AVAILQTY << endl;
		cout << "SUPPLYCOST: " << ps[i].SUPPLYCOST << endl;
		cout << "COMMNET: " << ps[i].COMMNET << endl;

		cout << "---------------------------------------" << endl;
	}
}

int main(int argc, char* argv[]) {


	const char* file1 = "part.tbl.txt";
	const char* file2 = "partsupp.tbl.txt";
	struct part driving_file[ing_size];
	struct partsupp driven_file[ven_size];

	// ���� �ð� ����
	auto start_time = std::chrono::high_resolution_clock::now();

	read_driving_file(file1, driving_file);
	read_driven_file(file2, driven_file);

	//print_part(driving_file);
	//print_partsupp(driven_file);

	// ������ ���� ����
	ofstream output_file("output.txt");

	int record_z = 0;
	int key_list[key_bunch] = { 0, };

	// Block Nested Loops Join
	for (int i = 0; i < ing_size/ key_bunch; i++) {
		//key ���� ���� 
		for (int z = record_z; z < key_bunch; z++) {
			key_list[z % key_bunch] = (driving_file[z].PARTKEY);
		}
		
		for (int j = 0; j < ven_size; j++) {
			for (int k = 0; k < key_bunch; k++) {
				if (key_list[k] == driven_file[j].PARTKEY) {
					int tmp = k + record_z;// 100 0��°  -1 ������?  1 % 10 
					output_file << driving_file[tmp].PARTKEY << "|"
						<< driving_file[tmp].NAME << "|"
						<< driving_file[tmp].MFGR << "|"
						<< driving_file[tmp].BRAND << "|"//1��[0] �� 1��[200] 
						<< driving_file[tmp].TYPE << "|"
						<< driving_file[tmp].SIZE << "|"
						<< driving_file[tmp].CONTAINER << "|"
						<< driving_file[tmp].RETAILPRICE << "|"
						<< driving_file[tmp].COMMENT << "|";

					// driven_file[j]�� ������ ���ο� ���Ͽ� �̾ ����
					output_file << driven_file[j].SUPPKEY << "|"
						<< driven_file[j].AVAILQTY << "|"
						<< driven_file[j].SUPPLYCOST << "|"
						<< driven_file[j].COMMNET << endl;
				}
			}
		}
		record_z += key_bunch;
	}
	// ���� �ð� ����
	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	std::cout << "���� �ð�: " << duration / 1000.0 << " ms" << std::endl;
	// �޸� Footprint ����
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T memory = pmc.PrivateUsage;
	std::cout << "�޸� Footprint: " << memory / 1024 << " KB" << std::endl;

	output_file.close();

	return 0;
}


// �� ���Ͽ� ���ؼ� �ҷ����� ����
// ing�� n���� key �������� ȣ��
// en�� ���ؼ� Ž��, Ž���� ����� ���ؼ� ing + �����̱�
// en�� ��ü Ž���� ���� key �������� �����
// ����
// 


// (n���� ���� = driving ���̺�)�� ��� �ѹ��� ��ġ
// PART ���̺��� n���� �ҷ�����
// n�ٿ��� key���� ������ �迭 ����
// ����Ʈ ���� �̿��Ͽ� driven ���̺� key���� ��Ī�Ͽ� ����

// [��, ������ ���� ����, �����ϸ� �����ϴ� �ð��� �߰���.]
// ���� ���̸� driving ���̺� ���Ŀ� add
// �ٸ� ���̸� pass