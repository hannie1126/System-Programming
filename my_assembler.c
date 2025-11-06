#include "assembler.h"

// 프로그램의 메인 진입점
int main(int argc, char *argv[]) {
    // 실행 인자 확인
    if (argc < 3) {
        printf("Usage: %s <input.sic> <inst.data>\n", argv[0]);
        return 1;
    }

    // 명령어 테이블 로드
    load_inst_table(argv[2]);
    // 어셈블리 입력 파일 읽기
    load_input_file(argv[1]);
    // 입력 라인을 토큰으로 분리
    tokenize_all_lines();
    // 각 명령어에 대해 OPCODE 매핑 결과 출력
    annotate_and_print(stdout, 1);
    return 0;
}
