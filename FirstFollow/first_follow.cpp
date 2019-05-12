#include "first_follow.hpp"
#include <list>

using namespace std;
vector<producer> pro_list;
vector<First_of_Vn> first_list;

/**
 * 从pro_list 和 first_list中查找某个字符对应的producer和first的下标,
 * 从而方便递归操作。
 * 查找成功返回值元素下标，查找失败返回-1.
*/
int find_Vn_from_vector(char Vn){       
    int len = pro_list.size();
    for(int i = 0; i < len; i++){
        char temp_str = pro_list[i].get_left();
        if(temp_str == Vn){
            return i;
        }
    }
    return -1;
}


void splitString(string &s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
         
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}


void list_init(){
    int num_of_list = 0;
    cout << "接下来按照输入初始化list" << endl;
    cout << "输入要构建的producer的个数:" << endl;
    cin >> num_of_list;
    cout << "输入产生式('~'表示空集):" << endl;
    for(int i = 0; i < num_of_list; i++){
        char left;
        int num_of_right = 1;
        string temp_input;
        vector<string> temp_right;
        cin >> temp_input;
        left = temp_input[0];
        int length = temp_input.length();
        temp_input = temp_input.substr(3,length);
        splitString(temp_input, temp_right, "|");
        producer    temp_pro(left, temp_right);
        First_of_Vn temp_first(left, NULL, 0);
        pro_list.push_back(temp_pro);
        first_list.push_back(temp_first);
    }
}

void show_pro_vector(){
    cout << "展示变量:" << endl;
    int length = pro_list.size();
    for(int i = 0; i < length; i++){
        pro_list[i].show();
    }
}

void show_first_vector(){
    cout << "展示first集合:" << endl;
    int length = first_list.size();
    for(int i = 0; i < length; i++){
        first_list[i].show();
    }
}

void First(producer & pro1, First_of_Vn& first1){
    if(pro1.get_left() != first1.get_Vn()){
        perror("函数调用异常!!!产生式的左部与需要传入的first集的Vn不相等!!");
        exit(EXIT_FAILURE);
    }
    char temp_Vn = pro1.get_left();
    string*    temp_set = pro1.get_right();
    int      num_of_set = pro1.get_rightnum();
    for(int i = 0; i < num_of_set; i++){
        string temp_str = temp_set[i];
        int    len      = temp_str.length();
        bool   flag_ex  = true;                                                     //判断求得的子first集中是否有空集，如果有则退出循环。
        if((temp_str[0] >= 'a' && temp_str[0] <= 'z') | (temp_str[0] == '~')){
            first1.insert_one_symbol(temp_str[0]);
        }else if((temp_str[0] >= 'A' && temp_str[0] <= 'Z')){
            for(int i = 0; i < len; i++){
                char temp_char = temp_str[i];
                int  fix       = find_Vn_from_vector(temp_char);
                if(-1 == fix){
                    perror("查找list失败,不执行递归操作!!");
                }else{
                    First(pro_list[i], first_list[i]);
                    flag_ex = first_list[i].whether_empty_exists();                    //求得的子first集合中存在空集，因此继续取下一个字符。
                    if(!flag_ex){
                        break;
                    }
                }
            }
        }
    }
}

int main(int argc, char * argv[]){
    list_init();
    int length = pro_list.size();
    for(int i = 0; i < length; i++){
        First(pro_list[i], first_list[i]);
    }
    show_pro_vector();
    show_first_vector();
    return 0;
}