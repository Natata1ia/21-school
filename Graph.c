#include <stdio.h>
#include <math.h>

// Максимальная длина одной строки
#define STRLEN 100

#define UNKNOWN     0
#define NUMBER      1
#define LETTER      2
#define PARENTHESES 3

// Возвращает длину строки str
int _strlen(char* str) {
    for (int i = 0; i < STRLEN; i++)
        if (str[i] == '\0') return i;
    return 0;
}

// Добавляет строку str1 к str2
void _stradd(char* str1, char* str2) {
    // Находим индекс конца строки str2
    int i = _strlen(str2);
    // Вставляем в конец строки str2 символы из строки str1
    for (int j = 0; j <= _strlen(str1); j++) {
        str2[i++] = str1[j];
    }
}

// Добавляет символ ch к str
void _straddch(char ch, char* str) {
    // Находим индекс конца строки str2
    int i = _strlen(str);
    // Вставляем в конец строки str2 символы из строки str1
    str[i++] = ch; str[i] = '\0';
}

// Очищает строку str
void _strclr(char* str) {
    for (int i = 0; i < STRLEN; i++) str[i] = '\0';
}

// Удаляет из строки str символ с индексом i
void _strremch(char* str, int i) {
    int length = _strlen(str);
    if (i < 0 || i >= length) return;
    for (int j = i; j < length; j++)
        str[j] = str[j + 1];
}

// Удаляет пробелы из строки
void _strnogap(char* str) {
    for (int i = 0; i < _strlen(str); i++)
        if (str[i] == ' ') _strremch(str, i--);
}

// Копирует строку str1 в str2
void _strcpy(char* str1, char* str2) {
    for (int i = 0; i < (_strlen(str1) + 1); i++)
        str2[i] = str1[i];
}

int _isdigit(char ch) {
    return (ch > 47 && ch < 58);
}

int _isletter(char ch) {
    return (ch > 64 && ch < 91) || (ch > 96 && ch < 123);
}

// Возвращает значение приоритета операции
int rating(char ch) {
    if (ch == '+' || ch == '-') return 1;
    else if (ch == '*' || ch == '/') return 2;
    else if (ch == '^') return 3;
    else return 0;
}

// Преобразует число в виде строки в настоящее число
float toNumber(char* str) {
    // Переменная с ответом
    float output = 0.0;
    // Тип числа (первый бит - положит.(0)/отриц.(1), второй - целая часть(0)/дробная часть(1))
    char flags = 0b00;
    // Индекс символа с точкой в случае, если чисто дробное
    int pointIndex = 0;
    for (int i = 0; i < _strlen(str); i++) {
        // Если число отрицательное, выставляем флаг
        if (str[i] == '-') flags |= 0b01;
        // Если это цифра
        else if (_isdigit(str[i])) {
            // Если это дробная часть числа
            if (flags & 0b10) {
                // Создаём новую переменную и присваиваем ему рассматриваемую цифру
                float temp = str[i] - 48;
                // В зависимости от дальности цифры от символа точки, делим это число несколько раз на 10
                for (int j = pointIndex; j < i; j++) temp /= 10;
                output += temp;
            } 
            // Если это целая часть числа
            else {
                output *= 10;
                output += str[i] - 48;
            }
        }
        // Если встретили точку, то выставляем флаг и запоминаем её индекс
        else if (str[i] == '.') {
            flags |= 0b10;
            pointIndex = i;
        }
    }
    // Если число было отрицательным, делаем его отрицательным
    if (flags & 0b01) output = 0 - output;
    return output;
}

// Удалить один элемент по индексу index из двумерного массива array размерности size
void removeElem(char array[][STRLEN], int* size, int index) {
    *size -= 1; 
    for (int i = index; i < *size; i++) {
        _strcpy(array[i + 1], array[i]);
    }
    _strclr(array[*size]);
}

// Разбивает строку input на отдельные элементы в output
void inputParser(char* input, char output[][STRLEN]) {
    int type, lastType;
    int index = 0;
    char buffer[STRLEN]; _strclr(buffer);
    for (int i = 0; i < (_strlen(input) + 1); i++) {
        // Определяем тип текущего символа
        if (_isdigit(input[i]) || input[i] == '.') type = NUMBER;
        else if (_isletter(input[i])) type = LETTER;
        else if (input[i] == '(' || input[i] == ')') type = PARENTHESES;
        else type = UNKNOWN;
        // Сравниваем его с предыдущим символом
        if (i > 0 && (lastType != type || type == PARENTHESES || (input[i] == '-' && (lastType == UNKNOWN) || input[i - 1] == '('))) {
            _strclr(output[index]);
            for (int j = 0; j < _strlen(buffer); j++)
                output[index][j] = buffer[j];
            index++;
            _strclr(buffer);
        }
        // Заносим текущий символ в буффер
        _straddch(input[i], buffer);
        // Запоминает тип текущего символа
        lastType = type;
    }

    //for (int i = 0; i < index; i++) printf("Elem %d is %s\n", i, output[i]);

    for (int i = 0; i < index; i++) {
        if (output[i][0] == '-' && _strlen(output[i]) == 1) {
            if (i == 0 || (i > 0 && _strlen(output[i - 1]) == 1 && (output[i - 1][0] == '(' || output[i - 1][0] == '-' || output[i - 1][0] == '+' || 
                output[i - 1][0] == '*' || output[i - 1][0] == '/' || output[i - 1][0] == '^'))) {
                    _stradd(output[i + 1], output[i]);
                    removeElem(output, &index, i + 1);
            }
        }
    }

    //for (int i = 0; i < index; i++) printf("Elem %d is %s\n", i, output[i]);
}

// Переводит обычное выражение в польскую обратную запись
// https://ru.wikipedia.org/wiki/%D0%9E%D0%B1%D1%80%D0%B0%D1%82%D0%BD%D0%B0%D1%8F_%D0%BF%D0%BE%D0%BB%D1%8C%D1%81%D0%BA%D0%B0%D1%8F_%D0%B7%D0%B0%D0%BF%D0%B8%D1%81%D1%8C
void expressionTransform(char* input, char* output) {
    // Массив, в котором будут лежать элементы выражения
    char array[STRLEN][STRLEN], ch;
    // Массив стека (по алгоритму)
    char stack[STRLEN][STRLEN], stackIndex = 0;
    // Массив выхода (по алгоритму)
    char out[STRLEN][STRLEN], outIndex = 0;

    // Очищаем стек и выход
    for (int i = 0; i < STRLEN; i++) {
        _strclr(stack[i]);
        _strclr(out[i]);
    }
    _strclr(output);

    // Разбиваем всё выражение на отдельные элементы
    inputParser(input, array);

    // Выводим разбитое на элементы выражение
    for (int i = 0; i < STRLEN; i++) {
        if (!_strlen(array[i])) break;
        //printf("[%d] - %s\n", i, array[i]);
    }

    // Преобразуем элементы выражения в польскую обратную запись
    for (int i = 0; i < STRLEN; i++) {
        //printf("Read char at index %d and it is %s\n", i, array[i]);
        if (!_strlen(array[i])) break;
        // Берём только первый символ из элемента
        ch = array[i][0];
        // Если символ - цифра, то просто заносим элемент в выход
        if (_isdigit(ch) || ch == 'x' || ch == 'X' || (ch == '-' && _strlen(array[i]) > 1))
            _strcpy(array[i], out[outIndex++]);
        // Если элемент - префиксная функция
        else if (ch == 'c' || ch == 's' || ch == 't' || ch == 'l') {
            // Заносим элемент в стек
            _strcpy(array[i], stack[stackIndex++]);
        }
        // Если символ - бинарная операция, то тут сложнее
        else if (_strlen(array[i]) == 1 && (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^')) {
            if (stackIndex > 0) {
                // Если на вершине стека лежит префиксная операция, выталкиваем её в выход из стека
                while (stack[stackIndex - 1][0] == 'c' || stack[stackIndex - 1][0] == 's'
                    || stack[stackIndex - 1][0] == 't' || stack[stackIndex - 1][0] == 'l') {
                    _strcpy(stack[stackIndex - 1], out[outIndex++]);
                    stackIndex--;
                }
                // Если на вершине стека операция приоритетнее или такая же по приоритету
                while (_strlen(stack[stackIndex - 1]) == 1 && (rating(stack[stackIndex - 1][0]) >= rating(ch))) {
                    _strcpy(stack[stackIndex - 1], out[outIndex++]);
                    stackIndex--;
                }
            }
            // Заносим элемент в стек
            _strcpy(array[i], stack[stackIndex++]);
        }
        // Если это открывающая скобка
        else if (ch == '(') {
            _strcpy(array[i], stack[stackIndex++]);
        }
        // И если это закрывающая скобка
        else if (ch == ')') {
            while(stack[stackIndex - 1][0] != '(') {
                _strcpy(stack[stackIndex - 1], out[outIndex++]);
                stackIndex--;
            }
            stackIndex--;
        }

        //for (int j = 0; j < outIndex; j++)
        //printf("[%d][%d] - %s\n", i, j, out[j]);
    }

    // Выдавливаем элементы из стэка в выход
    for (int i = stackIndex - 1; i >= 0; i--) _strcpy(stack[i], out[outIndex++]);

    // Создаём готовый ответ
    for (int i = 0; i < outIndex; i++) {
        //printf("[%d] - %s\n", i, out[i]);
        _stradd(out[i], output);
        _straddch(' ', output);
    }
}

// Возвращает значение из выражения expression (в польской обратной записи) с аргументом x
float calculate(char* expression, float x) {
    // Создаём и очищаем буфер
    char buffer[STRLEN]; _strclr(buffer);
    // Создаём стэк для разбиения выражения на логические части (числа, операции, функции)
    char stack[STRLEN][STRLEN]; int stackIndex = 0;
    for (int i = 0; i < STRLEN; i++) _strclr(stack[i]); // Чистим всё
    // Переменные для подсчёта
    float number1, number2, answer;
    // Промежуточная строка для записи ответов
    char temp[STRLEN]; _strclr(temp);

    // Анализируем полученное выражение посимвольно
    for (int i = 0; i <= _strlen(expression); i++) {
        if (expression[i] == ' ' || expression[i] == '\0') {
            //printf("Found elem - <%s>\n", buffer);
            _strcpy(buffer, stack[stackIndex++]);
            _strclr(buffer); continue;
        }
        _straddch(expression[i], buffer);
    }

    // Пробуем посчитать выражение
    for (int i = 0; i < stackIndex; i++) {
        // for (int j = 0; j < stackIndex; j++) printf("Stack at %d is %s\n", j, stack[j]);
        // Если попалась бинарная операция
        if (stack[i][0] == '+' || stack[i][0] == '-' || 
            stack[i][0] == '*' || stack[i][0] == '/' || stack[i][0] == '^') {
                // Проверка на дурачка
                if (i < 2) continue;
                
                // Записываем операнды для операции
                if (_isdigit(stack[i - 1][0]) || stack[i - 1][0] == '-') 
                    number2 = toNumber(stack[i - 1]);
                else 
                    number2 = x;
                if (_isdigit(stack[i - 2][0])  || stack[i - 2][0] == '-') 
                    number1 = toNumber(stack[i - 2]);
                else 
                    number1 = x;

                // Выполняем соответствующую операцию над операндами
                if (stack[i][0] == '+')         answer = number1 + number2;
                else if (stack[i][0] == '-')    answer = number1 - number2;
                else if (stack[i][0] == '*')    answer = number1 * number2;
                else if (stack[i][0] == '/')    answer = number1 / number2;
                else if (stack[i][0] == '^') {
                    answer = 1;
                    for (int j = 0; j < number2; j++) answer *= number1;
                }
                sprintf(temp, "%f", answer);

                // Удаляем лишнее из стека и записываем результат
                _strcpy(temp, stack[i - 2]);
                removeElem(stack, &stackIndex, i - 1);
                removeElem(stack, &stackIndex, i - 1);
                i -= 2;
        }
        else if (stack[i][0] == 'c' || stack[i][0] == 's' || stack[i][0] == 't' || stack[i][0] == 'l') {
            // Проверка на дурачка
            if (i < 1) continue;
            // Ещё одна проверка на дурачка
            if (_strlen(stack[i]) < 2) continue;
            
            // Записываем операнд для операции
            if (_isdigit(stack[i - 1][0])) 
                number1 = toNumber(stack[i - 1]);
            else 
                number1 = x;
            
            // Вычисляем
            if (stack[i][0] == 's' && stack[i][1] == 'i') answer = sin(number1);
            else if (stack[i][0] == 'c' && stack[i][1] == 'o') answer = cos(number1);
            else if (stack[i][0] == 't' && stack[i][1] == 'g') answer = tan(number1);
            else if (stack[i][0] == 'c' && stack[i][1] == 't') answer = 1.0 / tan(number1);
            else if (stack[i][0] == 's' && stack[i][1] == 'q') answer = sqrt(number1);
            else if (stack[i][0] == 'l' && stack[i][1] == 'n') answer = log(number1);
            else continue;
            sprintf(temp, "%f", answer);
            
            // Удаляем лишнее из стека и записываем результат
            _strcpy(temp, stack[i - 1]);
            removeElem(stack, &stackIndex, i);
            i--;
        }
    }

    if (stack[0][0] == 'x' || stack[0][0] == 'X') answer = x;
    else if (stack[0][0] == '-' && (stack[0][1] == 'x' || stack[0][1] == 'X')) answer = -x;
    else answer = toNumber(stack[0]);
    
    
    return answer;
}

int main (int argc, char **argv) 
{
    // Входная строка (для исходного выражения)
    char input[STRLEN] = "3 4 2 * 1 5 - 2 ^ / +";
    char output[STRLEN];
    float answer;

    //printf("%s = %f\n", input, calculate(input, 1));

    // 34.01 + sin(45) * 2
    // 34.01, +, sin, (, 45, ), *, 2
    

    // Пока есть данные в аргументах программы
    _strclr(input);
    while(--argc)
        _stradd(argv[argc], input);

    if (!_strlen(input))
        scanf("%s", input);

    expressionTransform(input, output);

    //printf("%s = %f\n", input, calculate(output, 0.1));
    //printf("%s = %f\n", output, calculate(output, 0.1));
    
    char graphic[25][80];

    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++) {
            graphic[i][j] = '.';
        }
    }
 
    for (int i = 0; i < 80; i++) {
        float x = i * 4.0 * 3.14 / 80.0 + 2.0 * 3.14 / 80.0;
        answer = calculate(output, x);
        //printf("%s for x = %f = %f\n", output, x, answer);
        if (answer > 1 || answer < -1) continue;
        answer += 1;
        for (int j = 0; j < 25; j++) {
            if (answer <= ((j+1) * 2.0 / 25.0)) {
                graphic[j][i] = '*';
                break;
            }
        }
    }

    for (int i = 24; i >= 0; i--) {
        for (int j = 0; j < 80; j++) {
            printf("%c", graphic[i][j]);
        }
        printf("\n");
    }
}