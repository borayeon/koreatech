#include <iostream>
#include <fstream>

using namespace std;

void readfiles(FILE *file, ) {
	
}
// (n개의 라인 = driving 테이블)을 묶어서 한번에 서치
// PART 테이블에서 n줄을 불러오기
// n줄에서 key값만 추출한 배열 생성
// 리스트 값을 이용하여 driven 테이블에 key값만 매칭하여 추출

// [단, 정렬은 하지 않음, 정렬하면 정렬하는 시간이 추가됨.]
// 같은 값이면 driving 테이블 이후에 add
// 다른 값이면 pass


int main(int argc, char* argv) {
	//일단 두 테이블을 호출
	FILE* file1 = fopen("part.tbl", "r");
	FILE* file2 = fopen("part.tbl", "r");


}
