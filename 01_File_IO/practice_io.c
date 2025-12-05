#include <stdio.h>  //printf 등 사용을 위해
#include <fcntl.h>  //openm O_RDWR, O_CREAT 등의 상수를 위해
#include <unistd.h> //pwrite, close, write 등의 시스템 콜을 위해
#include <stdlib.h> //exit 등을 위해
#include <string.h> //strlen 등을 위해

int main()
{
    int fd;
    const char *data1 = "START";
    const char *data2 = "MIDDLE";
    const char *data3 = "END";

    // 1단계:파일 열기(open)
    // O_RDWR:읽기/쓰기 모드로 열기, O_RDONLY (읽기만), O_WRONLY (쓰기만)
    // O_CREAT:파일이 없으면 새로 생성
    // O_TRUNC:파일이 이미 있다면 내용을 싹 지우고(0으로) 시작
    // 0644:파일 권한(나=읽기/쓰기,그룹:읽기,타인:읽기)
    // fd로 들어오는 반환값은 정수(실패 시 -1 반환), 연 파일을 fd(숫자)로 부르는 것.

    printf("[Process] test.db 파일을 엽니다...\n");
    fd = open("test.db", O_RDWR | O_CREAT | O_TRUNC, 0644);

    if (fd == -1)
    {
        perror("파일 열기 실패"); // 에러 발생 시 원인 출력
        exit(1);
    }

    // 2단계:데이터 쓰기(pwrite 사용)->lseek 대신 사용!
    // pwrite(파일디스크립터,데이터,데이터길이,오프셋(위치));
    // 이 함수는 커서를 움직이지 않고 지정된 위치에 바로 꽂아 넣음

    // 1.0번지(파일의 맨 앞)에 데이터 쓰기
    if (pwrite(fd, data1, strlen(data1), 0) == -1)
    {
        perror("0번지 쓰기 실패");
        close(fd);
        exit(1);
    }

    printf("[Success] 0번지에 \"%s\" 작성 완료.\n", data1);

    // 2.4000번지에 데이터 쓰기(중간에 구멍 생김)
    if (pwrite(fd, data2, strlen(data2), 4000) == -1)
    {
        perror("4000번지 쓰기 실패");
        exit(1);
    }

    printf("[Success] 4000번지에 \"%s\" 작성 완료.\n", data2);

    // 3.8000번지에 데이터 쓰기(더 큰 구멍 생성)
    if (pwrite(fd, data3, strlen(data3), 8000) == -1)
    {
        perror("8000번지 쓰기 실패");
        close(fd);
        exit(1);
    }

    printf("[Success] 8000번지에 \"%s\" 작성 완료\n", data3);

    close(fd);
    printf("[Completed] 모든 작업이 끝났습니다. 'hexdump -C test.db'로 확인해보세요\n");

    return 0;
}