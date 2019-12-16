#include <stdio.h>

class GlobalClass {
public:
    GlobalClass() {
        fprintf(stdout, "%s(%d)/%s(): GlobalClass constructor\n", __FILE__, __LINE__, __FUNCTION__);
    }
    ~GlobalClass() {
        fprintf(stdout, "%s(%d)/%s(): GlobalClass destructor\n", __FILE__, __LINE__, __FUNCTION__);
    }
};

GlobalClass demo;   // invoke constructor on before main()
                    // invoke destructor on after main()

int main() {
    fprintf(stdout, "Welcome, to the jungle of \"C with Classes\"\n");
}


