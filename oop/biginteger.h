#pragma once

#include <iostream>
#include <vector>
#include <string>

class BigInteger;
class Rational;

const BigInteger operator*(const BigInteger&, const BigInteger&);
const BigInteger operator/(const BigInteger&, const BigInteger&);
const BigInteger operator%(const BigInteger&, const BigInteger&);

class BigInteger
{
private:
	int8_t sign = 0;

	std::vector<uint8_t> data;

	explicit BigInteger(const std::string &_str)
	{	
		std::string str = _str;

		sign = 1;
		if (str[0] == '-')
		{
			sign = -1;
			str.erase(str.begin());
		}
		else if(str[0] == '+')
			str.erase(str.begin());
   
		auto it = str.rbegin();
	
		for(; it != str.rend(); ++it)
		{
			char digit = *it;
			
			if(digit < '0' || digit > '9')
			{
				std::cerr<<"Can not input "<<digit<<" as digit of BigInteger\n";
				exit(1);
			}
			
			digit -= '0';
			data.push_back(digit);
		}

		int new_size = data.size();
		while(new_size > 0 && data[new_size-1] == 0)
			--new_size;
		data.resize(new_size);
		if(new_size == 0)
			sign = 0;
	}

	/*
	 * as += but independs of sign
	 */
	void u_add_inplace(const BigInteger &bi)
	{
		if(bi.sign == 0)
			return;

		if(sign == 0)
		{
			*this = bi;
			return;
		}

		data.resize((data.size() > bi.data.size())?data.size():bi.data.size());

		uint8_t r = 0;
		for(size_t i=0; i<data.size() && !(i>=bi.data.size() && r==0); ++i)
		{
			if(i < bi.data.size())
				data[i] += bi.data[i];
			data[i] += r;
			r = data[i] / 10;
			data[i] %= 10;
		}

		if(r != 0)
			data.push_back(r);

		data.resize(data.size());
	}

	/*
	 * as -= but independs of sign
	 * |*this| >= |bi| !!!
	 */
	void u_sub_inplace(const BigInteger &bi)
	{
		if(bi.sign == 0)
			return;

		if(sign == 0)
		{
			*this = bi;
			sign = -sign;
			return;
		}

		uint8_t r=0;
		for(size_t i=0; i<data.size() && !(i>=bi.data.size() && r==0); ++i)
		{
			data[i] += 10 - r;

			if(i < bi.data.size())
				data[i] -= bi.data[i];

			r = data[i] < 10;
			data[i] %= 10;
		}

		int new_size=data.size();
		while(new_size > 0 && data[new_size-1] == 0) --new_size;
		data.resize(new_size);

		if(data.size() == 0)
			sign = 0;
	}

	const BigInteger mul(int n) const
	{
		BigInteger result;

		if(n == 0)
		{
			result = 0;
		}
		else
		{
			if(n < 0)
				result.sign = -sign;
			else
				result.sign = sign;

			n = (n>0)?n:-n;
			int r = 0;
			for(size_t i = 0; i<data.size(); ++i)
			{
				int new_digit = data[i]*n + r;
				r = new_digit / 10;
				new_digit %= 10;

				result.data.push_back(new_digit);
			}

			if(r != 0)
				result.data.push_back(r);
		}

		return result;
	}

	template<typename Iter>
	BigInteger(int sign, Iter from, Iter to):
		sign(sign),
		data(from, to)
	{
		if(data.size() != 0)
		{
			int new_size = data.size();
			while(new_size > 0 && data[new_size-1] == 0)
				--new_size;

			data.resize(new_size);
		}

		if(data.size() == 0)
			this->sign = 0;
	}

	// multyply BigInteger and 10(or radix) ^ exp
	const BigInteger qmul(int exp) const
	{
		if(sign == 0)
			return BigInteger();

		BigInteger result(sign);

		result.data.resize(data.size() + exp);

		for(int i=data.size()-1; i>=0; --i)
			result.data[i+exp] = data[i];

		for(int i=0; i<exp; ++i)
			result.data[i] = 0;

		return result;
	}

	BigInteger div(unsigned n)
	{
		BigInteger result(*this);

		int r=0;
		for(int i=data.size()-1; i>=0; --i)
		{
			result.data[i] = (data[i]+r*10) / n;
			r = (data[i]+r*10) % n;
		}

		int new_size = result.data.size();
		while(new_size > 0 && result.data[new_size-1] == 0)
			--new_size;
		result.data.resize(new_size);
		if(new_size == 0)
			result.sign = 0;

		return result;
	}

	const BigInteger abs() const
	{
		return (sign>0) ? *this : -*this;
	}
public:
	BigInteger() { sign = 0; }

	BigInteger(const BigInteger &bi):
		sign(bi.sign),
		data(bi.data)
	{}

	BigInteger(const int &_num)
	{
		sign = (_num == 0)?0:( (_num>0)?1:-1 );

		int num = (_num>=0)?_num:-_num;

		while(num > 0)
		{
			data.push_back(num % 10);	
			num /= 10;
		}
	}

	BigInteger &operator=(const BigInteger &bi)
	{
		if(this == &bi)
			return *this;

		sign = bi.sign;
		data = bi.data;

		return *this;
	}

	~BigInteger() {}

	bool operator<(const BigInteger &bi) const
	{
		if(sign < bi.sign)
			return true;
		else if(sign > bi.sign or (sign == 0 && sign == bi.sign))
			return false;

		// unsigned is less
		bool u_is_less = false;

		if(data.size() < bi.data.size())
			u_is_less = true;
		else if(data.size() == bi.data.size())
		{
			int it = data.size()-1;
			while(it >= 0 && data[it] == bi.data[it])
				--it;

			if(it < 0)
				return false; // equal
			else if(data[it] < bi.data[it])
				u_is_less = true;
		}

		return (sign>=0)?u_is_less:!u_is_less;
	}

	bool operator>(const BigInteger &bi) const { return bi<*this; }
	bool operator==(const BigInteger &bi) const { return !(*this<bi) && !(bi<*this);}
	bool operator!=(const BigInteger &bi) const { return *this<bi || bi<*this; }
	bool operator<=(const BigInteger &bi) const { return !(*this>bi); }
	bool operator>=(const BigInteger &bi) const { return !(*this<bi); }

	std::string toString() const
	{
		if(sign == 0)
			return "0";

		std::string str;

		for(size_t i=0; i<data.size(); ++i)
			str += ('0' + data[i]);

		if(sign < 0)
			str += '-';

		std::string result;
		for(auto it = str.rbegin(); it != str.rend(); ++it)
			result += *it;
		return result;
	}

	BigInteger operator-() const
	{
		BigInteger bi(*this);
		
		if(sign)
			bi.sign *= -1;

		return bi;
	}

	BigInteger &operator+=(const BigInteger &bi)
	{
		if(sign + bi.sign != 0)
			u_add_inplace(bi);
		else
		{
			bool is_this_abs_less = false;

			if(sign == -1)
			{
				sign *= -1;
				is_this_abs_less = *this < bi;
				sign *= -1;	
			}
			else
			{
				sign *= -1;	
				is_this_abs_less = *this > bi;
				sign *= -1;
			}

			if(!is_this_abs_less)
				u_sub_inplace(bi);
			else
			{
				BigInteger result=bi;
				result.u_sub_inplace(*this);
				*this = result;
			}
		}
		return *this;
	}

	BigInteger &operator-=(const BigInteger &bi)
	{
		*this += -bi;	
		return *this;
	}

	BigInteger &operator*=(const BigInteger &bi)
	{
		*this = *this * bi;

		return *this;
	}
	
	BigInteger &operator/=(const BigInteger &bi)
	{
		*this = *this / bi;

		return *this;
	}

	BigInteger &operator%=(const BigInteger &bi)
	{
		*this = *this % bi;

		return *this;
	}

	friend const BigInteger operator*(const BigInteger&, const BigInteger&);

	friend const BigInteger operator/(const BigInteger&, const BigInteger&);

	friend const BigInteger operator%(const BigInteger&, const BigInteger&);

	BigInteger &operator++()
	{	
		return *this+=1;
	}

	const BigInteger operator++(int)
	{
		const BigInteger tmp = *this;
		++*this;
		return tmp;
	}

	BigInteger &operator--()
	{
		return *this-=1;
	}

	const BigInteger operator--(int)
	{
		const BigInteger tmp = *this;
		--*this;
		return tmp;
	}

	explicit operator bool() const { return sign != 0; }

	friend std::istream &operator>>(std::istream&, BigInteger&);

	friend Rational;
};

std::ostream &operator<<(std::ostream &out, const BigInteger &bi)
{
	out<<bi.toString();
	return out;
}

std::istream &operator>>(std::istream &in, BigInteger &bi)
{
	std::string str;
	std::cin>>str;

	bi=BigInteger(str);

	return in;
}

const BigInteger operator+(const BigInteger &bi0, const BigInteger &bi1)
{
	BigInteger result = bi0;
	result += bi1;
	return result;
}

const BigInteger operator-(const BigInteger &bi0, const BigInteger &bi1)
{
	BigInteger result = bi0;
	result -= bi1;
	return result;
}

const BigInteger operator*(const BigInteger &bi0, const BigInteger &bi1)
{
	if(bi0.sign == 0 || bi1.sign == 0)
		return BigInteger();
	if(bi0.data.size() == 1)
		return bi1.mul(bi0.data[0] * bi0.sign);
	if(bi1.data.size() == 1)
		return bi0.mul(bi1.data[0] * bi1.sign);
	
	BigInteger _bi0(bi0);
	BigInteger _bi1(bi1);

	int new_size = (bi0.data.size() > bi1.data.size()) ? bi0.data.size() : bi1.data.size();

	_bi0.data.resize(new_size);
	_bi1.data.resize(new_size);

	for(int i=bi0.data.size(); i<new_size; ++i)
		_bi0.data[i] = 0;
	for(int i=bi1.data.size(); i<new_size; ++i)
		_bi1.data[i] = 0;

	// AB*CD = (A*C)^2sh + ((A+B)*(C+D) - A*C - B*D)^sh + B*D

	BigInteger bi0_most(_bi0.sign, _bi0.data.begin() + _bi0.data.size()/2, _bi0.data.end());
	BigInteger bi0_least(_bi0.sign, _bi0.data.begin(), _bi0.data.begin() + _bi0.data.size()/2);

	BigInteger bi1_most(_bi1.sign, _bi1.data.begin() + _bi1.data.size()/2, _bi1.data.end());
	BigInteger bi1_least(_bi1.sign, _bi1.data.begin(), _bi1.data.begin() + _bi1.data.size()/2);

	BigInteger most_mul = bi0_most * bi1_most;
	BigInteger least_mul = bi0_least * bi1_least;
	BigInteger sums_mul = (bi0_most + bi0_least) * (bi1_most + bi1_least);

	int shift = new_size / 2;

	BigInteger result = most_mul.qmul(2 * shift) + (sums_mul - most_mul - least_mul).qmul(shift) + least_mul;

	int result_size = result.data.size();
	while(result_size > 0 && result.data[result_size - 1] == 0)
		--result_size;
	result.data.resize(result_size);

	return result;
}

const BigInteger operator/(const BigInteger &bi0, const BigInteger &bi1)
{
	if(bi0 < 0)
		return - ((-bi0) / bi1);
	if(bi1 < 0)
		return - (bi0 / (-bi1));

	if(bi0 < bi1 || bi0 == 0)
		return BigInteger();
	if(bi1 == 1)
		return bi0;
	if(bi0 == bi1)
		return BigInteger(1);

	BigInteger l=0, r=bi0;
	BigInteger c;
	while(l < r)
	{
		c = l+r;
		c = c.div(2);

		BigInteger tmp = c*bi1;
		if(tmp > bi0)
			r = (r>c)?c:c-1;
		else if(tmp < bi0)
			l = (l<c)?c:c+1;
		else // if( tmp == bi0)
			break;
	}

	return c;
}

const BigInteger operator%(const BigInteger &bi0, const BigInteger &bi1)
{
	return bi0 - (bi0 / bi1)*bi1;
}

/* ######################################################################################### */

class Rational
{
private:
	BigInteger p;
	BigInteger q;

	BigInteger gcd(BigInteger bi0, BigInteger bi1) const
	{
		for(BigInteger tmp; bi1 != 0;)
		{
			tmp = bi0 % bi1;
			bi0 = bi1;
			bi1 = tmp;
		}
		
		return bi0.abs();
	}

	void transform()
	{
		BigInteger tmp = gcd(p, q);

		p /= tmp;
		q /= tmp;

		p.sign *= q.sign;
		q.sign = 1;
	}

public:
	Rational() = default;

	Rational(const Rational&) = default;

	Rational &operator=(const Rational&) = default;

	~Rational() = default;

	Rational(const int &num):
		p(num),
		q(1)
	{}

	Rational(const BigInteger &num):
		p(num),
		q(1)
	{}

	Rational operator-() const
	{
		Rational result = *this;
		result.p.sign *= -1;

		return result;
	}

	Rational &operator+=(const Rational &r)
	{
		p = p*r.q + r.p*q;
		q *= r.q;

		transform();

		return *this;
	}

	Rational &operator-=(const Rational &r)
	{
		*this += -r;

		return *this;
	}

	Rational &operator*=(const Rational &r)
	{
		p *= r.p;
		q *= r.q;

		transform();

		return *this;
	}

	Rational &operator/=(const Rational &r)
	{
		p *= r.q;
		q *= r.p;

		transform();

		return *this;
	}

	std::string toString() const
	{
		if(q != 1)
			return p.toString() + "/" + q.toString();
		else
			return p.toString();
	}

	std::string asDecimal(size_t precision=0) const
	{
		precision++;
		return "";
	}

	explicit operator double() const
	{
		return 0.0;
	}

	friend bool operator<(const Rational&, const Rational&);
};

const Rational operator+(const Rational &r0, const Rational &r1)
{
	Rational tmp = r0;
	tmp += r1;
	return tmp;
}

const Rational operator-(const Rational &r0, const Rational &r1)
{
	Rational tmp = r0;
	tmp -= r1;
	return tmp;
}

const Rational operator*(const Rational &r0, const Rational &r1)
{
	Rational tmp = r0;
	tmp *= r1;
	return tmp;
}

const Rational operator/(const Rational &r0, const Rational &r1)
{
	Rational tmp = r0;
	tmp /= r1;
	return tmp;
}

////////////////////////////

bool operator<(const Rational &r0, const Rational &r1)
{
	// a/b < c/d
	// a*d < c*b
	
	return r0.p*r1.q < r1.p*r0.q;
}

bool operator>(const Rational &r0, const Rational &r1)	{ return r1 < r0; }
bool operator==(const Rational &r0, const Rational &r1) { return !(r0 < r1) && !(r0 > r1); }
bool operator!=(const Rational &r0, const Rational &r1) { return (r0 < r1) || (r0 > r1); }
bool operator<=(const Rational &r0, const Rational &r1) { return !(r0 > r1); }
bool operator>=(const Rational &r0, const Rational &r1) { return !(r0 < r1); }




















