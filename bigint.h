#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#pragma once
using namespace std;

class BigInt
{
    static const int base = 100000000;
    static const int baseLen = 8;

    vector<int> number;
    int sign;

    void RemoveZero();
public:
    BigInt();
    BigInt(string str);
    BigInt(char* fileName);

    void ReadFromFile(char* fileName);
    void WriteToFile(char* fileName);
    void StringToInt(string str);
    void PrintNumber();

    BigInt AAKaratsuba(BigInt const& left, BigInt const& right);
    vector<long long> Karatsuba(vector<long long> left, vector<long long> right);

    BigInt abs() const;
    BigInt operator-() const;

    friend const BigInt operator +(BigInt left, const BigInt& right);
    BigInt& operator +=(const BigInt& right);
    friend const BigInt operator -(BigInt left, const BigInt& right);
    BigInt& operator -=(const BigInt& right);
    friend const BigInt operator *(BigInt left, const BigInt& right);
    BigInt& operator *=(const BigInt& right);
    friend const BigInt operator /(BigInt left, const BigInt& right);
    BigInt& operator /=(const BigInt& right);
    friend const BigInt operator %(BigInt left, const BigInt& right);
    BigInt& operator %=(const BigInt& right);

    BigInt operator^(const int r);
    BigInt operator^(const BigInt);

    bool operator<(const BigInt &r) const;
    bool operator>(const BigInt &r) const;
    bool operator<=(const BigInt &r) const;
    bool operator>=(const BigInt &r) const;
    bool operator==(const BigInt &r) const;
    bool operator!=(const BigInt &r) const;
};
