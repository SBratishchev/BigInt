#include <bigint.h>

int main(int argc, char* argv[])
{
    string str = "99999999999999999999999999999999999999999999999999999999999999999999";
    string str2 = "99999999999999999999999999999999999999999999999999999999999999999999";

    BigInt X(str);
    BigInt Y(str2);

    X *= Y;

    X.PrintNumber();

    return 0;
}
