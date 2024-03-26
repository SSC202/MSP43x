#include "driverlib.h"

int main()
{
    WDT_A_hold(WDT_A_BASE);
    while (1)
    {
        ;
    }
}