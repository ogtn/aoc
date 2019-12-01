#include <stdio.h>

int main(void)
{
    int total_fuel = 0;
    int mass;

    while(scanf("%d\n", &mass) == 1)
    {
        int fuel_for_module =  mass / 3 - 2;;
        int fuel_for_fuel = fuel_for_module / 3 - 2;

        while(fuel_for_fuel > 0)
        {
            fuel_for_module += fuel_for_fuel;
            fuel_for_fuel = fuel_for_fuel / 3 - 2;
        }

        total_fuel += fuel_for_module;
    }

    printf("Total fuel: %d\n", total_fuel);

    return 0;
}
