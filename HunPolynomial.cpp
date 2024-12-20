#include "HunPolynomial.h"

HunPolynomial::HunPolynomial(const std::vector<int> &coeffs) : coeffs(coeffs)
{
    exponents.resize(coeffs.size());
    for (size_t i = 0; i < coeffs.size(); i++)
    {
        exponents[i] = coeffs.size() - i - 1;
    }
}

HunPolynomial::HunPolynomial(const std::vector<std::vector<int>> &coeffs)
{
    this->coeffs = coeffs[0];
    this->exponents = coeffs[1];
}

void HunPolynomial::Set(const std::vector<int> &coeffs)
{
    this->coeffs = coeffs;
    exponents.resize(coeffs.size());
    for (size_t i = 0; i < coeffs.size(); i++)
    {
        exponents[i] = coeffs.size() - i - 1;
    }
}

HunPolynomial HunPolynomial::operator+(const HunPolynomial &other) const
{
    std::vector<int> result;
    std::deque<int> deque1;
    std::deque<int> deque2;

    for (size_t i = 0; i < coeffs.size(); i++)
        deque1.push_back(coeffs[i]);
    for (size_t i = 0; i < other.coeffs.size(); i++)
        deque2.push_back(other.coeffs[i]);

    if (deque1.size() > deque2.size())
        while (deque1.size() != deque2.size())
        {
            result.push_back(0 + deque1.front());
            deque1.pop_front();
        }
    else if (deque2.size() > deque1.size())
        while (deque2.size() != deque1.size())
        {
            result.push_back(0 + deque2.front());
            deque2.pop_front();
        }

    while (!deque1.empty())
    {
        result.push_back(deque1.front() + deque2.front());
        deque1.pop_front();
        deque2.pop_front();
    }

    return HunPolynomial(result);
}

HunPolynomial HunPolynomial::operator-(const HunPolynomial &other) const
{
    std::vector<int> result;
    std::deque<int> deque1;
    std::deque<int> deque2;

    for (size_t i = 0; i < coeffs.size(); i++)
        deque1.push_back(coeffs[i]);
    for (size_t i = 0; i < other.coeffs.size(); i++)
        deque2.push_back(other.coeffs[i]);

    if (deque1.size() > deque2.size())
        while (deque1.size() != deque2.size())
        {
            result.push_back(deque1.front());
            deque1.pop_front();
        }
    else if (deque2.size() > deque1.size())
        while (deque2.size() != deque1.size())
        {
            result.push_back(0 - deque2.front());
            deque2.pop_front();
        }
    while (!deque1.empty())
    {
        result.push_back(deque1.front() - deque2.front());
        deque1.pop_front();
        deque2.pop_front();
    }

    return HunPolynomial(result);
}

HunPolynomial HunPolynomial::operator*(HunPolynomial &other)
{
    if (coeffs.size() == 0 || other.coeffs.size() == 0)
        return HunPolynomial();

    std::vector<int> result;
    std::vector<int> expres;
    std::deque<int> cleanExp1;
    std::deque<int> cleanExp2;
    std::deque<int> deque1;
    std::deque<int> deque2;
    std::vector<std::vector<int>> output = {{}, {}};

    for (size_t i = 0; i < coeffs.size(); i++)
    {
        if (coeffs[i] != 0)
        {
            cleanExp1.push_back(exponents[i]);
            deque1.push_back(coeffs[i]);
        }
    }

    for (size_t i = 0; i < other.coeffs.size(); i++)
    {
        if (other.coeffs[i] != 0)
        {
            deque2.push_back(other.coeffs[i]);
            cleanExp2.push_back(other.exponents[i]);
        }
    }

    while (!deque1.empty())
    {
        for (size_t j = 0; j < deque2.size(); j++)
        {
            result.push_back(deque1.front() * deque2.at(j));
            expres.push_back(cleanExp1.front() + cleanExp2.at(j));
        }
        deque1.pop_front();
        cleanExp1.pop_front();
    }

    for (size_t i = 0; i < expres.size(); i++)
    {
        for (size_t j = 0; j < expres.size() - i - 1; j++)
        {
            if (expres[j] < expres[j + 1])
            {
                std::swap(expres[j], expres[j + 1]);
                std::swap(result[j], result[j + 1]);
            }
        }
    }

    for (size_t i = 0; i < result.size(); i++)
    {
        if (expres[i] == expres[i + 1])
        {
            result[i] = result[i] + result[i + 1];
            result.erase(result.begin() + i + 1);
            expres.erase(expres.begin() + i + 1);
            i++;
        }
    }

    output[0].resize(result.size());
    output[1].resize(expres.size());

    output[0] = result;
    output[1] = expres;

    return HunPolynomial(output);
}

float HunPolynomial::operator()(const float x)
{
    float result = 0;

    for (size_t i = 0; i < coeffs.size(); i++)
    {
        if (x < 0)
            result += coeffs[i] * -(pow(x, i));
        else
            result += (coeffs[i] * pow(x, i));
    }
    return result;
}

std::ostream &operator<<(std::ostream &os, const HunPolynomial &obj)
{
    int size = obj.coeffs.size();
    int ocount = 0;
    if (obj.coeffs[0] < 0)
        os << "- ";
    for (size_t i = 0; i < size; i++)
    {
        if (obj.coeffs[i] != 0)
        {
            if (obj.exponents[i] == 0)
            {
                os << abs(obj.coeffs[i]);
            }
            else if (obj.exponents[i] == 1)
            {
                if (abs(obj.coeffs[i]) == 1)
                    os << "x";
                else
                    os << abs(obj.coeffs[i]) << "*x";
            }
            else
            {
                if (abs(obj.coeffs[i]) == 1)
                    os << "x^" << obj.exponents[i];
                else
                    os << abs(obj.coeffs[i]) << "*x^" << obj.exponents[i];
            }
            ocount++;
        }
        if (i != size - 1)
        {
            if (obj.coeffs[i + 1] > 0)
            {
                os << " + ";
            }
            else if (obj.coeffs[i + 1] < 0)
            {
                if (ocount == 0)
                {
                    os << "- ";
                    continue;
                }
                os << " - ";
            }
        }
    }

    return os;
}