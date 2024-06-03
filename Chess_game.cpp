#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;

class Field; //за дефиниране на клас Field 

struct Point{ //структура за индексите не полетата
    int x, y;
    bool operator==(const Point& A){
      if(x == A.x && y == A.y) return true;
      else return false;
    }

    friend ostream& operator<<(ostream& out, const Point& a){ // изкарва ги като позиции в шаха (а1, а7, f4)
      Point print = a;
      print.x = 8 - print.x;
      out << char(print.y + 97) << char(print.x + 48);
      return out;
    }
    friend istream& operator>>(istream& in, Point& a){ //изисква да се въведат като позиции в шаха, за save използва се накрая
      char t, y;
      string temp;
      bool stopper = 0;
      while(stopper == 0){
        in >> temp;
        if(temp.length() == 2){
          t = temp.at(0);
          y = temp.at(1);
          if(t >= 'a' && t <= 'h' && y >= '1' && y <= '8') stopper = 1;
          else cout << "Invalid square! " << endl;
        }
        else{
          if(temp.length() == 4 && temp == "save"){
            a.x = -1;
            a.y = -1;
            return in;
          }
          cout << "Invalid square! " << endl;
        }
      }
      a.x = int(y - 48);
      a.y = int(t - 97);
      a.x = 8 - a.x;
      return in;
    }
};

class Figure{ // клас от тип фигура
  private:
    Point coordinate;
    bool color;
  public:
    Figure(){}
    Figure(Point co, bool col){coordinate = co; color = col;}
    Figure(const Figure& A){this->color = A.color; this->coordinate = A.coordinate;}
    Figure& operator=(const Figure& A){this->color = A.color; this->coordinate = A.coordinate; return *this;}
    ~Figure(){}
    virtual string get_type() = 0;
    virtual Point get_coordinates() const {cout << "sec ";Point p; p.x = -1, p.y = -1; return p;}
    virtual bool get_color()const{throw -1;} // 0 e за бяла, 1 за черна фигура
    virtual int get_number_of_position_to_go(Field*** a) const{cout << "da eba    " ;throw 'g';} //броят на позиции
    virtual Point* get_field_coordinates_to_go(const int& n, Field*** a) const{Point* p = new Point[0]; return p;} //координатите на позициите на които може да ходи
    virtual bool set_coordinates(const Point& a){throw '1'; return true;}
};//пойнтър към класа фигура, останалите класове фигури ще наследят този клас





class Field{ //представлява полетата, в които има пойнтър към фигура, ако на това поле няма фигура, то = Nullprt
  private:
    Point coordinates;
    Figure* on_field_figure;
  public:

    Field(int a, int b){
      coordinates.x = a;
      coordinates.y = b;
      on_field_figure = nullptr;
    }

    Field(const Field& A){
      this->coordinates = A.coordinates;
      set_new_figure(A.get_field_figure());
    }

    Field& operator=(const Field& A){
      if(this != &A){
        this->coordinates = A.coordinates;
        set_new_figure(A.get_field_figure());
      }
      return *this;
    }

    ~Field(){
      delete on_field_figure;
    }

    string get_figure_type(){ // типът не фигурата
      if(on_field_figure == nullptr) return "";
      return on_field_figure->get_type();
    }

    int get_field_figure_color()const{// цветът на фигурата
      if(on_field_figure == nullptr) {
        return -1;
        }
      bool temp = on_field_figure->get_color();
      if(temp == 0 || temp == 1) return temp;
      else return -1;
    }

    Figure* get_field_figure()const{ //достъп до фигурата на полето
      return on_field_figure;
    }

    bool check_if_figure_or_field_is_protected_or_attacked(Field*** &board, bool colorce){ //проверява дали полето е атакувано от другия цвят или фигурата(от другия цвят) е защитена
      Figure* temp;
      if(!colorce == 0){
        if(coordinates.x + 1 < 8){
          if(coordinates.y + 1 < 8){
            temp = board[coordinates.x + 1][coordinates.y + 1]->get_field_figure();
            if(temp != nullptr){
              if(temp->get_type() == "pawn" && 
                 temp->get_color() == 0){return 1;}
            }
          }
          if(coordinates.y - 1 >= 0){
            temp = board[coordinates.x + 1][coordinates.y - 1]->get_field_figure();
            if(temp != nullptr){
              if(temp->get_type() == "pawn" &&
                 temp->get_color() == 0){return 1;}
            }
          }
        }
      }
      if(!colorce == 1){
        if(coordinates.x - 1 >= 0){
          if(coordinates.y - 1 >= 0){
            temp = board[coordinates.x - 1][coordinates.y - 1]->get_field_figure();
            if(temp != nullptr){
              if(temp->get_type() == "pawn" && 
                 temp->get_color() == 1){return 1;}
            }
          }
          if(coordinates.y + 1 < 8){
            temp = board[coordinates.x - 1][coordinates.y + 1]->get_field_figure();
            if(temp != nullptr){
              if(temp->get_type() == "pawn" &&
                 temp->get_color() == 1){return 1;}
            }
          }
        }
      }

        if(coordinates.x + 2 < 8 && coordinates.y + 1 < 8){
          temp = board[coordinates.x + 2][coordinates.y + 1]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce &&
               temp->get_type() == "night") return 1;
          }
        }
        if(coordinates.x + 2 < 8 && coordinates.y - 1 >= 0){
          temp = board[coordinates.x + 2][coordinates.y - 1]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce &&
              temp->get_type() == "night") return 1;
          }
        }
        if(coordinates.x - 2 >= 0 && coordinates.y + 1 < 8){
          temp = board[coordinates.x - 2][coordinates.y + 1]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce &&
              temp->get_type() == "night") return 1;
          }
        }
        if(coordinates.x - 2 >= 0 && coordinates.y - 1 >= 0){
          temp = board[coordinates.x - 2][coordinates.y - 1]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce &&
              temp->get_type() == "night") return 1;
          }
        }
        if(coordinates.x + 1 < 8 && coordinates.y + 2 < 8){
          temp = board[coordinates.x + 1][coordinates.y + 2]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce &&
              temp->get_type() == "night") return 1;
          }
        }
        if(coordinates.x - 1 >= 0 && coordinates.y + 2 < 8){
          temp = board[coordinates.x - 1][coordinates.y + 2]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce &&
              temp->get_type() == "night") return 1;
          }
        }
        if(coordinates.x + 1 < 8 && coordinates.y - 2 >= 0){
          temp = board[coordinates.x + 1][coordinates.y - 2]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce &&
              temp->get_type() == "night") return 1;
          }
        }
        if(coordinates.x - 1 >= 0 && coordinates.y - 2 >= 0){
          temp = board[coordinates.x - 1][coordinates.y - 2]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce &&
              temp->get_type() == "night") return 1;
          }
        }

        if(coordinates.x + 1 < 8){
          temp = board[coordinates.x + 1][coordinates.y]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce && temp->get_type() == "king") return 1;
          }
        }
        if(coordinates.x - 1 >= 0){
          temp = board[coordinates.x - 1][coordinates.y]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce && temp->get_type() == "king") return 1;
          }
        }
        if(coordinates.y + 1 < 8){
          temp = board[coordinates.x][coordinates.y + 1]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce && temp->get_type() == "king") return 1;
          }
        }
        if(coordinates.y - 1 >= 0){
          temp = board[coordinates.x][coordinates.y - 1]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce && temp->get_type() == "king") return 1;
          }
        }
        if(coordinates.x + 1 < 8 && coordinates.y + 1 < 8){
          temp = board[coordinates.x + 1][coordinates.y + 1]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce && temp->get_type() == "king") return 1;
          }
        }
        if(coordinates.x + 1 < 8 && coordinates.y - 1 >= 0){
          temp = board[coordinates.x + 1][coordinates.y - 1]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce && temp->get_type() == "king") return 1;
          }
        }
        if(coordinates.x - 1 >= 0 && coordinates.y + 1 < 8){
          temp = board[coordinates.x - 1][coordinates.y + 1]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce && temp->get_type() == "king") return 1;
          }
        }
        if(coordinates.x - 1 >= 0 && coordinates.y - 1 >= 0){
          temp = board[coordinates.x - 1][coordinates.y - 1]->get_field_figure();
          if(temp != nullptr){
            if(temp->get_color() == !colorce && temp->get_type() == "king") return 1;
          }
        }

        bool flag[8];
        for(int i = 0; i < 8; i++){
          flag[i] = 0;
        }
        string tpe_of_figure;
        for(int i = 1; i < 8; i++){
          
          if(coordinates.x + i < 8){
            temp = board[coordinates.x + i][coordinates.y]->get_field_figure();
            if(flag[0] == 0 && temp != nullptr){
              flag[0] = 1;
              tpe_of_figure = temp->get_type();
              if(tpe_of_figure == "queen" || tpe_of_figure == "rook"){
                  if(temp->get_color() == !colorce) return 1;
                 }
            }
          }
          if(coordinates.x - i >= 0){
            temp = board[coordinates.x - i][coordinates.y]->get_field_figure();
            if(flag[1] == 0 && temp != nullptr){
              flag[1] = 1;
              tpe_of_figure = temp->get_type();
              if(tpe_of_figure == "queen" || tpe_of_figure == "rook"){
                  if(temp->get_color() == !colorce) return 1;
                 }
            }
          }
          if(coordinates.y + i < 8){
            temp = board[coordinates.x][coordinates.y + i]->get_field_figure();
            if(flag[2] == 0 && temp != nullptr){
              flag[2] = 1;
              tpe_of_figure = temp->get_type();
              if(tpe_of_figure == "queen" || tpe_of_figure == "rook"){
                  if(temp->get_color() == !colorce) return 1;
                 }
            }
          }
          if(coordinates.y - i >= 0){
            temp = board[coordinates.x][coordinates.y - i]->get_field_figure();
            if(flag[3] == 0 && temp != nullptr){
              flag[3] = 1;
              tpe_of_figure = temp->get_type();
              if(tpe_of_figure == "queen" || tpe_of_figure == "rook"){
                  if(temp->get_color() == !colorce) return 1;
                 }
            }
          }
          if(coordinates.x + i < 8 && coordinates.y + i < 8){
            temp = board[coordinates.x + i][coordinates.y + i]->get_field_figure();
            if(flag[4] == 0 && temp != nullptr){
              flag[4] = 1;
              tpe_of_figure = temp->get_type();
              if(tpe_of_figure == "queen" || tpe_of_figure == "bishop"){
                  if(temp->get_color() == !colorce) return 1;
                 }
            }
          }
          if(coordinates.x + i < 8 && coordinates.y - i >= 0){
            temp = board[coordinates.x + i][coordinates.y - i]->get_field_figure();
            if(flag[5] == 0 && temp != nullptr){
              flag[5] = 1;
              tpe_of_figure = temp->get_type();
              if(tpe_of_figure == "queen" || tpe_of_figure == "bishop"){
                  if(temp->get_color() == !colorce) return 1;
                 }
            }
          }
          if(coordinates.x - i >= 0 && coordinates.y + i < 8){
            temp = board[coordinates.x - i][coordinates.y + i]->get_field_figure();
            if(flag[6] == 0 && temp != nullptr){
              flag[6] = 1;
              tpe_of_figure = temp->get_type();
              if(tpe_of_figure == "queen" || tpe_of_figure == "bishop"){
                  if(temp->get_color() == !colorce) return 1;
                 }
            }
          }
          if(coordinates.x - i >= 0 && coordinates.y - i >= 0){
            temp = board[coordinates.x - i][coordinates.y - i]->get_field_figure();
            if(flag[7] == 0 && temp != nullptr){
              flag[7] = 1;
              tpe_of_figure = temp->get_type();
              if(tpe_of_figure == "queen" || tpe_of_figure == "bishop"){
                  if(temp->get_color() == !colorce) return 1;
                 }
            }
          }
        }
        return 0;
    }

    bool del_figure(){ //изтрива накъде сочи пойнтъра и го насочва към nullptr
      delete this->on_field_figure;
      this->on_field_figure = nullptr;
      return 1;
    }
    //методи за променяне на фигурата на дадено поле
    bool set_new_figure(Figure*);
    bool set_a_figure(Figure*);
    bool set_no_figure();
    bool set_rook(bool);
    bool set_bishop(bool);
    bool set_night(bool); 
    bool set_queen(bool);
    bool set_king(bool);
    bool set_pawn(bool);
    

};//двумерен масив от пойнтъри за дъската

bool check_if_WHITE_king_is_attacked(Field*** &board){ // проверява дали белият цар е атакуван
  Point king_posiotion;
  Figure *temp;
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      temp = board[i][j]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 0 && temp->get_type() == "king"){
          king_posiotion = temp->get_coordinates();
          break;
        }
      }
    }
  }
    if(king_posiotion.x - 1 >= 0){
      if(king_posiotion.y - 1 >= 0){
        temp = board[king_posiotion.x - 1][king_posiotion.y - 1]->get_field_figure();
        if(temp != nullptr){
          if(temp->get_type() == "pawn" && 
              temp->get_color() == 1){return 1;}
        }
      }
      if(king_posiotion.y + 1 < 8){
        temp = board[king_posiotion.x - 1][king_posiotion.y + 1]->get_field_figure();
        if(temp != nullptr){
          if(temp->get_type() == "pawn" &&
              temp->get_color() == 1){return 1;}
        }
      }
    }

    if(king_posiotion.x + 2 < 8 && king_posiotion.y + 1 < 8){
      temp = board[king_posiotion.x + 2][king_posiotion.y + 1]->get_field_figure();
      if(temp != nullptr){ 
        if(temp->get_color() == 1 &&
            temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x + 2 < 8 && king_posiotion.y - 1 >= 0){
      temp = board[king_posiotion.x + 2][king_posiotion.y - 1]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 1 &&
          temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x - 2 >= 0 && king_posiotion.y + 1 < 8){
      temp = board[king_posiotion.x - 2][king_posiotion.y + 1]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 1 &&
          temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x - 2 >= 0 && king_posiotion.y - 1 >= 0){
      temp = board[king_posiotion.x - 2][king_posiotion.y - 1]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 1 &&
          temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x + 1 < 8 && king_posiotion.y + 2 < 8){
      temp = board[king_posiotion.x + 1][king_posiotion.y + 2]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 1 &&
          temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x - 1 >= 0 && king_posiotion.y + 2 < 8){
      temp = board[king_posiotion.x - 1][king_posiotion.y + 2]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 1 &&
          temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x + 1 < 8 && king_posiotion.y - 2 >= 0){
      temp = board[king_posiotion.x + 1][king_posiotion.y - 2]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 1 &&
          temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x - 1 >= 0 && king_posiotion.y - 2 >= 0){
      temp = board[king_posiotion.x - 1][king_posiotion.y - 2]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 1 &&
          temp->get_type() == "night") return 1;
      }
    }

    bool flag[8];
    for(int i = 0; i < 8; i++){
      flag[i] = 0;
    }
    string tpe_of_figure;
    for(int i = 1; i < 8; i++){
      if(king_posiotion.x + i < 8){
        temp = board[king_posiotion.x + i][king_posiotion.y]->get_field_figure();
        if(flag[0] == 0 && temp != nullptr){
          flag[0] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "rook"){
              if(temp->get_color() == 1) return 1;
              }
        }
      }
      if(king_posiotion.x - i >= 0){
        temp = board[king_posiotion.x - i][king_posiotion.y]->get_field_figure();
        if(flag[1] == 0 && temp != nullptr){
          flag[1] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "rook"){
              if(temp->get_color() == 1) return 1;
              }
        }
      }
      if(king_posiotion.y + i < 8){
        temp = board[king_posiotion.x][king_posiotion.y + i]->get_field_figure();
        if(flag[2] == 0 && temp != nullptr){
          flag[2] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "rook"){
              if(temp->get_color() == 1) return 1;
              }
        }
      }
      if(king_posiotion.y - i >= 0){
        temp = board[king_posiotion.x][king_posiotion.y - i]->get_field_figure();
        if(flag[3] == 0 && temp != nullptr){
          flag[3] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "rook"){
              if(temp->get_color() == 1) return 1;
              }
        }
      }
      if(king_posiotion.x + i < 8 && king_posiotion.y + i < 8){
        temp = board[king_posiotion.x + i][king_posiotion.y + i]->get_field_figure();
        if(flag[4] == 0 && temp != nullptr){
          flag[4] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "bishop"){
              if(temp->get_color() == 1) return 1;
              }
        }
      }
      if(king_posiotion.x + i < 8 && king_posiotion.y - i >= 0){
        temp = board[king_posiotion.x + i][king_posiotion.y - i]->get_field_figure();
        if(flag[5] == 0 && temp != nullptr){
          flag[5] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "bishop"){
              if(temp->get_color() == 1) return 1;
              }
        }
      }
      if(king_posiotion.x - i >= 0 && king_posiotion.y + i < 8){
        temp = board[king_posiotion.x - i][king_posiotion.y + i]->get_field_figure();
        if(flag[6] == 0 && temp != nullptr){
          flag[6] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "bishop"){
              if(temp->get_color() == 1) return 1;
              }
        }
      }
      if(king_posiotion.x - i >= 0 && king_posiotion.y - i >= 0){
        temp = board[king_posiotion.x - i][king_posiotion.y - i]->get_field_figure();
        if(flag[7] == 0 && temp != nullptr){
          flag[7] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "bishop"){
              if(temp->get_color() == 1) return 1;
              }
        }
      }
    }
    return 0;
}

bool check_if_BLACK_king_is_attacked(Field*** &board){ // проверява дали черният цар е атакуван
  Point king_posiotion;
  Figure *temp;
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      temp = board[i][j]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 1 && temp->get_type() == "king"){
          king_posiotion = temp->get_coordinates();
          break;
        }
      }
    }
  }

    if(king_posiotion.x + 1 < 8){
      if(king_posiotion.y - 1 >= 0){
        temp = board[king_posiotion.x + 1][king_posiotion.y - 1]->get_field_figure();
        if(temp != nullptr){
          if(temp->get_type() == "pawn" && 
              temp->get_color() == 0){return 1;}
        }
      }
      if(king_posiotion.y + 1 < 8){
        temp = board[king_posiotion.x + 1][king_posiotion.y + 1]->get_field_figure();
        if(temp != nullptr){
          if(temp->get_type() == "pawn" &&
              temp->get_color() == 0){return 1;}
        }
      }
    }
    
    if(king_posiotion.x + 2 < 8 && king_posiotion.y + 1 < 8){
      temp = board[king_posiotion.x + 2][king_posiotion.y + 1]->get_field_figure();
      if(temp != nullptr){ 
        if(temp->get_color() == 0 &&
            temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x + 2 < 8 && king_posiotion.y - 1 >= 0){
      temp = board[king_posiotion.x + 2][king_posiotion.y - 1]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 0 &&
          temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x - 2 >= 0 && king_posiotion.y + 1 < 8){
      temp = board[king_posiotion.x - 2][king_posiotion.y + 1]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 0 &&
          temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x - 2 >= 0 && king_posiotion.y - 1 >= 0){
      temp = board[king_posiotion.x - 2][king_posiotion.y - 1]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 0 &&
          temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x + 1 < 8 && king_posiotion.y + 2 < 8){
      temp = board[king_posiotion.x + 1][king_posiotion.y + 2]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 0 &&
          temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x - 1 >= 0 && king_posiotion.y + 2 < 8){
      temp = board[king_posiotion.x - 1][king_posiotion.y + 2]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 0 &&
          temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x + 1 < 8 && king_posiotion.y - 2 >= 0){
      temp = board[king_posiotion.x + 1][king_posiotion.y - 2]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 0 &&
          temp->get_type() == "night") return 1;
      }
    }
    if(king_posiotion.x - 1 >= 0 && king_posiotion.y - 2 >= 0){
      temp = board[king_posiotion.x - 1][king_posiotion.y - 2]->get_field_figure();
      if(temp != nullptr){
        if(temp->get_color() == 0 &&
          temp->get_type() == "night") return 1;
      }
    }

    bool flag[8];
    for(int i = 0; i < 8; i++){
      flag[i] = 0;
    }
    string tpe_of_figure;
    for(int i = 1; i < 8; i++){
      
      if(king_posiotion.x + i < 8){
        temp = board[king_posiotion.x + i][king_posiotion.y]->get_field_figure();
        if(flag[0] == 0 && temp != nullptr){
          flag[0] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "rook"){
              if(temp->get_color() == 0) return 1;
              }
        }
      }
      if(king_posiotion.x - i >= 0){
        temp = board[king_posiotion.x - i][king_posiotion.y]->get_field_figure();
        if(flag[1] == 0 && temp != nullptr){
          flag[1] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "rook"){
              if(temp->get_color() == 0) return 1;
              }
        }
      }
      if(king_posiotion.y + i < 8){
        temp = board[king_posiotion.x][king_posiotion.y + i]->get_field_figure();
        if(flag[2] == 0 && temp != nullptr){
          flag[2] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "rook"){
              if(temp->get_color() == 0) return 1;
              }
        }
      }
      if(king_posiotion.y - i >= 0){
        temp = board[king_posiotion.x][king_posiotion.y - i]->get_field_figure();
        if(flag[3] == 0 && temp != nullptr){
          flag[3] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "rook"){
              if(temp->get_color() == 0) return 1;
              }
        }
      }
      if(king_posiotion.x + i < 8 && king_posiotion.y + i < 8){
        temp = board[king_posiotion.x + i][king_posiotion.y + i]->get_field_figure();
        if(flag[4] == 0 && temp != nullptr){
          flag[4] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "bishop"){
              if(temp->get_color() == 0) return 1;
              }
        }
      }
      if(king_posiotion.x + i < 8 && king_posiotion.y - i >= 0){
        temp = board[king_posiotion.x + i][king_posiotion.y - i]->get_field_figure();
        if(flag[5] == 0 && temp != nullptr){
          flag[5] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "bishop"){
              if(temp->get_color() == 0) return 1;
              }
        }
      }
      if(king_posiotion.x - i >= 0 && king_posiotion.y + i < 8){
        temp = board[king_posiotion.x - i][king_posiotion.y + i]->get_field_figure();
        if(flag[6] == 0 && temp != nullptr){
          flag[6] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "bishop"){
              if(temp->get_color() == 0) return 1;
              }
        }
      }
      if(king_posiotion.x - i >= 0 && king_posiotion.y - i >= 0){
        temp = board[king_posiotion.x - i][king_posiotion.y - i]->get_field_figure();
        if(flag[7] == 0 && temp != nullptr){
          flag[7] = 1;
          tpe_of_figure = temp->get_type();
          if(tpe_of_figure == "queen" || tpe_of_figure == "bishop"){
              if(temp->get_color() == 0) return 1;
              }
        }
      }
    }
    return 0;
}


bool chech_after_move_if_thisColor_KING_isAttacked(Point coordinates_now, Field*** &board, int move_x, int move_y, bool colore){ //проверява дали след изместване на фигурата, царят от същия цвят е атакуван
  Figure* p;
  p = board[move_x][move_y]->get_field_figure();
  board[move_x][move_y]->set_a_figure(board[coordinates_now.x][coordinates_now.y]->get_field_figure());
  board[coordinates_now.x][coordinates_now.y]->set_no_figure();
  Point a; a.x = move_x; a.y = move_y;
  board[move_x][move_y]->get_field_figure()->set_coordinates(a);
  a = coordinates_now;
  if(colore == 0){
    if(check_if_WHITE_king_is_attacked(board) == false){
      board[coordinates_now.x][coordinates_now.y]->set_a_figure(board[move_x][move_y]->get_field_figure());
      board[move_x][move_y]->set_a_figure(p);
      board[coordinates_now.x][coordinates_now.y]->get_field_figure()->set_coordinates(a);
      return 0;
    }
    else{
      board[coordinates_now.x][coordinates_now.y]->set_a_figure(board[move_x][move_y]->get_field_figure());
      board[move_x][move_y]->set_a_figure(p);
      board[coordinates_now.x][coordinates_now.y]->get_field_figure()->set_coordinates(a);
      return 1;
    }
  }
  if(colore == 1){
    if(check_if_BLACK_king_is_attacked(board) == false){
      board[coordinates_now.x][coordinates_now.y]->set_a_figure(board[move_x][move_y]->get_field_figure());
      board[move_x][move_y]->set_a_figure(p);
      board[coordinates_now.x][coordinates_now.y]->get_field_figure()->set_coordinates(a);
      return 0;
    }
    else{
      board[coordinates_now.x][coordinates_now.y]->set_a_figure(board[move_x][move_y]->get_field_figure());
      board[move_x][move_y]->set_a_figure(p);
      board[coordinates_now.x][coordinates_now.y]->get_field_figure()->set_coordinates(a);
      return 1;
      }
  }
  throw 1;
  board[coordinates_now.x][coordinates_now.y]->set_a_figure(board[move_x][move_y]->get_field_figure());
  board[move_x][move_y]->set_a_figure(p);
  board[coordinates_now.x][coordinates_now.y]->get_field_figure()->set_coordinates(a);
  throw 'd';
  return 0;
}



class Rook:public Figure{ // топ
  private:
    Point coordinates;
    bool color;
  public:

    Rook(){
      coordinates.x = -1;
      coordinates.y = -1;
      color = -1;
    }
    Rook(Point coordinat, bool colore){
      coordinates = coordinat;
      color = colore;
    }
    Rook(const Rook& A){
      this->color = A.color;
      this->coordinates = A.coordinates;
    }
    Rook& operator=(const Rook& A){
      if(this != &A){
        this->color = A.color;
        this->coordinates = A.coordinates;
      }
      return *this;
    }
    ~Rook(){}

    string get_type(){return "rook";} //тип на фигурата

    bool get_color()const{return color;}//цвят на фигурата
    Point get_coordinates()const{return coordinates;}//местоположение
    int get_number_of_position_to_go(Field*** board) const {//броят на местата, на които може да ходи
      int counter = 0, i = 1;
      bool king_check = 0, flag = 0;
      while (i < 8)
      {
        if(coordinates.x + i < 8){ // проверява в едната посока, докато не стигне до поле, което е от неговия цвят, проверява и полетата за евентуална прикритие на шах
          int r = board[coordinates.x + i][coordinates.y]->get_field_figure_color();
          if(board[coordinates.x + i][coordinates.y]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + i, coordinates.y, this->color) == false)counter++, i++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.y + i < 8){ // като в предния while
          int r = board[coordinates.x][coordinates.y + i]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x, coordinates.y + i, this->color) == false)counter++, i++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x - i >= 0){
          int r = board[coordinates.x - i][coordinates.y]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - i, coordinates.y, this->color) == false)counter++, i++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.y - i >= 0){
          int r = board[coordinates.x][coordinates.y - i]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x, coordinates.y - i, this->color) == false) counter++, i++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      return counter;
    }
    
    Point* get_field_coordinates_to_go(const int& n,  Field*** board) const { //полетата на които може да ходи
      Point* to_return = new Point[n];
      if(n == 0) return to_return;
      Point temp;
      int counter = 0, i = 1;
      bool flag = 0;
      while (i < 8) //абсолютно същия начин като в "get_number_of_position_to_go"
      {
        if(coordinates.x + i < 8){
          int r = board[coordinates.x + i][coordinates.y]->get_field_figure_color();
          if(board[coordinates.x + i][coordinates.y]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + i, coordinates.y, this->color) == false)
             temp.x = coordinates.x + i, temp.y = coordinates.y, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.y + i < 8){
          int r = board[coordinates.x][coordinates.y + i]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x, coordinates.y + i, this->color) == false)
             temp.x = coordinates.x, temp.y = coordinates.y + i, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x - i >= 0){
          int r = board[coordinates.x - i][coordinates.y]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - i, coordinates.y, this->color) == false)
             temp.x = coordinates.x - i, temp.y = coordinates.y, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.y - i >= 0){
          int r = board[coordinates.x][coordinates.y - i]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x, coordinates.y - i, this->color) == false)
             temp.x = coordinates.x, temp.y = coordinates.y - i, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      return to_return;
    }

    bool set_coordinates(const Point& a){coordinates = a; return true;} //задаване на координатите

    
};
//същите методи като в клас Rook
class Bishop:public Figure{
  private:
    Point coordinates;
    bool color;
  public:

    Bishop(){
      coordinates.x = -1;
      coordinates.y = -1;
      color = -1;
    }
    Bishop(Point coordinat, bool colore){
      coordinates = coordinat;
      color = colore;
    }
    Bishop(const Bishop& A){
      this->color = A.color;
      this->coordinates = A.coordinates;
    }
    Bishop& operator=(const Bishop& A){
      if(this != &A){
      this->color = A.color;
      this->coordinates = A.coordinates;
      }
      return *this;
    }
    ~Bishop(){}
    string get_type(){return "bishop";}

    Point get_coordinates()const{return coordinates;}
    bool get_color()const{return color;}
    int get_number_of_position_to_go(Field*** board) const {
      int counter = 0, i = 1;
      bool flag = 0;
      while (i < 8)
      {
        if(coordinates.x + i < 8 && coordinates.y + i < 8){
          int r = board[coordinates.x + i][coordinates.y + i]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + i, coordinates.y + i, this->color) == false) counter++, i++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x - i >= 0 && coordinates.y + i < 8){
          int r = board[coordinates.x - i][coordinates.y + i]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - i, coordinates.y + i, this->color) == false) counter++, i++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x - i >= 0 && coordinates.y - i >= 0){
          int r = board[coordinates.x - i][coordinates.y - i]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - i, coordinates.y - i, this->color) == false) counter++, i++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x + i < 8 && coordinates.y - i >= 0){
          int r = board[coordinates.x + i][coordinates.y - i]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + i, coordinates.y - i, this->color) == false) counter++, i++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      return counter;
    }
    
    Point* get_field_coordinates_to_go(const int& n, Field*** board) const {
      Point* to_return = new Point[n];
      if(n == 0) return to_return;
      Point temp;
      bool flag = 0;
      int counter = 0, i = 1;
      while (i < 8)
      {
        if(coordinates.x + i < 8 && coordinates.y + i < 8){
          int r = board[coordinates.x + i][coordinates.y + i]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + i, coordinates.y + i, this->color) == false)
             temp.x = coordinates.x + i, temp.y = coordinates.y + i, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x - i >= 0 && coordinates.y + i < 8){
          int r = board[coordinates.x - i][coordinates.y + i]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - i, coordinates.y + i, this->color) == false)
             temp.x = coordinates.x - i, temp.y = coordinates.y + i, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x - i >= 0 && coordinates.y - i >= 0){
          int r = board[coordinates.x - i][coordinates.y - i]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - i, coordinates.y - i, this->color) == false)
             temp.x = coordinates.x - i, temp.y = coordinates.y - i, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x + i < 8 && coordinates.y - i >= 0){
          int r = board[coordinates.x + i][coordinates.y - i]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + i, coordinates.y - i, this->color) == false)
             temp.x = coordinates.x + i, temp.y = coordinates.y - i, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      return to_return;
    }

    bool set_coordinates(const Point& a){coordinates = a; return true;}
};
//същите методи като в предните класове
class Night:public Figure{
  private:
    Point coordinates;
    bool color;
  public:

    Night(){
      coordinates.x = -1;
      coordinates.y = -1;
      color = -1;
    }
    Night(Point coordinat, bool colore){
      coordinates = coordinat;
      color = colore;
    }
    Night(const Night& A){
      this->color = A.color;
      this->coordinates = A.coordinates;
    }
    Night& operator=(const Night& A){
      if(this != &A){
      this->color = A.color;
      this->coordinates = A.coordinates;
      }
      return *this;
    }
    ~Night(){}
    string get_type(){return "night";}

    Point get_coordinates()const{return coordinates;}
    bool get_color()const{return color;}
    int get_number_of_position_to_go(Field*** board) const {
      int counter = 0;
      if(coordinates.x + 2 < 8 && coordinates.y + 1 < 8){ 
        if(board[coordinates.x + 2][coordinates.y + 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 2, coordinates.y + 1, this->color) == false) counter++;
      }
      if(coordinates.x + 2 < 8 && coordinates.y - 1 >= 0){
        if(board[coordinates.x + 2][coordinates.y - 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 2, coordinates.y - 1, this->color) == false) counter++;
      }
      if(coordinates.x - 2 >= 0 && coordinates.y + 1 < 8){
        if(board[coordinates.x - 2][coordinates.y + 1]->get_field_figure_color() != this->get_color() && 
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 2, coordinates.y + 1, this->color) == false) counter++;
      }
      if(coordinates.x - 2 >= 0 && coordinates.y - 1 >= 0){
        if(board[coordinates.x - 2][coordinates.y - 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 2, coordinates.y - 1, this->color) == false) counter++;
      }
      if(coordinates.x + 1 < 8 && coordinates.y + 2 < 8){
        if(board[coordinates.x + 1][coordinates.y + 2]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y + 2, this->color) == false) counter++;
      }
      if(coordinates.x - 1 >= 0 && coordinates.y + 2 < 8){
        if(board[coordinates.x - 1][coordinates.y + 2]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y + 2, this->color) == false) counter++;
      }
      if(coordinates.x + 1 < 8 && coordinates.y - 2 >= 0){
        if(board[coordinates.x + 1][coordinates.y - 2]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y - 2, this->color) == false) counter++;
      }
      if(coordinates.x - 1 >= 0 && coordinates.y - 2 >= 0){
        if(board[coordinates.x - 1][coordinates.y - 2]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y - 2, this->color) == false) counter++;
      }
      return counter;
    }
    
    Point* get_field_coordinates_to_go(const int& n, Field*** board) const {
      Point* to_return = new Point[n];
      if(n == 0) return to_return;
      Point temp;
      int counter = 0;
      if(coordinates.x + 2 < 8 && coordinates.y + 1 < 8){
        if(board[coordinates.x + 2][coordinates.y + 1]->get_field_figure_color() != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 2, coordinates.y + 1, this->color) == false){
          temp.x = coordinates.x + 2; temp.y = coordinates.y + 1;
          to_return[counter] = temp; counter++;
        }
      }
      if(coordinates.x + 2 < 8 && coordinates.y - 1 >= 0){
        if(board[coordinates.x + 2][coordinates.y - 1]->get_field_figure_color() != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 2, coordinates.y - 1, this->color) == false){
          temp.x = coordinates.x + 2; temp.y = coordinates.y - 1;
          to_return[counter] = temp; counter++;
        }
      }
      if(coordinates.x - 2 >= 0 && coordinates.y + 1 < 8){
        if(board[coordinates.x - 2][coordinates.y + 1]->get_field_figure_color() != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 2, coordinates.y + 1, this->color) == false){
          temp.x = coordinates.x - 2; temp.y = coordinates.y + 1;
          to_return[counter] = temp; counter++;
        }
      }
      if(coordinates.x - 2 >= 0 && coordinates.y - 1 >= 0){
        if(board[coordinates.x - 2][coordinates.y - 1]->get_field_figure_color() != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 2, coordinates.y - 1, this->color) == false){
          temp.x = coordinates.x - 2; temp.y = coordinates.y - 1;
          to_return[counter] = temp; counter++;
        }
      }
      if(coordinates.x + 1 < 8 && coordinates.y + 2 < 8){
        if(board[coordinates.x + 1][coordinates.y + 2]->get_field_figure_color() != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y + 2, this->color) == false){
          temp.x = coordinates.x + 1; temp.y = coordinates.y + 2;
          to_return[counter] = temp; counter++;
        }
      }
      if(coordinates.x - 1 >= 0 && coordinates.y + 2 < 8){
        if(board[coordinates.x - 1][coordinates.y + 2]->get_field_figure_color() != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y + 2, this->color) == false){
          temp.x = coordinates.x - 1; temp.y = coordinates.y + 2;
          to_return[counter] = temp; counter++;
        }
      }
      if(coordinates.x + 1 < 8 && coordinates.y - 2 >= 0){
        if(board[coordinates.x + 1][coordinates.y - 2]->get_field_figure_color() != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y - 2, this->color) == false){
          temp.x = coordinates.x + 1; temp.y = coordinates.y - 2; 
          to_return[counter] = temp; counter++;
        }
      }
      if(coordinates.x - 1 >= 0 && coordinates.y - 2 >= 0){
        if(board[coordinates.x - 1][coordinates.y - 2]->get_field_figure_color() != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y - 2, this->color) == false){
          temp.x = coordinates.x - 1; temp.y = coordinates.y - 2;
          to_return[counter] = temp; counter++;
        }
      }
      return to_return;
    }

    bool set_coordinates(const Point& a){coordinates = a; return true;}
};
//същите методи като в предните класове
class Queen:public Figure{
  private:
    Point coordinates;
    bool color;
  public:

    Queen(){
      coordinates.x = -1;
      coordinates.y = -1;
      color = -1;
    }
    Queen(Point coordinat, bool colore){
      coordinates = coordinat;
      color = colore;
    }
    Queen(const Queen& A){
      this->color = A.color;
      this->coordinates = A.coordinates;
    }
    Queen& operator=(const Queen& A){
      if(this != &A){
      this->color = A.color;
      this->coordinates = A.coordinates;
      }
      return *this;
    }
    ~Queen(){}
    string get_type(){return "queen";}

    Point get_coordinates()const{return coordinates;}
    bool get_color()const{return color;}
    int get_number_of_position_to_go(Field*** board) const {
      int counter = 0, i = 1;
      bool flag = 0;
      while (i < 8)
      {
        if(coordinates.x + i < 8 && coordinates.y + i < 8){
          int n = board[coordinates.x + i][coordinates.y + i]->get_field_figure_color();
          if(n != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + i, coordinates.y + i, this->color) == false) counter++, i++, flag = 1;
          if(n != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x - i >= 0 && coordinates.y + i < 8){
          int n = board[coordinates.x - i][coordinates.y + i]->get_field_figure_color();
          if(n != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - i, coordinates.y + i, this->color) == false) counter++, i++, flag = 1;
          if(n != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x - i >= 0 && coordinates.y - i >= 0){
          int n = board[coordinates.x - i][coordinates.y - i]->get_field_figure_color();
          if(n != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - i, coordinates.y - i, this->color) == false) counter++, i++, flag = 1;
          if(n != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x + i < 8 && coordinates.y - i >= 0){
          int n = board[coordinates.x + i][coordinates.y - i]->get_field_figure_color();
          if(n != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + i, coordinates.y - i, this->color) == false) counter++, i++, flag = 1;
          if(n != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x + i < 8){
          int n = board[coordinates.x + i][coordinates.y]->get_field_figure_color();
          if(n != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + i, coordinates.y, this->color) == false) counter++, i++, flag = 1;
          if(n != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.y + i < 8){
          int n = board[coordinates.x][coordinates.y + i]->get_field_figure_color();
          if(n != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x, coordinates.y + i, this->color) == false) counter++, i++, flag = 1;
          if(n != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x - i >= 0){
          int n = board[coordinates.x - i][coordinates.y]->get_field_figure_color();
          if(n != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - i, coordinates.y, this->color) == false) counter++, i++, flag = 1;
          if(n != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.y - i >= 0){
          int n = board[coordinates.x][coordinates.y - i]->get_field_figure_color();
          if(n != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x, coordinates.y - i, this->color) == false) counter++, i++, flag = 1;
          if(n != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      return counter;
    }
    
    Point* get_field_coordinates_to_go(const int& n,Field*** board) const {
      Point* to_return = new Point[n];
      if(n == 0) return to_return;
      Point temp;
      int counter = 0, i = 1;
      bool flag = 0;
      while (i < 8)
      {
        if(coordinates.x + i < 8 && coordinates.y + i < 8){
          int r = board[coordinates.x + i][coordinates.y + i]->get_field_figure_color();
          if(r != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + i, coordinates.y + i, this->color) == false)
             temp.x = coordinates.x + i, temp.y = coordinates.y + i, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x - i >= 0 && coordinates.y + i < 8){
          int r = board[coordinates.x - i][coordinates.y + i]->get_field_figure_color();
          if(r != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - i, coordinates.y + i, this->color) == false)
          temp.x = coordinates.x - i, temp.y = coordinates.y + i, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x - i >= 0 && coordinates.y - i >= 0){
          int r = board[coordinates.x - i][coordinates.y - i]->get_field_figure_color();
          if(board[coordinates.x - i][coordinates.y - i]->get_field_figure_color() != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - i, coordinates.y - i, this->color) == false)
              temp.x = coordinates.x - i, temp.y = coordinates.y - i, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x + i < 8 && coordinates.y - i >= 0){
          int r = board[coordinates.x + i][coordinates.y - i]->get_field_figure_color();
          if(board[coordinates.x + i][coordinates.y - i]->get_field_figure_color() != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + i, coordinates.y - i, this->color) == false)
             temp.x = coordinates.x + i, temp.y = coordinates.y - i, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x + i < 8){
          int r = board[coordinates.x + i][coordinates.y]->get_field_figure_color();
          if(board[coordinates.x + i][coordinates.y]->get_field_figure_color() != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + i, coordinates.y, this->color) == false)
             temp.x = coordinates.x + i, temp.y = coordinates.y, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.y + i < 8){
          int r = board[coordinates.x][coordinates.y + i]->get_field_figure_color();
          if(board[coordinates.x][coordinates.y + i]->get_field_figure_color() != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x, coordinates.y + i, this->color) == false)
             temp.x = coordinates.x, temp.y = coordinates.y + i, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.x - i >= 0){
          int r = board[coordinates.x - i][coordinates.y]->get_field_figure_color();
          if(r != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - i, coordinates.y, this->color) == false)
             temp.x = coordinates.x - i, temp.y = coordinates.y, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      i = 1;
      while (i < 8)
      {
        if(coordinates.y - i >= 0){
          int r = board[coordinates.x][coordinates.y - i]->get_field_figure_color();
          if(r != this->get_color()&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x, coordinates.y - i, this->color) == false)
             temp.x = coordinates.x, temp.y = coordinates.y - i, i++, to_return[counter] = temp, counter++, flag = 1;
          if(r != -1) break;
          if(flag == 0) i++;
          flag = 0;
        }
        else break;
      }
      return to_return;
    }

    bool set_coordinates(const Point& a){coordinates = a; return true;}
};
//същите методи като в предните класове
class King:public Figure{
  private:
    Point coordinates;
    bool color;
  public:

    King(){
      coordinates.x = -1;
      coordinates.y = -1;
      color = -1;
    }
    King(Point coordinat, bool colore){
      coordinates = coordinat;
      color = colore;
    }
    King(const King& A){
      this->color = A.color;
      this->coordinates = A.coordinates;
    }
    King& operator=(const King& A){
      if(this != &A){
      this->color = A.color;
      this->coordinates = A.coordinates;
      }
      return *this;
    }
    ~King(){}
    string get_type(){return "king";}

    Point get_coordinates()const{return coordinates;}
    bool get_color()const{return color;}
    int get_number_of_position_to_go(Field*** board) const {
      int counter = 0;
      if(coordinates.x + 1 < 8){
        if(board[coordinates.x + 1][coordinates.y]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y, this->color) == false &&
             board[coordinates.x + 1][coordinates.y]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0) counter++;
      }
      if(coordinates.x - 1 >= 0){
        if(board[coordinates.x - 1][coordinates.y]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y, this->color) == false &&
             board[coordinates.x - 1][coordinates.y]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0) counter++;
      }
      if(coordinates.y + 1 < 8){
        if(board[coordinates.x][coordinates.y + 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x, coordinates.y + 1, this->color) == false &&
             board[coordinates.x][coordinates.y + 1]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0) counter++;
      }
      if(coordinates.y - 1 >= 0){
        if(board[coordinates.x][coordinates.y - 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x, coordinates.y - 1, this->color) == false &&
             board[coordinates.x][coordinates.y - 1]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0) counter++;
      }
      if(coordinates.x + 1 < 8 && coordinates.y + 1 < 8){
        if(board[coordinates.x + 1][coordinates.y + 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y + 1, this->color) == false &&
             board[coordinates.x + 1][coordinates.y + 1]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0) counter++;
      }
      if(coordinates.x + 1 < 8 && coordinates.y - 1 >= 0){
        if(board[coordinates.x + 1][coordinates.y - 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y - 1, this->color) == false &&
             board[coordinates.x + 1][coordinates.y - 1]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0) counter++;
      }
      if(coordinates.x - 1 >= 0 && coordinates.y + 1 < 8){
        if(board[coordinates.x - 1][coordinates.y + 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y + 1, this->color) == false &&
             board[coordinates.x - 1][coordinates.y + 1]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0) counter++;
      }
      if(coordinates.x - 1 >= 0 && coordinates.y - 1 >= 0){
        if(board[coordinates.x - 1][coordinates.y - 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y - 1, this->color) == false &&
             board[coordinates.x - 1][coordinates.y - 1]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0) counter++;
      }
      return counter;
    }
    
    Point* get_field_coordinates_to_go(const int& n,Field*** board) const {
      Point* to_return = new Point[n];
      if(n == 0) return to_return;
      Point temp;
      int counter = 0;  
      if(coordinates.x + 1 < 8){
        if(board[coordinates.x + 1][coordinates.y]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y, this->color) == false &&
             board[coordinates.x + 1][coordinates.y]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0)
        temp.x = coordinates.x + 1, temp.y = coordinates.y, to_return[counter] = temp, counter++;
      }
      if(coordinates.x - 1 >= 0){
        if(board[coordinates.x - 1][coordinates.y]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y, this->color) == false &&
             board[coordinates.x - 1][coordinates.y]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0)
        temp.x = coordinates.x - 1, temp.y = coordinates.y, to_return[counter] = temp, counter++;
      }
      if(coordinates.y + 1 < 8){
        if(board[coordinates.x][coordinates.y + 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x, coordinates.y + 1, this->color) == false &&
             board[coordinates.x][coordinates.y + 1]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0)
        temp.x = coordinates.x, temp.y = coordinates.y + 1, to_return[counter] = temp, counter++;
      }
      if(coordinates.y - 1 >= 0){
        if(board[coordinates.x][coordinates.y - 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x, coordinates.y - 1, this->color) == false &&
             board[coordinates.x][coordinates.y - 1]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0)
        temp.x = coordinates.x, temp.y = coordinates.y - 1, to_return[counter] = temp, counter++;
      }
      if(coordinates.x + 1 < 8 && coordinates.y + 1 < 8){
        if(board[coordinates.x + 1][coordinates.y + 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y + 1, this->color) == false &&
             board[coordinates.x + 1][coordinates.y + 1]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0)
        temp.x = coordinates.x + 1, temp.y = coordinates.y + 1, to_return[counter] = temp, counter++;
      }
      if(coordinates.x + 1 < 8 && coordinates.y - 1 >= 0){
        if(board[coordinates.x + 1][coordinates.y - 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y - 1, this->color) == false &&
             board[coordinates.x + 1][coordinates.y - 1]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0)
        temp.x = coordinates.x + 1, temp.y = coordinates.y - 1, to_return[counter] = temp, counter++;
      }
      if(coordinates.x - 1 >= 0 && coordinates.y + 1 < 8){
        if(board[coordinates.x - 1][coordinates.y + 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y + 1, this->color) == false &&
             board[coordinates.x - 1][coordinates.y + 1]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0)
        temp.x = coordinates.x - 1, temp.y = coordinates.y + 1, to_return[counter] = temp, counter++;
      }
      if(coordinates.x - 1 >= 0 && coordinates.y - 1 >= 0){
        if(board[coordinates.x - 1][coordinates.y - 1]->get_field_figure_color() != this->get_color() &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y - 1, this->color) == false &&
             board[coordinates.x - 1][coordinates.y - 1]->check_if_figure_or_field_is_protected_or_attacked(board, color) == 0)
             temp.x = coordinates.x - 1, temp.y = coordinates.y - 1, to_return[counter] = temp, counter++;
      }
      return to_return;
    }

    bool set_coordinates(const Point& a){coordinates = a; return true;}
};
//същите методи като в предните класове
class Pawn:public Figure{
  private:
    Point coordinates;
    bool color;
  public:

    Pawn(){
      coordinates.x = -1;
      coordinates.y = -1;
      color = -1;
    }
    Pawn(Point coordinat, bool colore){
      coordinates = coordinat;
      color = colore;
    }
    Pawn(const Pawn& A){
      this->color = A.color;
      this->coordinates = A.coordinates;
    }
    Pawn& operator=(const Pawn& A){
      if(this != &A){
      this->color = A.color;
      this->coordinates = A.coordinates;
      }
      return *this;
    }
    ~Pawn(){}
    string get_type(){return "pawn";}

    Point get_coordinates()const{return coordinates;}
    bool get_color()const{return color;}
    int get_number_of_position_to_go(Field*** board) const {
      int counter = 0;
      if(this->get_color() == 1){
        if(coordinates.x + 2 < 8){
          if(coordinates.x == 1 && board[coordinates.x + 2][coordinates.y]->get_field_figure() == nullptr && board[coordinates.x + 1][coordinates.y]->get_field_figure() == nullptr &&
              chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 2, coordinates.y, this->color) == false) counter++;
        }
        if(coordinates.x + 1 < 8){
          if(board[coordinates.x + 1][coordinates.y]->get_field_figure() == nullptr &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y, this->color) == false) counter++;
          if(coordinates.y + 1 < 8){
            if(board[coordinates.x + 1][coordinates.y + 1]->get_field_figure_color() == 0 &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y + 1, this->color) == false) counter++;
          }
          if(coordinates.y - 1 >= 0){
            if(board[coordinates.x + 1][coordinates.y - 1]->get_field_figure_color() == 0&&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y - 1, this->color) == false) counter++;
          }
        }
      }
      if(this->get_color() == 0){
        if(coordinates.x - 2 >= 0){
          if(coordinates.x == 6 && board[coordinates.x - 2][coordinates.y]->get_field_figure() == nullptr && board[coordinates.x - 1][coordinates.y]->get_field_figure() == nullptr &&
              chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 2, coordinates.y, this->color) == false) counter++;
        }
        if(coordinates.x - 1 >= 0){
          if(board[coordinates.x - 1][coordinates.y]->get_field_figure() == nullptr &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y, this->color) == false) counter++;
          if(coordinates.y + 1 < 8){
            if(board[coordinates.x - 1][coordinates.y + 1]->get_field_figure_color() == 1 &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y + 1, this->color) == false) counter++;
          }
          if(coordinates.y - 1 >= 0){
            if(board[coordinates.x - 1][coordinates.y - 1]->get_field_figure_color() == 1 &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y - 1, this->color) == false) counter++;
          }
        }
      }
      return counter;
    }
    
    Point* get_field_coordinates_to_go(const int& n,Field*** board) const {
      Point* to_return = new Point[n];
      if(n == 0) return to_return;
      Point temp;
      int counter = 0;
      // cout << "Started: ";
      if(this->get_color() == 1){
        if(coordinates.x + 2 < 8){
          if(coordinates.x == 1 && board[coordinates.x + 2][coordinates.y]->get_field_figure() == nullptr && board[coordinates.x + 1][coordinates.y]->get_field_figure() == nullptr &&
              chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 2, coordinates.y, this->color) == false)
              temp.x = coordinates.x + 2, temp.y = coordinates.y, to_return[counter] = temp, counter++;
        }
        if(coordinates.x + 1 < 8){
          if(board[coordinates.x + 1][coordinates.y]->get_field_figure() == nullptr &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y, this->color) == false)
             temp.x = coordinates.x + 1, temp.y = coordinates.y, to_return[counter] = temp, counter++;
          if(coordinates.y + 1 < 8){
            if(board[coordinates.x + 1][coordinates.y + 1]->get_field_figure_color() == 0 &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y + 1, this->color) == false)
            temp.x = coordinates.x + 1, temp.y = coordinates.y + 1, to_return[counter] = temp, counter++;
          }
          if(coordinates.y - 1 >= 0){
            if(board[coordinates.x + 1][coordinates.y - 1]->get_field_figure_color() == 0 &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x + 1, coordinates.y - 1, this->color) == false)
            temp.x = coordinates.x + 1, temp.y = coordinates.y - 1, to_return[counter] = temp, counter++;
          }
        }
      }

      if(this->get_color() == 0){
        if(coordinates.x - 2 >= 0){
          if(coordinates.x == 6 && board[coordinates.x - 2][coordinates.y]->get_field_figure() == nullptr && board[coordinates.x - 1][coordinates.y]->get_field_figure() == nullptr &&
              chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 2, coordinates.y, this->color) == false) 
              temp.x = coordinates.x - 2, temp.y = coordinates.y, to_return[counter] = temp, counter++;
        }
        if(coordinates.x - 1 >= 0){
          if(board[coordinates.x - 1][coordinates.y]->get_field_figure() == nullptr &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y, this->color) == false)
             temp.x = coordinates.x - 1, temp.y = coordinates.y, to_return[counter] = temp, counter++;
          if(coordinates.y + 1 < 8){
            if(board[coordinates.x - 1][coordinates.y + 1]->get_field_figure_color() == 1 &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y + 1, this->color) == false)
             temp.x = coordinates.x - 1, temp.y = coordinates.y + 1, to_return[counter] = temp, counter++;
          }
          if(coordinates.y - 1 >= 0){
            if(board[coordinates.x - 1][coordinates.y - 1]->get_field_figure_color() == 1 &&
             chech_after_move_if_thisColor_KING_isAttacked(this->coordinates, board, coordinates.x - 1, coordinates.y - 1, this->color) == false)
             temp.x = coordinates.x - 1, temp.y = coordinates.y - 1, to_return[counter] = temp, counter++;
          }
        }
      }
      // cout << "finished";
      return to_return;
    }

    bool set_coordinates(const Point& a){coordinates = a; return true;}
};



bool Field::set_new_figure(Figure* A){ // слага нова фигура на това поле
  if(A != nullptr){
    string typee = A->get_type();
    bool color = A->get_color();
    Point coord = A->get_coordinates();
    if(typee == "rook"){
      this->on_field_figure = new Rook(coord, color);
    }
    else{
      if(typee == "bishop"){
        this->on_field_figure = new Bishop(coord, color);
      }
      else{
        if(typee == "night"){
          this->on_field_figure = new Night(coord, color);
        }
        else{
          if(typee == "queen"){
            this->on_field_figure = new Queen(coord, color);
          }
          else{
            if(typee == "king"){
              this->on_field_figure = new King(coord, color);
            }
            else{
              this->on_field_figure = new Pawn(coord, color);
            }
          }
        }
      }
    }
  }
  return true;
}

bool Field::set_a_figure(Figure* p){ // помощен метод при алгоритъма
  this->on_field_figure = p;
  return 1;
}

bool Field::set_no_figure(){
  this->on_field_figure = nullptr;
  return 1;
}

bool Field::set_rook(bool col){
  Figure* p = new Rook(this->coordinates, col);
  this->on_field_figure = p;
  return 1;
}

bool Field::set_bishop(bool col){
  Figure* p = new Bishop(this->coordinates, col);
  this->on_field_figure = p;
  return 1;
}

bool Field::set_night(bool col){
  Figure* p = new Night(this->coordinates, col);
  this->on_field_figure = p;
  return 1;
}

bool Field::set_king(bool col){
  Figure* p = new King(this->coordinates, col);
  this->on_field_figure = p;
  return 1;
}

bool Field::set_queen(bool col){
  Figure* p = new Queen(this->coordinates, col);
  this->on_field_figure = p;
  return 1;
}

bool Field::set_pawn(bool col){
  Figure* p = new Pawn(this->coordinates, col);
  this->on_field_figure = p;
  return 1;
}



struct best_move_struct{ //структура помощна при алгоритъма, запазва ми най-оптималния ход, както и фигурата, която да изкара от пешката, ако има такава
  char new_figure = 'z';
  int number_of_moves;
  double value = 0;
  Point from_place;
  Point to_place;
};

class Game{
  private:
    Field*** board; //дъската
    bool choose_figures; // избор с кои фигури да играе
    bool white_small_castle; //дали може да направи белия малка рокада
    bool black_small_castle; //може ли черния малка рокада
    bool white_big_castle; //може ли белия голяма рокада
    bool black_big_castle; //може ли черния голяма рокада
    int number_move; //броят на ходове
    bool whos_move; // 0 - beli, 1 - cerni na hod
    bool players; //0 - computer, 1 - 2ma igraci
    Point previous_place; // предното място на последно местената фигура
    Point current_place; //сегашното мястно на последно местената фигура

    void make_move(Point from_place, Point to_place, char if_new_fig = 'z'){ // извършва ход на дъската
      bool flag_not_specific_move = 0;
      if(from_place.x == 7 && from_place.y == 4 && to_place.x == 7 && to_place.y == 6 && board[from_place.x][from_place.y]->get_figure_type() == "king" ){ //проверява за малка рокада бели
        make_move_for_wsc();
        white_small_castle = 0;
        flag_not_specific_move = 1;
      }
      if(from_place.x == 7 && from_place.y == 4 && to_place.x == 7 && to_place.y == 2 && board[from_place.x][from_place.y]->get_figure_type() == "king" ){ //проверява за голяма рокада бели
        make_move_for_wbc();
        white_big_castle = 0;
        flag_not_specific_move = 1;
      }
      if(from_place.x == 0 && from_place.y == 4 && to_place.x == 0 && to_place.y == 6 && board[from_place.x][from_place.y]->get_figure_type() == "king" ){ //проверява за малка рокада черни
        make_move_for_bsc();
        black_small_castle = 0;
        flag_not_specific_move = 1;
      }
      if(from_place.x == 0 && from_place.y == 4 && to_place.x == 0 && to_place.y == 2 && board[from_place.x][from_place.y]->get_figure_type() == "king" ){ //проверява за голяма рокада черни
        make_move_for_bbc();
        black_big_castle = 0;
        flag_not_specific_move = 1;
      }
      if(white_small_castle == 1){ // изчиства възможността за рокада след изместена фигура за 1ви път
        if(board[from_place.x][from_place.y]->get_figure_type() == "rook" && board[from_place.x][from_place.y]->get_field_figure_color() == 0) white_small_castle = 0;
        if(board[from_place.x][from_place.y]->get_figure_type() == "king" && board[from_place.x][from_place.y]->get_field_figure_color() == 0) white_small_castle = 0;
      }
      if(white_big_castle == 1){ // изчиства възможността за рокада след изместена фигура за 1ви път
        if(board[from_place.x][from_place.y]->get_figure_type() == "rook" && board[from_place.x][from_place.y]->get_field_figure_color() == 0) white_big_castle = 0;
        if(board[from_place.x][from_place.y]->get_figure_type() == "king" && board[from_place.x][from_place.y]->get_field_figure_color() == 0) white_big_castle = 0;
      }
      if(black_big_castle == 1){ // изчиства възможността за рокада след изместена фигура за 1ви път
        if(board[from_place.x][from_place.y]->get_figure_type() == "rook" && board[from_place.x][from_place.y]->get_field_figure_color() == 1) black_big_castle = 0;
        if(board[from_place.x][from_place.y]->get_figure_type() == "king" && board[from_place.x][from_place.y]->get_field_figure_color() == 1) black_big_castle = 0;
      }
      if(black_small_castle == 1){ // изчиства възможността за рокада след изместена фигура за 1ви път
        if(board[from_place.x][from_place.y]->get_figure_type() == "rook" && board[from_place.x][from_place.y]->get_field_figure_color() == 1) black_small_castle = 0;
        if(board[from_place.x][from_place.y]->get_figure_type() == "king" && board[from_place.x][from_place.y]->get_field_figure_color() == 1) black_small_castle = 0;
      }

      if(whos_move == 0){ // бели - проверява дали пешката може да се изкара и да избере фигура
        if(board[from_place.x][from_place.y]->get_figure_type() == "pawn" && to_place.x == 0){
          if(players == 1 || choose_figures == 0){ // ако тук е на ход компютъра не изисква въвеждане за иброр на фигура
            do
            {
              cout << "Choose a new figure (enter: q, r, k, b): ";
              cin >> if_new_fig;
            } while (if_new_fig != 'q' && if_new_fig != 'r' && if_new_fig != 'b' && if_new_fig != 'n');
          }
          switch (if_new_fig)
          {
          case 'q':
            board[to_place.x][to_place.y]->set_queen(0);
            board[to_place.x][to_place.y]->get_field_figure()->set_coordinates(to_place);
            break;

          case 'r':
            board[to_place.x][to_place.y]->set_rook(0);
            board[to_place.x][to_place.y]->get_field_figure()->set_coordinates(to_place);
          
          case 'n':
            board[to_place.x][to_place.y]->set_night(0);
            board[to_place.x][to_place.y]->get_field_figure()->set_coordinates(to_place);

          case 'b':
            board[to_place.x][to_place.y]->set_bishop(0);
            board[to_place.x][to_place.y]->get_field_figure()->set_coordinates(to_place);
          
          default:
            break;
          }
          board[from_place.x][from_place.y]->del_figure();
          flag_not_specific_move = 1;
        }
        if(number_move > 1){ // проверява за анпасан за белите
          if(board[from_place.x][from_place.y]->get_figure_type() == "pawn" && board[current_place.x][current_place.y]->get_figure_type() == "pawn" &&
            find_if_enpassant_white_can_capture(previous_place.x, previous_place.y, current_place) == 1 && (current_place.x - 1) == to_place.x){
            board[to_place.x][to_place.y]->set_a_figure(board[from_place.x][from_place.y]->get_field_figure());
            board[to_place.x][to_place.y]->get_field_figure()->set_coordinates(to_place);
            board[from_place.x][from_place.y]->set_no_figure();
            board[to_place.x + 1][to_place.y]->del_figure();
            flag_not_specific_move = 1;
          }
        }
      }
      else{// черни - проверява дали пешката може да се изкара и да избере фигура 
        if(board[from_place.x][from_place.y]->get_figure_type() == "pawn" && to_place.x == 7){
          if(players == 1 || choose_figures == 1){ // ако тук е на ход компютъра не изисква въвеждане за иброр на фигура
            do
            {
              cout << "Choose a new figure (enter: q, r, k, b): ";
              cin >> if_new_fig;
            } while (if_new_fig != 'q' && if_new_fig != 'r' && if_new_fig != 'b' && if_new_fig != 'n');
          }
          switch (if_new_fig)
          {
          case 'q':
            board[to_place.x][to_place.y]->set_queen(1);
            board[to_place.x][to_place.y]->get_field_figure()->set_coordinates(to_place);
            break;

          case 'r':
            board[to_place.x][to_place.y]->set_rook(1);
            board[to_place.x][to_place.y]->get_field_figure()->set_coordinates(to_place);
          
          case 'n':
            board[to_place.x][to_place.y]->set_night(1);
            board[to_place.x][to_place.y]->get_field_figure()->set_coordinates(to_place);

          case 'b':
            board[to_place.x][to_place.y]->set_bishop(1);
            board[to_place.x][to_place.y]->get_field_figure()->set_coordinates(to_place);
          
          default:
            break;
          }
          board[from_place.x][from_place.y]->del_figure();
          flag_not_specific_move = 1;
        }

        if(number_move > 1){ // проверява за анпасан за черните
          if(board[from_place.x][from_place.y]->get_figure_type() == "pawn" && board[current_place.x][current_place.y]->get_figure_type() == "pawn" &&
            find_if_enpassant_black_can_capture(previous_place.x, previous_place.y, current_place) == 1 && (current_place.x + 1) == to_place.x){
            board[to_place.x][to_place.y]->set_a_figure(board[from_place.x][from_place.y]->get_field_figure());
            board[to_place.x][to_place.y]->get_field_figure()->set_coordinates(to_place);
            board[from_place.x][from_place.y]->set_no_figure();
            board[to_place.x - 1][to_place.y]->del_figure();
            flag_not_specific_move = 1;
          }
        }
      }
      if(flag_not_specific_move == 0){ // ако нито един от специалните ходове се е извършил, изпълнява прост ход
        board[to_place.x][to_place.y]->del_figure();
        board[to_place.x][to_place.y]->set_a_figure(board[from_place.x][from_place.y]->get_field_figure());
        board[to_place.x][to_place.y]->get_field_figure()->set_coordinates(to_place);
        board[from_place.x][from_place.y]->set_no_figure();
      }
      // применя останалата информация за играта
      this->previous_place = from_place;
      this->current_place = to_place;
      number_move += whos_move;
      change_whos_turn();
    }
    
    double material_adjustment(string name, int x, int y){ // променя тежестта на всяка фигура спрямо мястото на дъската си
      if(name == "night"){
        if(board[x][y]->get_field_figure()->get_color() == 0){
          double table[8][8] = {
            -0.5, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.5,

            -0.4, -0.2,    0,    0,    0,    0, -0.2, -0.4,

            -0.3,    0,  0.1, 0.15, 0.15,  0.1,    0, -0.3,

            -0.3, 0.05, 0.15,  0.2,  0.2, 0.15, 0.05, -0.3,

            -0.3,    0, 0.15,  0.2,  0.2, 0.15,    0, -0.3,

            -0.3, 0.05,  0.1, 0.15, 0.15,  0.1, 0.05, -0.3,

            -0.4, -0.2,    0,    0,    0,    0, -0.2, -0.4,
            
            -0.5, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.5,
          };
          return table[x][y];
        }
        if(board[x][y]->get_field_figure()->get_color() == 1){
          double table[8][8] = {
            -0.5, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.5,

            -0.4, -0.2,    0,    0,    0,    0, -0.2, -0.4,

            -0.3, 0.05,  0.1, 0.15, 0.15,  0.1, 0.05, -0.3,

            -0.3,    0, 0.15,  0.2,  0.2, 0.15,    0, -0.3,

            -0.3, 0.05, 0.15,  0.2,  0.2, 0.15, 0.05, -0.3,

            -0.3,    0,  0.1, 0.15, 0.15,  0.1,    0, -0.3,

            -0.4, -0.2,    0,    0,    0,    0, -0.2, -0.4,
            
            -0.5, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.5,
          };
          return table[x][y];
        }
      }
      if(name == "queen"){
        if(board[x][y]->get_field_figure()->get_color() == 0){
          double table[8][8] = {
            -0.2, -0.1, -0.1,-0.05,-0.05, -0.1, -0.1,  -0.2,

            -0.1,    0,    0,    0,    0,    0,    0,  -0.1,

            -0.1,    0, 0.05, 0.05, 0.05, 0.05,    0,  -0.1,

           -0.05,    0, 0.05, 0.05, 0.05, 0.05,    0, -0.05,

               0,    0, 0.05, 0.05, 0.05, 0.05,    0, -0.05,

            -0.1, 0.05, 0.05, 0.05, 0.05, 0.05,    0,  -0.1,

            -0.1,    0, 0.05,    0,    0,    0,    0,  -0.1,

            -0.2, -0.1, -0.1, -0.05,-0.05, -0.1, -0.1,  -0.2,
          };
          return table[x][y];
        }
        if(board[x][y]->get_field_figure()->get_color() == 1){
          double table[8][8] = {
            -0.2, -0.1, -0.1,-0.05,-0.05, -0.1, -0.1,  -0.2,
            -0.1,    0, 0.05,    0,    0,    0,    0,  -0.1,
            -0.1, 0.05, 0.05, 0.05, 0.05, 0.05,    0,  -0.1,
               0,    0, 0.05, 0.05, 0.05, 0.05,    0, -0.05,
           -0.05,    0, 0.05, 0.05, 0.05, 0.05,    0, -0.05,
            -0.1,    0, 0.05, 0.05, 0.05, 0.05,    0,  -0.1,
            -0.1,    0,    0,    0,    0,    0,    0,  -0.1,
            -0.2, -0.1, -0.1,-0.05,-0.05, -0.1, -0.1,  -0.2,
          };
          return table[x][y];
        }
      }
      if(name == "bishop"){
        if(board[x][y]->get_field_figure()->get_color() == 0){
          double table[8][8] = 
          {-0.2, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2,
           -0.1,    0,    0,    0,    0,    0,    0, -0.1,
           -0.1,    0, 0.05,  0.1,  0.1, 0.05,    0, -0.1,
           -0.1, 0.05, 0.05,  0.1,  0.1, 0.05, 0.05, -0.1,
           -0.1,    0,  0.1,  0.1,  0.1,  0.1,    0, -0.1,
           -0.1, 0.05,  0.1,  0.1,  0.1,  0.1, 0.05, -0.1,
           -0.1,  0.1,    0,    0,    0,    0,  0.1, -0.1,
           -0.2, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2,
          };
          return table[x][y];
        }
        if(board[x][y]->get_field_figure()->get_color() == 1){
          double table[8][8] = 
          {-0.2, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2,
           -0.1,  0.1,    0,    0,    0,    0,  0.1, -0.1,
           -0.1, 0.05,  0.1,  0.1,  0.1,  0.1, 0.05, -0.1,
           -0.1,    0,  0.1,  0.1,  0.1,  0.1,    0, -0.1,
           -0.1, 0.05, 0.05,  0.1,  0.1, 0.05, 0.05, -0.1,
           -0.1,    0, 0.05,  0.1,  0.1, 0.05,    0, -0.1,
           -0.1,    0,    0,    0,    0,    0,    0, -0.1,
           -0.2, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2,
          };
          return table[x][y];
        }
      }
      if(name == "rook"){
        if(board[x][y]->get_field_figure()->get_color() == 0){
          double table[8][8] = {
                0,   0,   0,   0,   0,   0,   0,   0,
            0.05, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.05,
            -0.05,   0,   0,   0,   0,   0,   0,-0.05,
            -0.05,   0,   0,   0,   0,   0,   0,-0.05,
            -0.05,   0,   0,   0,   0,   0,   0,-0.05,
            -0.05,   0,   0,   0,   0,   0,   0,-0.05,
            -0.05,   0,   0,   0,   0,   0,   0,-0.05,
                0,   0,   0,0.05,0.05,   0,   0,    0,
          };
          return table[x][y];
        }
        if(board[x][y]->get_field_figure()->get_color() == 1){
          double table[8][8] = {
                0,   0,   0,0.05,0.05,   0,   0,    0,
            -0.05,   0,   0,   0,   0,   0,   0,-0.05,
            -0.05,   0,   0,   0,   0,   0,   0,-0.05,
            -0.05,   0,   0,   0,   0,   0,   0,-0.05,
            -0.05,   0,   0,   0,   0,   0,   0,-0.05,
            -0.05,   0,   0,   0,   0,   0,   0,-0.05,
             0.05, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.05,
                0,   0,   0,   0,   0,   0,   0,    0,
          };
          return table[x][y];
        }
      }
      if(name == "pawn"){
        if(board[x][y]->get_field_figure()->get_color() == 0){
          double table[8][8] = {
                0,    0,    0,    0,    0,    0,    0,    0,
              0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,
              0.1,  0.1,  0.2,  0.3,  0.3,  0.2,  0.1,  0.1,
             0.05, 0.05,  0.1, 0.25, 0.25,  0.1, 0.05, 0.05,
            -0.05,-0.05,    0,  0.2,  0.2,    0,-0.05,-0.05,
             0.05,    0, -0.1,    0,    0, -0.1,    0, 0.05,
             0.05,  0.1,  0.1, -0.2, -0.2,  0.1,  0.1, 0.05,
                0,    0,    0,    0,    0,    0,    0,    0,
          };
          return table[x][y];
        }
        if(board[x][y]->get_field_figure()->get_color() == 1){
          double table[8][8] = {
              0,    0,    0,    0,    0,    0,    0,    0,
           0.05,  0.1,  0.1, -0.2, -0.2,  0.1,  0.1, 0.05,
           0.05,    0, -0.1,    0,    0, -0.1,    0, 0.05,
          -0.05,-0.05,    0,  0.2,  0.2,    0,-0.05,-0.05,
           0.05, 0.05,  0.1, 0.25, 0.25,  0.1, 0.05, 0.05,
            0.1,  0.1,  0.2,  0.3,  0.3,  0.2,  0.1,  0.1,
            0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,
              0,    0,    0,    0,    0,    0,    0,    0,
          };
          return table[x][y];
        }
      }
      if(name == "king"){
        if(board[x][y]->get_field_figure()->get_color() == 0){
          if(number_move < 22){ // в началото искаме да е по-защитен, после в центъра
            double table[8][8] = {
              -0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3,
              -0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3,
              -0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3,
              -0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3,
              -0.2,-0.3,-0.3,-0.4,-0.4,-0.3,-0.3,-0.2,
              -0.1,-0.2,-0.2,-0.2,-0.2,-0.2,-0.2,-0.1,
               0.2, 0.2,   0,   0,   0,   0, 0.2, 0.2,
               0.2, 0.3, 0.1,   0,   0, 0.1, 0.3, 0.2,
            };
            return table[x][y];
          }
          else{
            double table[8][8] = {
              -0.5,-0.4,-0.3,-0.2,-0.2,-0.3,-0.4,-0.5,
              -0.3,-0.2,-0.1,   0,   0,-0.1,-0.2,-0.3,
              -0.3,-0.1, 0.2, 0.3, 0.3, 0.2,-0.1,-0.3,
              -0.3,-0.1, 0.3, 0.4, 0.4, 0.3,-0.1,-0.3,
              -0.3,-0.1, 0.3, 0.4, 0.4, 0.3,-0.1,-0.3,
              -0.3,-0.1, 0.2, 0.3, 0.3, 0.2,-0.1,-0.3,
              -0.3,-0.3,   0,   0,   0,   0,-0.3,-0.3,
              -0.5,-0.3,-0.3,-0.3,-0.3,-0.3,-0.3,-0.5,
            };
          }
        }
        if(board[x][y]->get_field_figure()->get_color() == 1){
          if(number_move < 22){ // в началото искаме да е по-защитен, после в центъра
            double table[8][8] = {
                 0.2, 0.3, 0.1,   0,   0, 0.1, 0.3, 0.2,
                 0.2, 0.2,   0,   0,   0,   0, 0.2, 0.2,
                -0.1,-0.2,-0.2,-0.2,-0.2,-0.2,-0.2,-0.1,
                -0.2,-0.3,-0.3,-0.4,-0.4,-0.3,-0.3,-0.2,
                -0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3,
                -0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3,
                -0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3,
                -0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3,
            };
            return table[x][y];
          }
          else{
            double table[8][8] = {
              -0.5,-0.3,-0.3,-0.3,-0.3,-0.3,-0.3,-0.5,
              -0.3,-0.3,   0,   0,   0,   0,-0.3,-0.3,
              -0.3,-0.1, 0.2, 0.3, 0.3, 0.2,-0.1,-0.3,
              -0.3,-0.1, 0.3, 0.4, 0.4, 0.3,-0.1,-0.3,
              -0.3,-0.1, 0.3, 0.4, 0.4, 0.3,-0.1,-0.3,
              -0.3,-0.1, 0.2, 0.3, 0.3, 0.2,-0.1,-0.3,
              -0.3,-0.2,-0.1,   0,   0,-0.1,-0.2,-0.3,
              -0.5,-0.4,-0.3,-0.2,-0.2,-0.3,-0.4,-0.5,
            };
            return table[x][y];
          }
        }
      }
      throw 1;
      return 0;
    }

    double calculate_white_materials(){ // изчислява дъската за белите
      double counter = 0;
      Figure* p;
      string temp;
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          p = board[i][j]->get_field_figure();
          if(p != nullptr){
            if(p->get_color() == 0){
              temp = p->get_type();
              if(temp == "pawn") counter = counter + 1 + material_adjustment("pawn", i, j);
              if(temp == "bishop") counter = counter + 3 + material_adjustment("bishop", i, j);
              if(temp == "night") counter = counter + 3 + material_adjustment("night", i, j);
              if(temp == "rook") counter = counter + 5 + material_adjustment("rook", i, j);
              if(temp == "queen") counter = counter + 9 + material_adjustment("queen", i, j);
              if(temp == "king") counter = counter + 1000 + material_adjustment("king", i, j);
            }
          }
        }
      }
      return counter;
    }

    double calculate_black_materials(){ // изчислява дъската за черните
      double counter = 0;
      Figure* p;
      string temp;
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          p = board[i][j]->get_field_figure();
          if(p != nullptr){
            if(p->get_color() == 1){
              temp = p->get_type();
              if(temp == "pawn") counter = counter + 1 + material_adjustment("pawn", i, j);
              if(temp == "bishop") counter = counter + 3 + material_adjustment("bishop", i, j);
              if(temp == "night") counter = counter + 3 + material_adjustment("night", i, j);
              if(temp == "rook") counter = counter + 5 + material_adjustment("rook", i, j);
              if(temp == "queen") counter = counter + 9 + material_adjustment("queen", i, j);
              if(temp == "king") counter = counter + 1000 + material_adjustment("king", i, j);
            }
          }
        }
      }
      return counter;
    }

    double evaluate_board(bool who_moving){ // изчислява дъската изцялo
      if(white_mated() == 1) return -1000;
      if(black_mated() == 1) return 1000;
      if(is_draw(who_moving) == 1) return 0;
      double score = 0;
      score = calculate_white_materials();
      double temp = calculate_black_materials();
      if(score - temp > -000.1 && score - temp < 000.1) return 0;
      score -= calculate_black_materials();
      return score;
    }

    
    bool find_if_white_small_castle(){ // метод дали белите могат да направят рокада
      if(white_small_castle == 0) return 0;
      else{
        if(board[7][5]->check_if_figure_or_field_is_protected_or_attacked(board, 0) == 0 &&
           board[7][6]->check_if_figure_or_field_is_protected_or_attacked(board, 0) == 0 &&
           board[7][5]->get_field_figure() == nullptr && board[7][6]->get_field_figure() == nullptr &&
           check_if_WHITE_king_is_attacked(board) == 0) return 1;
           else return 0;
      }
    }
    bool find_if_black_small_castle(){  // метод дали черните могат да направят рокада
      if(black_small_castle == 0) return 0;
      else{
        if(board[0][5]->check_if_figure_or_field_is_protected_or_attacked(board, 1) == 0 &&
           board[0][6]->check_if_figure_or_field_is_protected_or_attacked(board, 1) == 0 &&
           board[0][5]->get_field_figure() == nullptr && board[0][6]->get_field_figure() == nullptr &&
           check_if_WHITE_king_is_attacked(board) == 0) return 1;
           else return 0;
      }
    }
    bool find_if_white_big_castle(){ // метод дали белите могат да направят рокада
      if(white_big_castle == 0) return 0;
      else{
        if(board[7][1]->check_if_figure_or_field_is_protected_or_attacked(board, 0) == 0 &&
           board[7][2]->check_if_figure_or_field_is_protected_or_attacked(board, 0) == 0 &&
           board[7][3]->check_if_figure_or_field_is_protected_or_attacked(board, 0) == 0 &&
           board[7][1]->get_field_figure() == nullptr && board[7][2]->get_field_figure() == nullptr && board[7][3]->get_field_figure() == nullptr &&
           check_if_WHITE_king_is_attacked(board) == 0) return 1;
           else return 0;
      }
    }
    bool find_if_black_big_castle(){ // метод дали черните могат да направят рокада
      if(black_big_castle == 0) return 0;
      else{
        if(board[0][1]->check_if_figure_or_field_is_protected_or_attacked(board, 0) == 0 &&
           board[0][2]->check_if_figure_or_field_is_protected_or_attacked(board, 0) == 0 &&
           board[0][3]->check_if_figure_or_field_is_protected_or_attacked(board, 0) == 0 &&
           board[0][1]->get_field_figure() == nullptr && board[0][2]->get_field_figure() == nullptr && board[0][3]->get_field_figure() == nullptr &&
           check_if_WHITE_king_is_attacked(board) == 0) return 1;
           else return 0;
      }
    }


    void make_move_for_wsc(){ // прави малка рокада за белите
      Point a;
      board[7][6]->set_a_figure(board[7][4]->get_field_figure());
      board[7][5]->set_a_figure(board[7][7]->get_field_figure());
      a.x = 7; a.y = 6;
      board[7][6]->get_field_figure()->set_coordinates(a);
      a.y = 5;
      board[7][5]->get_field_figure()->set_coordinates(a);
      board[7][4]->set_no_figure();
      board[7][7]->set_no_figure();
    }
    void make_move_for_wbc(){ // прави голяма рокада за белите
      Point a;
      board[7][2]->set_a_figure(board[7][4]->get_field_figure());
      board[7][3]->set_a_figure(board[7][0]->get_field_figure());
      a.x = 7; a.y = 2;
      board[7][2]->get_field_figure()->set_coordinates(a);
      a.y = 3;
      board[7][3]->get_field_figure()->set_coordinates(a);
      board[7][4]->set_no_figure();
      board[7][0]->set_no_figure();
    }
    void make_move_for_bsc(){ // прави малка рокада за черните
      board[0][6]->set_a_figure(board[0][4]->get_field_figure());
      board[0][5]->set_a_figure(board[0][7]->get_field_figure());
      board[0][4]->set_no_figure();
      board[0][0]->set_no_figure();
    }
    void make_move_for_bbc(){ // прави малка голяма рокада за черните
      Point a;
      board[0][2]->set_a_figure(board[0][4]->get_field_figure());
      board[0][3]->set_a_figure(board[0][0]->get_field_figure());
      a.x = 0; a.y = 2;
      board[0][2]->get_field_figure()->set_coordinates(a);
      a.y = 3;
      board[0][3]->get_field_figure()->set_coordinates(a);
      board[0][4]->set_no_figure();
      board[0][0]->set_no_figure();
    }
    
    void return_move_for_wsc(){ // връща ходът, помага при алгоритъма търсене на най-оптимален код
      Point a;
      board[7][4]->set_a_figure(board[7][6]->get_field_figure());
      board[7][7]->set_a_figure(board[7][5]->get_field_figure());
      a.x = 7; a.y = 4;
      board[7][4]->get_field_figure()->set_coordinates(a);
      a.y = 7;
      board[7][7]->get_field_figure()->set_coordinates(a);
      board[7][6]->set_no_figure();
      board[7][5]->set_no_figure();
    }
    void return_move_for_wbc(){ // връща ходът, помага при алгоритъма търсене на най-оптимален код
      Point a;
      board[7][4]->set_a_figure(board[7][2]->get_field_figure());
      board[7][0]->set_a_figure(board[7][3]->get_field_figure());
      a.x = 7; a.y = 4;
      board[7][4]->get_field_figure()->set_coordinates(a);
      a.y = 0;
      board[7][0]->get_field_figure()->set_coordinates(a);
      board[7][2]->set_no_figure();
      board[7][3]->set_no_figure();
    }
    void return_move_for_bsc(){ // връща ходът, помага при алгоритъма търсене на най-оптимален код
      Point a;
      board[0][4]->set_a_figure(board[0][6]->get_field_figure());
      board[0][7]->set_a_figure(board[0][5]->get_field_figure());
      a.x = 0; a.y = 4;
      board[0][4]->get_field_figure()->set_coordinates(a);
      a.y = 7;
      board[0][7]->get_field_figure()->set_coordinates(a);
      board[0][6]->set_no_figure();
      board[0][5]->set_no_figure();
    }
    void return_move_for_bbc(){ // връща ходът, помага при алгоритъма търсене на най-оптимален код
      Point a;
      board[0][4]->set_a_figure(board[0][2]->get_field_figure());
      board[0][0]->set_a_figure(board[0][3]->get_field_figure());
      a.x = 0; a.y = 4;
      board[0][4]->get_field_figure()->set_coordinates(a);
      a.y = 0;
      board[0][0]->get_field_figure()->set_coordinates(a);
      board[0][2]->set_no_figure();
      board[0][3]->set_no_figure();
    }


    
    int find_if_enpassant_white_can_capture(int prevplaceX, int prevplaceY, Point now){ // проверява за анпасан за белите
      int counter = 0;
      if(board[now.x][now.y]->get_figure_type() == "pawn" && now.x == 3 && prevplaceX == 1){
        if(now.y - 1 >= 0){
          if(board[now.x][now.y - 1]->get_field_figure_color() == 0 && board[now.x][now.y - 1]->get_figure_type() == "pawn"){
            Figure* p;
            make_move_for_enpassant_white(now.x, now.y - 1, now, p);
            if(check_if_WHITE_king_is_attacked(board) == 1){ // проверява дали царят ще е шах след хода
              now.x--;
              return_move_for_enpassant_white(now, now.x + 1, now.y - 1, p);
            }
            else{
              now.x--;
              return_move_for_enpassant_white(now, now.x + 1, now.y - 1, p);
              counter++;
            }
          }
        }
        if(now.y + 1 < 8){
          if(board[now.x][now.y + 1]->get_field_figure_color() == 0 && board[now.x][now.y + 1]->get_figure_type() == "pawn"){
            Figure* p;
            make_move_for_enpassant_white(now.x, now.y + 1, now, p);
            if(check_if_WHITE_king_is_attacked(board) == 1){
              now.x--;
              return_move_for_enpassant_white(now, now.x + 1, now.y + 1, p);
            }
            else{
              now.x--;
              return_move_for_enpassant_white(now, now.x + 1, now.y + 1, p);
              counter++;
            }
          }
        }
      }
      return counter;
    }
    int find_if_enpassant_black_can_capture(int prevplaceX, int prevplaceY, Point now){ // приверява за анпасан за черните
      int counter = 0;
      if(board[now.x][now.y]->get_figure_type() == "pawn" && now.x == 4 && prevplaceX == 6){
        if(now.y - 1 >= 0){
          if(board[now.x][now.y - 1]->get_field_figure_color() == 1 && board[now.x][now.y - 1]->get_figure_type() == "pawn"){
            Figure* p;
            make_move_for_enpassant_black(now.x, now.y - 1, now, p);
            if(check_if_BLACK_king_is_attacked(board) == 1){
              now.x++;
              return_move_for_enpassant_black(now, now.x - 1, now.y - 1, p);
            }
            else{
              now.x++;
              return_move_for_enpassant_black(now, now.x - 1, now.y - 1, p);
              counter++;
            }
          }
        }
        if(now.y + 1 < 8){
          if(board[now.x][now.y + 1]->get_field_figure_color() == 1 && board[now.x][now.y + 1]->get_figure_type() == "pawn"){
            Figure* p;
            make_move_for_enpassant_black(now.x, now.y + 1, now, p);
            if(check_if_BLACK_king_is_attacked(board) == 1){
              now.x++;
              return_move_for_enpassant_black(now, now.x - 1, now.y + 1, p);
            }
            else{
              now.x++;
              return_move_for_enpassant_black(now, now.x - 1, now.y + 1, p);
              counter++;
            }
          }
        }
      }
      return counter;
    }


    void make_move_for_enpassant_white(int white_placeX, int white_placeY, Point now, Figure* &p){ // помощен метод при алгоритъма
      Point a;
      p = board[now.x][now.y]->get_field_figure();
      board[now.x][now.y]->set_no_figure();
      board[now.x - 1][now.y]->set_a_figure(board[white_placeX][white_placeY]->get_field_figure());
      a.x = now.x - 1; a.y = now.y;
      board[now.x - 1][now.y]->get_field_figure()->set_coordinates(a);
      board[white_placeX][white_placeY]->set_no_figure();
    }
    void make_move_for_enpassant_black(int black_placeX, int black_placeY, Point now, Figure* &p){ // помощен метод при алгоритъма
      Point a;
      p = board[now.x][now.y]->get_field_figure();
      board[now.x][now.y]->set_no_figure();
      board[now.x + 1][now.y]->set_a_figure(board[black_placeX][black_placeY]->get_field_figure());
      a.x = now.x + 1; a.y = now.y;
      board[now.x + 1][now.y]->get_field_figure()->set_coordinates(a);
      board[black_placeX][black_placeY]->set_no_figure();
    }
    
    void return_move_for_enpassant_white(Point from, int to_x, int to_y, Figure* &p){ // помощен метод при алгоритъма
      Point a;
      board[to_x][to_y]->set_a_figure(board[from.x][from.y]->get_field_figure());
      board[from.x][from.y]->set_no_figure();
      a.x = to_x; a.y = to_y;
      board[to_x][to_y]->get_field_figure()->set_coordinates(a);
      board[from.x + 1][from.y]->set_a_figure(p);
    }
    void return_move_for_enpassant_black(Point from, int to_x, int to_y, Figure* &p){ // помощен метод при алгоритъма
      Point a;
      board[to_x][to_y]->set_a_figure(board[from.x][from.y]->get_field_figure());
      board[from.x][from.y]->set_no_figure();
      a.x = to_x; a.y = to_y;
      board[to_x][to_y]->get_field_figure()->set_coordinates(a);
      board[from.x - 1][from.y]->set_a_figure(p);
    }

    void change_whos_turn(){ // променя кой е на ход
      whos_move = !whos_move;
    }

    void set_strarting_board(bool number_of_playersPLAYING){ // нарежда начална позиция
      previous_place.x = -1;
      previous_place.y = -1;
      current_place.x = -1;
      current_place.y = -1;
      number_move = 1;
      choose_figures = 0;
      whos_move = 0;
      white_small_castle = 1;
      black_small_castle = 1;
      white_big_castle = 1;
      black_big_castle = 1;
      players = number_of_playersPLAYING;
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          board[i][j]->del_figure(); // изтрива, ако е имало игра преди това
        }
      }
      // нарежда останалите фигури
      for(int i = 2; i < 6; i++){
        for(int j = 0; j < 8; j++){
          board[i][j]->set_no_figure();
        }
      }
      board[0][0]->set_rook(1);
      board[0][1]->set_night(1);
      board[0][2]->set_bishop(1);
      board[0][3]->set_queen(1);
      board[0][4]->set_king(1);
      board[0][5]->set_bishop(1);
      board[0][6]->set_night(1);
      board[0][7]->set_rook(1);
      board[1][0]->set_pawn(1);
      board[1][1]->set_pawn(1);
      board[1][2]->set_pawn(1);
      board[1][3]->set_pawn(1);
      board[1][4]->set_pawn(1);
      board[1][5]->set_pawn(1);
      board[1][6]->set_pawn(1);
      board[1][7]->set_pawn(1);

      board[7][0]->set_rook(0);
      board[7][1]->set_night(0);
      board[7][2]->set_bishop(0);
      board[7][3]->set_queen(0);
      board[7][4]->set_king(0);
      board[7][5]->set_bishop(0);
      board[7][6]->set_night(0);
      board[7][7]->set_rook(0);
      board[6][0]->set_pawn(0);
      board[6][1]->set_pawn(0);
      board[6][2]->set_pawn(0);
      board[6][3]->set_pawn(0);
      board[6][4]->set_pawn(0);
      board[6][5]->set_pawn(0);
      board[6][6]->set_pawn(0);
      board[6][7]->set_pawn(0);
    }

    void set_saved_game(){ // задава запазената последно игра
      ifstream ifs("prev_game", ios::binary);
      string saved_game;
      ifs >> saved_game;
      int counter = 0, row = 0, column = 0;
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          board[i][j]->del_figure();
        }
      }
      for(int i = 0; i < saved_game.length(); i++){
        if(saved_game[i] == '/'){row++; column = 0;}
        else{
          if(saved_game[i] > '0' && saved_game[i] < '9'){
            for(int j = 0; j < int(saved_game[i] - '0'); j++){
              board[row][column]->set_no_figure();
              column++;
            }
          }
          else{
            switch (saved_game[i])
            {
            case 'r':
              board[row][column]->set_rook(1);
              column++;
              break;

            case 'b':
              board[row][column]->set_bishop(1);
              column++;
              break;

            case 'n':
              board[row][column]->set_night(1);
              column++;
              break;

            case 'q':
              board[row][column]->set_queen(1);
              column++;
              break;

            case 'k':
              board[row][column]->set_king(1);
              column++;
              break;

            case 'p':
              board[row][column]->set_pawn(1);
              column++;
              break;

            case 'R':
              board[row][column]->set_rook(0);
              column++;
              break;

            case 'B':
              board[row][column]->set_bishop(0);
              column++;
              break;

            case 'N':
              board[row][column]->set_night(0);
              column++;
              break;

            case 'Q':
              board[row][column]->set_queen(0);
              column++;
              break;

            case 'K':
              board[row][column]->set_king(0);
              column++;
              break;

            case 'P':
              board[row][column]->set_pawn(0);
              column++;
              break;
            
            default:
              throw "asdf";
              break;
            }
          }
        }
      }
      ifs >> choose_figures >> whos_move >> number_move >> white_small_castle >> white_big_castle >> black_small_castle >>
          black_big_castle >> previous_place.x >> previous_place.y >> current_place.x >> current_place.y;
    }

    void print_board(){ // извежда дъската
      cout << endl;
      int a, b;
      string temp = "";
      for(int i = 0; i < 8; i++){
        cout << 8 - i << " ";
        for(int j = 0; j < 8; j++){
            if(board[i][j]->get_field_figure() != nullptr){
              if(board[i][j]->get_field_figure()->get_color() == 0) temp.append(" W");
              else temp.append("  B");
              // cout << board[i][j]->get_field_figure()->get_type() << " ";
              temp.append(board[i][j]->get_field_figure()->get_type());
              for(int i =  temp.length(); i < 9; i++){
                temp.append(" ");
              }
              cout << temp;
              temp = "";
            }
            else cout << "    *    ";
        }
        cout << endl << endl;
      }
      cout << " ";
      for(int i = 0; i < 8; i++){
        cout << "    " << char(i + 'a') << "    ";
      }
      cout << endl;
    }

    void save_game_to_file()const{ // запазва позицията на моментната игра 
      ofstream ofs("prev_game", ios::binary);
      int counter = 0;
      string save = "";
      Figure* p;
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          p = board[i][j]->get_field_figure(); 
          if(p == nullptr){counter++;}
          else{
            if(counter != 0) save.push_back('0' + counter);
            if(p->get_color() == 0){
              if(p->get_type() == "rook") save.push_back('R');
              if(p->get_type() == "night") save.push_back('N');
              if(p->get_type() == "bishop") save.push_back('B');
              if(p->get_type() == "queen") save.push_back('Q');
              if(p->get_type() == "king") save.push_back('K');
              if(p->get_type() == "pawn") save.push_back('P');
            }
            if(p->get_color() == 1){
              if(p->get_type() == "rook") save.push_back('r');
              if(p->get_type() == "night") save.push_back('n');
              if(p->get_type() == "bishop") save.push_back('b');
              if(p->get_type() == "queen") save.push_back('q');
              if(p->get_type() == "king") save.push_back('k');
              if(p->get_type() == "pawn") save.push_back('p');
            }
            counter = 0;
          }
        }
        if(counter != 0) save.push_back('0' + counter);
        counter = 0;
        if(i != 7) save.push_back('/');
      }
      ofs << save << " ";
      ofs << choose_figures << " " << whos_move << " " << number_move << " " << white_small_castle << " " << white_big_castle << " " << black_small_castle << " " 
          << black_big_castle << " " << previous_place.x << " " << previous_place.y << " " << current_place.x << " " << current_place.y;
    }

    string get_game_string(){ // връща позицията на играта в стринг
      int counter = 0;
      string save = "";
      Figure* p;
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          p = board[i][j]->get_field_figure(); 
          if(p == nullptr){counter++;}
          else{
            if(counter != 0) save.push_back('0' + counter);
            if(p->get_color() == 0){
              if(p->get_type() == "rook") save.push_back('R');
              if(p->get_type() == "night") save.push_back('N');
              if(p->get_type() == "bishop") save.push_back('B');
              if(p->get_type() == "queen") save.push_back('Q');
              if(p->get_type() == "king") save.push_back('K');
              if(p->get_type() == "pawn") save.push_back('P');
            }
            if(p->get_color() == 1){
              if(p->get_type() == "rook") save.push_back('r');
              if(p->get_type() == "night") save.push_back('n');
              if(p->get_type() == "bishop") save.push_back('b');
              if(p->get_type() == "queen") save.push_back('q');
              if(p->get_type() == "king") save.push_back('k');
              if(p->get_type() == "pawn") save.push_back('p');
            }
            counter = 0;
          }
        }
        if(counter != 0) save.push_back('0' + counter);
        counter = 0;
        if(i != 7) save.push_back('/');
      }
      return save;
    }

    bool white_mated(){ // проверява дали белите са мат
      Figure* k;
      Point king_pos;
      int n;
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          k = board[i][j]->get_field_figure();
          if(k != nullptr){
            if(k->get_color() == 0){
              if(k->get_type() == "king") king_pos = k->get_coordinates();
              else{
                n = k->get_number_of_position_to_go(board);
                if(n != 0) {
                  return false;
                }
              }
            }
          }
        }
      }
      k = board[king_pos.x][king_pos.y]->get_field_figure();
      n = k->get_number_of_position_to_go(board);
      if(n == 0 && check_if_BLACK_king_is_attacked(board) == 1) return true;
      else return false;
    }

    bool black_mated(){ // проверява дали черните са мат
      Figure* k;
      Point king_pos;
      int n;
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          k = board[i][j]->get_field_figure();
          if(k != nullptr){
            if(k->get_color() == 1){
              if(k->get_type() == "king") king_pos = k->get_coordinates();
              else{
                n = k->get_number_of_position_to_go(board);
                if(n != 0) return false;
              }
            }
          }
        }
      }
      k = board[king_pos.x][king_pos.y]->get_field_figure();
      n = k->get_number_of_position_to_go(board);
      if(n == 0 && check_if_BLACK_king_is_attacked(board) == 1) return true;
      else return false;
    }

    bool is_draw(bool who_moving){ // проверява дали е равенство
      bool flag1 = 0, flag2 = 0;
      Figure* k;
      Point king_pos;
      int n;
      if(who_moving == 0){
        for(int i = 0; i < 8; i++){
          for(int j = 0; j < 8; j++){
            k = board[i][j]->get_field_figure();
            if(k != nullptr){
              if(k->get_color() == 0){
                if(k->get_type() == "king") king_pos = k->get_coordinates();
                else{
                  flag1 = 1;
                  n = k->get_number_of_position_to_go(board);
                  if(n != 0) return false; // ако има възможен ход с някоя друга фигура връща не
                }
              }
              if(k->get_color() == 1 && k->get_type() != "king") flag2 = 1; // проверява дали единствената фигура е само царят от другия цвят
            }
          }
        }
        k = board[king_pos.x][king_pos.y]->get_field_figure();
        n = k->get_number_of_position_to_go(board);
        if(n == 0 && check_if_BLACK_king_is_attacked(board) == 0) return true; // ако няма възможни ходове с другите фигури и с царя и не той не е атакуван връща реми
      }
      else{
        for(int i = 0; i < 8; i++){
          for(int j = 0; j < 8; j++){
            k = board[i][j]->get_field_figure();
            if(k != nullptr){
              if(k->get_color() == 1){
                if(k->get_type() == "king") king_pos = k->get_coordinates();
                else{
                  flag2 = 1;
                  n = k->get_number_of_position_to_go(board);
                  if(n != 0) return false;
                }
              }
              if(k->get_color() == 0 && k->get_type() != "king") flag1 = 1;
            }
          }
        }
        k = board[king_pos.x][king_pos.y]->get_field_figure();
        n = k->get_number_of_position_to_go(board);
        if(who_moving == 0){
          if(n == 0 && check_if_WHITE_king_is_attacked(board) == 0) return true;
        }
        else{
          if(n == 0 && check_if_BLACK_king_is_attacked(board) == 0) return true;
        }
      }
      if(flag1 == 0 && flag2 == 0) return true; // ако са останали само царете връща реми
      else return false;
    }

    bool game_over(bool whosmove){
      if(white_mated() == 1) return 1;
      if(black_mated() == 1) return 1;
      if(is_draw(whosmove) == 1) return 1;
      return 0;
    }
    
    void make_move_for_algorithm(int x, int y, Point to_field, Figure* &p){ // помощен метод за алгоритъма, прави ход и запазва фигурата, ако е взета
      p = board[to_field.x][to_field.y]->get_field_figure();
      board[to_field.x][to_field.y]->set_a_figure(board[x][y]->get_field_figure());
      board[x][y]->set_no_figure();
      board[to_field.x][to_field.y]->get_field_figure()->set_coordinates(to_field);
    }

    void return_the_move(Point from_next, int x, int y, Figure* &p){ // помощен метод за алгоритъма, прави ход и запазва фигурата, ако е взета
      board[x][y]->set_a_figure(board[from_next.x][from_next.y]->get_field_figure());
      Point a; a.x = x; a.y = y;
      board[x][y]->get_field_figure()->set_coordinates(a);
      board[from_next.x][from_next.y]->set_a_figure(p);
    }

    best_move_struct find_from_saved_boards(){ // за първите 2 хода има готова ход за НЯКОИ позиции
      best_move_struct returning;
      returning.number_of_moves = -1;
      returning.from_place.x = 8;
      string temp = "";
      int killer = 0;
      if(number_move < 3){
        if(choose_figures == 1) temp = "White";
        else temp = "Black";
        temp.append("_move/");
        temp.append(1, char(number_move + '0'));
        temp.append("_move");
      }
      else return returning;
      ifstream getting(temp, ios::binary);
      string game_tag_file = "";
      string game_tag_now = get_game_string();
      while(game_tag_file != "!"){
        getting >> game_tag_file;
        if(game_tag_file == game_tag_now){
          getting >> returning.from_place.x >> returning.from_place.y >> returning.to_place.x >> returning.to_place.y;
          returning.new_figure = 'z';
          returning.number_of_moves = 1;
          returning.value = 0;
          break;
        }
        else{
          if(game_tag_file != "!"){
            getting >> killer >> killer >> killer >> killer;
          }
        }
        if(game_tag_file == "!" && returning.number_of_moves == -1){
          getting >> returning.from_place.x >> returning.from_place.y >> returning.to_place.x >> returning.to_place.y;
          returning.new_figure = 'z';
          returning.number_of_moves = 0;
          returning.value = 0;
        }
      }
      return returning;
    }

    best_move_struct minimax_algorithm(int prev_x, int prev_y, Point to_move, bool who_moves, int debth){ //алгоритъмът за намиране на най-оптиманлия ход
      best_move_struct p;
      double besti = 0;
      best_move_struct to_return;
      Figure* temp = nullptr;
      bool checker_flag = 0;
      if(debth != 0){ // проверява за анпасан 1во, ако има ходове
        if(who_moves == 0){
          if(number_move != 1){ // ако не е 1вият ход продължава
          if(find_if_enpassant_white_can_capture(prev_x, prev_y, to_move) == 1){ // ако има възможност за анпасан, я играе
            besti = -10000;
            if(to_move.y - 1 >= 0){
              if(board[to_move.x][to_move.y - 1]->get_field_figure_color() == 0 && board[to_move.x][to_move.y - 1]->get_figure_type() == "pawn"){ // проверява дали може да вземе отляво
                make_move_for_enpassant_white(to_move.x, to_move.y - 1, to_move, temp);
                to_move.x--;
                p = minimax_algorithm(to_move.x + 1, to_move.y - 1, to_move, !who_moves, debth - 1);
                return_move_for_enpassant_white(to_move, to_move.x + 1, to_move.y - 1, temp);
                  if(p.value > besti){ // проверява дали това по-добър ход
                    besti = p.value;
                    to_return.value = besti;
                    to_return.from_place.x = to_move.x + 1;
                    to_return.from_place.y = to_move.y - 1;
                    to_return.to_place = to_move;
                    to_return.number_of_moves = p.number_of_moves;
                    to_return.new_figure = p.new_figure;
                  }
                  else{
                    if(p.value - to_return.number_of_moves < 0.001 && p.value - to_return.number_of_moves > -0.001){
                      if(p.number_of_moves > to_return.number_of_moves){
                        besti = p.value;
                        to_return.value = besti;
                        to_return.from_place.x = to_move.x + 1;
                        to_return.from_place.y = to_move.y - 1;
                        to_return.to_place = to_move;
                        to_return.number_of_moves = p.number_of_moves;
                        to_return.new_figure = p.new_figure;
                      }
                    }
                  }
                  to_move.x++;
                  checker_flag = 1;
                }
              }
            if(to_move.y + 1 < 8){
              if(board[to_move.x][to_move.y + 1]->get_field_figure_color() == 0 && board[to_move.x][to_move.y + 1]->get_figure_type() == "pawn"){ // проверява дали може да вземе отдясно
                make_move_for_enpassant_white(to_move.x, to_move.y + 1, to_move, temp);
                to_move.x--;
                p = minimax_algorithm(to_move.x + 1, to_move.y + 1, to_move, !who_moves, debth - 1);
                return_move_for_enpassant_white(to_move, to_move.x + 1, to_move.y + 1, temp);
                if(p.value > besti){
                  besti = p.value;
                  to_return.value = besti;
                  to_return.from_place.x = to_move.x + 1;
                  to_return.from_place.y = to_move.y + 1;
                  to_return.to_place = to_move;
                  to_return.number_of_moves = p.number_of_moves;
                  to_return.new_figure = p.new_figure;
                }
                else{
                  if(p.value - to_return.number_of_moves < 0.001 && p.value - to_return.number_of_moves > -0.001){
                    if(p.number_of_moves > to_return.number_of_moves){
                      besti = p.value;
                      to_return.value = besti;
                      to_return.from_place.x = to_move.x + 1;
                      to_return.from_place.y = to_move.y + 1;
                      to_return.to_place = to_move;
                      to_return.number_of_moves = p.number_of_moves;
                      to_return.new_figure = p.new_figure;
                    }
                  }
                }
                to_move.x++;
                checker_flag = 1;
              }
            }
          }
          }
        }
        else{ // същото прави и за черните
          if(number_move != 1){
          if(find_if_enpassant_black_can_capture(prev_x, prev_y, to_move) == 1){
            besti = 10000;
            if(to_move.y - 1 >= 0){
              if(board[to_move.x][to_move.y - 1]->get_field_figure_color() == 1 && board[to_move.x][to_move.y - 1]->get_figure_type() == "pawn"){
                make_move_for_enpassant_black(to_move.x, to_move.y - 1, to_move, temp);
                to_move.x++;
                p = minimax_algorithm(to_move.x - 1, to_move.y - 1, to_move, !who_moves, debth - 1);
                return_move_for_enpassant_black(to_move, to_move.x - 1, to_move.y - 1, temp);
                  if(p.value > besti){
                    besti = p.value;
                    to_return.value = besti;
                    to_return.from_place.x = to_move.x - 1;
                    to_return.from_place.y = to_move.y - 1;
                    to_return.to_place = to_move, to_return.new_figure = p.new_figure;
                  }
                  else{
                    if(p.value - to_return.number_of_moves < 0.001 && p.value - to_return.number_of_moves > -0.001){
                      if(p.number_of_moves > to_return.number_of_moves){
                        besti = p.value;
                        to_return.value = besti;
                        to_return.from_place.x = to_move.x - 1;
                        to_return.from_place.y = to_move.y - 1;
                        to_return.to_place = to_move;
                        to_return.number_of_moves = p.number_of_moves;
                        to_return.new_figure = p.new_figure;
                      }
                    }
                  }
                  to_move.x--;
                  checker_flag = 1;
              }
            }
            if(to_move.y + 1 < 8){
              if(board[to_move.x][to_move.y + 1]->get_field_figure_color() == 1 && board[to_move.x][to_move.y + 1]->get_figure_type() == "pawn"){
                make_move_for_enpassant_black(to_move.x, to_move.y + 1, to_move, temp);
                to_move.x++;
                p = minimax_algorithm(to_move.x - 1, to_move.y + 1, to_move, !who_moves, debth - 1);
                return_move_for_enpassant_black(to_move, to_move.x - 1, to_move.y + 1, temp);
                if(p.value > besti){
                  besti = p.value;
                  to_return.value = besti;
                  to_return.from_place.x = to_move.x - 1;
                  to_return.from_place.y = to_move.y + 1;
                  to_return.to_place = to_move, to_return.new_figure = p.new_figure;
                }
                else{
                  if(p.value - to_return.number_of_moves < 0.001 && p.value - to_return.number_of_moves > -0.001){
                    if(p.number_of_moves > to_return.number_of_moves){
                      besti = p.value;
                      to_return.value = besti;
                      to_return.from_place.x = to_move.x - 1;
                      to_return.from_place.y = to_move.y + 1;
                      to_return.to_place = to_move;
                      to_return.number_of_moves = p.number_of_moves;
                      to_return.new_figure = p.new_figure;
                    }
                  }
                }
                to_move.x--;
                checker_flag = 1;
              }
            }
          }
          }
        }
      }

      if((game_over(who_moves) == 1 || debth == 0) && checker_flag == 0){ // проверява дали и дъното на търсенето и дали анпасан не се е играл
        p.from_place.x = prev_x; p.from_place.y = prev_y; p.to_place.x = to_move.x; p.to_place.y = to_move.y, p.number_of_moves = debth;
        p.value = evaluate_board(who_moves);
        return p;
      }
      Point temo;
      Figure* l;
      int n;
      bool flags = 0, flagb = 0;
      if(who_moves == 0){
        if(checker_flag == 0) besti = -10000;
        for(int i = 0; i < 8; i++){
          for(int j = 0; j < 8; j++){
            l = board[i][j]->get_field_figure();
            if(l != nullptr){
              if(l->get_color() == 0){//проверява за бели фигури

                if(l->get_type() == "king"){//ще разгледа възможността за рокада - голяма и малка и ако има я играе
                  if(find_if_white_small_castle() == 1){
                    temo.x = 7; temo.y = 6;
                    make_move_for_wsc();
                    p = minimax_algorithm(7, 4, temo, !who_moves, debth - 1);
                    return_move_for_wsc();
                    if(p.value > besti) besti = p.value, to_return.value = besti, to_return.from_place.x = 7, to_return.from_place.y = 4, to_return.to_place = temo, to_return.new_figure = p.new_figure;
                  }
                  if(find_if_white_big_castle() == 1){
                    temo.x = 7; temo.y = 2;
                    make_move_for_wbc();
                    p = minimax_algorithm(7, 4, temo, !who_moves, debth - 1);
                    return_move_for_wbc();
                    if(p.value > besti) besti = p.value, to_return.value = besti, to_return.from_place.x = 7, to_return.from_place.y = 2, to_return.to_place = temo, to_return.new_figure = p.new_figure;
                  }
                }
                
                n = l->get_number_of_position_to_go(board);
                Point* moves = new Point[n];
                moves = l->get_field_coordinates_to_go(n, board);

                  for(int h = 0; h < n; h++){//цикли през всички възможни ходове на тази фигура

                    if(l->get_type() == "pawn" && i == 1){//гледа дали пешката може да се превърне в друга фигура и разглежда всеки случай
                      make_move_for_algorithm(i, j, moves[h], temp);
                      Figure* m = board[moves[h].x][moves[h].y]->get_field_figure();
                      board[moves[h].x][moves[h].y]->set_queen(0);
                      p = minimax_algorithm(i - 1, j, moves[h], !who_moves, debth - 1);
                      board[moves[h].x][moves[h].y]->del_figure();
                      board[moves[h].x][moves[h].y]->set_a_figure(m);
                      return_the_move(moves[h], i, j, temp);
                      if(p.value > besti) besti = p.value, to_return.value = besti, to_return.from_place.x = i, to_return.from_place.y = j, to_return.to_place = moves[h], to_return.new_figure = 'Q';

                      make_move_for_algorithm(i, j, moves[h], temp);
                      m = board[moves[h].x][moves[h].y]->get_field_figure();
                      board[moves[h].x][moves[h].y]->set_rook(0);
                      p = minimax_algorithm(i, j, moves[h], !who_moves, debth - 1);
                      board[moves[h].x][moves[h].y]->del_figure();
                      board[moves[h].x][moves[h].y]->set_a_figure(m);
                      return_the_move(moves[h], i, j, temp);
                      if(p.value > besti) besti = p.value, to_return.value = besti, to_return.from_place.x = i, to_return.from_place.y = j, to_return.to_place = moves[h], to_return.new_figure = 'R';

                      make_move_for_algorithm(i, j, moves[h], temp);
                      m = board[moves[h].x][moves[h].y]->get_field_figure();
                      board[moves[h].x][moves[h].y]->set_night(0);
                      p = minimax_algorithm(i, j, moves[h], !who_moves, debth - 1);
                      board[moves[h].x][moves[h].y]->del_figure();
                      board[moves[h].x][moves[h].y]->set_a_figure(m);
                      return_the_move(moves[h], i, j, temp);
                      if(p.value > besti) besti = p.value, to_return.value = besti, to_return.from_place.x = i, to_return.from_place.y = j, to_return.to_place = moves[h], to_return.new_figure = 'N';

                      make_move_for_algorithm(i, j, moves[h], temp);
                      m = board[moves[h].x][moves[h].y]->get_field_figure();
                      board[moves[h].x][moves[h].y]->set_bishop(0);
                      p = minimax_algorithm(i, j, moves[h], !who_moves, debth - 1);
                      board[moves[h].x][moves[h].y]->del_figure();
                      board[moves[h].x][moves[h].y]->set_a_figure(m);
                      return_the_move(moves[h], i, j, temp);
                      if(p.value > besti) besti = p.value, to_return.value = besti, to_return.from_place.x = i, to_return.from_place.y = j, to_return.to_place = moves[h], to_return.new_figure = 'B';
                    }
                    else{//всички останали случаи
                      if(white_big_castle == 1 && (l->get_type() == "king" || (l->get_type() == "rook" && i == 7 && j == 0))){//премахва възможността за рокада
                        white_big_castle = 0;
                        flagb = 1;
                      }
                      if(white_small_castle == 1 && (l->get_type() == "king" || (l->get_type() == "rook" && i == 7 && j == 7))){//премахва възможността за рокада
                        white_small_castle = 0;
                        flags = 1;
                      }

                      make_move_for_algorithm(i, j, moves[h], temp);
                      p = minimax_algorithm(i, j, moves[h], !who_moves, debth - 1);
                      return_the_move(moves[h], i, j, temp);

                      if(flagb == 1){//връща я
                        white_big_castle = 1;
                        flagb = 0;
                      }
                      if(flags == 1){//връща я
                        white_small_castle = 1;
                        flags = 0;
                      }
                      //променя най-добрият ход, ако е открил такъв
                      if(p.value > besti){besti = p.value; to_return.value = besti; to_return.from_place.x = i; to_return.from_place.y = j; to_return.to_place = moves[h], to_return.new_figure = p.new_figure, to_return.number_of_moves = p.number_of_moves;}
                      else{
                        if(p.value - to_return.number_of_moves < 0.001 && p.value - to_return.number_of_moves > -0.001){
                          if(p.number_of_moves > to_return.number_of_moves){
                            besti = p.value, to_return.value = besti, to_return.from_place.x = i, to_return.from_place.y = j, to_return.to_place = moves[h], to_return.number_of_moves = p.number_of_moves, to_return.new_figure = p.new_figure;
                          }
                        }
                      }
                    }
                  }
                delete[] moves;
              }
            }
          }
        }
      }
      else{//същата последователност като за белите
      if(checker_flag == 0) besti = 10000;
        for(int i = 0; i < 8; i++){
          for(int j = 0; j < 8; j++){
            l = board[i][j]->get_field_figure();
            if(l != nullptr){
              if(l->get_color() == 1){

                if(l->get_type() == "king"){
                  if(find_if_black_small_castle() == 1){
                    temo.x = 0; temo.y = 6;
                    make_move_for_bsc();
                    p = minimax_algorithm(0, 4, temo, !who_moves, debth - 1);
                    return_move_for_bsc();
                    if(p.value > besti) besti = p.value, to_return.value = besti, to_return.from_place.x = 0, to_return.from_place.y = 4, to_return.to_place = temo, to_return.new_figure = p.new_figure;
                  }
                  if(find_if_black_big_castle() == 1){
                    temo.x = 0; temo.y = 2;
                    make_move_for_bbc();
                    p = minimax_algorithm(0, 4, temo, !who_moves, debth - 1);
                    return_move_for_bbc();
                    if(p.value > besti) besti = p.value, to_return.value = besti, to_return.from_place.x = 0, to_return.from_place.y = 2, to_return.to_place = temo, to_return.new_figure = p.new_figure;
                  }
                }

                n = l->get_number_of_position_to_go(board);
                Point* moves = new Point[n];
                moves = l->get_field_coordinates_to_go(n, board);

                if(n != 0){
                  for(int h = 0; h < n; h++){

                    if(l->get_type() == "pawn" && i == 6){
                      make_move_for_algorithm(i, j, moves[h], temp);
                      Figure* m = board[moves[h].x][moves[h].y]->get_field_figure();
                      board[moves[h].x][moves[h].y]->set_queen(1);
                      p = minimax_algorithm(i, j, moves[h], !who_moves, debth - 1);
                      board[moves[h].x][moves[h].y]->del_figure();
                      board[moves[h].x][moves[h].y]->set_a_figure(m);
                      return_the_move(moves[h], i, j, temp);
                      if(p.value > besti) besti = p.value, to_return.value = besti, to_return.from_place.x = i, to_return.from_place.y = j, to_return.to_place = moves[h], to_return.new_figure = 'q';

                      make_move_for_algorithm(i, j, moves[h], temp);
                      m = board[moves[h].x][moves[h].y]->get_field_figure();
                      board[moves[h].x][moves[h].y]->set_rook(1);
                      p = minimax_algorithm(i, j, moves[h], !who_moves, debth - 1);
                      board[moves[h].x][moves[h].y]->del_figure();
                      board[moves[h].x][moves[h].y]->set_a_figure(m);
                      return_the_move(moves[h], i, j, temp);
                      if(p.value > besti) besti = p.value, to_return.value = besti, to_return.from_place.x = i, to_return.from_place.y = j, to_return.to_place = moves[h], to_return.new_figure = 'r';

                      make_move_for_algorithm(i, j, moves[h], temp);
                      m = board[moves[h].x][moves[h].y]->get_field_figure();
                      board[moves[h].x][moves[h].y]->set_night(1);
                      p = minimax_algorithm(i, j, moves[h], !who_moves, debth - 1);
                      board[moves[h].x][moves[h].y]->del_figure();
                      board[moves[h].x][moves[h].y]->set_a_figure(m);
                      return_the_move(moves[h], i, j, temp);
                      if(p.value > besti) besti = p.value, to_return.value = besti, to_return.from_place.x = i, to_return.from_place.y = j, to_return.to_place = moves[h], to_return.new_figure = 'n';

                      make_move_for_algorithm(i, j, moves[h], temp);
                      m = board[moves[h].x][moves[h].y]->get_field_figure();
                      board[moves[h].x][moves[h].y]->set_bishop(1);
                      p = minimax_algorithm(i, j, moves[h], !who_moves, debth - 1);
                      board[moves[h].x][moves[h].y]->del_figure();
                      board[moves[h].x][moves[h].y]->set_a_figure(m);
                      return_the_move(moves[h], i, j, temp);
                      if(p.value > besti) besti = p.value, to_return.value = besti, to_return.from_place.x = i, to_return.from_place.y = j, to_return.to_place = moves[h], to_return.new_figure = 'b';
                    }

                    else{
                      if(black_big_castle == 1 && (l->get_type() == "king" || (l->get_type() == "rook" && i == 0 && j == 0))){
                        black_big_castle = 0;
                        flagb = 1;
                      }
                      if(black_small_castle == 1 && (l->get_type() == "king" || (l->get_type() == "rook" && i == 0 && j == 7))){
                        black_small_castle = 0;
                        flags = 1;
                      }

                      make_move_for_algorithm(i, j, moves[h], temp);
                      p = minimax_algorithm(i, j, moves[h], !who_moves, debth - 1);
                      return_the_move(moves[h], i, j, temp);

                      if(flagb == 1){
                        black_big_castle = 1;
                        flagb = 0;
                      }
                      if(flags == 1){
                        black_small_castle = 1;
                        flags = 0;
                      }

                      if(p.value < besti) {besti = p.value, to_return.value = besti, to_return.from_place.x = i, to_return.from_place.y = j, to_return.to_place = moves[h], to_return.new_figure = p.new_figure, to_return.number_of_moves = p.number_of_moves;}
                      else{
                        if(p.value - to_return.number_of_moves < 0.001 && p.value - to_return.number_of_moves > -0.001){
                          if(p.number_of_moves > to_return.number_of_moves){
                            besti = p.value, to_return.value = besti, to_return.from_place.x = i, to_return.from_place.y = j, to_return.to_place = moves[h], to_return.number_of_moves = p.number_of_moves, to_return.new_figure = p.new_figure;
                          }
                        }
                      }
                    }
                  }
                }
                delete[] moves;
              }
            }
          }
        }
      }
      return to_return;
    }

    best_move_struct find_best_move(int n){ // метод за намиране на най-добрия ход, който извиква алгоритъма
      best_move_struct p;
      p = find_from_saved_boards();
      if(p.from_place.x != -1 && p.from_place.x != 8) return p;
      p = minimax_algorithm(previous_place.x, previous_place.y, current_place, whos_move, n);
      return p;
    }
  
    
    void start_loop(bool &flag_to_stop_not_ended_game){//add the needed things
      string p = "";
      char temp = '9';
      cout << "To stop the game while choosing enter 9. To save the game enter:  save " << endl;
      do
      {
        cout << "Choose game mode. 0 play vs computer || 1 play vs another player" << endl;
        cin >> p;
        temp = p.at(0);
        if(temp == '9') return;
      } while (temp < '0' || temp > '1');
      players = temp - '0';

      do
      {
        cout << "Choose new game or saved game. 0 - new game || 1 saved game" << endl;
        cin >> p;
        temp = p.at(0);
        if(temp == '9') return;
      } while (temp < '0' || temp > '1');

      if(temp == '0') set_strarting_board(players);
      else set_saved_game();

      if(temp == '0' && players == 0){
        do
        {
          cout << "Choose color to play with. 0 - white || 1 - black" << endl;
          cin >> p;
          temp = p.at(0);
          if(temp == '9') return;
        } while (temp < '0' || temp > '1');
        choose_figures = temp - '0';
      }



      int n = 0, k = 0;
      p = "";
      Point from_position, to_position;
      bool game_stopper = 0;
      bool wbc = 0, wsc = 0, bbc = 0, bsc = 0;
      int enpass_white = 0, enpass_black = 0;
      while (game_stopper == 0)
      {
        if(whos_move == 0){// белите да играят
          if(players == 1 || choose_figures == 0){
            enpass_white = 0, wbc = 0, wsc = 0;
            print_board();
            do{
              k = 2;
              do{
                do{
                  cout << endl << "Choose a square with your figure:  ";
                  cin >> from_position;
                  if(from_position.x == -1 || from_position.y == -1){
                    flag_to_stop_not_ended_game = 1;
                    return;
                  }
                } while (board[from_position.x][from_position.y]->get_field_figure() == nullptr || board[from_position.x][from_position.y]->get_field_figure_color() != whos_move);
                n = board[from_position.x][from_position.y]->get_field_figure()->get_number_of_position_to_go(board);
                if(number_move > 1){
                  if(board[from_position.x][from_position.y]->get_field_figure()->get_type() == "pawn" && from_position.x == current_place.x &&
                      (current_place.y + 1 == from_position.y || current_place.y - 1 == from_position.y)){
                    enpass_white = find_if_enpassant_white_can_capture(previous_place.x, previous_place.y, current_place);
                    n += enpass_white;
                  }
                }
                if(board[from_position.x][from_position.y]->get_field_figure()->get_type() == "king"){
                  wbc = find_if_white_big_castle(); 
                  wsc = find_if_white_small_castle();
                  n += wbc;
                  n += wsc;
                }
                if(n == 0){
                  cout << endl << "No possible moves with this figure: " << endl;
                }
              }while(n == 0);
              Point* fields_to_go = new Point[n];
              fields_to_go = board[from_position.x][from_position.y]->get_field_figure()->get_field_coordinates_to_go(n, board);
              if(wbc == 1){
                fields_to_go[n - 1].x = 7;
                fields_to_go[n - 1].y = 2;
                if(wsc == 1){
                  fields_to_go[n - 2].x = 7;
                  fields_to_go[n - 2].y = 6;
                }
              }
              else{
                if(wsc == 1){
                  fields_to_go[n - 1].x = 7;
                  fields_to_go[n - 1].y = 6;
                }
              }
              if(enpass_white == 2){
                fields_to_go[n - 1].x = from_position.x - 1;
                fields_to_go[n - 1].y = from_position.y - 1;
                fields_to_go[n - 2].x = from_position.x - 1;
                fields_to_go[n - 2].y = from_position.y + 1;
              }
              else{
                if(enpass_white == 1){
                  if(current_place.x == from_position.x && current_place.y + 1 == from_position.y){
                    fields_to_go[n - 1].x = from_position.x - 1;
                    fields_to_go[n - 1].y = from_position.y - 1;
                  }
                  if(current_place.x == from_position.x && current_place.y - 1 == from_position.y){
                    fields_to_go[n - 1].x = from_position.x - 1;
                    fields_to_go[n - 1].y = from_position.y + 1;
                    }
                }
              }

              for(int i = 0; i < n; i++){
                cout << fields_to_go[i];
                if(i != n - 1)cout << ", ";
              }
              do{
                cout << endl << "Choose square to go from above: ";
                cin >> to_position;
                if(from_position.x == -1 || from_position.y == -1){
                    flag_to_stop_not_ended_game = 1;
                    return;
                  }
                for(int i = 0; i < n; i++){
                  if(to_position == fields_to_go[i]) k = 1;
                }
                if(from_position == to_position || k == 0) k = 2;
              } while (k == 0);
            }while (k == 2);
            make_move(from_position, to_position);
          }
          else{
            print_board();
            cout << endl << "Computer thinking! " << endl;
            best_move_struct computers_move = find_best_move(4);
            make_move(computers_move.from_place, computers_move.to_place, computers_move.new_figure);
            cout << endl << "Computer played: " << computers_move.from_place << " to " << computers_move.to_place;
          }
        }
        else{//черните да играят
          if(players == 1 || choose_figures == 1){// ако 
            enpass_black = 0, bbc = 0, bsc = 0;
            print_board();
            do{
              k = 2;
              do{
                do{
                  cout << endl << "Choose a square with your figure:  ";
                  cin >> from_position;
                  if(from_position.x == -1 || from_position.y == -1){
                    flag_to_stop_not_ended_game = 1;
                    return;
                  }
                } while (board[from_position.x][from_position.y]->get_field_figure() == nullptr || board[from_position.x][from_position.y]->get_field_figure_color() != whos_move);
                n = board[from_position.x][from_position.y]->get_field_figure()->get_number_of_position_to_go(board);
                if(number_move > 1){
                  if(board[from_position.x][from_position.y]->get_field_figure()->get_type() == "pawn" && from_position.x == current_place.x &&
                      (current_place.y + 1 == from_position.y || current_place.y - 1 == from_position.y)){
                    enpass_black = find_if_enpassant_black_can_capture(previous_place.x, previous_place.y, current_place);
                    n += enpass_black;
                  }
                }
                if(board[from_position.x][from_position.y]->get_field_figure()->get_type() == "king"){
                  bbc = find_if_black_big_castle(); 
                  bsc = find_if_black_small_castle();
                  n += bbc;
                  n += bsc;
                }
                if(n == 0){
                  cout << endl << "No possible moves with this figure: " << endl;
                }
              }while(n == 0);
              Point* fields_to_go = new Point[n];
              fields_to_go = board[from_position.x][from_position.y]->get_field_figure()->get_field_coordinates_to_go(n, board);
              if(bbc == 1){
                fields_to_go[n - 1].x = 0;
                fields_to_go[n - 1].y = 2;
                if(bsc == 1){
                  fields_to_go[n - 2].x = 0;
                  fields_to_go[n - 2].y = 6;
                }
              }
              else{
                if(bsc == 1){
                  fields_to_go[n - 1].x = 0;
                  fields_to_go[n - 1].y = 6;
                }
              }
              if(enpass_black == 2){
                fields_to_go[n - 1].x = from_position.x + 1;
                fields_to_go[n - 1].y = from_position.y - 1;
                fields_to_go[n - 2].x = from_position.x + 1;
                fields_to_go[n - 2].y = from_position.y + 1;
              }
              else{
                if(enpass_black == 1){
                  if(current_place.x == from_position.x && current_place.y + 1 == from_position.y){
                    fields_to_go[n - 1].x = from_position.x + 1;
                    fields_to_go[n - 1].y = from_position.y - 1;
                  }
                  if(current_place.x == from_position.x && current_place.y - 1 == from_position.y){
                    fields_to_go[n - 1].x = from_position.x + 1;
                    fields_to_go[n - 1].y = from_position.y + 1;
                    }
                }
              }


              for(int i = 0; i < n; i++){
                cout << fields_to_go[i];
                if(i != n - 1)cout << ", ";
              }
              do{
                cout << endl << "Choose square to go from above: ";
                cin >> to_position;
                if(from_position.x == -1 || from_position.y == -1){
                    flag_to_stop_not_ended_game = 1;
                    return;
                  }
                for(int i = 0; i < n; i++){
                  if(to_position == fields_to_go[i]) k = 1;
                }
                if(from_position == to_position || k == 0) k = 2;
              } while (k == 0);
            }while (k == 2);
            make_move(from_position, to_position);
          }
          else{
            print_board();
            cout << endl << "Computer thinking! " << endl;
            best_move_struct computers_move = find_best_move(4);
            make_move(computers_move.from_place, computers_move.to_place, computers_move.new_figure);
            cout << endl << "Computer played: " << computers_move.from_place << " to " << computers_move.to_place;
          }
        }
        game_stopper = game_over(whos_move);
      }
      if(game_stopper == 1){
        if(black_mated() == 1) cout << "Black mated. White is the winner! ";
        else{
          if(white_mated() == 1) cout << "White mated. Black is the winner! ";
          else cout << "The game is a draw! ";
        }
        cout << endl << "Do you want to play again  Y/N ? ";
        string new_game = "";
        do
        {
          cin >> new_game;
          if(new_game == "Y") start_loop(flag_to_stop_not_ended_game);
          if(new_game == "N") return;
        } while (new_game != "Y" && new_game != "N");
      }
    }

  public:
    Game(){
      players = 1;
      choose_figures = 0;
      number_move = 1;
      whos_move = 0;
      previous_place.x = -1;
      previous_place.y = -1;
      current_place.x = -1;
      current_place.y = -1;
      white_small_castle = 0;
      black_small_castle = 0;
      white_big_castle = 0;
      black_big_castle = 0;
      board = new Field**[8];
      for(int i = 0; i < 8; i++){
        board[i] = new Field*[8];
      }
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          board[i][j] = new Field(i, j);
        }
      }
    }

    Game(const Game& A){
      white_small_castle = A.white_small_castle;
      black_small_castle = A.black_small_castle;
      white_big_castle = A.white_big_castle;
      black_big_castle = A.black_big_castle;
      number_move = A.number_move;
      whos_move = A.whos_move;
      players = A.players;
      previous_place = A.previous_place;
      current_place = A.current_place;
      board = new Field**[8];
      for(int i = 0; i < 8; i++){
        board[i] = new Field*[8];
      }
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          board[i][j] = new Field(i, j);
        }
      }
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          if(A.board[i][j]->get_field_figure() != nullptr){
            board[i][j]->set_new_figure(A.board[i][j]->get_field_figure());
          }
        }
      }
    }

    Game& operator=(const Game& A){
      if(this != &A){
        white_small_castle = A.white_small_castle;
        black_small_castle = A.black_small_castle;
        white_big_castle = A.white_big_castle;
        black_big_castle = A.black_big_castle;
        number_move = A.number_move;
        whos_move = A.whos_move;
        players = A.players;
        previous_place = A.previous_place;
        current_place = A.current_place;
        for(int i = 0; i < 8; i++){
          for(int j = 0; j < 8; j++){
            board[i][j]->del_figure();
          }
        }
        for(int i = 0; i < 8; i++){
          for(int j = 0; j < 8; j++){
            if(A.board[i][j]->get_field_figure() != nullptr){
              board[i][j]->set_new_figure(A.board[i][j]->get_field_figure());
            }
          }
        }
      }
      return *this;
    }

    ~Game(){
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          delete board[i][j];
        }
      }
      for(int i = 0; i < 8; i++){
        delete[] board[i];
      }
      delete[] board;
    }

    void start_game(){
      bool flag_to_stop_not_ended_game = 0;
      start_loop(flag_to_stop_not_ended_game);
      if(flag_to_stop_not_ended_game == 1){
        save_game_to_file();
        cout << "Game saved for later! ";
      }
    }
};

void begin(){
  Game new_game;
  new_game.start_game();
  cout << "!END";
}

int main(){
  // ios::sync_with_stdio(false);
  // cin.tie(nullptr);
  // cout.tie(nullptr);
  begin();
  return 0;
}