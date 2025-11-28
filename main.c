#include "./lib/graph.h"
#include "./lib/algorythm.h"

void message() {
    printf("1. Вставка вершины\n");
    printf("2. Вставка ребра\n");
    printf("3. Вывод графический\n");
    printf("4. Очистка графа\n");
    printf("5. Удаление ребра\n");
    printf("6. Удаление вершины\n");
    printf("7. Изменить информацию вершины\n");
    printf("8. Изменить информацию ребра\n");
    printf("9. Найти все компьютеры, доступные с исходного\n");
    printf("10. Поиск кратчайшего пути от компьютера A до компьютера Б\n");
    printf("11. Ввести из текстового файла\n");
    printf("12. Разбить на компоренты связности\n");
    printf("13. Вывести в формате списка смежности\n");
}

#ifndef ADDITIONAL 
int main() {
    int opt;
    Graph g = create_graph(10);
    message();
    char * from;
    char * to;
    int check = 1;
    check = scanf("%d", &opt);
    char * name;
    size_t size;
    uns_i * ports;
    uns_i port;
    Vertex ** res;
    char * new_name;
    while (check != -1) {
        switch (opt) {
            case 1:
                name = readline("Введите имя компьютера >");
                if (!name) {
                    clean_graph(&g);
                    return 1;
                }
                printf("Введите порт для передечи данных\n");
                check = scanf("%u", &port);
                if (check == -1) {
                    clean_graph(&g);
                    return 1;
                }
                check = paste_vertex(&g, name, port);
                if (check != CORRECT) {
                    printf("Ошибка при вставке вершин, пожалуйста проверьте входные данные и попробуйте еще раз\n");
                }
                break;
            case 2:
                from = readline("Откуда>");
                if (!from) {
                    clean_graph(&g);
                    return 1;
                }
                to = readline("Куда>");
                if (!to) {
                    free(from);
                    clean_graph(&g);
                    return 1;
                }
                printf("Введите пожалуйста количество разрешенных портов для передачи данных\n");
                check = scanf("%ld", &size);
                if (check == -1) {
                    clean_graph(&g);
                    return 1;
                }
                printf("Введите сами порты\n");
                ports = calloc(size, sizeof(uns_i));
                for (size_t i = 0; i < size; i++) {
                    check = scanf("%u", ports + i);
                    if (check == -1) {
                        clean_graph(&g);
                        return 1;
                    }
                }
                int check = paste_edge(&g, ports, size, from, to);
                if (check != CORRECT) {
                    printf("Ошибка при вставке ребра, пожалуйста проверьте входные данные и попробуйте еще раз\n");
                }
                free(from);
                free(to);
                break;

            case 3:
                visuzlize(&g);
                break;
            case 4:
                clean_graph(&g);
                return 0;
                break;
            case 5:
                from = readline("Откуда>");
                if (!from) {
                    clean_graph(&g);
                    return 1;
                }
                to = readline("Куда>");
                if (!to) {
                    free(from);
                    clean_graph(&g);
                    return 1;
                }
                check = remove_edge(&g, from, to);
                if (check != CORRECT) {
                    printf("Ошибка при удалении ребра, пожалуйста проверьте входные данные и попробуйте еще раз\n");
                }
                free(from);
                free(to);
                break;
            case 6:
                name = readline("Введите имя удаляемого компьютера>");
                if (!name) {
                    clean_graph(&g);
                    return 1;
                }
                check = remove_vertex(&g, name);
                if (check != CORRECT) {
                    printf("Ошибка при удалении вершины, пожалуйста проверьте входные данные и попробуйте еще раз\n");
                }
                free(name);
                break;
            case 7:
                name = readline("Введите имя для изменения > ");
                if (!name) {
                    clean_graph(&g);
                    return 1;
                }
                new_name = readline("ВВедите новое имя > ");
                if (!new_name) {
                    free(name);
                    clean_graph(&g);
                    return 1;
                }
                uns_i port;
                printf("Введите новый порт\n");
                check = scanf("%u", &port);
                if (check == -1) {
                    clean_graph(&g);
                    return 1;
                }
                change_vertex(&g, name, new_name, &port);
                free(name);
                break;
            case 8:
                from = readline("Откуда >");
                if (!from) {
                    clean_graph(&g);
                    return 1;
                }
                to = readline("Куда >");
                if (!to) {
                    free(from);
                    clean_graph(&g);
                    return 1;
                }
                printf("Введите количество портов ребра\n");
                check = scanf("%ld", &size);
                if (check == -1) {
                    clean_graph(&g);
                    return 1;
                }
                uns_i * new = calloc(size, sizeof(uns_i));
                printf("Введите порты ребра\n");
                for (size_t i = 0; i < size; i++) {
                    check = scanf("%u", new + i);
                    if (check == -1) {
                        clean_graph(&g);
                        return 1;
                    }
                }
                change_edge(&g, from, to, size, new);
                free(from);
                free(to);
                break;
            case 9:
                name = readline("Введите имя исходного компьютера >");
                if (!name) {
                    clean_graph(&g);
                    return 1;
                }
                res = find_all_path(&g, name);
                size_t i = 0;
                while (res[i] != NULL) {
                    printf("\t%s\n", res[i] -> name);
                    i++;
                }
                free(res);
                free(name);
                break;
            case 10:
                from = readline("Введите начало пути > ");
                if (!from) {
                    clean_graph(&g);
                    return 1;
                }
                to = readline("Bведите конец пути > ");
                if (!to) {
                    clean_graph(&g);
                    return 1;
                }
                printf("Введите порт передачи\n");
                check = scanf("%u", &port);
                if (check == -1) {
                    clean_graph(&g);
                    return 1;
                }
                res = find_min_path(&g, from, to, port);
                if (res == NULL) {
                    printf("Нет пути\n");
                    free(from);
                    free(to);
                    break;
                }
                for (size_t i = 0; res[i] != NULL; i++) {
                    printf("%s--", res[i] -> name);
                }
                free(res);
                free(from);
                free(to);
                break;
            case 11:
                name = readline("Введите название файла > ");
                if (!name) {
                    clean_graph(&g);
                    return 1;
                }
                clean_graph(&g);
                read_graph(&g, name);
                free(name);
                break;
            case 12:
                Vertex *** res = split_graph(&g);
                int num = 1;
                for (size_t i = 0; i < g.csize; i++) {
                    if (res[i] != NULL) {
                        printf("Подсеть %d:\n", num);
                        num++;
                        for (size_t k = 0; res[i][k] != NULL; k++) {
                            printf("\t%s\n", res[i][k] -> name);
                        }
                        free(res[i]);
                    }
                }
                free(res);
                break;
            case 13:
                print_sent_list(&g);
                break;
        }
        message();
        check = scanf("%d", &opt);
        
    }
    if (check == -1) {
        clean_graph(&g);
        return 1;
    }
}
#else
int main() {
    Graph g;
    read_graph(&g, "input.txt");
    visuzlize(&g);
    Vertex *** res = split_graph(&g);
    FILE * out = fopen("additional_task_result.txt", "wt");
    int num = 1;
    for (size_t i = 0; i < g.csize; i++) {
        if (res[i] != NULL) {
            fprintf(out, "Подсеть %d:\n", num);
            num++;
            for (size_t k = 0; res[i][k] != NULL; k++) {
                fprintf(out, "\t%s\n", res[i][k] -> name);
            }
            free(res[i]);
        }
    }
    clean_graph(&g);
    fclose(out);
    free(res);

}

#endif /*ADDITIOL_TAST*/
