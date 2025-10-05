//경영학과/인공지능공학과 20222066 한지은 사용언어 : JAVA


import java.util.Scanner;

public class HexCodeAnalyze {
    // === 실습 환경 고정값(과제 설명에 나온 값) ==========================
    // PC : 현재 명령 "다음"을 가리키는 값. PC-relative일 때 기준점으로 사용
    private static final int PC_VALUE = 0x3000;
    // BASE : base-relative일 때 기준점
    private static final int BASE_VALUE = 0x0000;
    // 메모리[TA]에 있다고 가정한 값(누산기 A로 로드했다고 가정)
    private static final int MEMORY_VALUE_AT_TA = 0x103000;
    // ===============================================================

    public static void main(String[] args) {
        try (Scanner scanner = new Scanner(System.in)) {
            System.out.print("Hex 입력(6자리 또는 8자리, '0x' 허용): ");
            String hexCode = scanner.nextLine().trim();
            analyzeInstruction(hexCode);
        } catch (Exception e) {
            // 어떤 예외든 잡아 사용자에게 친절한 메시지 출력
            System.err.println("[오류] " + e.getClass().getSimpleName() + ": " + e.getMessage());
        }
    }

    /**
     * 입력한 HEX 한 줄을 SIC/XE 규칙에 따라 분석
     * - 2진수, opcode, nixbpe, 주소지정 모드, disp, TA, 레지스터 A 값을 사람이 읽기 좋게 출력.
     */
    public static void analyzeInstruction(String raw) {
        // 1) 전처리: 공백 제거, 0x 접두어 제거, 대문자 통일
        String hex = raw.replaceAll("\\s+", "");
        if (hex.startsWith("0x") || hex.startsWith("0X")) hex = hex.substring(2);
        hex = hex.toUpperCase();

        // 이번 과제는 포맷3(24비트)만 다룬다고 가정 → 6자리 HEX 강제
        if (!hex.matches("^[0-9A-F]{6}$")) {
            System.out.println("입력 형식 오류: 6자리 16진수여야 함. 예) 032600");
            return;
        }

        // 2) 16진수 → 정수(24비트로 해석)
        final int instruction;
        try {
            instruction = Integer.parseInt(hex, 16);
        } catch (NumberFormatException nfe) {
            System.out.println("숫자 파싱 오류: 16진수만 입력하세요.");
            return;
        }

        // 3) 24비트 이진 문자열로 표시(앞쪽 0도 보이도록 패딩)
        String binary = String.format("%24s", Integer.toBinaryString(instruction)).replace(' ', '0');
        System.out.println("Binary : " + binary);

        // 4) 상위 8비트 = opcode+ni(즉 opcode byte). SIC/XE에서 실제 연산코드는 상위 6비트.
        int opcode8 = (instruction >> 16) & 0xFF; // [23:16]
        // 상위 6비트만 남기려면 하위 2비트 제거 후 오른쪽으로 2비트 쉬프트하면 "6비트 값"만 얻는다.
        int opcode6 = (opcode8 & 0xFC) >> 2;      // base opcode(6-bit)

        // 5) nixbpe 추출 (비트 위치: [17]=n, [16]=i, [15]=x, [14]=b, [13]=p, [12]=e)
        int n = (instruction >> 17) & 1;
        int i = (instruction >> 16) & 1;
        int x = (instruction >> 15) & 1;
        int b = (instruction >> 14) & 1;
        int p = (instruction >> 13) & 1;
        int e = (instruction >> 12) & 1;

        // 6) 하위 12비트 = disp/addr (포맷3에서 변위로 쓰임)
        int disp = instruction & 0x0FFF; // [11:0]

        // 가독성이 있게 출력
        System.out.printf("Opcode : %6s (opcode6=0x%02X)%n",
                String.format("%6s", Integer.toBinaryString(opcode6)).replace(' ', '0'),
                opcode6);
        System.out.printf("nixbpe : %d%d%d%d%d%d (n=%d i=%d x=%d b=%d p=%d e=%d)%n",
                n,i,x,b,p,e, n,i,x,b,p,e);

        // 주소지정 모드 문자열 만들기
        String format = (e==1) ? "Format 4" : "Format 3";
        String niMode = (n==1 && i==1) ? "Simple"
                : (n==1)        ? "Indirect"
                : (i==1)        ? "Immediate"
                : "SIC Direct";
        String rel    = (p==1) ? "PC-relative"
                : (b==1) ? "Base-relative"
                : "Direct";
        String idx    = (x==1) ? ", Indexed" : "";

        System.out.printf("Flag bit : SIC/XE, %s, %s, %s%s%n", niMode, rel, format, idx);

        // disp/addr 12비트를 2진으로 그대로 보여주기(부호 확장 전 원형)
        System.out.printf("disp/addr : %12s%n",
                String.format("%12s", Integer.toBinaryString(disp)).replace(' ', '0'));

        // 7) disp 부호 확장: PC-relative 또는 BASE-relative일 때만 의미가 있음
        int dispSigned = disp;
        if (p==1 || b==1) {
            // 최상위 비트(0x800)가 1이면 음수 → 12비트 부호 확장
            if ((disp & 0x800) != 0) dispSigned |= 0xFFFFF000; // 0b1111_..._0000로 채움
        }

        // 8) TA(Target Address) 계산
        final int X = 0; // X index 레지스터 값. 필요시 과제 요구대로 바꿔도 됨.
        int targetAddress;
        if (p==1 && b==0) {
            // PC-relative
            targetAddress = PC_VALUE + dispSigned;
        } else if (b==1 && p==0) {
            // Base-relative
            targetAddress = BASE_VALUE + dispSigned;
        } else {
            // Direct(포맷3에서 흔치는 않지만, e=0 & p=b=0이면 단순 disp로 처리)
            targetAddress = disp;
        }
        // 인덱스 모드면 X 더함
        if (x==1) targetAddress += X;

        System.out.printf("Target Address = 0x%X%n", targetAddress);

        // 9) (시뮬레이션) 메모리[TA] 값 → A 레지스터로 읽어왔다고 가정
        System.out.printf("Register A value = 0x%X%n", MEMORY_VALUE_AT_TA);

    }
}
