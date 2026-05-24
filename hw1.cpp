#include<iostream>
#include<string>
#include<cctype>
#include<iomanip>
#include<cmath>
#include<cstdlib>
using namespace std;
struct CharNode{
    char data;
    CharNode*next;
};
class CharStack{
private:
    CharNode*top;
public:
    CharStack(){
        top=nullptr;
    }
    ~CharStack(){
        while(!isEmpty())pop();
    }
    void push(char value){
        CharNode*newNode=new CharNode;
        newNode->data=value;
        newNode->next=top;
        top=newNode;
    }
    char pop(){
        char value=top->data;
        CharNode*temp=top;
        top=top->next;
        delete temp;
        return value;
    }
    char peek(){
        return top->data;
    }
    bool isEmpty(){
        return top==nullptr;
    }
};
struct DoubleNode{
    double data;
    DoubleNode*next;
};
class DoubleStack{
private:
    DoubleNode*top;
public:
    DoubleStack(){
        top=nullptr;
    }
    ~DoubleStack(){
        while(!isEmpty())pop();
    }
    void push(double value){
        DoubleNode*newNode=new DoubleNode;
        newNode->data=value;
        newNode->next=top;
        top=newNode;
    }
    double pop(){
        double value=top->data;
        DoubleNode*temp=top;
        top=top->next;
        delete temp;
        return value;
    }
    double peek(){
        return top->data;
    }
    bool isEmpty(){
        return top==nullptr;
    }
};
bool isOperator(char ch){
    return ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='^'||ch=='%';
}
int precedence(char op){
    if(op=='^')return 3;
    else if(op=='*'||op=='/'||op=='%')return 2;
    else if(op=='+'||op=='-')return 1;
    else return 0;
}
void infixToPostfix(string infix,string postfix[],int&postfixCount){
    CharStack operators;
    postfixCount=0;
    for(int i=0;i<infix.length();i++){
        char ch=infix[i];
        if(ch==' ')continue;
        else if(isdigit(ch)||ch=='.'){
            string number="";
            while(i<infix.length()&&(isdigit(infix[i])||infix[i]=='.')){
                number+=infix[i];
                i++;
            }
            i--;
            postfix[postfixCount]=number;
            postfixCount++;
        }
        else if(ch=='('){
            operators.push(ch);
        }
        else if(ch==')'){
            while(!operators.isEmpty()&&operators.peek()!='('){
                string op="";
                op+=operators.pop();
                postfix[postfixCount]=op;
                postfixCount++;
            }
            if(!operators.isEmpty())operators.pop();
        }
        else if(isOperator(ch)){
            while(!operators.isEmpty()&&operators.peek()!='('&&(precedence(operators.peek())>precedence(ch)||(precedence(operators.peek())==precedence(ch)&&ch!='^'))){
                string op="";
                op+=operators.pop();
                postfix[postfixCount]=op;
                postfixCount++;
            }
            operators.push(ch);
        }
    }
    while(!operators.isEmpty()){
        string op="";
        op+=operators.pop();
        postfix[postfixCount]=op;
        postfixCount++;
    }
}
double evaluatePostfix(string postfix[],int postfixCount){
    DoubleStack values;
    for(int i=0;i<postfixCount;i++){
        string token=postfix[i];
        if(token.length()==1&&isOperator(token[0])){
            double right=values.pop();
            double left=values.pop();
            if(token[0]=='+')values.push(left+right);
            else if(token[0]=='-')values.push(left-right);
            else if(token[0]=='*')values.push(left*right);
            else if(token[0]=='/')values.push(left/right);
            else if(token[0]=='^')values.push(pow(left,right));
            else if(token[0]=='%')values.push(fmod(left,right));
        }
        else{
            values.push(atof(token.c_str()));
        }
    }
    return values.pop();
}
void printPostfix(string postfix[],int postfixCount){
    cout<<"Postfix expression: ";
    for(int i=0;i<postfixCount;i++){
        cout<<postfix[i];
        if(i!=postfixCount-1)cout<<" ";
    }
    cout<<endl;
}
int main(){
    string infix;
    string postfix[100];
    int postfixCount=0;
    cout<<"Enter an Infix expression: ";
    getline(cin,infix);
    infixToPostfix(infix,postfix,postfixCount);
    printPostfix(postfix,postfixCount);
    double result=evaluatePostfix(postfix,postfixCount);
    cout<<fixed<<setprecision(1);
    cout<<"Result: "<<result<<endl;
    return 0;
}
