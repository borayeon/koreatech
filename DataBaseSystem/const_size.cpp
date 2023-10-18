#include <iostream>
#include <fstream>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <chrono> // �ð� ������ ���� ���
#include <cstdlib> // �޸� ��뷮�� �����ϱ� ���� ���
#include <time.h>
#include <windows.h> //windows ������� include

#define _CRT_SECURE_NO_WARNINGS
using namespace std;
// �ҷ��� ������ �� = ����ü ��
static const int size_l = 500;
// 
static int size_limit = size_l;
static float main_momory_size;//1~48gb���� �ʹ� ũ�Ƿ� MB�� �ޱ��
//1MB = 1000000 ����Ʈ

//1000000
// partsupp ���̺� ����ü 1����ü�� 239����Ʈ �Ҵ� �� 250����Ʈ�� ����
// 1000000 / 250 = 4000���� ����ü Ÿ ���α׷��� Ȱ���ϱ� ���� �޸� ������ ����дٸ�
// 1�ް��� ����ü 4000�� 
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
		cout << "�ҷ��� ���ڵ� �� : " << size_limit << endl;
		cout << "main momory limit\n";
		cout << "---------------------------------------" << endl;
	}
}
//���� �о���̱�
void read_file(const char* file_name, struct line_item* l) {
	ifstream file(file_name);
	if (!file.is_open()) {
		cout << "������ �� �� �����ϴ�." << endl;
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
//���� ����ϱ�
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
//������ �Ӽ����� �����ϱ�
void sort_by_attribute(int selected_attribute, struct line_item* l) {
	switch (selected_attribute) {
	case 1: {
		// ORDERKEY�� �������� �����ϴ� �ڵ� �ۼ�
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
		// PARTKEY�� �������� �����ϴ� �ڵ� �ۼ�
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
		// SUPPKEY�� �������� �����ϴ� �ڵ� �ۼ�
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
		// LINENUMBER�� �������� �����ϴ� �ڵ� �ۼ�
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
		// QUANTITY�� �������� �����ϴ� �ڵ� �ۼ�
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
		// EXTENDEDPRICE�� �������� �����ϴ� �ڵ� �ۼ�
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
		// SUPPKEY�� �������� �����ϴ� �ڵ� �ۼ�
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
		// TAX�� �������� �����ϴ� �ڵ� �ۼ�
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
		// SUPPKEY�� �������� �����ϴ� �ڵ� �ۼ�
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
		// LINESTATUS�� �������� �����ϴ� �ڵ� �ۼ�
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
		// SHIPDATE�� �������� �����ϴ� �ڵ� �ۼ�
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
		// COMMITDATE�� �������� �����ϴ� �ڵ� �ۼ�
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
		// RECEIPTDATE�� �������� �����ϴ� �ڵ� �ۼ�
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
		// SHIPINSTRUCT�� �������� �����ϴ� �ڵ� �ۼ�
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
		// SHIPMODE�� �������� �����ϴ� �ڵ� �ۼ�
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
		// COMMENT�� �������� �����ϴ� �ڵ� �ۼ�
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
		cout << "�߸��� �Ӽ� �����Դϴ�." << endl;
		return;
	}
	print_limit();
}
//������ �Ӽ� ���� ����ϱ�
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

	cout << "�� �޸� ũ�� �Է�(MB) : ";
	cin >> main_momory_size;
	// �޸� �Ѱ�� ������ ũ�� ����
	size_limit = main_momory_size * 4000 / 250;
	cout << "��� ������ ���ڵ� �� : " << size_limit << endl<< endl;
	

	int wait_seconds = 3; // ����� �ð� (�� ����)
	printf("��� �غ���.\n");
	// ���� �ð��� ���ϱ�
	time_t start_time = time(NULL);

	// ����ϱ�
	while (difftime(time(NULL), start_time) < wait_seconds) {
		// ���
	}
	system("cls"); //�ܼ�â clear
	cout << "�� �޸� ũ�� �Է�(MB) : "<< main_momory_size << endl << "��� ������ ���ڵ� �� : " << size_limit << endl << endl;;
	//���� ��ũ���� �ҷ�����
	read_file(file, file_s);

	//���
	print_lineitem(file_s);

	// ����ڷκ��� �Ӽ� ���� �ޱ�
	int selected_attribute;
	cout << "������ �Ӽ��� �����ϼ���\n" <<"(1. ORDERKEY, 2. PARTKEY, 3. SUPPKEY, 4. LINENUMBER, 5. QUANTITY, 6. EXTENDEDPRICE, 7. DISCOUNT, 8. TAX, 9. RETURNFLAG, 10. LINESTATUS, 11. SHIPDATE, 12. COMMITDATE, 13. RECEIPTDATE, 14. SHIPINSTRUCT, 15. SHIPMODE, 16. COMMENT) \n : ";
	cin >> selected_attribute;
	//������ �Ӽ��� ���� ����
	sort_by_attribute(selected_attribute, file_s);
	//������ �Ӽ��� ���� ���
	print_selected_attributes(selected_attribute, file_s);


	return 0;
}