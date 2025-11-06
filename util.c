#include "assembler.h"

// 문자열 복사용 malloc 함수
char* xstrdup(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *p = (char*)malloc(n);
    memcpy(p, s, n);
    return p;
}

// 문자열 앞뒤 공백 제거 함수
char* str_trim(char *s) {
    if (!s) return s;
    while (*s && isspace((unsigned char)*s)) s++;       // 앞 공백 제거
    if (*s == 0) return s;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--; // 뒤 공백 제거
    end[1] = '\0';
    return s;
}

// 문자열을 모두 대문자로 변환
void str_upper(char *s) {
    if (!s) return;
    for (; *s; ++s) *s = toupper((unsigned char)*s);
}

// 콤마(,) 기준으로 피연산자 분리
int split_commas(char *s, char *out[], int max_out) {
    int count = 0;
    char *p = s;
    while (p && *p && count < max_out) {
        char *comma = strchr(p, ',');  // 콤마 위치 찾기
        if (comma) {
            *comma = '\0';
            out[count++] = str_trim(p);
            p = comma + 1;
        } else {
            out[count++] = str_trim(p);
            break;
        }
    }
    return count;
}
