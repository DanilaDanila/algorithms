#pragma once

#include <iostream>
#include <vector>
#include <string>

class BigInteger
{
private:
	int8_t sign = 0;

	std::vector<uint8_t> data;

	explicit BigInteger(const std::string &str)
	{
		auto it = str.rbegin();
		if (*it == '-')
		{
			sign = -1;
			++it;
		}
		else
			sign = 1;
		
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
	}

	/*
	 * as += but independs of sign
	 */
	void u_add_inplace(const BigInteger &bi)
	{
		if(bi.sign == 0)
			return;

		data.resize(std::max(data.size(), bi.data.size()));

		uint8_t r = 0;
		for(int i=0; i<std::max(data.size(), bi.data.size()); ++i)
		{
			data[i] += bi.data[i] + r;
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

		uint8_t r=0;
		for(int i=0; i<bi.data.size(); ++i)
		{
			data[i] += 10 - bi.data[i] - r;
			r = data[i] < 10;
			data[i] %= 10;
		}

		if(r > 0)
			data[bi.data.size()] -= r;

		while(data.size()>1 && *data.rbegin()==0)
			data.erase(data.end()-1);

		int new_size=data.size();
		while(new_size > 0 && data[new_size-1] == 0) --new_size;
		if(new_size == 0) new_size = 1;
		data.resize(new_size);
	}

public:
	BigInteger() { data.push_back(0); }

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

	~BigInteger() { }

	bool operator<(const BigInteger &bi) const
	{
		if(sign < bi.sign)
			return true;
		else if(sign > bi.sign)
			return false;

		// unsigned is less
		bool u_is_less = false;

		if(data.size() < bi.data.size())
			u_is_less = true;
		else if(data.size() == bi.data.size())
		{
			int it = data.size();
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

		for(int i=0; i<data.size(); ++i)
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
		if(sign == bi.sign)
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

	BigInteger &operator*=(const BigInteger &bi);
	
	BigInteger &operator/=(const BigInteger &bi);

	BigInteger &operator%=(const BigInteger &bi);

	friend const BigInteger operator+(const BigInteger&, const BigInteger&);

	friend const BigInteger operator-(const BigInteger&, const BigInteger&);

	friend const BigInteger operator*(const BigInteger&, const BigInteger&);

	friend const BigInteger operator/(const BigInteger&, const BigInteger&);

	friend const BigInteger operator%(const BigInteger&, const BigInteger&);

	BigInteger &operator++()
	{
		if(sign >= 0)
		{
			++data[0];
			for(uint8_t i=0; i<data.size() && data[i]==10; ++i)
			{
				data[i] %= 10;
				if(i+1 < data.size())
					++data[i+1];
				else
					data.push_back(1);
			}
		}
		else
		{
			// TODO
			if(data.size() == 1 && data[0] == 0);
		}
		
		return *this;
	}

	const BigInteger operator++(int)
	{
		const BigInteger cpy = *this;
		++*this;
		return cpy;
	}

	BigInteger &operator--();

	const BigInteger operator--(int);

	explicit operator bool() const;

	friend std::istream &operator>>(std::istream&, BigInteger&);
};

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
