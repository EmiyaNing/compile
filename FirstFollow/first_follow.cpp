#include "first_follow.hpp"
#include <list>

using namespace std;
vector<producer> pro_list;
vector<First_of_Vn> first_list;
vector<Follow_of_Vn> follow_list;
int front_Vn = -1;
int * userd;                       
//记录上一层求解的是哪一个follow集，防止出现求follow(S)的时候，要先求follow(A)
//然后程序发现要求follow(A)得先求出follow(S)，从而出现了无限递归的情况.

/**
 * 从pro_list 和 first_list中查找某个字符对应的producer和first的下标,
 * 从而方便递归操作。
 * 查找成功返回值元素下标，查找失败返回-1.
*/
int find_Vn_from_vector(char Vn){       
    int len = pro_list.size();
    for(int i = 0; i < len; i++){
        char temp_char = pro_list[i].get_left();
        if(temp_char == Vn){
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
    userd = new int[num_of_list];
    for(int i = 0; i < num_of_list; i++){
        userd[i] = 1;
    }
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
        producer     temp_pro(left, temp_right);
        First_of_Vn  temp_first(left, NULL, 0);
        Follow_of_Vn temp_follow(left, NULL, 0);
        pro_list.push_back(temp_pro);
        first_list.push_back(temp_first);
        follow_list.push_back(temp_follow);
    }
    follow_list[0].insert_one_symbol('$');
}

void show_pro_vector(){
    cout << "展示输入的产生式:" << endl;
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

void show_follow_vector(){
    cout << "展示follow集合:" << endl;
    int length = follow_list.size();
    for(int i = 0; i < length; i++){
        follow_list[i].show();
    }
    delete [] userd;
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
        bool   flag_ex  = false;                                                     //判断求得的子first集中是否有空集，如果有则退出循环。
        if((temp_str[0] >= 'a' && temp_str[0] <= 'z') | (temp_str[0] == '~')){
            first1.insert_one_symbol(temp_str[0]);
            //cout << "添加一个符号:" << temp_str[0] << endl;
        }else if((temp_str[0] >= 'A' && temp_str[0] <= 'Z')){
            for(int i = 0; i < len; i++){
                char temp_char = temp_str[i];
                int  fix       = find_Vn_from_vector(temp_char);
                if(-1 == fix){
                    perror("查找list失败,不执行递归操作!!");
                }else{
                    First(pro_list[fix], first_list[fix]);
                    first1.insert_one_set(first_list[fix].get_set(), first_list[fix].get_num_of_first());
                    flag_ex = !(first_list[fix].whether_empty_exists());                    //求得的子first集合中存在空集，因此继续取下一个字符。
                    if(flag_ex){
                        break;
                    }
                }
            }
        }
    }
}

int Follow(Follow_of_Vn& follow1){
    char Vn         = follow1.get_Vn();
    int  len_of_pro = pro_list.size();
    int  fix_ed     = find_Vn_from_vector(Vn);
    if(-1 == userd[fix_ed]){
        return -1;
    }
    for(int i = 0; i < len_of_pro; i++){
        int num_of_right = pro_list[i].get_rightnum();
        string * right     = pro_list[i].get_right();
        for(int j = 0; j < num_of_right; j++){
            int len_of_str  = right[j].length();
            string temp_str = right[j];
            for(int r = 0; r < len_of_str; r++){
                if(temp_str[r] == Vn){
                    if('\0' == temp_str[r+1]){
                        if(i != front_Vn){
                            front_Vn = fix_ed;
                            int return_of_follow = Follow(follow_list[i]);
                            //if(-1 == return_of_follow){
                                follow1.insert_one_set(follow_list[i].get_follow(), follow_list[i].get_num_of_follow());
                                //若A->aB 或A->aBc,且 c->* ~（即~ 属于FIRST(c)）,则将 FOLLOW(A)加入FOLLOW(B)中（此处a可以为空）。
                            //}
                        }else{
                            return -1;
                        }  
                    }else if(temp_str[r+1] >= 'a' && temp_str[r+1] <= 'z'){
                        follow1.insert_one_symbol(temp_str[r+1]);
                    }else if(temp_str[r+1] >= 'A' && temp_str[r+1] <= 'Z'){
                        int fix = find_Vn_from_vector(temp_str[r+1]);
                        if(-1 != fix){
                            follow1.insert_one_set(first_list[fix].get_set(), first_list[fix].get_num_of_first());
                            bool flag_empty = first_list[fix].whether_empty_exists();
                            if(flag_empty){
                                if(i != front_Vn){
                                    front_Vn = fix_ed;
                                    int return_of_follow = Follow(follow_list[i]);
                                    if(-1 == return_of_follow)
                                        follow1.insert_one_set(follow_list[i].get_follow(),follow1.get_num_of_follow());
                                        //若A->aB 或A->aBc,且 c->* ~（即~ 属于FIRST(c)）,则将 FOLLOW(A)加入FOLLOW(B)中（此处a可以为空）。
                                }else{
                                    return -1;
                                }
                            }
                        }
                        
                    }

                    
                }
            }
        }
    }
    return 0;
}


int main(int argc, char * argv[]){
    list_init();
    int length = pro_list.size();
    for(int i = 0; i < length; i++){
        First(pro_list[i], first_list[i]);
    }
    show_first_vector();
    for(int i = 0; i < length; i++){
        front_Vn = -1;
        Follow(follow_list[i]);
        userd[i] = -1;
    }
    show_follow_vector();
    return 0;
}