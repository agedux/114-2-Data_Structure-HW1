#include <iostream>
#include <cctype>
#include <cstring>
#include <typeinfo>
#include <string>
#include <cmath>
#include <iomanip>
using namespace std;

// 將node 寫成template → 省略第二個stack
template <typename T>
struct Node
{
    T data ;
    Node<T>* next;
};

// 將stack 寫成template → 省略第二個stack 。Stack<char> 就是字元stack，Stack<double> 就是數字stack
template <typename T>
class Stack
{
private:
    Node<T>* top;
public:
    Stack() { top = nullptr; } // 初始化stack

    // Push 操作: 將元素放入stack
    void push (T ch)
    {
        Node<T>* newNode = new Node<T>;
        newNode->data = ch;
        newNode->next = top;
        top = newNode;
    }

    // pop 操作: 移除並回傳頂端元素
    T pop()
    {
        if (isEmpty()) return '\0';
        T ch = top->data;
        Node<T>* temp = top;
        top = top->next;
        delete temp;
        return ch;
    }

    // Peek 操作: 取得頂端元素但不移除
    T peek()
    {
        return (top ? top->data : '\0');
    }

    // 判斷stack 是否為空
    bool isEmpty()
    {
        return top == nullptr;
    }
};

// 判斷輸入的字元是不是operator
bool isOperator(char ch) {
    return ch == '+' or ch == '-' or ch == '*' or ch == '/' or ch == '^' or ch == '%';
}

// 錯誤處裡: 判斷輸入的是不是數字、括號或operator
bool isValidChar(char ch) {
    return isdigit(ch) or ch == '.' or ch == '+' or ch == '-' or ch == '*' or ch == '/' or ch == '^' or ch == '%' or ch == '(' or ch == ')';
}

// 賦予operator 不同的優先順序: 次方 > 乘、除與求餘 > 加、減
int precedence(char op)
{
    if (op == '^')
    {
        return 3;
    }
    else if (op == '*' or op == '/' or op == '%')
    {
        return 2;
    }
    else if (op == '+' or op == '-')
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}

// infix 轉 postfix
bool infixToPostfix(string infix, string postfix[], int& postfixcount)
{
    Stack<char> s; // 字元stack
    postfixcount = 0;

    // 遍歷infix
    for (int i = 0; i < infix.length(); i++) {

        // 錯誤處理: 先判斷infix[i] 有沒有輸入錯誤
        if (not isValidChar(infix[i])) {
            cout << "Error: Invalid character '" << infix[i] << "'" << endl;
            return false;
        }

        // 如果不是operator 就加入到postfix
        if (isdigit(infix[i]) or infix[i] == '.') {
            string number = "";

            // 繼續遍歷直到非數字或小數點
            while (i < infix.length() and (isdigit(infix[i]) or infix[i] == '.')) {
                number += infix[i]; // 打包成number
                i++;
            }
            i--;
            postfix[postfixcount++] = number; // 把number 加入到postfix
        }

        // 如果是左括號
        else if (infix[i] == '(') {
            
            // 錯誤處理: 判斷是不是空括號
            if (i + 1 < infix.length() and infix[i+1] == ')') {
                cout << "Error: Empty parentheses" << endl;
                return false;
            }
            s.push(infix[i]); // 左括號加到stack
        }

        // 如果是右括號
        else if (infix[i] == ')') {
            
            // pop operators 直到左括號
            while (not s.isEmpty() and s.peek() != '(') {
                postfix[postfixcount++] = s.pop();
            }

            // 錯誤處理: 判斷Stack 裡面是不是空的，避免只有右括號
            if (s.isEmpty()) {
                cout << "Error: Unmatched closing parenthesis" << endl;
                return false;
            }

            s.pop();  // pop 左括號
        }

        // 負數處理: 讀到'-' 且位置在一開始或前一個字元是左括號或operator
        else if (infix[i] == '-' and (i == 0 or infix[i-1] == '(' or infix[i-1] == '+' or infix[i-1] == '-' or infix[i-1] == '*' or infix[i-1] == '/' or infix[i-1] == '^' or infix[i-1] == '%')) {
            string number = "-";
            i++; // 從負數下一個字元開始處理

            // 繼續遍歷直到非數字或小數點
            while (i < infix.length() and (isdigit(infix[i]) or infix[i] == '.')) {
                number += infix[i];  // 打包成number
                i++;
            }
            i--;
            postfix[postfixcount++] = number; // 把number 加入到postfix
        }

        // 如果是operator
        else if (isOperator(infix[i])) {

            // 錯誤處理: 結尾是operator
            if (i == infix.length() - 1) {
                cout << "Error: Expression ends with operator" << endl;
                return false;
            }

            // 錯誤處理: 如果連續兩個operators
            if (i > 0 and isOperator(infix[i-1])) {
                cout << "Error: Consecutive operators" << endl;
                return false;
            }

            // 如果stack 裡operator 的precedence 比infix 的operator 還要大，就放到postfix 裡
            while (not s.isEmpty() and s.peek() != '(' and precedence(s.peek()) >= precedence(infix[i])) {
                postfix[postfixcount++] = s.pop();
            }
            s.push(infix[i]); // 把目前的operator 放入stack
        }
    }

    // 錯誤處理: 如果只有輸入左括號卻沒輸入右括號
    while (not s.isEmpty()) {
        if (s.peek() == '(') {
            cout << "Error: Unmatched opening parenthesis" << endl;
            return false;
        }
        postfix[postfixcount++] = s.pop();
    }

    // 錯誤處理: 如果postfix 是空的
    if (postfixcount == 0) {
        cout << "Error: Empty expression" << endl;
        return false;
    }

    return true; // 都沒錯→回報true 繼續
}

// 輸出postfix
void printPostfix (string postfix[], int postfixcount)
{
    cout << "Postfix expression: ";
    // 遍歷postfix
    for (int i = 0; i < postfixcount; i++)
    {
        cout << postfix[i];
    }
    cout << endl;
}

bool evaluatePostfix(string postfix[], int postfixcount)
{
    Stack<double> evaluation; // 浮點數stack

    //逐步顯示計算過程
    cout << "\n--- Step-by-step Evaluation ---" << endl;

    // 遍歷postfix
    for (int i = 0; i < postfixcount; i++)
    {
        string token = postfix[i]; // 把目前的element 作為token

        // 如果token 長度為1 並且是operator 則開始處理計算
        if (token.length() == 1 and isOperator(token[0]))
        {
            // 錯誤處理: 避免只有operator 沒有operand
            if (evaluation.isEmpty()) {
                cout << "Error: Missing operands" << endl;
                return false;
            }

            double a = evaluation.pop(); // pop 頂層operand
            
            // 錯誤處理: 避免pop a 後只有一個operand
            if (evaluation.isEmpty()) {
                cout << "Error: Missing operands" << endl;
                return false;
            }

            double b = evaluation.pop(); // pop 第二層operand

            cout << "Pop: " << b << ", Pop: " << a << " -> " << b << " " << token[0] << " " << a; // 印出計算過程

            // 判斷token[0] 是哪種operator
            if (token[0] == '+')
            {
                evaluation.push(b + a); // b+a
            }
            else if (token[0] == '-')
            {
                evaluation.push(b - a); // b-a
            }
            else if (token[0] == '*')
            {
                evaluation.push(b * a); // b*a
            }
            else if (token[0] == '/')
            {
                // 錯誤處理: 避免分母為0
                if (a == 0) {
                    cout << "Error: Division by zero" << endl;
                    return false;
                }
                evaluation.push(b / a); // b/a
            }
            else if (token[0] == '^')
            {
                evaluation.push(pow(b, a)); // b^a
            }
            else if (token[0] == '%')
            {
                // 錯誤處理: 避免除數為0
                if (a == 0) {
                    cout << "Error: Modulo by zero" << endl;
                    return false;
                }
                evaluation.push(fmod(b, a)); // b%a
            }

            cout << " = " << evaluation.peek() << " -> Push: " << evaluation.peek() << endl; // 印出計算結果
        }
        // 如果token 不是operator
        else {
            evaluation.push(stod(token)); // 把token 轉為double 存入stack
            cout << "Push: " << token << endl;
        }
    }

    // 把evaluation 存入result
    double result = evaluation.pop();
    
    // 錯誤處理: 避免evaluation 裡還有東西
    if (not evaluation.isEmpty()) {
        cout << "Error: Expression incomplete" << endl;
        return false;
    }

    // 印出計算結果
    cout << fixed << setprecision(1); // 控制輸出格式: 5 → 5.0
    cout << "------------------------------" << endl;
    cout << "Result: ";
    cout << result << endl;
    return true;
}

int main()
{
    string infix, postfix[100];
    int postfixcount = 0;

    cout << "Enter an Infix expression: ";
    getline(cin,infix); // 輸入中序表達式。getline: 避免輸入空白導致錯誤

    // 假設infixTOPostfix 沒有問題
    if (infixToPostfix(infix, postfix, postfixcount)) {
        printPostfix(postfix, postfixcount); // 輸出後序表達式
        evaluatePostfix(postfix, postfixcount); // 計算postfix 結果
    }

    return 0;
}