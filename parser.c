#include "assembler.h"

// 전역 변수 선언 및 초기화
char *input_data[MAX_LINES];
int line_num = 0;
token *token_table[MAX_LINES];

// 새로운 토큰 구조체 생성
static token* new_token(void) {
    token *t = (token*)calloc(1, sizeof(token));
    return t;
}

// 입력 파일 읽기
int load_input_file(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) { perror("fopen input"); return -1; }

    char buf[1024];
    line_num = 0;
    // 한 줄씩 읽어 input_data 배열에 저장
    while (fgets(buf, sizeof(buf), fp)) {
        input_data[line_num++] = xstrdup(buf);
    }
    fclose(fp);
    return line_num;
}

// 입력된 코드 전체를 토큰 단위로 분리
int tokenize_all_lines(void) {
    for (int i = 0; i < line_num; ++i) {
        token *t = new_token();
        token_table[i] = t;

        char line[1024];
        strcpy(line, input_data[i]);
        char *p = str_trim(line);
        if (*p == '.' || *p == '\0') continue; // 주석 라인은 무시

        // 공백 단위로 단어 분리
        char *tok[10] = {0};
        int count = 0;
        char *save;
        for (char *q = strtok_r(p, " \t\n", &save); q; q = strtok_r(NULL, " \t\n", &save)) {
            tok[count++] = q;
            if (count >= 10) break;
        }
        if (count == 0) continue;

        // inst_table에서 첫 번째 단어가 명령어인지 확인
        extern inst* find_inst(const char*);
        inst *found = find_inst(tok[0]);
        int idx = 0;
        if (found) {
            // 첫 단어가 명령어면 label 없음
            t->operator = xstrdup(tok[idx++]);
        } else {
            // 첫 단어가 inst_table에 없으면 label로 간주
            t->label = xstrdup(tok[idx++]);
            if (idx < count) t->operator = xstrdup(tok[idx++]);
        }

        // 나머지는 operand로 처리
        int opi = 0;
        while (idx < count && opi < MAX_OPERAND) {
            strcpy(t->operand[opi++], tok[idx++]);
        }
    }
    return 0;
}
