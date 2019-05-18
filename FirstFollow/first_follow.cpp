#include "first_follow.hpp"
#include <stack>
using namespace std;
vector<producer> pro_list;
vector<First_of_Vn> first_list;
vector<Follow_of_Vn> follow_list;
//输入的终结符列表
vector<char> input_list;
char input_buf[1024];
//输入串.
int  len_of_inputbuf;
//运行分析栈
stack<char> stack1;             

//预测分析表
int **table;
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
/**
 * 往输入符号向量中插入一个输入符号（也就是一个非终结符）
*/
void insert_a_symbol_to_input(char symbol){
    int len = input_list.size();
    bool flag = true;
    if(symbol == '~'){                      //不允许出现'~'
        flag = false;
    }
    for(int i = 0; i < len; i++){
        if(input_list[i] == symbol){
            flag = false;
        }
    }
    if(flag){
        input_list.push_back(symbol);
    }
}

/**
 * 构造输入符号表
*/

void instruct_the_input_list(){
    int len_first = first_list.size();
    char *temp;
    for(int i = 0; i < len_first; i++){
        int len_num_of_list = first_list[i].get_num_of_first();
        temp = first_list[i].get_set();
        for(int j = 0; j < len_num_of_list; j++){
            insert_a_symbol_to_input(temp[j]);
        }
    }
    insert_a_symbol_to_input('$');
}
/**
 * 初始化预测分析表。
 * **/
void init_the_table(){
    int len_pro_list = pro_list.size();
    int len_of_input = input_list.size();
    table = new int * [len_pro_list];
    for(int i = 0; i < len_pro_list; i++){
        table[i] = new int [len_of_input];
    }
    for(int i = 0; i < len_pro_list; i++){
        for(int j = 0; j < len_of_input; j++){
            table[i][j] = -1;
        }
    }
}

/***
 * 获取该符号在input列表中的位置.
 * */
int get_position_in_input(char symbol){
    int len_of_input = input_list.size();
    int position = 0;
    while(position < len_of_input){
        if(input_list[position] == symbol){
            return position;
        }
        position++;
    }
    return -2;
}


/**
 * 根据已经求得的first集合和follow集合求解预测分析表.
 * 将匹配的产生式在right中的位置下标赋值给对应的table的某个元素。
*/
void instruct_the_table(){
    int len_pro_list = pro_list.size();
    for(int i = 0; i < len_pro_list; i++){              //每个非终结符
        int temp_num       = pro_list[i].get_rightnum();
        string* temp_right = pro_list[i].get_right();
        for(int j = 0; j < temp_num; j++){              //每个非终结符拥有的每个产生式。
            string temp_str = temp_right[j];
            int len_of_str  = temp_str.size();
            if(temp_str[0] >= 'a' && temp_str[0] <= 'z'){
                int position = get_position_in_input(temp_str[0]);
                if(-1 == table[i][position])
                    table[i][position] = j;
                else{
                    perror("table instruct error!!");
                    exit(EXIT_FAILURE);
                }
            }else if(temp_str[0] >= 'A' && temp_str[0] <= 'Z'){
                int pos_in_prolist = find_Vn_from_vector(temp_str[0]);
                if(first_list[pos_in_prolist].whether_empty_exists()){
                    // 将first集合中的元素进行插入
                    int num_of_first = first_list[pos_in_prolist].get_num_of_first();
                    char* temp_first = first_list[pos_in_prolist].get_set();
                    for(int k = 0; k < num_of_first; k++){
                        char temp_char = temp_first[k];
                        int temp_position = get_position_in_input(temp_char);
                        if(-2 != temp_position)
                            if(-1 == table[i][temp_position])
                                table[i][temp_position] = j;
                            else{
                                perror("table instruct error!!该文法不是ll1文法");
                                exit(EXIT_FAILURE);
                            }
                        else if('~' != temp_char)
                            cout << "符号:" << temp_char << "不在input_list中" << endl;
                        
                    }
                    // 将follow集合中的每个元素近插入
                    int num_of_follow = follow_list[pos_in_prolist].get_num_of_follow();
                    char* temp_follow = follow_list[pos_in_prolist].get_follow();
                    for(int k = 0; k < num_of_follow; k++){
                        char temp_char = temp_follow[k];
                        int temp_position = get_position_in_input(temp_char);
                        if(-2 != temp_position){
                            if(-1 == table[i][temp_position])
                                table[i][temp_position] = j;
                            else{
                                perror("table instruct error!!该文法不是ll1文法");
                                exit(EXIT_FAILURE);
                            }
                        }else if(temp_char != '~'){
                            cout << "符号:" << temp_char << "不在input_list中" << endl;
                        }
                    }
                }else{
                    //将first集合中的元素进行插入
                    int num_of_first = first_list[pos_in_prolist].get_num_of_first();
                    char* temp_first = first_list[pos_in_prolist].get_set();
                    for(int k = 0; k < num_of_first; k++){
                        char temp_char = temp_first[k];
                        int temp_position = get_position_in_input(temp_char);
                        if(-2 != temp_position)
                            if(-1 == table[i][temp_position])
                                table[i][temp_position] = j;
                            else{
                                perror("table instruct error!!");
                                exit(EXIT_FAILURE);
                            }
                        else if('~' != temp_char)
                            cout << "符号:" << temp_char << "不在input_list中" << endl;                       
                    }
                }
            }
        }
    }
}

void show_input_list(){
    int len = input_list.size();
    for(int i = 0; i < len; i++){
        cout << input_list[i] << "    ";
    }
    cout << endl;
}

void show_table(){
    int len_pro_list   = pro_list.size();
    int len_input_list = input_list.size();
    for(int i = 0; i < len_pro_list; i++){
        for(int j = 0; j < len_input_list; j++){
            cout << table[i][j] << "  ";
        }
        cout << endl;
    }
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
        return -2;
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
                            if(-2 == return_of_follow){
                                follow1.set_follow(follow_list[i].get_follow(), follow_list[i].get_num_of_follow());
                            }else{
                                follow1.insert_one_set(follow_list[i].get_follow(), follow_list[i].get_num_of_follow());
                            }
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
                                    if(-2 == return_of_follow){
                                        follow1.set_follow(follow_list[i].get_follow(), follow_list[i].get_num_of_follow());
                                    }else{
                                        follow1.insert_one_set(follow_list[i].get_follow(),follow1.get_num_of_follow());
                                    }
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

void instruct_the_input_string(){
    cout << "输入一个想要构造的字符串:";
    cin >> input_buf;
    len_of_inputbuf = strlen(input_buf);
    //往输入串的尾端添加$
    input_buf[len_of_inputbuf] = '$';
    len_of_inputbuf++;
}

/***
 * 初始化运行栈。。。
*/
void init_the_stack(){
    stack1.push('$');
    stack1.push(pro_list[0].get_left());
}

void analyze_from_left(){
    int count = 0;
    char temp_stack = stack1.top();
    while(temp_stack != '$'){
        if(temp_stack == input_buf[count]){
            stack1.pop();
            count++;
        }else if(temp_stack >= 'a' && temp_stack <= 'z'){
            perror("分析过程出错!!");
            exit(EXIT_FAILURE);
        }else{
            int row_pos = find_Vn_from_vector(temp_stack);
            int col_pos = get_position_in_input(input_buf[count]);
            if(-1 == table[row_pos][col_pos]){
                perror("该文法不是ll1文法。。");
                exit(EXIT_FAILURE);
            }else{
                int temp = table[row_pos][col_pos];
                string * right_temp = pro_list[row_pos].get_right();
                cout << pro_list[row_pos].get_left() << "->" << right_temp[temp] << endl;
                stack1.pop();
                //压栈相关表达式。。。。
                string temp_str = right_temp[temp];
                int len = temp_str.size();
                for(int i = len - 1; i >= 0; i--){
                    stack1.push(temp_str[i]);
                }
            }
        }
        temp_stack = stack1.top();
    }
}

int main(int argc, char * argv[]){
    list_init();
    int length = pro_list.size();
    for(int i = 0; i < length; i++){
        First(pro_list[i], first_list[i]);
    }
    show_first_vector();
    //update 和求解 follow 不能放在一个循环中完成。否则会出现更新不完全的情况。。
    for(int i = 0; i < length; i++){
        front_Vn = -1;
        Follow(follow_list[i]);
        userd[i] = -1;
    }
    for(int i = 0; i < length; i++){
        follow_list[i].update_the_follow();
    }
    show_follow_vector();
    instruct_the_input_list();
    init_the_table();
    instruct_the_table();
    //show_input_list();
    //show_table();
    instruct_the_input_string();
    init_the_stack();
    analyze_from_left();
    return 0;
}