
#if !defined( CONTROL_H )
#define CONTROL_H

#include <iostream>

#define CTR_DEBUG( A, B ) ({\
    if (ctr_debug) cout << "CTR-DEBUG: " << A << " : " << B << "\n";\
})

using namespace std;

class Control{
    private:
        long long pages;
        long long page_size;
        long long freeMemory;
        long long availableMemory;
        long long usedMemory;

        void init();
        void getValues();
        
    public:
        Control();
        long getTotalSystemMemory();
        long getFreeMemory();
        long getAvailableMemory();
        long getUsedMemory();
};

#endif // !defined( CONTROL_H )