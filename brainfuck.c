#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#define LEN 50

void help(char * argv0, FILE *f) {
    fprintf(f, "Usage: %s [OPTION] FILE\n", argv0);
    printf("Options:\n");
    printf("\t-h \t display this help text and exit\n");
    printf("\t-d \t dump memory tape\n");
    printf("\t-n \t input integer instead of ASCII\n");
    printf("\t-t NUM \t pause NUM ms each step\n");
}

int iscomm(char c){
    if (c == '<' || c == '>' || c == '+' || c == '-' || c == '.' || c == ',' || c == '[' || c == ']'){
        return 0;
    }
    else {
        return 1;
    }
}

void dump(uint64_t *ptr, int c){
    printf("----------------------\n");
    for (int i = 0; i < LEN; i++){
        if (i == c){
            printf(">%lu< ", ptr[i]);
        }
        else {
            printf("%lu ", ptr[i]);
        }
    }
    printf("\n");
}

void brainfuck(FILE *f, int t_ms, int dump_flag, int int_flag){
    uint8_t buf;
    uint8_t c = 0;
    char out[128] = "";
    uint8_t out_i = 0;
    
    uint64_t *ptr = malloc(LEN * sizeof(uint64_t));
    for (int i = 0; i < LEN; i++){
        *ptr = 0;
    }
    
    while(fread(&buf, sizeof(buf), 1, f)){
        if (dump_flag && !iscomm(buf) && buf != '.'){
            printf("%c", buf);
            dump(&(ptr[-c]), c);
            printf("%s\n", out);
            usleep(t_ms*1000);
        }
        
        switch (buf){
            case '>':
                ptr++;
                c++;
                break;
            case '<':
                ptr--;
                c--;
                break;
            case '+':
                (*ptr)++;
                break;
            case '-':
                if (*ptr - 1 > 18446744073709505161U){
                    *ptr = 256;
                }
                (*ptr)--;
                break;
            case '.':
                printf("%c", (char)*ptr);
                if ((char)*ptr != '\n'){
                    out[out_i] = (char)*ptr;
                    out_i++;
                }
                
                break;
            case ',':
                printf("INPUT: ");
                if (int_flag){
                    scanf("%lu", ptr);
                }
                else {
                    char a;
                    scanf("%c", &a);
                    *ptr = (uint64_t)a;
                }
                break;                
            case '[':                
                if (*ptr == 0){
                    uint8_t open_bracket = 1;
                    uint8_t close_bracket = 0;
                    while(buf != ']' || open_bracket != close_bracket){
                        fread(&buf, sizeof(buf), 1, f);
                        if(buf == ']'){
                            close_bracket++;
                        }
                        else if(buf == '['){
                            open_bracket++;
                        }

                    }
                }
                break;
                
            case ']':
                if (*ptr != 0){
                    uint8_t open_bracket = 0;
                    uint8_t close_bracket = 1;
                    while(buf != '[' || open_bracket != close_bracket){
                        fseek(f, -2, SEEK_CUR);
                        fread(&buf, sizeof(buf), 1, f);
                        if(buf == '['){
                            open_bracket++;
                        }
                        else if (buf == ']'){
                            close_bracket++;
                        }
                    }
                    
                }
                break;
            default:
                break;
        }

    }
}

int main(int argc, char *argv[]){
    int opt;
    int int_flag, dump_flag, t_ms;

    t_ms = 0;
    dump_flag = 0;
    int_flag = 0;
    while ((opt = getopt(argc, argv, "hndt:")) != -1) {
        switch (opt) {
        case 'd':
            dump_flag = 1;
            break;
        case 'n':
            int_flag = 1;
            break;
        case 't':
            t_ms = atoi(optarg);
            break;
        case 'h':
            help(argv[0], stdout);
            return 0;
        default: /* '?' */
            help(argv[0], stderr);
            exit(EXIT_FAILURE);
        }
    }
    
    char *file;
    if (optind < argc){
        file = argv[optind];   
    }
    else {
        help(argv[0], stderr);
        exit(EXIT_FAILURE);
    }


    FILE *f;
    f = fopen(file, "r");
    if (f == NULL){
        if(access( file, F_OK ) == 0) {
            if(access( file, R_OK ) == 0) {
                fprintf(stderr, "%s: %s: Unknown error\n", argv[0], file);
            }
            else {
            fprintf(stderr, "%s: %s: Permission denied\n", argv[0], file);
            }
        }
        else {
            fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], file);
        }
        exit(EXIT_FAILURE);
    }

    brainfuck(f, t_ms, dump_flag, int_flag);
    
    fclose(f);
    printf("\n");

    return 0;
}
