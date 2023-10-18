#include <iostream>
#include <fstream>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <chrono> // 시간 측정을 위한 헤더
#include <cstdlib> // 메모리 사용량을 측정하기 위한 헤더
#include <time.h>
#include <windows.h> //windows 헤더파일 include

#define _CRT_SECURE_NO_WARNINGS
using namespace std;
// 불러올 데이터 줄 = 구조체 수
static const int size_l = 500;
// 
static int size_limit = size_l;
static float main_momory_size;//1~48gb지만 너무 크므로 MB로 받기로
//1MB = 1000000 바이트

//1000000
// partsupp 테이블 구조체 1구조체당 239바이트 할당 약 250바이트로 지정
// 1000000 / 250 = 4000개의 구조체 타 프로그램을 활용하기 위한 메모리 공간을 비워둔다면
// 1메가에 구조체 4000개 
struct line_item {

	char ORDERKEY[7]; //int
	char PARTKEY[7];//int
	char SUPPKEY[7];//int
	char LINENUMBER[4];//int
	char QUANTITY[4];//int

	char EXTENDEDPRICE[10];//float
	char DISCOUNT[5];//float
	char TAX[5];//float

	char RETURNFLAG[2];
	char LINESTATUS[2];

	char SHIPDATE[11];
	char COMMITDATE[11];
	char RECEIPTDATE[11];

	char SHIPINSTRUCT[31];
	char SHIPMODE[21];
	char COMMENT[101];
};

void print_limit() {
	if (size_limit < size_l) {
		cout << "불러온 레코드 수 : " << size_limit << endl;
		cout << "main momory limit\n";
		cout << "---------------------------------------" << endl;
	}
}
//파일 읽어들이기
void read_file(const char* file_name, struct line_item* l) {
	ifstream file(file_name);
	if (!file.is_open()) {
		cout << "파일을 열 수 없습니다." << endl;
		return;
	}
	for (int i = 0; i < size_limit; i++) {
		file.getline(l[i].ORDERKEY, 7, '|');
		file.getline(l[i].PARTKEY, 7, '|');
		file.getline(l[i].SUPPKEY, 7, '|');
		file.getline(l[i].LINENUMBER, 4, '|');
		file.getline(l[i].QUANTITY, 4, '|');

		file.getline(l[i].EXTENDEDPRICE, 10, '|');
		file.getline(l[i].DISCOUNT, 5, '|');
		file.getline(l[i].TAX, 5, '|');

		file.getline(l[i].RETURNFLAG, 2, '|');
		file.getline(l[i].LINESTATUS, 2, '|');

		file.getline(l[i].SHIPDATE, 11, '|');
		file.getline(l[i].COMMITDATE, 11, '|');
		file.getline(l[i].RECEIPTDATE, 11, '|');

		file.getline(l[i].SHIPINSTRUCT, 31, '|');
		file.getline(l[i].SHIPMODE, 21, '|');
		file.getline(l[i].COMMENT, 101, '|');
	}
	print_limit();
	file.close();
}
//파일 출력하기
void print_lineitem(struct line_item* p) {
	for (int i = 0; i < size_limit; i++) {
		cout << "ORDERKEY: " << p[i].ORDERKEY << endl;
		cout << "PARTKEY: " << p[i].PARTKEY << endl;
		cout << "SUPPKEY: " << p[i].SUPPKEY << endl;
		cout << "LINENUMBER: " << p[i].LINENUMBER << endl;
		cout << "QUANTITY: " << p[i].QUANTITY << endl;
		cout << "EXTENDEDPRICE: " << p[i].EXTENDEDPRICE << endl;
		cout << "DISCOUNT: " << p[i].DISCOUNT << endl;
		cout << "TAX: " << p[i].TAX << endl;
		cout << "RETURNFLAG: " << p[i].RETURNFLAG << endl;
		cout << "LINESTATUS: " << p[i].LINESTATUS << endl;
		cout << "SHIPDATE: " << p[i].SHIPDATE << endl;
		cout << "COMMITDATE: " << p[i].COMMITDATE << endl;
		cout << "RECEIPTDATE: " << p[i].RECEIPTDATE << endl;
		cout << "SHIPINSTRUCT: " << p[i].SHIPINSTRUCT << endl;
		cout << "SHIPMODE: " << p[i].SHIPMODE << endl;
		cout << "COMMENT: " << p[i].COMMENT << endl;

		cout << "---------------------------------------" << endl;
	}
	print_limit();
}
//선택한 속성으로 정렬하기
void sort_by_attribute(int selected_attribute, struct line_item* l) {
	switch (selected_attribute) {
	case 1: {
		// ORDERKEY를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].ORDERKEY) > atoi(l[j].ORDERKEY)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 2: {
		// PARTKEY를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].PARTKEY) > atoi(l[j].PARTKEY)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 3: {
		// SUPPKEY를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].SUPPKEY) > atoi(l[j].SUPPKEY)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 4: {
		// LINENUMBER를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].LINENUMBER) > atoi(l[j].LINENUMBER)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 5: {
		// QUANTITY를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].QUANTITY) > atoi(l[j].QUANTITY)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 6: {
		// EXTENDEDPRICE를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].EXTENDEDPRICE) > atoi(l[j].EXTENDEDPRICE)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 7: {
		// SUPPKEY를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].DISCOUNT) > atoi(l[j].DISCOUNT)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 8: {
		// TAX를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].TAX) > atoi(l[j].TAX)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 9: {
		// SUPPKEY를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].RETURNFLAG) > atoi(l[j].RETURNFLAG)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 10: {
		// LINESTATUS를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].LINESTATUS) > atoi(l[j].LINESTATUS)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 11: {
		// SHIPDATE를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].SHIPDATE) > atoi(l[j].SHIPDATE)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 12: {
		// COMMITDATE를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].COMMITDATE) > atoi(l[j].COMMITDATE)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 13: {
		// RECEIPTDATE를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].RECEIPTDATE) > atoi(l[j].RECEIPTDATE)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 14: {
		// SHIPINSTRUCT를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].SHIPINSTRUCT) > atoi(l[j].SHIPINSTRUCT)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 15: {
		// SHIPMODE를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].SHIPMODE) > atoi(l[j].SHIPMODE)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	case 16: {
		// COMMENT를 기준으로 정렬하는 코드 작성
		for (int i = 0; i < size_limit - 1; i++) {
			for (int j = i + 1; j < size_limit; j++) {
				if (atoi(l[i].COMMENT) > atoi(l[j].COMMENT)) {
					swap(l[i], l[j]);
				}
			}
		}
		break;
	}
	default:
		cout << "잘못된 속성 선택입니다." << endl;
		return;
	}
	print_limit();
}
//선택한 속성 먼저 출력하기
void print_selected_attributes(int selected_attribute, struct line_item* l) {
	switch (selected_attribute) {
	case 1: {
		for (int i = 0; i < size_limit; i++) {
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 2: {
		for (int i = 0; i < size_limit; i++) {
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 3: {
		for (int i = 0; i < size_limit; i++) {
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 4: {
		for (int i = 0; i < size_limit; i++) {
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 5: {
		for (int i = 0; i < size_limit; i++) {
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 6: {
		for (int i = 0; i < size_limit; i++) {
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 7: {
		for (int i = 0; i < size_limit; i++) {
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 8: {
		for (int i = 0; i < size_limit; i++) {
			cout << "TAX: " << l[i].TAX << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 9: {
		for (int i = 0; i < size_limit; i++) {
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 10: {
		for (int i = 0; i < size_limit; i++) {
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 11: {
		for (int i = 0; i < size_limit; i++) {
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 12: {
		for (int i = 0; i < size_limit; i++) {
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 13: {
		for (int i = 0; i < size_limit; i++) {
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 14: {
		for (int i = 0; i < size_limit; i++) {
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 15: {
		for (int i = 0; i < size_limit; i++) {
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	case 16: {
		for (int i = 0; i < size_limit; i++) {
			cout << "COMMENT: " << l[i].COMMENT << endl;
			cout << "ORDERKEY: " << l[i].ORDERKEY << endl;
			cout << "PARTKEY: " << l[i].PARTKEY << endl;
			cout << "SUPPKEY: " << l[i].SUPPKEY << endl;
			cout << "LINENUMBER: " << l[i].LINENUMBER << endl;
			cout << "QUANTITY: " << l[i].QUANTITY << endl;
			cout << "EXTENDEDPRICE: " << l[i].EXTENDEDPRICE << endl;
			cout << "DISCOUNT: " << l[i].DISCOUNT << endl;
			cout << "TAX: " << l[i].TAX << endl;
			cout << "RETURNFLAG: " << l[i].RETURNFLAG << endl;
			cout << "LINESTATUS: " << l[i].LINESTATUS << endl;
			cout << "SHIPDATE: " << l[i].SHIPDATE << endl;
			cout << "COMMITDATE: " << l[i].COMMITDATE << endl;
			cout << "RECEIPTDATE: " << l[i].RECEIPTDATE << endl;
			cout << "SHIPINSTRUCT: " << l[i].SHIPINSTRUCT << endl;
			cout << "SHIPMODE: " << l[i].SHIPMODE << endl;
			cout << "---------------------------------------" << endl;
		}
		break;
	}
	default:
		cout << "exit" << endl;
		cout << "---------------------------------------" << endl;
		return;
	}
	print_limit();
}

int main(int argc, char* argv[]) {

	const char* file = "lineitem.tbl.txt";
	struct line_item file_s[size_l];

	cout << "주 메모리 크기 입력(MB) : ";
	cin >> main_momory_size;
	// 메모리 한계로 사이즈 크기 지정
	size_limit = main_momory_size * 4000 / 250;
	cout << "출력 가능한 레코드 수 : " << size_limit << endl<< endl;
	

	int wait_seconds = 3; // 대기할 시간 (초 단위)
	printf("출력 준비중.\n");
	// 현재 시간을 구하기
	time_t start_time = time(NULL);

	// 대기하기
	while (difftime(time(NULL), start_time) < wait_seconds) {
		// 대기
	}
	system("cls"); //콘솔창 clear
	cout << "주 메모리 크기 입력(MB) : "<< main_momory_size << endl << "출력 가능한 레코드 수 : " << size_limit << endl << endl;;
	//파일 디스크에서 불러오기
	read_file(file, file_s);

	//출력
	print_lineitem(file_s);

	// 사용자로부터 속성 선택 받기
	int selected_attribute;
	cout << "정렬할 속성을 선택하세요\n" <<"(1. ORDERKEY, 2. PARTKEY, 3. SUPPKEY, 4. LINENUMBER, 5. QUANTITY, 6. EXTENDEDPRICE, 7. DISCOUNT, 8. TAX, 9. RETURNFLAG, 10. LINESTATUS, 11. SHIPDATE, 12. COMMITDATE, 13. RECEIPTDATE, 14. SHIPINSTRUCT, 15. SHIPMODE, 16. COMMENT) \n : ";
	cin >> selected_attribute;
	//선택한 속성에 따라 정렬
	sort_by_attribute(selected_attribute, file_s);
	//선택한 속성에 따라 출력
	print_selected_attributes(selected_attribute, file_s);


	return 0;
}