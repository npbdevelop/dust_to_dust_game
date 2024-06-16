#ifndef VECTOR_H
#define VECTOR_H

#include <SFML/Graphics.hpp>
#include <cmath>

// Custom vector class

class Vector
{
public:
    Vector();
    Vector(float xin, float yin);
    Vector(sf::Vector2i vin);
    Vector(sf::Vector2f vin);

    float x;
    float y;

    float sin30 { 0.5f };
    float cos30 { std::sqrt(3) / 2 };

    bool operator == (const Vector& v) const;
    bool operator != (const Vector& v) const;

    Vector operator + (const Vector& v) const;
    Vector operator - (const Vector& v) const;
    Vector operator / (const float c) const;
    Vector operator * (const float c) const;

    void operator += (const Vector& v);
    void operator -= (const Vector& v);
    void operator /= (const float c);
    void operator *= (const float c);

    float& operator[] (int index);

    float dist(const Vector& v) const;
    float length() const;
    float slength() const;
    void normalize();

    Vector rot30(bool clock);
};

#endif