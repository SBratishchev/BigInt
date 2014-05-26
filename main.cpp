#include "bigint.h"

int main(int argc, char* argv[])
{
    string str = "4";
    string str2 = "99999999999999999999999999";

    BigInt X(str);
    BigInt Y(str2);

    Y ^= X;

    Y.PrintNumber();

    return 0;
}
