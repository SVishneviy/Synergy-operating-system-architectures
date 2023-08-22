#include <iostream>
#include <unistd.h>

using namespace std;

int main() {

    int c, s, n = 0;
    pid_t p;

    cout << "Input count iteratons: ";
    cin >> c;
    cout << "Input sleep seconds: ";
    cin >> s;

    p = fork();

    if (p < 0) {
        cout << stderr << "Fork failed" << endl;
        return 1;
    } else if (p > 0) {
        for (int i = 0; i < c; i++) {
            sleep(s);
            n++;
            cout << "Parent process n = " << n << endl; 
        }
    } else {
        for (int i = 0; i < c; i++) {
            n++;
            cout << "Child process n = " << n << endl; 
        }
    }
    return 0;
}