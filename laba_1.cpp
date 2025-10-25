#include<iostream>
#include <stack>
#include <map>
#include <string>
#include <cmath>
#include <cctype>

using namespace std;

struct List
{
	int data;
	List* head;
	List* tail;
};

List* create_list(int length)
{
    List* curr = 0, 
        * next = 0;      
    for (int i = 1; i <= length; ++i) {
        curr = new List;

        cout << "Введите данные для элемента " << i << ": ";
        cin >> curr->data;

        curr->tail = next;
        if (next)
            next->head = curr;
        next = curr;
    }
    curr->head = 0;

    return curr;
}

int length_list(List* beg) {
    int count = 0;
    List* curr = beg;
    while (curr != nullptr) {
        count++;
        curr = curr->tail;
    }
    return count;
}

List* list_item(List* beg, int index) {
    List* curr = beg;
    for (int i = 0; i < index && curr != nullptr; i++) {
        curr = curr->tail;
    }
    return curr;
}

void del_item(List*& beg, int index) {
    if (index >= length_list(beg)) {
        return;
    }

    if (!index) {
        List* item = beg->tail;
        delete beg;
        beg = item;
        if (beg) beg->head = nullptr;
        return;
    }

    List* item = list_item(beg, index - 1);  
    List* d_item = item->tail;
    item->tail = d_item->tail;
    if (item->tail) {
        item->tail->head = item;
    }
    delete d_item;
}

List* ins_item(List*& beg, int index) {
    List* item = new List;

    if (!index || !beg) {  
        item->head = nullptr;
        item->tail = beg;
        if (beg) {
            beg->head = item;
        }
        beg = item;
        return item;
    }

    List* pred_item = beg;
    --index;
    while (pred_item->tail && (index--)) {
        pred_item = pred_item->tail;
    }

    item->head = pred_item;
    item->tail = pred_item->tail;
    if (pred_item->tail) {
        pred_item->tail->head = item;
    }
    pred_item->tail = item;
    return item;
}

void print_list_forward(List* beg) {
    List* curr = beg;
    cout << "Список (вперед): ";
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->tail != nullptr) {
            cout << " -> ";
        }
        curr = curr->tail;
    }
    cout << endl;
}

void work_with_dynamic_array() {
    int n1, n2;

    cout << "Введите начальный размер массива: ";
    cin >> n1;

    int* arr = new int[n1]; 

    cout << "Заполните массив (" << n1 << " элементов): ";
    for (int i = 0; i < n1; ++i) {
        cin >> arr[i];
    }

    cout << "Исходный массив (" << n1 << " элементов): ";
    for (int i = 0; i < n1; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    cout << "Введите новый размер массива (больше " << n1 << "): ";
    cin >> n2;

    if (n2 <= n1) {
        cout << "Новый размер должен быть больше исходного!" << endl;
        delete[] arr;
        return;
    }

    int* rez = new int[n2];

    for (int i = 0; i < n1; ++i) {
        rez[i] = arr[i];
    }

    delete[] arr;
    arr = rez;

    cout << "Введите " << (n2 - n1) << " новых элементов для расширения массива: ";
    for (int i = n1; i < n2; ++i) {
        cin >> arr[i];
    }

    cout << "Расширенный массив (" << n2 << " элементов): ";
    for (int i = 0; i < n2; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    delete[] arr;
}

template<typename T>
struct Node {
    T data;
    Node* next;

    Node(const T& value) : data(value), next(nullptr) {}
};

template<typename T>
class list_stack {
private:
    Node<T>* top_node;

public:
    list_stack() : top_node(nullptr) {}

    ~list_stack() {
        while (!empty()) {
            pop();
        }
    }

    void push(const T& value) {
        Node<T>* new_node = new Node<T>(value);
        new_node->next = top_node;
        top_node = new_node;
    }

    void pop() {
        if (empty()) {
            throw runtime_error("Stack is empty");
        }
        Node<T>* temp = top_node;
        top_node = top_node->next;
        delete temp;
    }

    T& top() {
        if (empty()) {
            throw runtime_error("Stack is empty");
        }
        return top_node->data;
    }

    const T& top() const {
        if (empty()) {
            throw runtime_error("Stack is empty");
        }
        return top_node->data;
    }

    bool empty() const {
        return top_node == nullptr;
    }

    size_t size() const {
        size_t count = 0;
        Node<T>* current = top_node;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }
};

bool is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool is_math_function(const string& token) {
    return token == "sin" || token == "cos";
}

int priority(char a) {
    if (a == '^') return 3;
    if (a == '*' || a == '/') return 2;
    if (a == '+' || a == '-') return 1;
    return 0;
}

string reverse_Polish_notation(string infix) {
    list_stack<string> s;  
    string postfix;
    string current_token;

    for (int i = 0; i < infix.size(); i++) {
        char c = infix[i];
        if (c == ' ')
            continue;

        if (isalpha(c)) {
            current_token += c;
            if (i + 1 >= infix.size() || !isalpha(infix[i + 1])) {
                if (is_math_function(current_token)) {
                    s.push(current_token);
                }
                current_token.clear();
            }
        }
        else if (isdigit(c) || c == '.') {
            current_token += c;
            if (i + 1 >= infix.size() || (!isdigit(infix[i + 1]) && infix[i + 1] != '.')) {
                postfix += current_token + " ";
                current_token.clear();
            }
        }
        else if (c == '(') {
            s.push("(");
        }
        else if (c == ')') {
            while (!s.empty() && s.top() != "(") {
                postfix += s.top() + " ";
                s.pop();
            }
            if (!s.empty()) s.pop();
            if (!s.empty() && is_math_function(s.top())) {
                postfix += s.top() + " ";
                s.pop();
            }
        }
        else if (is_operator(c)) {
            string op(1, c);
            while (!s.empty() && s.top() != "(" &&
                priority(s.top()[0]) >= priority(c)) {
                postfix += s.top() + " ";
                s.pop();
            }
            s.push(op);
        }
    }

    while (!s.empty()) {
        postfix += s.top() + " ";
        s.pop();
    }

    return postfix;
}

double evaluate_postfix(const string& postfix) {
    list_stack<double> s;  
    string token;

    for (int i = 0; i < postfix.size(); i++) {
        char c = postfix[i];
        if (c == ' ') {
            if (!token.empty()) {
                if (isdigit(token[0]) || token[0] == '.') {
                    s.push(stod(token));
                }
                else if (is_math_function(token)) {
                    if (s.empty())
                        throw runtime_error("Недостаточно операндов для функции");
                    double arg = s.top();
                    s.pop();
                    if (token == "sin")
                        s.push(sin(arg));
                    else if (token == "cos")
                        s.push(cos(arg));
                }
                else if (is_operator(token[0]) && token.length() == 1) {
                    if (s.size() < 2)
                        throw runtime_error("Недостаточно операндов для оператора");
                    double b = s.top(); s.pop();
                    double a = s.top(); s.pop();
                    switch (token[0]) {
                    case '+': s.push(a + b);
                        break;
                    case '-': s.push(a - b);
                        break;
                    case '*': s.push(a * b);
                        break;
                    case '/':
                        if (b == 0)
                            throw runtime_error("Деление на ноль");
                        s.push(a / b);
                        break;
                    case '^': s.push(pow(a, b));
                        break;
                    }
                }
                token.clear();
            }
        }
        else {
            token += c;
        }
    }

    if (s.size() != 1) throw runtime_error("Некорректное выражение");
    return s.top();
}

int main() {
    setlocale(0, "");

    string expression;

    cout << "Реализация алгоритма сортировочной станции" << endl;
    cout << "Стек реализован на базе списка" << endl;
    cout << "Введите выражение: ";

    while (getline(cin, expression)) {
        if (expression == "0")
            break;
        if (expression.empty())
            continue;

        string postfix = reverse_Polish_notation(expression);
        cout << "Обратная польская нотация: " << postfix << endl;

        try {
            double result = evaluate_postfix(postfix);
            cout << "Результат: " << result << endl;
        }
        catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }

        cout << "\nВведите следующее выражение (или '0' для выхода): ";
    }

    return 0;
}