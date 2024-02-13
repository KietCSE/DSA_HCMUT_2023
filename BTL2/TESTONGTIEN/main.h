/*
! Võ Tiến ..............
* Võ Tiến ..............
~ Võ Tiến ..............
& Võ Tiến ..............
TODO Võ Tiến ..............
*/
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include<vector>
#include<map>
#include<stack>
#include<queue>
#include<list>
#include <utility> 
#include <algorithm>
#include <set>

#ifndef MAIN_H
#define MAIN_H
using namespace std;

ofstream solution;
int COUNTDELETE = 0;


//* LỆNH LAPSE
vector<pair<char, int>> freq_LAPSE;         
int result_LAPSE;  //! kết quả sau mỗi lần tính thôi                          
string root_LAPSE;   
/*
! root vui lòng print ra theo dạng [weight, char](left, right)
! nếu char không tồn tại thì [weight](left, right)
! nếu left = right = nullptr thì [weight] nếu 1 trong 2 thì [weight](NULL, right) || [weight](left,NULL)

! freq vui lòng print ra theo dạng vector<pair<char,int>> {{char, number}} chiều giảm dần giống ví dụ
! ví dụ freq = {{'b',12},{'e',9},{'k',8},{'i',6},{'d',6},{'w',5},{'o',5},{'l',5},{'z',4},{'x',4},{'t',4},{'s',4},{'q',3},{'g',3},{'y',2},{'r',2},{'Y',1},{'R',1}}
*/

//* LỆNH KOKUSEN
vector<int> number_KOKUSEN;
vector<string> root_before_KOKUSEN; 
vector<string> root_after_KOKUSEN; 
/*
!  number_KOKUSEN số hoán vị đưa vào 1 -> MAXSIZE: chỉ đưa các khu đã có khác

! root_KOKUSEN lưu danh sách khách hàng của mỗi khu 1 -> MAXSIZE
! nếu mà khu không có khách thì không cần đưa vào 
! AreaTable ID="ID" : result(left, right) nếu left = right = nullptr thì result nếu 1 trong 2 thì result(NULL, right) || result(left,NULL)
! before danh sách cây trước khi xóa 
! after danh sách cây sau khi xóa
! VD: AreaTable ID=4 : 1423(3,2133(1565(NULL,1707),NULL))
*/

//* LỆNH KEITEIKEN
vector<pair<int, int>> Heap_before_KEITEIKEN;
vector<pair<int, int>> Heap_after_KEITEIKEN;

vector<string> customers_before_KEITEIKEN;
vector<string> customers_after_KEITEIKEN;

/*
! Heap_KEITEIKEN lưu danh sách gồm vector<pair<int,int>> {{ID, Len}} (id mã của khu, len là số lượng khách hàng)
!  Heap_KEITEIKEN lưu từ đầu heap -> cuối heap
! VD {{ID=6,Len=1},{ID=51,Len=2}}  với {ID=6,Len=1} là đầu heap

! customers_KEITEIKEN lưu danh sách mỗi khu theo cơ chế FIFO print ra full khách luôn
! print dạng mảng  AreaTable ID="ID" : FIFO toàn bộ khách hàng

! before danh sách cây trước khi xóa 
! after danh sách cây sau khi xóa 
*/


//*HAND và LIMITLESS và CLEAVE thì print như thầy cho









#endif



