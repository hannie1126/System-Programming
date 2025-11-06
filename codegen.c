#include "assembler.h"

// 명령어를 inst_table에서 찾아 OPCODE와 함께 출력
int annotate_and_print(FILE *out, int skip_comment_lines) {
    for (int i = 0; i < line_num; i++) {
        token *t = token_table[i];
        if (!t || !t->operator) continue; // 비어 있으면 건너뜀

        inst *cmd = find_inst(t->operator);

        // 라벨과 명령어 출력
        fprintf(out, "%-10s %-10s ",
                t->label ? t->label : "",
                t->operator ? t->operator : "");

        // 피연산자 출력
        for (int j = 0; j < MAX_OPERAND; j++) {
            if (strlen(t->operand[j]) > 0)
                fprintf(out, "%s ", t->operand[j]);
        }

        // OPCODE 출력
        if (cmd)
            fprintf(out, "=> OPCODE: %02X\n", cmd->op);
        else
            fprintf(out, "=> OPCODE: ??\n");
    }
    return 0;
}
