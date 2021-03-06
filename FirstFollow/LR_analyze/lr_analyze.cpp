#include <iostream>
#include <vector>
#include <iomanip>
#include <cstring>
#include <sstream>
#include "LR.hpp"
#include <stack>
 
using namespace std;
 
//定义状态栈
vector<int> status;
//定义符号栈
vector<char> sign;
//定义输入的字符串
vector<char> inputStr;

vector<int> value;
stack<int> value_stack;
//定义文法
Grammar grammar;
//定义LR分析表
LRAnalyseTable analyseTable;
//读取输入的字符串
void readStr();
//对栈容器进行输出,i=0,返回status中的字符串,i=1,返回sign中的字符串，i=2返回inputStr
string vectTrancStr(int i);
//总控，对输入的字符串进行分析
void LRAnalyse();
 
int main()
{
    readStr();
    LRAnalyse();
    return 0;
}

int caculate_the_value(int number,int popnumber){
    int result = 0;
    int temp_num1 = 0;
    int temp_num2 = 0;
    if(number == 1){
        //result = value[top] + value[top+2];
        temp_num1 = value_stack.top();
        value_stack.pop();
        value_stack.pop();
        temp_num2 = value_stack.top();
        value_stack.pop();
        result = temp_num1 + temp_num2;
    }else if(number == 2){
        //result = value[top];
        result = value_stack.top();
        value_stack.pop();
    }else if(number == 3){
        temp_num1 = value_stack.top();
        value_stack.pop();
        value_stack.pop();
        temp_num2 = value_stack.top();
        value_stack.pop();
        result = temp_num1 * temp_num2;
        //result = value[top] * value[top + 2];
    }else if(number == 4){
        //result = value[top];
        result = value_stack.top();
        value_stack.pop();
    }else if(number == 5){
        //result = value[top + 1];
        result = value_stack.top();
        value_stack.pop();
    }else{
        //result = value[top];
        result = value_stack.top();
        value_stack.pop();
    }
    return result;
}


//对栈容器进行输出,i=0,返回status中的字符串,i=1,返回sign中的字符串，i=2返回inputStr中的字符串, i=3返回value中的字符串
string vectTrancStr(int i){
    char buf[100];
    int count = 0;
    //输出状态栈
    if(i == 0){
        vector<int>::iterator it =status.begin();
        //将数字转化为字符串
        string str,tempStr;
        for(it;it!= status.end();it++){
            stringstream ss;
            ss << *it;
            ss >> tempStr;
            str+=tempStr;
        }
        return str;
    }
    //输出符号栈
    else if(i == 1){
        vector<char>::iterator it = sign.begin();
        for(it ; it != sign.end() ;it++){
            buf[count] = *it;
            count++;
        }
    }
    //输出待分析的字符串
    else if(i == 2){
        vector<char>::iterator it = inputStr.begin();
        for(it ; it != inputStr.end();it++){
            buf[count] = *it;
            count++;
        }
    }
    else if(i == 3){
        vector<int>::iterator it = value.begin();
        string str,tempStr;
        for(it;it!= value.end();it++){
            stringstream ss;
            ss << *it;
            ss >> tempStr;
            str+=tempStr;
        }
        return str;
    }
    buf[count] = '\0';
    string str(buf);
    return str;
}

/***
 * 将该函数改造成能够处理带数字的串。。。
 * */
void readStr(){
    char ch;
    cout<<"请输入分析的字符串：";
    cin>>ch;
    while( ch != '$'){
        if(ch >= '0' && ch <= '9'){
            inputStr.push_back('i');
            value.push_back(ch - 48);
        }else if(ch == '+'){
            inputStr.push_back(ch);
            value.push_back(0);
        }else if(ch == '*'){
            inputStr.push_back(ch);
            value.push_back(0);
        }
        cin>>ch;
    }
   //把#加入容器
   inputStr.push_back('$');
   cout << vectTrancStr(3) << endl;
}
//总控，对输入的字符串进行分析
void LRAnalyse(){
    //步骤
    int step = 1;
    //把状态0入栈
    status.push_back(0);
    //把$加入符号栈
    sign.push_back('$');
    //输出初始栈状态,setw表示该流符号输出的字符串长度。
    cout<<setw(10)<<"步骤"<<setw(10)<<"状态栈"<<setw(10)<<"符号栈"<<setw(10)<<"值数组"<<setw(10)<<"输入串"<<setw(25)<<"动作说明"<<endl;
    //初始状态
    int s =0;
    //保存之前的状态
    int oldStatus;
    //获取初始符号
    char ch = inputStr.front();
    //如果action[s][ch] =="acc" ，则分析成功
    while(analyseTable.action[s][analyseTable.getTerminalIndex(ch)] != "acc"){
        //获取字符串
        string str = analyseTable.action[s][analyseTable.getTerminalIndex(ch)];
        //如果str为空，报错并返回
        if(str.size() == 0){
            cout<<"出错";
            return ;
        }
        //获取r或s后面的数字
        stringstream ss;
        ss << str.substr(1);
        ss >> s;
        //如果是移进
        if(str.substr(0,1) == "s"){
            cout<<setw(10)<<step<<setw(10)<<vectTrancStr(0)<<setw(10)<<vectTrancStr(1)<<setw(10)<<vectTrancStr(3)<<setw(10)<<vectTrancStr(2)<<setw(10)<<"A"<<"CTION["<<status.back()<<","<<ch<<"]=S"<<s<<","<<"状态"<<s<<"入栈"<<endl;
            //输入符号入栈
            sign.push_back(ch);
            inputStr.erase(inputStr.begin());
            //将状态数字入栈
            status.push_back(s);
            value_stack.push(value[0]);
            value.erase(value.begin());
        }
        //如果是归约
        else if(str.substr(0,1) == "r"){
            //获取第S个产生式
            string formu = grammar.formula[s];
            int strSize = formu.size();
            //将产生式转化为字符数组
            char buf[100];
            strcpy(buf,formu.c_str());
            //获取产生式的首字符
            char nonTerCh = buf[0];
            //获取符号栈的出栈次数,该类中产生式是带有前缀的,而前缀符号长度正好是3
            int popCount = strSize - 3;
            //反向迭代
            vector<int>::reverse_iterator rit = status.rbegin();
            int i= 0;
            for(rit;rit != status.rend();rit++){
                i++;
                if(i == popCount+1){
                    //获取出栈操作完成之后的状态栈的栈顶值。
                    oldStatus = * rit;
                    break;
                }
            }
            //进行规约操作的表达式的编号。。。
            int r = s;
            //查找goto之后得到的需要压入状态栈的状态。。。
            s = analyseTable.goTo[oldStatus][analyseTable.getNonTerminalIndex(nonTerCh)];
            cout<<setw(10)<<step<<setw(10)<<vectTrancStr(0)<<setw(10)<<vectTrancStr(1)<<setw(10)<<vectTrancStr(3)<<setw(10)<<vectTrancStr(2)<<setw(10)<<"r"<<r<<(string)":"+grammar.formula[r]+(string)"归约,GOTO{"<<oldStatus<<","<<nonTerCh<<"}="<<s<<"入栈"<<endl;
            int result = caculate_the_value(r,popCount);
            //对符号栈进行出栈和状态栈进行出栈
            for(int i=0 ;i< popCount;i++){
                sign.pop_back();
                status.pop_back();
                //value_stack.pop();
            }
            //再对产生式的开始符号入栈
            sign.push_back(nonTerCh);
            //再把新的状态入栈
            status.push_back(s);
            //在把新的结果入栈
            //value.insert(value.begin(),result);
            value_stack.push(result);
            
        }
        else{
           //什么都不处理
        }
        //步骤数加1
        step++;
 
        //获取栈顶状态
        s = status.back();
        //获取输入的字符
        ch = inputStr.front();
    }
    cout<<setw(10)<<step<<setw(10)<<vectTrancStr(0)<<setw(10)<<vectTrancStr(1)<<setw(10)<<vectTrancStr(3)<<setw(10)<<vectTrancStr(2)<<setw(10)<<"A"<<"cc:分析成功"<<endl;
    cout<<"分析结果是:"<<setw(10)<<value_stack.top()<<endl;
}