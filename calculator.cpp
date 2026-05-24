#include <iostream>
#include <cctype>
#include <cstring>
#include <typeinfo>
#include <string>
#include <cmath>
#include <iomanip>
using namespace std;

template <typename T>
struct Node
{
    T data ;
    Node<T>* next;
};

template <typename T>
class Stack
{
private:
    Node<T>* top;
public:
    Stack() { top = nullptr; }

    void push (T ch)
    {
        Node<T>* newNode = new Node<T>;
        newNode->data = ch;
        newNode->next = top;
        top = newNode;
    }

    T pop()
    {
        if (isEmpty()) return '\0';
        T ch = top->data;
        Node<T>* temp = top;
        top = top->next;
        delete temp;
        return ch;
    }

    T peek()
    {
        return (top ? top->data : '\0');
    }

    bool isEmpty()
    {
        return top == nullptr;
    }
};

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

void infixToPostfix(string infix, string postfix[], int& postfixcount)
{
    Stack<char> s;
    postfixcount = 0;

    for (int i = 0; i < infix.length(); i ++) { // 遍歷每個infix 元素
        
        if (isdigit(infix[i]) or infix[i] == '.') {
            string number = "";

            while (i<infix.length() and (isdigit(infix[i]) or infix[i] == '.'))
            {
                number += infix[i]; //如果是字母或數字就放到postfix 裡，然後K 往後一格
                i++;
            }
            i--;
            postfix[postfixcount++] = number;
        }
        else if (infix[i] == '(') {
            s.push(infix[i]); // 把左括號放入stack 裡
        }
        else if (infix[i] == ')') {
            while (s.peek() != '(' && not s.isEmpty()) {
                postfix[postfixcount++] = s.pop(); // 如果是右括號，開始pop stack 裡的元素並放入postfix 裡，直到左括號為止
            }
            if (s.peek() == '(') s.pop(); // 把左括號pop 掉
        }
        else if (i == 0 or not isdigit(infix[i-1])) {
            string number = "";
            while (i<infix.length() and (isdigit(infix[i]) or infix[i] == '.'))
            {
                number += infix[i]; //如果是字母或數字就放到postfix 裡，然後K 往後一格
                i++;
            }
            i--;
            postfix[postfixcount++] = number;
        }
        else {
            // 處理operators
            // 如果stack 裡operator 的precedence 比infix 的operator 還要大，就放到postfix 裡
            while (not s.isEmpty() && precedence(s.peek()) >= precedence(infix[i])) {
                postfix[postfixcount++] = s.pop();
            }
            s.push(infix[i]); // 把目前的operator 放入stack
        }
    }
    while (not s.isEmpty()) {
        postfix[postfixcount++] = s.pop(); // 確保沒有任何operators 殘留在stack
    }
}

void printPostfix (string postfix[], int postfixcount)
{
    cout << "Postfix expression: ";
    for (int i = 0; i < postfixcount; i++)
    {
        cout << postfix[i];
    }
    cout << endl;
}

void evaluatePostfix(string postfix[], int postfixcount)
{
    Stack<double> evaluation;

    for (int i = 0; i < postfixcount; i++)
    {
        string token = postfix[i];

        if (token.length() == 1 and not isdigit(token[0]) and token[0] != '.')
        {  
            double a = evaluation.pop(); // 提取頂層元素
            double b = evaluation.pop(); // 提取第二層元素
            
            
            if (token[0] == '+')
            {
                evaluation.push(a + b);
            }
            else if (token[0] == '-')
            {
                evaluation.push(b - a);
            }
            else if (token[0] == '*')
            {
                evaluation.push(a * b);
            }
            else if (token[0] == '/')
            {
                evaluation.push(b / a);
            }
            else if (token[0] == '^')
            {
                evaluation.push(pow(b, a));
            }
            else if (token[0] == '%')
            {
                evaluation.push(fmod(b, a));
            }
        }
        else {
            evaluation.push(stod(token)); // 把postfix[i] 以int 存入stack
        }
    }
    cout << fixed << setprecision(1);
    cout<<"Result: ";
    cout << evaluation.peek() << endl;
}

int main()
{
    string infix, postfix[100];
    int postfixcount = 0;

    cout << "Enter an Infix expression: ";
    getline(cin,infix); // 輸入中序表達式

    infixToPostfix(infix, postfix, postfixcount); // 轉換為後序表達式
    printPostfix(postfix, postfixcount); // 輸出後序表達式

    evaluatePostfix(postfix, postfixcount);

    return 0;
}