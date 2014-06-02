#include "bigint.h"
#include <ctime>

int main(int argc, char* argv[])
{
    string str = "24456495874876357876348989768859655246566456";
    string str2 = "58934506746575764563097870453084577656354353454576568456349748165980089325913603234654526248576349530";
    string str3 = "4534556738656758693657578442565642";

    BigInt X1(str);
    BigInt Y1(str2);
    BigInt Z1(str3);
    BigInt Res;
    BigInt Res2;
    BigInt ResPow;

    clock_t start = clock();
    cout << endl;
    ResPow = ResPow.PowMod(Y1, X1, Z1);
    ResPow.PrintNumber();
    fprintf(stderr, "time=%.3lfsec\n", 0.001 * (clock() - start));

    return 0;
}
