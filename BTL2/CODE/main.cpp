#include "main.h"
#include "restaurant.cpp"
#include "chrono"
 
 
int main(int argc, char* argv[]) {    
    string fileName = "test.txt";
    simulate(fileName);           
    //Test();                                                                           
                                              
    //auto start = chrono::high_resolution_clock::now();
    //string fileName = "test.txt";   
    //simulate(fileName); 
    //// simulate();
    //auto stop = chrono::high_resolution_clock::now();
    //auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    //cout << "Execution time: " << duration.count() << endl;
          
            
    return 0;                                                                              
}                      
                                         