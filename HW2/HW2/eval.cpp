//
//  eval.cpp
//  HW2
//
//  Created by Quentin Truong on 2/4/17.
//  Copyright © 2017 QT. All rights reserved.
//

#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

int evaluate(string infix, const bool values[], string& postfix, bool& result);
string toPostfix(string infix, int& valid);//infix -> postfix
bool toResult(string postfix, const bool values[]);//postfix -> result
int precedence(char op);
int preCheck(string infix);
int endCheck(string postfix);

//returns 0 if valid, 1 if invalid
int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
    int isValid = 0;
    postfix = toPostfix(infix, isValid);
    
    if (isValid == 1)
        return 1;
    
    result = toResult(postfix, values);
    return 0;
}

string toPostfix(string infix, int& isValid){
    while (infix.find("!!") != string::npos)
        infix = infix.erase(infix.find("!!"), 2);
    
    isValid = preCheck(infix);
    if (isValid == 1)
        return "";
    
    string result = "";
    stack<char> opStack;
    for (int a = 0; a < infix.size(); a++){
        switch(infix[a]){
            case ' ':
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                result += infix[a];
                break;
            case '(':
                opStack.push(infix[a]);
                break;
            case ')':
                while (opStack.top() != '('){
                    result += opStack.top();
                    opStack.pop();
                }
                opStack.pop();
                break;
            case '&':
            case '|':
            case '!':
                while (!opStack.empty() && opStack.top() != '(' && precedence(infix[a]) <= precedence(opStack.top())){
                    result += opStack.top();
                    opStack.pop();
                }
                opStack.push(infix[a]);
                break;
            default:
                isValid = 1;
                return result;
        }
    }
    while (!opStack.empty()){
        result += opStack.top();
        opStack.pop();
    }
    isValid = endCheck(result);
    return result;
}

bool toResult(string postfix, const bool values[]){//1 is true, 0 is false
    stack<bool> opStack;
    
    for (int a = 0; a < postfix.size(); a++){
        if (postfix[a] != '&' && postfix[a] != '|' && postfix[a] != '!'){
            opStack.push(values[postfix[a] - '0']);
        }
        else if (postfix[a] != '!'){
            bool op2 = opStack.top();
            opStack.pop();
            bool op1 = opStack.top();
            opStack.pop();
            
            if (postfix[a] == '&'){
                if (op1 && op2)
                    opStack.push(true);
                else
                    opStack.push(false);
            }
            else if (postfix[a] == '|'){
                if (op1 || op2)
                    opStack.push(true);
                else
                    opStack.push(false);
            }
        }
        else if (postfix[a] == '!'){
            bool op1 = opStack.top();
            opStack.pop();
            opStack.push(!op1);
        }
    }
    
    return opStack.top();
}

int precedence(char op){
    switch (op){
        case '&':
            return 2;
        case '|':
            return 1;
        case '!':
            return 3;
    }
    return 0;
}

int preCheck(string infix){
    if (infix.size() == 0)
        return 1;
    
    while (infix.find('!') != string::npos)
        infix = infix.erase(infix.find('!'), 1);
    while (infix.find(' ') != string::npos)
        infix = infix.erase(infix.find(' '), 1);
    
    if (infix.size() == 0)
        return 1;
    
    for (int a = 1; a < infix.size() - 1; a++){
        if (infix[a] == '&' || infix[a] == '|'){
            if (infix[a + 1] == '&' || infix[a + 1] == '|' || infix[a + 1] == ')')
                return 1;
            if (infix[a - 1] == '&' || infix[a - 1] == '|' || infix[a - 1] == '(')
                return 1;
        }
    }
    
    if (infix.size() == 0)
        return 1;
    
    if (infix[0] == ')' || infix[infix.size() - 1] == '(')
        return 1;
    
    int endParen = 0;
    int startParen = 0;
    while (infix.find(')') != string::npos){
        infix = infix.erase(infix.find(')'), 1);
        endParen++;
    }
    if (infix.size() == 0)
        return 1;
    while (infix.find('(') != string::npos){
        infix = infix.erase(infix.find('('), 1);
        startParen++;
    }
    if (endParen != startParen)
        return 1;
    
    return 0;
}

int endCheck(string postfix){
    if (postfix == "")
        return 1;
    
    if (postfix.find('(') != string::npos || postfix.find(')') != string::npos )
        return 1;
    
    int numAssoc = 0;
    int numNums = 0;
    for (int a = 0; a < postfix.size(); a++){
        if (postfix[a] == '&' || postfix[a] == '|')
            numAssoc++;
        else if (postfix[a] != '!')
            numNums++;
    }
    if (numAssoc + 1 != numNums)
        return 1;
    
    return 0;
}

int main()
{
    int isValid = 0;
    cout << "A" << toPostfix("2| 3", isValid) << endl; assert(isValid == 0);
    cout << "B" << toPostfix("8|", isValid) << endl; assert(isValid == 1);
    cout << "C" << toPostfix("4 5", isValid) << endl; assert(isValid == 1);
    cout << "D" << toPostfix("01", isValid) << endl; assert(isValid == 1);
    cout << "E" << toPostfix("()", isValid) << endl; assert(isValid == 1);
    cout << "F" << toPostfix("2(9|8)", isValid) << endl; assert(isValid == 1);
    cout << "G" << toPostfix("2(&8)", isValid) << endl; assert(isValid == 1);
    cout << "H" << toPostfix("(6&(7|7)", isValid) << endl; assert(isValid == 1);
    cout << "I" << toPostfix("", isValid) << endl; assert(isValid == 1);
    cout << "J" << toPostfix("4  |  !3 & (0&3)", isValid) << endl; assert(isValid == 0);
    cout << "K" << toPostfix(" 9  ", isValid) << endl; assert(isValid == 0);
    cout << "L" << toPostfix("((9))", isValid) << endl; assert(isValid == 0);
    
    cout << "M" << toPostfix(")6&(7|7)", isValid) << endl; assert(isValid == 1);
    cout << "N" << toPostfix("6&!5(7|7)", isValid) << endl; assert(isValid == 1);
    cout << "O" << toPostfix("!5", isValid) << endl; assert(isValid == 0);
    cout << "P" << toPostfix("||555", isValid) << endl; assert(isValid == 1);
    cout << "Q" << toPostfix("5||55", isValid) << endl; assert(isValid == 1);
    cout << "R" << toPostfix("!5!5||!5", isValid) << endl; assert(isValid == 1);
    cout << "S" << toPostfix("!5|((!5)|!5)", isValid) << endl; assert(isValid == 0);
    cout << "T" << toPostfix("!5|((!5)|!5)(", isValid) << endl; assert(isValid == 1);
    
    bool ba[10] = {
        //  0      1      2      3      4      5      6      7      8      9
        true,  true,  true,  false, false, false, true,  false, true,  false
    };
    string pf;
    bool answer;

    assert(evaluate("2", ba, pf, answer) == 0 && pf == "2" &&  answer);
    assert(evaluate("3", ba, pf, answer) == 0 && pf == "3" &&  !answer);
    assert(evaluate("!2", ba, pf, answer) == 0 && pf == "2!" &&  !answer);
    assert(evaluate("!3", ba, pf, answer) == 0 && pf == "3!" &&  answer);
    assert(evaluate("2|3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
    assert(evaluate("2&3", ba, pf, answer) == 0 && pf == "23&" &&  !answer);
    assert(evaluate("!2|!3", ba, pf, answer) == 0 && pf == "2!3!|" && answer);
    assert(evaluate("!2&!3", ba, pf, answer) == 0 && pf == "2!3!&" && !answer);
    assert(evaluate("2|3|4", ba, pf, answer) == 0 && pf == "23|4|" && answer);
    assert(evaluate("2&3&4", ba, pf, answer) == 0 && pf == "23&4&" && !answer);
    assert(evaluate("!2|!3|!4", ba, pf, answer) == 0 && pf == "2!3!|4!|" && answer);
    assert(evaluate("!2&!3&!4", ba, pf, answer) == 0 && pf == "2!3!&4!&" && !answer);
    assert(evaluate("!2|!3&!4", ba, pf, answer) == 0 && pf == "2!3!4!&|" && answer);
    assert(evaluate("!7|8", ba, pf, answer) == 0 && pf == "7!8|" && answer);
    assert(evaluate("(5&)6", ba, pf, answer) == 1);
    
    assert(evaluate("a", ba, pf, answer) == 1);
    assert(evaluate(" ", ba, pf, answer) == 1);
    assert(evaluate("", ba, pf, answer) == 1);
    assert(evaluate("(", ba, pf, answer) == 1);
    assert(evaluate(")", ba, pf, answer) == 1);
    assert(evaluate("(8", ba, pf, answer) == 1);
    assert(evaluate("8)", ba, pf, answer) == 1);
    assert(evaluate("8|", ba, pf, answer) == 1);
    assert(evaluate("|", ba, pf, answer) == 1);
    assert(evaluate("&", ba, pf, answer) == 1);
    assert(evaluate("!", ba, pf, answer) == 1);
    assert(evaluate("8|", ba, pf, answer) == 1);
    assert(evaluate("8||2", ba, pf, answer) == 1);
    assert(evaluate("8|||2", ba, pf, answer) == 1);
    assert(evaluate("8|&2", ba, pf, answer) == 1);
    assert(evaluate("1&8|", ba, pf, answer) == 1);
    assert(evaluate("!!!8", ba, pf, answer) == 0);
    
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
    assert(evaluate("8|", ba, pf, answer) == 1);
    assert(evaluate("4 5", ba, pf, answer) == 1);
    assert(evaluate("01", ba, pf, answer) == 1);
    assert(evaluate("()", ba, pf, answer) == 1);
    assert(evaluate("2(9|8)", ba, pf, answer) == 1);
    assert(evaluate("2(&8)", ba, pf, answer) == 1);
    assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
    assert(evaluate("", ba, pf, answer) == 1);
    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
           &&  pf == "43!03&&|"  &&  !answer);
    assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
    ba[2] = false;
    ba[9] = true;
    assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
    //assert(evaluate(" 1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, answer) == 0 && answer);
    
    cout << "Passed all tests" << endl;
    
}





