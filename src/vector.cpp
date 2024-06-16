#include "vector.h"

Vector::Vector()
    : x { 0.f }, y { 0.f }
{}

Vector::Vector(float xin, float yin)
    : x { xin }, y { yin }
{}

Vector::Vector(sf::Vector2i vin)
    : x { static_cast<float>(vin.x) }, y { static_cast<float>(vin.y) }
{}

Vector::Vector(sf::Vector2f vin)
    : x { vin.x }, y { vin.y }
{}

bool Vector::operator == (const Vector& v) const
{
    return (x == v.x && y == v.y);
}

bool Vector::operator != (const Vector& v) const
{
    return (x != v.x || y != v.y);
}

Vector Vector::operator + (const Vector& v) const
{
    return Vector { x + v.x, y + v.y };
}

Vector Vector::operator - (const Vector& v) const
{
    return Vector { x - v.x, y - v.y };
}

Vector Vector::operator * (const float c) const
{
    return Vector { c * x, c * y };
}

Vector Vector::operator / (const float c) const
{
    return Vector { x / c, y / c };
}

void Vector::operator += (const Vector& v)
{
    x += v.x;
    y += v.y;
}

void Vector::operator -= (const Vector& v)
{
    x -= v.x;
    y -= v.y;
}

void Vector::operator *= (const float c)
{
    x *= c;
    y *= c;
}

void Vector::operator /= (const float c)
{
    x /= c;
    y /= c;
}

float& Vector::operator[] (int index)
{
    while(true)
    {
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        default:
            abort();
        }
    }
}

float Vector::length() const
{
    return std::sqrt(x * x + y * y);
}

float Vector::slength() const
{
    return x * x + y * y;
}

float Vector::dist(const Vector& v) const
{
    return (*this - v).length();
}

void Vector::normalize()
{
    *this /= length();
}

Vector Vector::rot30(bool clock)
{
    if (clock)
    {
        return Vector { x * cos30 - y * sin30, x * sin30 + y * cos30 };
    }
    else
    {
        return Vector { x * cos30 + y * sin30, x * -sin30 + y * cos30 };
    }
}
