#include <bits/stdc++.h>

using namespace std;

int main() {
    srand(time(0));
    int i = 10;
    while (i--) {
        double x = (rand() % 9974) * 1.0 / 9973;
        cout << x << endl;
    }
    return 0;
}