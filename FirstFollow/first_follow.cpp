#include "first_follow.hpp"
#include <list>

using namespace std;
vector<producer> pro_list;
vector<First_of_Vn> first_list;

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
        producer temp_pro(left, temp_right);
        first    temp_first(left, NULL, 0);
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
        int    len      = strlen(temp_str);
        if((temp_str[0] >= 'a' && temp_str[0] <= 'z') | (temp_str[0] == '~')){
            first1.insert_one_symbol(temp_str[0]);
        }
    }
}

int main(int argc, char * argv[]){
    list_init();
    show_pro_vector();
    return 0;
}