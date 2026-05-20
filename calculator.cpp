#include <iostream>
#include <cctype>
#include <cstring>
using namespace std;


struct Node
{
    char data ;
    Node* next;
};


class Stack
{
private:
    Node* top;
public:
    Stack() { top = nullptr; }

    void push (char ch)
    {
        Node* newNode = new Node;
        newNode->data = ch;
        newNode->next = top;
        top = newNode;
    }

    char pop()
    {
        if (isEmpty()) return '\0';
        char ch = top->data;
        Node* temp = top;
        top = top->next;
        delete temp;
        return ch;
    }

    char peek()
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
    if (op == '*' or op == '/')
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

void infixToPostfix(const char* infix, char* postfix)
{
    Stack s;
    int k = 0;

    for (int i = 0; i < strlen(infix); i ++) { // 遍歷每個infix 元素
        
        if (isalnum(infix[i])) {
            postfix[k++] = infix[i]; //如果是字母或數字就放到postfix 裡，然後K 往後一格
        }
        else if (infix[i] == '(') {
            s.push(infix[i]); // 把左括號放入stack 裡
        }
        else if (infix[i] == ')') {
            while (s.peek() != '(' && not s.isEmpty()) {
                postfix[k++] = s.pop(); // 如果是右括號，開始pop stack 裡的元素並放入postfix 裡，直到左括號為止
            }
            if (s.peek() == '(') s.pop(); // 把左括號pop 掉
        }
        else {
            // 處理operators
            // 如果stack 裡operator 的precedence 比infix 的operator 還要大，就放到postfix 裡
            while (not s.isEmpty() && precedence(s.peek()) >= precedence(infix[i])) {
                postfix[k++] = s.pop();
            }
            s.push(infix[i]); // 把目前的operator 放入stack
        }
    }
    while (not s.isEmpty()) {
        postfix[k++] = s.pop(); // 確保沒有任何operators 殘留在stack
    }
    postfix[k] = '\0'; // 避免印出亂碼
}

void evaluatePostfix(char* postfix)
{
    Stack evaluation;

    for (int i = 0; i < strlen(postfix); i++)
    {
        if (isalnum(postfix[i]))
        {
            evaluation.push(postfix[i]);
            // postfix: 12+3+ -> evaluation: 1 and 2 -> 
        }
        else if (not isalnum(postfix[i]))
        {   
            char a = evaluation.pop();
            char b = evaluation.pop();
                
            int intA = a - '0';
            int intB = b - '0';
            
            if (postfix[i] == '+')
            {
                evaluation.push(intA + intB + '0');
            }
            else if (postfix[i] == '-')
            {
                evaluation.push(intB - intA + '0');
            }
            else if (postfix[i] == '*')
            {
                evaluation.push(intA * intB + '0');
            }
            else if (postfix[i] == '/')
            {
                evaluation.push(intB / intA + '0');
            }
            
        }
        
    }
    // cout << "evaluation";
    cout << evaluation.peek() << endl;
}

int main()
{
    char infix[100], postfix[100];
    cout << "Enter an Infix expression: ";
    cin >> infix; // 輸入中序表達式

    infixToPostfix(infix, postfix); // 轉換為後序表達式
    cout << "Postfix expression: " << postfix << endl; // 輸出後序表達式

    evaluatePostfix(postfix);

    return 0;

    
}