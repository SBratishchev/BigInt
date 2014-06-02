#include "bigint.h"


BigInt::BigInt()
{
    sign = 1;
}


BigInt::BigInt(int num)
{
    if (num >= 0)
    {
        this->sign = 1;
    }
    else
    {
        this->sign = -1;
        num *= -1;
    }

    this->number.push_back(num % BigInt::base);

    num /= BigInt::base;

    if (num != 0)
        this->number.push_back(num);
}


BigInt::BigInt(string str)
{
    StringToInt(str);
}


BigInt::BigInt(char* fileName)
{
    ReadFromFile(fileName);
}


void BigInt::RemoveZero()
{
    while (this->number.size() > 1 && this->number.back() == 0)
    {
        this->number.pop_back();
    }

    if (this->number.size() == 1 && this->number[0] == 0) this->sign = 1;
}


void BigInt::ShiftRight()
{
    if (this->number.size() == 0)
    {
        this->number.push_back(0);
        return;
    }

    this->number.push_back(this->number[this->number.size() - 1]);

    for (int i = this->number.size() - 2; i > 0; --i)
    {
        this->number[i] = this->number[i - 1];
    }

    this->number[0] = 0;
}



void BigInt::ReadFromFile(char* fileName)
{
    ifstream ifst(fileName);
    string str;
    getline(ifst, str);
    StringToInt(str);
}


void BigInt::WriteToFile(char* fileName)
{
    ofstream ofst(fileName);

    if (this->sign == -1)
    {
        ofst << "-";
    }

    ofst << number[number.size() - 1];

    for (int i = number.size() - 2; i >= 0; i--)
    {
        if (number[i] < (BigInt::base / 10))
        {
            int count = 0;
            int buf = number[i];

            if (number[i] == 0) count += 1;

            while (buf){
                buf /= 10;
                count++;
            }
            for (int j = 0; j < (BigInt::baseLen - count); j++)
            {
                ofst << 0;
            }
        }
        ofst << number[i];
    }
}


void BigInt::ReadFromBinFile(char* fileName)
{
    ifstream ifst;
    ifst.open(fileName, ios::binary | ios::in);

    ifst.seekg(0, ifst.end);
    int fileSize = ifst.tellg();
    ifst.seekg(0);

    this->number.resize((fileSize / BigInt::baseLen) + 1);

    for (int i = 0; i < fileSize / BigInt::baseLen; i++)
    {
        ifst.read((char*)&this->number[i], BigInt::baseLen);
    }

    ifst.read((char*)&this->sign, 1);

    ifst.close();
}


void BigInt::WriteTiBinFile(char* fileName)
{
    ofstream ofst;
    ofst.open(fileName, ios::binary | ios::out);

    for (int i = 0; i < this->number.size(); i++)
    {
        ofst.write((char*)&(this->number[i]), BigInt::baseLen);
    }

    ofst.write((char*)&this->sign, 1);

    ofst.close();
}


void BigInt::PrintNumber()
{
    if (this->sign == -1)
    {
        cout << "-";
    }

    if (this->number.size() != 0)
    {
        cout << number[number.size() - 1];

        for (int i = number.size() - 2; i >= 0; i--)
        {
            if (number[i] < (BigInt::base / 10))
            {
                int count = 0;
                int buf = number[i];

                if (number[i] == 0) count += 1;

                while (buf){
                    buf /= 10;
                    count++;
                }

                for (int j = 0; j < (BigInt::baseLen - count); j++)
                {
                    cout << 0;
                }
            }
            cout << number[i];
        }
    }
    cout << endl;
}


void BigInt::StringToInt(string str)
{
    if (str[0] == '-')
    {
        str = str.substr(1);
        this->sign = -1;
    }
    else
    {
        this->sign = 1;
    }

    for (int i = str.length(); i > 0; i -= BigInt::baseLen)
    {
        if (i < 9)
            this->number.push_back(atoi(str.substr(0, i).c_str()));
        else
            this->number.push_back(atoi(str.substr(i - BigInt::baseLen, BigInt::baseLen).c_str()));
    }

    this->RemoveZero();
}


BigInt BigInt::AAKaratsuba(BigInt const& left, BigInt const& right)
{
    BigInt Result;
    vector<long long> res;

    int size = left.number.size() >= right.number.size() ? left.number.size() : right.number.size();

    vector<long long> l;
    l.resize(size);

    for (int i = 0; i < size; i++)
    {
        l[i] = (i < left.number.size() ? left.number[i] : 0);
    }

    vector<long long> r;
    r.resize(size);

    for (int i = 0; i < size; i++)
    {
        r[i] = (i < right.number.size() ? right.number[i] : 0);
    }

    if (left.number.size() >= right.number.size())
    {
        res = Karatsuba(l, r);
    }
    else
    {
        res = Karatsuba(r, l);
    }

    for (int i = 0, carry = 0; i < res.size(); i++)
    {
        long long cur = res[i] + carry;
        Result.number.push_back((int)(cur % BigInt::base));
        carry = (cur / BigInt::base);
    }

    Result.RemoveZero();
    return Result;
}


vector<long long> BigInt::Karatsuba(vector<long long> left, vector<long long> right)
{
    int n = left.size();
    vector<long long> res;
    res.resize(n + n);

    if (n <= 8) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                res[i + j] += left[i] * right[j];
        return res;
    }

    int k = n >> 1;
    vector<long long> a1(left.begin(), left.begin() + k);
    vector<long long> a2(left.begin() + k, left.end());
    vector<long long> b1(right.begin(), right.begin() + k);
    vector<long long> b2(right.begin() + k, right.end());

    vector<long long> a1b1 = Karatsuba(a1, b1);
    vector<long long> a2b2 = Karatsuba(a2, b2);

    for (int i = 0; i < k; i++)
        a2[i] += a1[i];
    for (int i = 0; i < k; i++)
        b2[i] += b1[i];

    vector<long long> r = Karatsuba(a2, b2);

    if ((n % 2) == 1) n--;

    for (int i = 0; i < (int)a1b1.size(); i++)
        r[i] -= a1b1[i];
    for (int i = 0; i < (int)a2b2.size(); i++)
        r[i] -= a2b2[i];

    for (int i = 0; i < (int)r.size(); i++)
        res[i + k] += r[i];
    for (int i = 0; i < (int)a1b1.size(); i++)
        res[i] += a1b1[i];
    for (int i = 0; i < (int)a2b2.size(); i++)
        res[i + n] += a2b2[i];
    return res;
}


BigInt BigInt::PowMod(BigInt a, BigInt k, BigInt n)
{
    BigInt res(1);

    while (k != 0)
    {
        if ((k % 2) == 1)
        {
            res = (res * a) % n;
        }

        a = (a * a) % n;
        k /= 2;
    }

    return res;
}


BigInt BigInt::abs() const
{
    BigInt res = *this;
    res.sign *= res.sign;
    return res;
}


BigInt BigInt::operator-() const
{
    BigInt res = *this;
    res.sign *= -1;
    return res;
}


const BigInt operator +(BigInt left, const BigInt& right)
{
    if (left.sign != right.sign)
    {
        return left - (-right);
    }

    BigInt res = right;

    int carry = 0;
    int maxLen = (left.number.size() >= right.number.size() ? left.number.size() : right.number.size());

    for (int i = 0; i < maxLen || carry; ++i)
    {
        if (i == res.number.size())
        {
            res.number.push_back(0);
        }

        res.number[i] += carry + (i < left.number.size() ? left.number[i] : 0);

        carry = res.number[i] >= BigInt::base;

        if (carry)
        {
            res.number[i] -= BigInt::base;
        }
    }
    return res;
}


BigInt& BigInt::operator +=(const BigInt& right) {
    return *this = (*this + right);
}


const BigInt operator -(BigInt left, const BigInt& right)
{
    if (left.sign != right.sign)
    {
        return left - (-right);
    }

    if (left.abs() >= right.abs())
    {
        BigInt res = left;
        int carry = 0;

        for (int i = 0; i < right.number.size() || carry; ++i)
        {
            res.number[i] -= carry + (i < right.number.size() ? right.number[i] : 0);
            carry = res.number[i] < 0;

            if (carry)
            {
                res.number[i] += BigInt::base;
            }
        }
        res.RemoveZero();
        return res;
    }
    else
    {
        return -(right - left);
    }
}


BigInt& BigInt::operator -=(const BigInt& right)
{
    return *this = (*this - right);
}


const BigInt operator *(BigInt left, const BigInt& right)
{
    BigInt res;
    res.sign = left.sign * right.sign;

    if ((left.number.size() + right.number.size()) <= 8)
    {
        res.number.resize(right.number.size() + left.number.size() + 1);

        for (int i = 0; i < left.number.size(); i++)
        {
            for (int j = 0, carry = 0; j < right.number.size() || carry; j++)
            {
                long long buf = res.number[i + j] + left.number[i] * 1ll * (j < right.number.size() ? right.number[j] : 0) + carry;
                res.number[i + j] = (int)(buf % BigInt::base);
                carry = (int)(buf / BigInt::base);
            }
        }
        res.RemoveZero();
        return res;
    }
    else
    {
        res = res.AAKaratsuba(left, right);
        res.RemoveZero();
        return res;
    }
}


BigInt& BigInt::operator *=(const BigInt& right)
{
    return *this = (*this * right);
}


const BigInt operator /(BigInt left, const BigInt& right)
{
    BigInt b = right, result, current;
    result.number.resize(left.number.size());

    for (long long i = (long long)(left.number.size() - 1); i >= 0; --i)
    {
        current.ShiftRight();
        current.number[0] = left.number[i];
        current.RemoveZero();

        int x = 0, l = 0, r = BigInt::base;

        while (l <= r)
        {
            int m = (l + r) / 2;
            BigInt t = b * m;
            if (t <= current)
            {
                x = m;
                l = m + 1;
            }
            else r = m - 1;
        }

        result.number[i] = x;
        current = current - b * x;
    }

    result.sign = left.sign * right.sign;
    result.RemoveZero();

    return result;
}


BigInt& BigInt::operator /=(const BigInt& right)
{
    return *this = (*this / right);
}


const BigInt operator %(BigInt left, const BigInt& right)
{   
    BigInt cur = left / right;

    cur *= right;

    BigInt res = left - cur;

    res.sign *= right.sign;

    return res;
}



BigInt& BigInt::operator %=(const BigInt& right)
{
    return *this = (*this % right);
}


const BigInt operator ^(BigInt left, BigInt& right)
{
    BigInt res(1);

    while (right != 0)
    {
        if ((right % 2) == 1)
        {
            res *= left;
        }

        left *= left;
        right /= 2;
    }

    return res;
}


BigInt& BigInt::operator ^=(BigInt& right)
{
    return *this = (*this ^ right);
}


BigInt BigInt::operator*(int num) const
{
    BigInt res = *this;
    res *= num;
    return res;
}


void BigInt::operator*=(int num)
{
    if (num < 0)
    {
        this->sign = -sign;
        num = -num;
    }

    for (int i = 0, carry = 0; i < this->number.size() || carry; ++i)
    {
        if (i == (int) this->number.size())
        {
            this->number.push_back(0);
        }

        long long cur = this->number[i] * (long long)num + carry;
        carry = (int)(cur / base);
        this->number[i] = (int)(cur % base);
    }

    this->RemoveZero();
}


void BigInt::operator/=(int num)
{
    if (num < 0)
    {
        sign = -sign;
        num = -num;
    }

    for (int i = this->number.size() - 1, rem = 0; i >= 0; --i)
    {
        long long cur = this->number[i] + rem * (long long)base;
        this->number[i] = (int)(cur / num);
        rem = (int)(cur % num);
    }
    this->RemoveZero();
}


BigInt BigInt::operator/(int num) const
{
    BigInt res = *this;
    res /= num;
    return res;
}


void BigInt::operator+=(int num)
{
    if (num < 0)
    {
        sign = -sign;
        num = -num;
    }

    for (int i = 0, carry = 0; i < this->number.size() || carry; ++i)
    {
        if (i == (int) this->number.size())
        {
            this->number.push_back(0);
        }

        long long cur = this->number[i] + (long long)num + carry;
        carry = (int)(cur / base);
        this->number[i] = (int)(cur % base);
    }

    this->RemoveZero();
}


BigInt BigInt::operator+(int num) const
{
    BigInt res = *this;
    res += num;
    return res;
}


void BigInt::operator-=(int num)
{
    if (num < 0)
    {
        sign = -sign;
        num = -num;
    }

    for (int i = 0, carry = 0; i < this->number.size() || carry; ++i)
    {
        long long cur = this->number[i] - (long long)num - carry;
        carry = (int)(cur / base);
        this->number[i] = (cur % base) < 0 ? (int)(-1 * (cur % base)) : (int)(cur % base);
    }

    this->RemoveZero();
}


BigInt BigInt::operator-(int num) const
{
    BigInt res = *this;
    res -= num;
    return res;
}


void BigInt::operator%=(int num)
{
    if (num < 0)
    {
        sign = -sign;
        num = -num;
    }

    int rem = 0;

    for (int i = this->number.size() - 1; i >= 0; --i)
    {
        long long cur = this->number[i] + rem * (long long)base;
        this->number[i] = (int)(cur / num);
        rem = (int)(cur % num);
    }

    BigInt res;
    res.number.push_back(rem);

    *this = res;

    this->RemoveZero();
}


BigInt BigInt::operator%(int num) const
{
    BigInt res = *this;
    res %= num;
    return res;
}


void BigInt::operator^=(int num)
{
    BigInt res;
    BigInt left = *this;
    res.number.push_back(1);

    while (num)
    {
        if (num & 1)
        {
            res *= left;
        }

        left *= left;
        num >>= 1;
    }
    *this = res;
}


BigInt BigInt::operator^(int num) const
{
    BigInt res = *this;
    res ^= num;
    return res;
}


bool BigInt::operator<(const BigInt &r) const
{
    if (this->sign != r.sign)
        return this->sign < r.sign;
    if (this->number.size() != r.number.size())
        return this->number.size() * this->sign < r.number.size() * r.sign;
    for (int i = this->number.size() - 1; i >= 0; i--)
        if (this->number[i] != r.number[i])
            return number[i] * this->sign < r.number[i] * this->sign;
    return false;
}


bool BigInt::operator>(const BigInt &r) const
{
    return r < *this;
}


bool BigInt::operator<=(const BigInt &r) const
{
    return !(r < *this);
}


bool BigInt::operator>=(const BigInt &r) const
{
    return !(*this < r);
}


bool BigInt::operator==(const BigInt &r) const
{
    return !(*this < r) && !(r < *this);
}


bool BigInt::operator!=(const BigInt &r) const
{
    return *this < r || r < *this;
}
