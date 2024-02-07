#pragma once
#include <cmath>

template <typename T> struct Vector2D {
    T x, y;

    // Constructors
    Vector2D() : x(0), y(0) {}
    Vector2D(T x, T y) : x(x), y(y) {}

    // Arithmetic operators
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D operator*(T scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D operator/(T scalar) const {
        return Vector2D(x / scalar, y / scalar);
    }

    Vector2D& operator=(Vector2D another){
        x = another.x;
        y = another.y;
        return *this;
    }

    // Common functions
    T dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }

    T magnitude() const {
        return sqrt(x * x + y * y);
    }

    Vector2D normalize() const {
        T mag = magnitude();
        if (mag != 0) {
            return *this * (1 / mag);
        }
        return *this;
    }

};

#define VecI Vector2D<int>
#define VecF Vector2D<float>
#define VecD Vector2D<double>

