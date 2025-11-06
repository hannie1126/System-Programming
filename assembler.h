#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// 프로그램 전체에서 사용하는 상수 정의
#define MAX_LINES   5000
#define MAX_OPERAND 3
#define MAX_INST    256

// [1] 입력된 어셈블리 파일 내용을 저장할 배열
extern char *input_data[MAX_LINES];  // 각 줄의 문자열 저장
extern int line_num;                 // 현재 저장된 줄 개수


// [2] 한 줄의 정보를 토큰으로 분리하여 저장할 구조체
struct token_unit {
    char *label;                     // 명령어 앞의 라벨
    char *operator;                  // 명령어 (예: LDA, STA 등)
    char operand[MAX_OPERAND][20];   // 피연산자 (최대 3개)
    char comment[100];               // 주석 저장
};
typedef struct token_unit token;     // 별칭 token으로 사용

// 토큰 테이블: 프로그램 전체를 라인별로 저장
extern token *token_table[MAX_LINES];

// [3] 명령어 테이블 구조체
// inst.data 파일의 명령어(OPCODE, 형식 등)를 관리
struct inst_unit {
    char str[10];         // 명령어 이름 (예: LDA)
    unsigned char op;     // OPCODE (16진수 값)
    int format;           // 명령어 포맷
    int ops;              // 피연산자 개수
};
typedef struct inst_unit inst;

// 명령어 테이블
extern inst *inst_table[MAX_INST];
extern int inst_index;

// [4] 함수 원형 선언 (모든 .c 파일에서 사용)

int load_input_file(const char *path);
int tokenize_all_lines(void);
int load_inst_table(const char *path);
inst* find_inst(const char *mnemonic);
int annotate_and_print(FILE *out, int skip_comment_lines);

// 유틸리티 함수
char* str_trim(char *s);
void  str_upper(char *s);
int   split_commas(char *s, char *out[], int max_out);
char* xstrdup(const char *s);

#endif
