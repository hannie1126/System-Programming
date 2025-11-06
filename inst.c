#include "assembler.h"

// 명령어 테이블 초기화
inst *inst_table[MAX_INST];
int inst_index = 0;

// inst.data 파일 읽어서 명령어 테이블에 저장
int load_inst_table(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) { perror("fopen inst.data"); return -1; }

    char name[20], type[10];
    int fmt;
    unsigned int opcode;

    // inst.data 예: ADD M 3 18
    while (fscanf(fp, "%s %s %d %x", name, type, &fmt, &opcode) == 4) {
        inst *it = (inst*)calloc(1, sizeof(inst));
        strcpy(it->str, name);
        it->op = (unsigned char)opcode;
        it->format = fmt;
        it->ops = 1;
        inst_table[inst_index++] = it;
    }
    fclose(fp);
    return inst_index;
}

// 명령어 이름으로 OPCODE 찾기 (대소문자 무시)
inst* find_inst(const char *mnemonic) {
    for (int i = 0; i < inst_index; i++) {
        if (strcasecmp(inst_table[i]->str, mnemonic) == 0)
            return inst_table[i];
    }
    return NULL; // 없으면 NULL 반환
}
