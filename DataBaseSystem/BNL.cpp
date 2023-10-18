#include <iostream>
#include <fstream>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <chrono> // 시간 측정을 위한 헤더
#include <cstdlib> // 메모리 사용량을 측정하기 위한 헤더
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

static const int key_bunch = 10;
static const int ing_size = 100;		
static const int ven_size = 300;


//part 테이블 구조체
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
//partsupp 테이블 구조체
struct partsupp {
	int PARTKEY;
	int SUPPKEY;
	int AVAILQTY;
	float SUPPLYCOST;
	char COMMNET[500];
};
//파일 읽어서 저장하는 함수
void read_driving_file(const char* file_name, struct part* p) {
	ifstream file(file_name);
	
	if (!file.is_open()) {
		cout << "파일을 열 수 없습니다." << endl;
		return;
	}
	for (int i = 0; i < ing_size; i++) {
		file >> p[i].PARTKEY;
		file.ignore(); // '|' 무시

		file.getline(p[i].NAME, 100, '|');
		file.getline(p[i].MFGR, 20, '|');
		file.getline(p[i].BRAND, 10, '|');
		file.getline(p[i].TYPE, 30, '|');

		file >> p[i].SIZE;
		file.ignore(); // '|' 무시

		file.getline(p[i].CONTAINER, 15, '|');
		file >> p[i].RETAILPRICE;
		file.ignore(); // '|' 무시

		file.getline(p[i].COMMENT, 30);
	}
	file.close();
}
//파일 읽어서 저장하는 함수
void read_driven_file(const char* file_name, struct partsupp * ps) {
	ifstream file(file_name);
	if (!file.is_open()) {
		cout << "파일을 열 수 없습니다." << endl;
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

	// 시작 시간 측정
	auto start_time = std::chrono::high_resolution_clock::now();

	read_driving_file(file1, driving_file);
	read_driven_file(file2, driven_file);

	//print_part(driving_file);
	//print_partsupp(driven_file);

	// 저장할 파일 생성
	ofstream output_file("output.txt");

	int record_z = 0;
	int key_list[key_bunch] = { 0, };

	// Block Nested Loops Join
	for (int i = 0; i < ing_size/ key_bunch; i++) {
		//key 묶음 생성 
		for (int z = record_z; z < key_bunch; z++) {
			key_list[z % key_bunch] = (driving_file[z].PARTKEY);
		}
		
		for (int j = 0; j < ven_size; j++) {
			for (int k = 0; k < key_bunch; k++) {
				if (key_list[k] == driven_file[j].PARTKEY) {
					int tmp = k + record_z;// 100 0번째  -1 마지막?  1 % 10 
					output_file << driving_file[tmp].PARTKEY << "|"
						<< driving_file[tmp].NAME << "|"
						<< driving_file[tmp].MFGR << "|"
						<< driving_file[tmp].BRAND << "|"//1번[0] 과 1번[200] 
						<< driving_file[tmp].TYPE << "|"
						<< driving_file[tmp].SIZE << "|"
						<< driving_file[tmp].CONTAINER << "|"
						<< driving_file[tmp].RETAILPRICE << "|"
						<< driving_file[tmp].COMMENT << "|";

					// driven_file[j]의 정보를 새로운 파일에 이어서 쓰기
					output_file << driven_file[j].SUPPKEY << "|"
						<< driven_file[j].AVAILQTY << "|"
						<< driven_file[j].SUPPLYCOST << "|"
						<< driven_file[j].COMMNET << endl;
				}
			}
		}
		record_z += key_bunch;
	}
	// 종료 시간 측정
	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	std::cout << "수행 시간: " << duration / 1000.0 << " ms" << std::endl;
	// 메모리 Footprint 측정
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T memory = pmc.PrivateUsage;
	std::cout << "메모리 Footprint: " << memory / 1024 << " KB" << std::endl;

	output_file.close();

	return 0;
}


// 두 파일에 대해서 불러오기 수행
// ing가 n개의 key 묶음으로 호출
// en에 대해서 탐색, 탐색된 결과에 대해서 ing + 덧붙이기
// en를 전체 탐색후 다음 key 묶음으로 재수행
// 종료
// 


// (n개의 라인 = driving 테이블)을 묶어서 한번에 서치
// PART 테이블에서 n줄을 불러오기
// n줄에서 key값만 추출한 배열 생성
// 리스트 값을 이용하여 driven 테이블에 key값만 매칭하여 추출

// [단, 정렬은 하지 않음, 정렬하면 정렬하는 시간이 추가됨.]
// 같은 값이면 driving 테이블 이후에 add
// 다른 값이면 pass