#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> 

// --- 함수 선언 ---
void start_timer(int duration_minutes, const char* session_name);
void display_quote();
void run_pomodoro();
void run_custom_timer();
void display_menu();

// 뽀모도로 기본 설정 시간 (분 단위)
#define DEFAULT_FOCUS_MINUTES 25
#define DEFAULT_BREAK_MINUTES 5
#define DEFAULT_LONG_BREAK_MINUTES 30
#define DEFAULT_CYCLES 4

// 명언 목록
const char* quotes[] = {
    "집중은 성공의 열쇠이며, 몰입은 그 문을 여는 손잡이이다.",
    "습관이 모든 것을 결정한다. 좋은 습관을 만들면 집중은 자연스럽게 따라온다.",
    "가장 짧은 집중의 순간들이 모여 가장 위대한 결과를 만든다.",
    "할 일을 미루는 것은 집중력을 갉아먹는 가장 큰 적이다.",
    "당신이 진정으로 원하는 것에 집중할 때, 세상은 당신을 돕기 위해 움직인다."
};
const int num_quotes = sizeof(quotes) / sizeof(quotes[0]);

/**
 * @brief 주어진 시간(분) 동안 타이머를 실행합니다.
 * * @param duration_minutes 타이머 지속 시간 (분)
 * @param session_name 현재 진행 중인 세션 이름 (예: "집중", "휴식")
 */
void start_timer(int duration_minutes, const char* session_name) {
    // 분 단위를 초 단위로 변환
    int duration_seconds = duration_minutes * 60;
    time_t start_time = time(NULL); // 현재 시간 저장
    time_t current_time;
    int elapsed_seconds;
    int remaining_seconds;
    int minutes, seconds;

    printf("\n--- %s 타이머 시작 (%d분) ---\n", session_name, duration_minutes);

    // 타이머가 끝날 때까지 루프를 실행
    while (1) {
        current_time = time(NULL);
        // 경과 시간 계산
        elapsed_seconds = (int)difftime(current_time, start_time);
        // 남은 시간 계산
        remaining_seconds = duration_seconds - elapsed_seconds;

        // 남은 시간이 0 이하가 되면 타이머 종료
        if (remaining_seconds <= 0) {
            break;
        }

        // 남은 시간을 분과 초로 변환
        minutes = remaining_seconds / 60;
        seconds = remaining_seconds % 60;

        // 커서를 처음으로 되돌려 시간을 덮어씁니다 (콘솔 화면 지저분함을 방지)
        // \r: 캐리지 리턴 (줄의 시작으로 커서를 이동)
        printf("\r%s 남은 시간: %02d분 %02d초", session_name, minutes, seconds);
        fflush(stdout); // 출력 버퍼를 비워 즉시 화면에 표시

        Sleep(1000);
    }

    // 타이머 종료 메시지
    printf("\n--- %s 타이머 종료! ---\n", session_name);
}

/**
 * @brief 동기 부여 명언을 무작위로 출력합니다.
 */
void display_quote() {
    // 난수 생성 시드 초기화 (프로그램 시작 시 한 번만 실행)
    static int initialized = 0;
    if (!initialized) {
        srand(time(NULL));
        initialized = 1;
    }

    // 0부터 num_quotes-1 사이의 난수 생성
    int index = rand() % num_quotes;

    printf("\n============================================\n");
    printf(" 오늘의 동기 부여 명언: %s\n", quotes[index]);
    printf("============================================\n");
}

/**
 * @brief 뽀모도로 기본 설정(25분 집중/5분 휴식/30분 장기 휴식)에 따라 타이머를 실행합니다.
 */
void run_pomodoro() {
    printf("\n[  뽀모도로 타이머 시작: %d분 집중 / %d분 휴식 (총 %d 사이클) ]\n",
        DEFAULT_FOCUS_MINUTES, DEFAULT_BREAK_MINUTES, DEFAULT_CYCLES);

    for (int i = 1; i <= DEFAULT_CYCLES; i++) {
        printf("\n\n--- [ %d / %d 사이클 ] ---\n", i, DEFAULT_CYCLES);

        // 1. 집중 시간
        display_quote(); // 집중 전에 명언 표시
        start_timer(DEFAULT_FOCUS_MINUTES, "집중");

        // 2. 휴식 시간 (마지막 사이클 후에는 장기 휴식)
        if (i < DEFAULT_CYCLES) {
            start_timer(DEFAULT_BREAK_MINUTES, "짧은 휴식");
        }
        else {
            printf("\n--- 모든 사이클 완료! 장기 휴식에 들어갑니다. ---\n");
            start_timer(DEFAULT_LONG_BREAK_MINUTES, "장기 휴식");
        }
    }

    printf("\n[  뽀모도로 전체 과정이 완료되었습니다! 훌륭합니다! ]\n");
}

/**
 * @brief 사용자로부터 집중 및 휴식 시간을 입력받아 타이머를 실행합니다.
 */
void run_custom_timer() {
    int focus_min, break_min, cycles;

    printf("\n[ ⚙ 사용자 맞춤 타이머 설정 ]\n");
    printf("집중 시간(분)을 입력하세요: ");
    scanf("%d", &focus_min);

    printf("휴식 시간(분)을 입력하세요: ");
    scanf("%d", &break_min);

    printf("반복할 사이클 수(회)를 입력하세요: ");
    scanf("%d", &cycles);

    // 입력 값 유효성 검사 (0분 이하의 시간 입력 방지)
    if (focus_min <= 0 || break_min <= 0 || cycles <= 0) {
        printf(" 시간과 사이클 수는 1분 이상, 1회 이상이어야 합니다. 설정을 다시 해주세요.\n");
        // 입력 버퍼 비우기 (오류 방지)
        while (getchar() != '\n');
        return;
    }

    printf("\n[  맞춤 타이머 시작: %d분 집중 / %d분 휴식 (총 %d 사이클) ]\n",
        focus_min, break_min, cycles);

    for (int i = 1; i <= cycles; i++) {
        printf("\n\n--- [ %d / %d 사이클 ] ---\n", i, cycles);

        // 1. 집중 시간
        display_quote();
        start_timer(focus_min, "집중");

        // 2. 휴식 시간
        if (i < cycles) {
            start_timer(break_min, "휴식");
        }
    }

    printf("\n[  맞춤 타이머 과정이 완료되었습니다! ]\n");
}

/**
 * @brief 메인 메뉴를 출력하고 사용자 입력을 받습니다.
 */
void display_menu() {
    printf("\n============================================\n");
    printf("         C 언어 집중 타이머\n");
    printf("============================================\n");
    printf("1.  뽀모도로 타이머 시작 (25분 집중 / 5분 휴식)\n");
    printf("2.  사용자 맞춤 시간 설정 및 시작\n");
    printf("3.  프로그램 종료\n");
    printf("============================================\n");
    printf("메뉴를 선택하세요 (1-3): ");
}


/**
 * @brief 메인 함수: 프로그램의 진입점입니다.
 */
int main() {
    int choice;

    // 프로그램이 시작될 때 난수 시드를 한 번 초기화합니다.
    srand(time(NULL));

    while (1) { // 무한 루프를 통해 사용자가 프로그램을 여러 번 실행할 수 있도록 합니다.
        display_menu();

        // 사용자 입력 받기
        if (scanf("%d", &choice) != 1) {
            // 입력 실패 처리 (예: 문자를 입력한 경우)
            printf(" 잘못된 입력입니다. 숫자를 입력해주세요.\n");
            // 입력 버퍼 비우기
            while (getchar() != '\n');
            continue;
        }

        // 메뉴 선택에 따른 동작
        switch (choice) {
        case 1:
            run_pomodoro();
            break;
        case 2:
            run_custom_timer();
            break;
        case 3:
            printf("\n프로그램을 종료합니다. 오늘 집중 목표 달성하세요! 💪\n");
            return 0; // 프로그램 종료
        default:
            printf(" 잘못된 메뉴 선택입니다. 1, 2, 3 중 하나를 입력해주세요.\n");
        }

     
        while (getchar() != '\n');
        printf("\n\n(엔터를 눌러 메인 메뉴로 돌아가세요...)");
        getchar(); // 사용자 입력을 기다립니다.
    
        system("clear");
    }

    return 0;
}
