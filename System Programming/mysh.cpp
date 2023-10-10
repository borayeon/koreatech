#include <stdio.h>
#include <stdlib.h>
#include <string.h>// 문자열 처리
#include <dirent.h>// 디렉터리, 파일 조작을 위한 함수와 구조체 제공
#include <errno.h>//오류 코드 관리
#include <sys/stat.h>// 파일 상태 및 특성 조사 함수 제공
#include <sys/types.h>// 데이터 타입, 기본적 시스템 루틴 정의 포함
#include <unistd.h> // POSIX 운영체제 서비스와 상호작용하기 위해  다양하 시스템 호출 제공


//strtol(숫자가 든 문자열, null, 몇진수로 할건지)

#define MAX_CMDLINE_SIZE    (128)
#define MAX_CMD_SIZE        (32)
#define MAX_ARG             (4)

// 함수 포인터 변수를 선언할 때는 그냥 typedef 없이 해도 되지만
// 추가되면 선언할 때 함수명으로 간단하게 작성가능
typedef int  (*cmd_func_t)(int argc, char** argv);
typedef void (*usage_func_t)(void);


// 명령어 구조체
typedef struct cmd_t {
    char            cmd_str[MAX_CMD_SIZE];
    cmd_func_t      cmd_func;
    usage_func_t    usage_func;
    char            comment[128];
} cmd_t;


// 매크로 함수                  // cmd_help 등으로 입력되어짐
#define DECLARE_CMDFUNC(str)    int cmd_##str(int argc, char **argv); \
                                void usage_##str(void)
DECLARE_CMDFUNC(help);
DECLARE_CMDFUNC(mkdir);
DECLARE_CMDFUNC(rmdir);
DECLARE_CMDFUNC(cd);
DECLARE_CMDFUNC(mv);
DECLARE_CMDFUNC(ls);
DECLARE_CMDFUNC(quit);

//과제 5
//ls 추가구현 (파일 종류, 파일 UID, GID. 파일의 접근, 수정, 생성 시간 값. 하드 링크 수.)
DECLARE_CMDFUNC(ln);//하드링크
DECLARE_CMDFUNC(rm);

//과제6
DECLARE_CMDFUNC(chmod);
//ln 추가 구현 (심볼릭 링크)
//ls (파일 정보+ 파일 크기)
DECLARE_CMDFUNC(cat);
DECLARE_CMDFUNC(cp);

/* Command List */
static cmd_t cmd_list[] = {
    //명령어 명령어함수 명령어사용법 명령어기능 
    {"help",    cmd_help,    usage_help,    "show usage, ex) help <command>"},
    {"mkdir",   cmd_mkdir,   usage_mkdir,   "create directory"},
    {"rmdir",   cmd_rmdir,   usage_rmdir,   "remove directory"},
    {"cd",      cmd_cd,      usage_cd,      "change current directory"},
    {"mv",      cmd_mv,      usage_mv,      "rename directory & file"},
    {"ls",      cmd_ls,      usage_ls,      "show directory contents"},
    {"quit",    cmd_quit,    NULL,          "terminate shell"},

    {"ln",      cmd_ln,     usage_ln,       "make hard link"},
    {"rm",      cmd_rm,     usage_rm,       "remove file"},

    {"chmod",   cmd_chmod,  usage_chmod,    "change access permission"},
    {"cat",     cmd_cat,    usage_cat,      ""},
    {"cp",      cmd_cp,     usage_cp,       ""},
};

// 상수는 수동적으로 관리하지 않게 계산식으로
const int command_num = sizeof(cmd_list) / sizeof(cmd_t);
static char* chroot_path = "/tmp/test"; // 상위 경로
static char* current_dir; // 현재 경로

//명령어 존재 유무 함수
static int search_command(char* cmd)
{
    int i;

    for (i = 0; i < command_num; i++) {
        if (strcmp(cmd, cmd_list[i].cmd_str) == 0) {
            /* found */
            return (i);
        }
    }

    /* not found */
    return (-1);
}

// 실제 주소 가져오기
static void get_realpath(char* usr_path, char* result)
{
    char* stack[32];
    int   index = 0;
    char  fullpath[128];
    char* tok;
    int   i;
#define PATH_TOKEN   "/"

    //루트로 이동
    if (usr_path[0] == '/') {
        //문자열 복사 함수
        strncpy(fullpath, usr_path, sizeof(fullpath) - 1);
    }
    else {
        //문자열을 형식화 -> 다른 문자열로 저장
        //저장할 공간, 버퍼크기, 문자열 형식, 저장할 문자열들(현재 주소)
        snprintf(fullpath, sizeof(fullpath) - 1, "%s/%s", current_dir + strlen(chroot_path), usr_path);
    }

    /* parsing */
    //구분자로 분할하여 배열로 저장
    tok = strtok(fullpath, PATH_TOKEN);
    if (tok == NULL) {
        goto out;
    }

    do {
        //문자열 비교; 동일하면 0, 작으면 음수, 크면 양수
        // . 이면 현재 위치이므로 스킵
        if (strcmp(tok, ".") == 0 || strcmp(tok, "") == 0) {
            ; // skip
        }
        // ..이면 이전 위치로
        else if (strcmp(tok, "..") == 0) {
            if (index > 0) index--;
        }
        // 스택에 저장
        else {
            stack[index++] = tok;
        }
    } while ((tok = strtok(NULL, PATH_TOKEN)) && (index < 32));

out:
    //result에 chroot_path 값 저장
    strcpy(result, chroot_path);

    // TODO: boundary check
    // 마지막 /로 끝나기
    for (i = 0; i < index; i++) {
        //문자열 이어 붙이기
        strcat(result, "/");
        strcat(result, stack[i]);
    }
}

int main(int argc, char** argv)
{
    char* command, * tok_str;
    char* cmd_argv[MAX_ARG];
    int  cmd_argc, i, ret;

    //명령어 동적할당
    command = (char*)malloc(MAX_CMDLINE_SIZE);
    //동적할당이 안되면 에러
    if (command == NULL) {
        perror("command malloc");
        exit(1);
    }
    // 현재 경로 동적 할당
    current_dir = (char*)malloc(MAX_CMDLINE_SIZE);
    if (current_dir == NULL) {
        perror("current_dir malloc");
        free(command);
        exit(1);
    }
    // "/tmp/test" 이 없다면 만들기
    if (chdir(chroot_path) < 0) {
        mkdir(chroot_path, 0755);
        chdir(chroot_path);
    }


    //현재 프로세스의 루트 디렉터리가 지정된 디렉터리로 변경
    // chroot(chroot_path);


    do {
        // 현재 경로를 받아오는 
        if (getcwd(current_dir, MAX_CMDLINE_SIZE) == NULL) {
            perror("getcwd");
            break;
        }

        //현재 경로와 루트 경로의 길이가 같다 -> 현재 경로가 루트 경로다 -> / 출력
        if (strlen(current_dir) == strlen(chroot_path)) {
            printf("/"); // for root path
        }
        // 현재 경로에 대해서 추가 출력 G $ 출력 
        printf("%s $ ", current_dir + strlen(chroot_path));

        // command를 가져오는데 null값이면 while문 종료
        if (fgets(command, MAX_CMDLINE_SIZE - 1, stdin) == NULL) break;


        /* get arguments */
        // command 값을 공백과 \n값을 통해 분할한다.
        tok_str = strtok(command, " \n");
        // 명령어가 null이면 계속 실행
        if (tok_str == NULL) continue;


        // tok_str를 cmd_atgv에 저장 
        cmd_argv[0] = tok_str;

        //4번 반복 null 값을 공백과 \n를 분할 저장
        for (cmd_argc = 1; cmd_argc < MAX_ARG; cmd_argc++) {
            if (tok_str = strtok(NULL, " \n")) {
                cmd_argv[cmd_argc] = tok_str;
            }
            else {
                break;
            }
        }

        /* search command in list and call command function */
        // 명령어가 존재여부 확인
        i = search_command(cmd_argv[0]);
        if (i < 0) {
            //명령어가 존재하지 않는다면
            printf("%s: command not found\n", cmd_argv[0]);
        }
        else {
            //명령어 리스트에서 명령어를 찾아서 함수 실행
            if (cmd_list[i].cmd_func) {
                // 해당 함수에 명령어 인자 넣어서 결과값 ret에 저장
                ret = cmd_list[i].cmd_func(cmd_argc, cmd_argv);
                //ret이 0이면 리턴 성공
                if (ret == 0) {
                    printf("return success\n");
                }
                // -2면 사용법 에러
                else if (ret == -2 && cmd_list[i].usage_func) {
                    printf("usage : ");
                    cmd_list[i].usage_func();
                }
                // 실패 에러
                else {
                    printf("return fail(%d)\n", ret);
                }
                // 커멘드 함수가 없다면 출력 
            }
            else {
                printf("no command function\n");
            }
        }
    } while (1);

    //동적할당 해제
    free(command);
    free(current_dir);

    return 0;
}

//help 명령어 함수
int cmd_help(int argc, char** argv)
{
    int i;
    //argc가 1이면 명령어 인자가 1개라는 것 
    if (argc == 1) {
        // 명령어 개수만큼 기능 출력
        for (i = 0; i < command_num; i++) {
            printf("%32s: %s\n", cmd_list[i].cmd_str, cmd_list[i].comment);
        }
    }
    else if (argv[1] != NULL) {
        // 두번째 명령어 인자에 대해서
        i = search_command(argv[1]);
        // null이면 아래값 반환
        if (i < 0) {
            printf("%s command not found\n", argv[1]);
        }
        // null이 아니면 사용법 출력
        else {
            if (cmd_list[i].usage_func) {
                printf("usage : ");
                cmd_list[i].usage_func();
                return (0);
            }
            // 없다면 없다고 출력
            else {
                printf("no usage\n");
                return (-2);
            }
        }
    }
    return (0);
}

int cmd_mkdir(int argc, char** argv)
{
    int  ret = 0;
    char rpath[128];
    // 명령어 인자가 2개면
    if (argc == 2) {
        //진짜 경로 가져오기
        get_realpath(argv[1], rpath);
        // ret에 mkdir 한 값이 음수면 에러
        if ((ret = mkdir(rpath, 0755)) < 0) {
            perror(argv[0]);
        }
    }
    // 인자가 2개가 아니면 -2 리턴 -> 사용법 에러
    else {
        ret = -2; // syntax error
    }
    return (ret);
}

int cmd_rmdir(int argc, char** argv)
{
    int  ret = 0;
    char rpath[128];

    if (argc == 2) {
        get_realpath(argv[1], rpath);

        if ((ret = rmdir(rpath)) < 0) {
            perror(argv[0]);
        }
    }
    else {
        ret = -2; // syntax error
    }

    return (ret);
}

int cmd_cd(int argc, char** argv)
{
    int  ret = 0;
    char rpath[128];

    if (argc == 2) {
        get_realpath(argv[1], rpath);

        if ((ret = chdir(rpath)) < 0) {
            perror(argv[0]);
        }
    }
    else {
        ret = -2;
    }

    return (ret);
}

int cmd_mv(int argc, char** argv)
{
    int  ret = 0;
    char rpath1[128];
    char rpath2[128];

    if (argc == 3) {

        get_realpath(argv[1], rpath1);
        get_realpath(argv[2], rpath2);

        if ((ret = rename(rpath1, rpath2)) < 0) {
            perror(argv[0]);
        }
    }
    else {
        ret = -2;
    }

    return (ret);
}

static const char* get_type_str(char type)
{
    switch (type) {
    case DT_BLK:
        return "BLK";
    case DT_CHR:
        return "CHR";
    case DT_DIR:
        return "DIR";
    case DT_FIFO:
        return "FIFO";
    case DT_LNK:
        return "LNK";
    case DT_REG:
        return "REG";
    case DT_SOCK:
        return "SOCK";
    default: // include DT_UNKNOWN
        return "UNKN";
    }
}
//ls에 symbolic 기능 추가
int cmd_ls(int argc, char** argv)
{
    int ret = 0;
    DIR* dp;
    struct dirent* dep;
    struct stat for_detail;
    char buf[BUFSIZ];

    // ls만 입력되는 게 아니면 사용법 에러 -2 반환
    if (argc != 1) {
        ret = -2;
        goto out;
    }

    // 현재 디렉토리를 열어 dp에 저장 열기 실패시 -1 반환
    if ((dp = opendir(".")) == NULL) {
        ret = -1;
        goto out;
    }

    // dp 를 읽어서 다음을 출력함
    // 번호 타입 이름 각파일 및 디렉토리 접근권한 출력 
    // 파일 종류, 파일 UID, GID. 파일의 접근, 수정, 생성 시간 값. 하드 링크 수
    while (dep = readdir(dp)) {
        if (stat(dep, for_detail) != -1) {
            perror("stat error")
        }
        else {
            print_access_permission(for_detail);//파일 종류 및 접근권한
            //하드링크 수 uid gid 파일 크기 
            printf("%s %d %d %d ", dep->st_nlink, dep->st_uid, dep->st_gid, dep->st_size
                , dep->st_atime//접근시간
                , dep->st_mtime//수정시간
            );
            //파일 생성시간
            char date[20];
            strftime(date, 20, "%b %d %H:%M", localtime(&file_info->st_mtime));
            printf("%s %s ", date);
            printf("%10ld %4s ", dep->d_ino, get_type_str(dep->d_type));
            // 파일명과 심볼릭 링크 파일명
            if (readlink(dep->d_name,buf,BUFSIZ) < 0) {
                printf("%s\n", dep->d_name);
            }
            else {
                printf("%s -> %s\n", dep->d_name, buf);
            }
        }
    }

    closedir(dp);

out:
    return (ret);
}

void print_access_permission(struct stat for_detail) {

    // 파일 종류 및 권한 출력
    printf("\t");
    printf((S_ISDIR(for_detail->st_mode)) ? "d" : "-");
    printf((for_detail->st_mode & S_IRUSR) ? "r" : "-");
    printf((for_detail->st_mode & S_IWUSR) ? "w" : "-");
    printf((for_detail->st_mode & S_IXUSR) ? "x" : "-");
    printf((for_detail->st_mode & S_IRGRP) ? "r" : "-");
    printf((for_detail->st_mode & S_IWGRP) ? "w" : "-");
    printf((for_detail->st_mode & S_IXGRP) ? "x" : "-");
    printf((for_detail->st_mode & S_IROTH) ? "r" : "-");
    printf((for_detail->st_mode & S_IWOTH) ? "w" : "-");
    printf((for_detail->st_mode & S_IXOTH) ? "x" : "-");

}

int cmd_quit(int argc, char** argv)
{
    exit(1);
    return 0;
}
//ln 기능 추가 + symbolic 링크 기능 추가
int cmd_ln(int argc, char** argv)
{
    int  ret = 0;
    char tmp[2];
    char o_path[128];
    char n_path[128];

    if (argc == 3) {

        get_realpath(argv[1], o_path);
        get_realpath(argv[2], n_path);

        if ((ret = link(o_path, n_path)) < 0) {
            perror(argv[0]);
        }
    }
    else if (argc == 4) {

        get_realpath(argv[1], tmp);
        get_realpath(argv[2], o_path);
        get_realpath(argv[3], n_path);
        if (tmp = "-s" & (ret = symlink(o_path, n_path) < 0)) {
            perror(argv[0]+" -s");
        }
    }
    else {
        ret = -2;
    }

    return (ret);
}
//rm 기능 추가
int cmd_rm(int argc, char** argv)
{
    int  ret = 0;
    char rpath[128];

    if (argc == 2) {
        get_realpath(argv[1], rpath);

        if ((ret = unlink(rpath)) < 0) {
            perror(argv[0]);
        }
    }
    else {
        ret = -2; // syntax error
    }

    return (ret);
}
//chmod 기능 추가
int cmd_chmod(int argc, char** argv)
{
    int  ret = 0;
    char permission[4];
    char path[128];

    if (argc == 3) {
        get_realpath(argv[1], permission);
        get_realpath(argv[2], path);
        if ((ret = chmod(permission, path)) < 0) {
            perror(argv[0]);
        }
    }
    else {
        ret = -2;
    }

    return (ret);
}


void usage_help(void)
{
    printf("help <command>\n");
}

void usage_mkdir(void)
{
    printf("mkdir <directory>\n");
}

void usage_rmdir(void)
{
    printf("rmdir <directory>\n");
}

void usage_cd(void)
{
    printf("cd <directory>\n");
}

void usage_mv(void)
{
    printf("mv <old_name> <new_name>\n");
}
//ln을 위한 사용방법 + symbolic 링크 기능 추가
void usage_ln(void)
{
    printf("usage: ln [option : -s ] <original file> <new file>\n");
}
//rm을 위한 사용방법
void usage_rm(void)
{
    printf("usage: rm <file>\n");
}
//chmod을 위한 사용 방법
void usage_chmod(void)
{
    printf("usage: chmod <Access Permission in Octal digit> <file>\nex) chmod 0644 test.txt\n");
}