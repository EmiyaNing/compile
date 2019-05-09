#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
#define Bufsize 50
using namespace std;
class producer{
    private:
        char    left_Vn;
        string  right[Bufsize];
        int     num_of_right;

    public:
        producer(char left_Vn, vector<string> right){
            this->left_Vn      = left_Vn;
            this->num_of_right = right.size();
            for(int i = 0; i < this->num_of_right; i++){
                this->right[i] = right[i];
            }
        }

        int get_rightnum(){
            return this->num_of_right;
        }

        void insert_into_producer(string pro_right){
            this->right[num_of_right] = pro_right;
            this->num_of_right++;
        }
        char get_left(){
            return this->left_Vn;
        }
        string * get_right(){
            return this->right;
        }
        void set_left(char left){
            this->left_Vn = left;
        }
        void set_right(vector<string> right){
            this->num_of_right = right.size();
            for(int i = 0; i < this->num_of_right; i++){
                this->right[i] = right[i];
            }
        }
        void show(){
            cout << "该产生是的左部是:" << this->left_Vn << endl;
            cout << "该产生式右部包含:" << endl;
            for(int i = 0; i < this->num_of_right; i++){
                cout << right[i] << endl;
            }
        }
};

class First_of_Vn{
    private:
        char Vn;
        char first[Bufsize];
        int num_of_first;
    public:
        First_of_Vn(char Vn, char *first, int num_of_first){
            this->Vn = Vn;
            this->num_of_first = num_of_first;
            for(int i = 0; i < num_of_first; i++){
                this->first[i] = first[i];
            }
        }
        /***
         * 往first集中插入一个终结符，首先检查是否重复。
         * 然后再插入.
         * */
        void insert_one_symbol(char symbol){
            bool flag = true;
            for(int i = 0; i < num_of_first; i++){
                if(symbol == this->first[i]){
                    flag = false;
                }
            }
            if(flag){
                first[num_of_first] = symbol;
                num_of_first++;
            }
        }
        /***
         * 插入一个集合,在插入之前先判断该符号是否已在集合中.
         * */
        void insert_one_set(char * set, int num_of_set){
            for(int i = 0; i < num_of_set; i++){
                this.insert_one_symbol(set[i]);
            }
        }

        bool whether_empty_exists(){
            bool flag = false;
            for(int i = 0; i < num_of_first; i++){
                if('~' == first[i]){
                    flag = true;
                }
            }
            return flag;
        }

        char get_Vn(){
            return this->Vn;
        }

        void set_Vn(char * Vn){
            this->Vn = Vn;
        }

        void set_first(char * first, int num_of_first){
            this->num_of_first = num_of_first;
            for(int i = 0; i < num_of_first; i++){
                this->first[i] = first[i];
            }
        }

        void show(){
            cout << "非终结符为:" << this->Vn << endl;
            cout << "First内容为:" << endl;
            for(int i = 0 ; i < this->num_of_first; i++){
                cout << first[i] << "\r";
            }
            cout << endl;
        }
};