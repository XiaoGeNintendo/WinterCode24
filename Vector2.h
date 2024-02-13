#pragma once
#include <cmath>
#include <utility>
using namespace std;

template <typename T> struct Vector2D {
    T x, y;

    // Constructors
    Vector2D() : x(0), y(0) {}
    Vector2D(T x, T y) : x(x), y(y) {}
    Vector2D(pair<T, T> p) :x(p.first), y(p.second) {}

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

    bool operator<(Vector2D another) {
        return this->p() < another.p();
    }

    bool operator!=(Vector2D another) const {
        return x != another.x || y != another.y;
    }

    bool operator==(Vector2D another) const {
        return x == another.x && y == another.y;
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

    T sum() const {
        return x + y;
    }

    pair<T, T> p() const {
        return make_pair(x, y);
    }
};

#define VecI Vector2D<int>
#define VecF Vector2D<float>
#define VecD Vector2D<double>

VecD i2d(VecI x);
VecI d2i(VecD x);