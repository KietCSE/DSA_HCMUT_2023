#include "main.h"
#include "restaurant.cpp"

int compareSolution(int i) {
    int j=1;
    ifstream sol("run/solution/output"+to_string(i)+".txt");
    ifstream sol_you("run/solution_you/output"+to_string(i)+".txt");
    if (sol.fail() || sol_you.fail()) return -2;
    string s, s_you;
    while (sol>>s) {
        if (sol_you>>s_you) {
            if (s!=s_you) {
                sol.close(); sol_you.close();
                return j;
            } else j++;
        } else {
            sol.close(); sol_you.close();
            return j;
        }
    }
    sol.close(); sol_you.close();
    return -1;
}

int main(int argc, char* argv[]) {
    clock_t begin = clock(); //ghi lại thời gian đầu
    int passTest=0;
    streambuf *coutBuffer = cout.rdbuf();
    for (int i=0; i<=600; i++) {
        cout<<"Testcase_"<<i<<"; ";
        string fileName="run/test/testcase_"+to_string(i)+".txt";
        ofstream outputFile("run/solution_you/output"+to_string(i)+".txt");
        cout.rdbuf(outputFile.rdbuf());
        clock_t begin1 = clock();
        simulate(fileName);
        clock_t end1 = clock();
        outputFile.close();
        cout.rdbuf(coutBuffer);
        int fail = compareSolution(i);
        if (fail!=-1) {
            if (fail==-2) cout<<endl<<"Fail tc "<<i<<" error reading file"<<endl;
            else cout<<endl<<"Fail tc "<<i<<" at line "<<fail<<endl;
        } else passTest++;
        cout<<"Timerun test: "<<(float)(end1-begin1)/CLOCKS_PER_SEC<<" s"<<endl;
    }
    cout<<endl<<"Pass "<<passTest<<"/601 tc ~ "<<passTest*100/601<<'%'<<endl;
    clock_t end = clock(); //ghi lại thời gian lúc sau
    cout<<"Time run: "<<(float)(end-begin)/CLOCKS_PER_SEC<<" s"<<endl;
    return 0;
}