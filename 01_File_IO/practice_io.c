#include <stdio.h>  //printf 등 사용을 위해
#include <fcntl.h>  //openm O_RDWR, O_CREAT 등의 상수를 위해
#include <unistd.h> //pwrite, close, write 등의 시스템 콜을 위해
#include <stdlib.h> //exit 등을 위해
#include <string.h> //strlen 등을 위해

int main()
{
    // lseek(fd,이동할 거리,기준점):파일에서 커서(위치)를 순간 이동시키는 함수
    // SEEK_SET:맨 앞 기준(절대이동)
    // SEEK_CUR:현재 기준(상대이동)
    // SEEK_END:맨 끝 기준
    int fd;

    // 1.파일 열기
    // 이번엔 mixed.db 라는 파일을 만듦

    fd = open("mixed.db", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("파일 열기 실패");
        exit(1);
    }

    //[step 1] 일반 write: "AAAA" 쓰기(커서가 쓰면서 움직임),현재 커서 위치부터 숫자만큼 쓰고 이동(3이었으면 AAA씀)
    // 현재 커서:0
    // 쓰고 난 후 커서 예상:4
    write(fd, "AAAA", 4);
    // lseek(fd,0,SEEK_CUR):현재위치(SEEL_CUR)에서 0칸만큼 이동해라, 반환값:파일의 맨 처음부터 현재 커서까지의 거리(바이트수) 반환
    printf("1. 'AAAA' 작성 완료. 현재 커서 위치: %ld\n", lseek(fd, 0, SEEK_CUR));

    //[step 2] pwrite:저 멀리 100번지에 "ZZZZ" 쓰기(4아니고 3으로 쓰면 ZZZ만 100번지에 써짐)
    // pwrite은 커서를 건드리지 않아야함
    // 쓰고난 후 커서 예상:여전히 4여야함(이게 핵심)
    pwrite(fd, "ZZZZ", 4, 100);
    printf("2. 100번지에 'ZZZZ' (pwrite) 작성 완료.\n");
    printf(" ->pwrite 직후 커서 위치 확인:%ld (변하면 안됨)\n", lseek(fd, 0, SEEK_CUR));

    //[step 3] 다시 일반 write:"BBBB" 쓰기
    // 만약 커서가 4에 그대로 있다면 "AAAA" 바로 뒤에 "BBBB"가 붙어야함
    write(fd, "BBBB", 4);
    printf("3. 'BBBB' 작성 완료. 현재  커서 위치:%ld\n", lseek(fd, 0, SEEK_CUR));

    close(fd);
    return 0;
}