#pragma once

#include "mathematics/vector.h"
#include "mathematics/angle.h"

#include <cassert>

namespace neko
{
template<typename T>
class Mat3
{
private:
    std::array<Vec4 < T>, 4>
    columns_; //row vector
};

template<typename T>
class alignas(4 * sizeof(T)) Mat4
{
public:
    Mat4()
    {
        columns_ = Identity.columns_;
    }

    Mat4& operator=(Mat4 m)
    {
        columns_ = m.columns_;
        return *this;
    }

    Mat4(const Mat4& m) noexcept
    {
        columns_ = m.columns_;
    }

    explicit Mat4(const std::array<Vec4 < T>,4>& v)
    {
        columns_ = v;
    }

    const T& operator()(size_t row, size_t column) const
    {
        return columns_[column][row];
    }

    T& operator()(size_t row, size_t column)
    {
        return columns_[column][row];
    }

    const Vec4 <T>& operator[](size_t column) const
    {
        return columns_[column];
    }

    Vec4 <T>& operator[](size_t column)
    {
        return columns_[column];
    }

    inline Mat4<T> Transpose() const
    {
        std::array<Vec4<T>, 4> v;
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                v[column][row] = columns_[row][column];
            }
        }
        return Mat4<T>(v);
    }

    Mat4<T> operator+(const Mat4<T>& rhs) const
    {
        std::array<Vec4<T>, 4> v;
        for (int i = 0; i < 4; i++)
        {
            v[i] = columns_[i] + rhs.columns_[i];
        }
        return Mat4<T>(v);
    }

    Mat4<T> operator-(const Mat4<T>& rhs) const
    {
        std::array<Vec4<T>, 4> v;
        for (int i = 0; i < 4; i++)
        {
            v[i] = columns_[i] - rhs.columns_[i];
        }
        return Mat4<T>(v);
    }

    Vec4 <T> operator*(const Vec4 <T>& rhs) const
    {
        Vec4<T> v;
        for (int column = 0; column < 4; column++)
        {
            v[column] = 0;
            for (int row = 0; row < 4; row++)
            {
                v[column] += columns_[column][row] * rhs[row];
            }
        }
        return Mat4<T>(v);
    }

    Mat4<T> operator*(const Mat4<T>& rhs) const
    {
        return MultiplyIntrinsincs(rhs);
    }

    inline Mat4<T> MultiplyNaive(const Mat4<T>& rhs) const noexcept
    {
        std::array<Vec4<T>, 4> v;
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                v[column][row] = 0;
                for (int i = 0; i < 4; i++)
                {
                    v[column][row] += columns_[i][column] * rhs[row][i];
                }
            }
        }
        return Mat4<T>(v);
    }

    inline Mat4<T> MultiplyTranpose(const Mat4<T>& rhs) const noexcept
    {
        const auto lhsT = this->Transpose();
        std::array<Vec4<T>, 4> v;
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                v[column][row] = Vec4<T>::Dot(lhsT[row], Vec4<T>(rhs.columns_[column]));
            }
        }
        return Mat4<T>(v);
    }

    Mat4<T> MultiplyAoSoA(const Mat4<T>& rhs) const noexcept;

    Mat4<T> MultiplyIntrinsincs(const Mat4<T>& rhs) const noexcept;

    static T MatrixDifference(const Mat4<T>& rhs, const Mat4<T>& lhs)
    {
        T result = 0;
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                result += rhs[column][row] - lhs[column][row];
            }
        }
        return result;
    };

    static Mat4<T> Translate(const Mat4<T>& transform, Vec3 <T> pos);

    static Mat4<T> Scale(const Mat4<T>& transform, Vec3 <T> scale);

    static Mat4<T> Rotate(const Mat4<T>& transform, radian_t angle, Vec3 <T> axis);

    static Mat4<T> Rotate(const Mat4<T>& transform, Vec4 <T> quaternion);

    static Mat4<T> Rotate(const Mat4<T>& transform, Vec3 <degree_t> eulerAngles);
	
    static Mat4<T> FromQuaternion(Vec4<T> quaternion);

    static Mat4<T> Perspective(radian_t fovy, float aspect, float near, float far);
    const static Mat4<T> Identity;
    const static Mat4<T> Zero;
private:
    std::array<Vec4 < T>, 4>
    columns_; //row vector
};

template<typename T, int N>
struct alignas(N * sizeof(T)) NVec4
{
    std::array<T, N> xs;
    std::array<T, N> ys;
    std::array<T, N> zs;
    std::array<T, N> ws;

    NVec4()
            : xs{}, ys{}, zs{}, ws{}
    {
    };

    explicit NVec4(const Mat4<T>& m)
    {
        for (int row = 0; row < N; row++)
        {
            xs[row] = m[0][row];
            ys[row] = m[1][row];
            zs[row] = m[2][row];
            ws[row] = m[3][row];
        }
    }

    //Transpose the matrix
    explicit NVec4(const std::array<Vec4 < T>, N

    >& soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i][0];
            ys[i] = soaV[i][1];
            zs[i] = soaV[i][2];
            ws[i] = soaV[i][3];
        }
    }

    explicit NVec4(const Vec3 <T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
            ws[i] = 0.0f;
        }
    }

    explicit NVec4(const Vec4 <T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i][0];
            ys[i] = soaV[i][1];
            zs[i] = soaV[i][2];
            ws[i] = soaV[i][3];
        }
    }

    explicit NVec4(const Vec4 <T>& v)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = v.x;
            ys[i] = v.y;
            zs[i] = v.z;
            ws[i] = v.w;
        }
    }

    static std::array<T, N> Dot(const NVec4<T, N>& v1, const Vec4 <T>& v)
    {
        std::array<T, N> result{};
        for (int i = 0; i < N; i++)
        {
            result[0] += v1.xs[i] * v[i];
            result[1] += v1.ys[i] * v[i];
            result[2] += v1.zs[i] * v[i];
            result[3] += v1.ws[i] * v[i];
        }
        return result;
    }

    static std::array<T, N> Dot(const NVec4<T, N>& v1, const NVec4<T, N>& v2)
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = v1.xs[i] * v2.xs[i] + v1.ys[i] * v2.ys[i] + v1.zs[i] * v2.zs[i] + v1.ws[i] * v2.ws[i];
        }
        return result;
    }

    static std::array<T, N> DotIntrinsics(const NVec4<T, N>& v1, const NVec4<T, N>& v2);

    static std::array<T, N> DotIntrinsics(const NVec4<T, N>& v1, const Vec4 <T>& v);


    std::array<T, N> GetSquareMagnitude() const
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = xs[i] * xs[i] + ys[i] * ys[i] + zs[i] * zs[i] + ws[i] * ws[i];
        }
        return result;
    }

    std::array<T, N> GetMagnitude() const
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = xs[i] * xs[i] + ys[i] * ys[i] + zs[i] * zs[i] + ws[i] * ws[i];
            result[i] = std::sqrt(result[i]);
        }
        return result;
    }

    std::array<T, N> GetMagnitudeIntrinsincs() const;


};


using FourVec4f = NVec4<float, 4>;
using EightVec4f = NVec4<float, 8>;

#ifdef __SSE__

template<>
inline std::array<float, 4> FourVec4f::DotIntrinsics(const FourVec4f& v1, const FourVec4f& v2)
{
    __m128 x1 = _mm_load_ps(&v1.xs[0]);
    __m128 y1 = _mm_load_ps(&v1.ys[0]);
    __m128 z1 = _mm_load_ps(&v1.zs[0]);
    __m128 w1 = _mm_load_ps(&v1.ws[0]);

    __m128 x2 = _mm_load_ps(&v2.xs[0]);
    __m128 y2 = _mm_load_ps(&v2.ys[0]);
    __m128 z2 = _mm_load_ps(&v2.zs[0]);
    __m128 w2 = _mm_load_ps(&v2.ws[0]);

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);
    z1 = _mm_mul_ps(z1, z2);
    w1 = _mm_mul_ps(w1, w2);

    x1 = _mm_add_ps(x1, y1);
    z1 = _mm_add_ps(z1, w1);

    x1 = _mm_add_ps(x1, z1);

    std::array<float, 4> result;
    _mm_store_ps(&result[0], x1);
    return result;
}

template<>
inline std::array<float, 4> FourVec4f::DotIntrinsics(const FourVec4f& v1, const Vec4f& v2)
{
    __m128 x1 = _mm_load_ps(&v1.xs[0]);
    __m128 y1 = _mm_load_ps(&v1.ys[0]);
    __m128 z1 = _mm_load_ps(&v1.zs[0]);
    __m128 w1 = _mm_load_ps(&v1.ws[0]);

    __m128 v = _mm_load_ps(&v2[0]);

    x1 = _mm_mul_ps(x1, v);
    y1 = _mm_mul_ps(y1, v);
    z1 = _mm_mul_ps(z1, v);
    w1 = _mm_mul_ps(w1, v);

    x1 = _mm_add_ps(x1, y1);
    z1 = _mm_add_ps(z1, w1);

    x1 = _mm_add_ps(x1, z1);

    std::array<float, 4> result;
    _mm_store_ps(&result[0], x1);
    return result;
}

template<>
inline std::array<float, 4> FourVec4f::GetMagnitudeIntrinsincs() const
{
    __m128 x = _mm_load_ps(&xs[0]);
    __m128 y = _mm_load_ps(&ys[0]);
    __m128 z = _mm_load_ps(&zs[0]);
    __m128 w = _mm_load_ps(&ws[0]);

    x = _mm_mul_ps(x, x);
    y = _mm_mul_ps(y, y);
    z = _mm_mul_ps(z, z);
    w = _mm_mul_ps(w, w);

    x = _mm_add_ps(x, y);
    z = _mm_add_ps(z, w);

    x = _mm_add_ps(x, z);
    x = _mm_sqrt_ps(x);
    std::array<float, 4> result;
    _mm_store_ps(&result[0], x);
    return result;
}

#endif
#ifdef __AVX2__

template<>
inline std::array<float, 8> EightVec4f::DotIntrinsics(const EightVec4f& v1, const EightVec4f& v2)
{
    auto x1 = _mm256_load_ps(&v1.xs[0]);
    auto y1 = _mm256_load_ps(&v1.ys[0]);
    auto z1 = _mm256_load_ps(&v1.zs[0]);
    auto w1 = _mm256_load_ps(&v1.ws[0]);

    const auto x2 = _mm256_load_ps(&v2.xs[0]);
    const auto y2 = _mm256_load_ps(&v2.ys[0]);
    const auto z2 = _mm256_load_ps(&v2.zs[0]);
    const auto w2 = _mm256_load_ps(&v2.ws[0]);

    x1 = _mm256_mul_ps(x1, x2);
    y1 = _mm256_mul_ps(y1, y2);
    z1 = _mm256_mul_ps(z1, z2);
    w1 = _mm256_mul_ps(w1, w2);

    x1 = _mm256_add_ps(x1, y1);
    z1 = _mm256_add_ps(z1, w1);
    x1 = _mm256_add_ps(x1, z1);

    std::array<float, 8> result;
    _mm256_store_ps(&result[0], x1);
    return result;
}

template<>
inline std::array<float, 8> EightVec4f::GetMagnitudeIntrinsincs() const
{
    __m256 x = _mm256_load_ps(&xs[0]);
    __m256 y = _mm256_load_ps(&ys[0]);
    __m256 z = _mm256_load_ps(&zs[0]);
    __m256 w = _mm256_load_ps(&ws[0]);

    x = _mm256_mul_ps(x, x);
    y = _mm256_mul_ps(y, y);
    z = _mm256_mul_ps(z, z);
    w = _mm256_mul_ps(w, w);

    x = _mm256_add_ps(x, y);
    z = _mm256_add_ps(z, w);

    x = _mm256_add_ps(x, z);
    x = _mm256_sqrt_ps(x);
    std::array<float, 8> result;
    _mm256_storeu_ps(&result[0], x);
    return result;
}

#endif
using Mat3f = Mat3<float>;
using Mat4f = Mat4<float>;
using Transform3d = Mat4f;


#ifdef __SSE__
template<>
inline Mat4f Mat4f::Transpose() const
{
    std::array<Vec4f, 4> v;
    auto xmm3 = _mm_load_ps(&columns_[1][0]);
    auto xmm4 = _mm_load_ps(&columns_[3][0]);
    auto xmm0 = _mm_load_ps(&columns_[0][0]);
    auto xmm1 = _mm_load_ps(&columns_[2][0]);

    auto xmm2 = _mm_shuffle_ps(xmm0, xmm3, 136);
    xmm0 = _mm_shuffle_ps(xmm0, xmm3, 221);
    xmm3 = _mm_shuffle_ps(xmm1, xmm4, 136);
    xmm1 = _mm_shuffle_ps(xmm1, xmm4, 221);
    xmm4 = _mm_shuffle_ps(xmm2, xmm3, 136);
    xmm2 = _mm_shuffle_ps(xmm2, xmm3, 221);
    _mm_store_ps(&v[0][0], xmm4);
    xmm4 = _mm_shuffle_ps(xmm0, xmm1, 136);
    _mm_store_ps(&v[2][0], xmm2);
    xmm0 = _mm_shuffle_ps(xmm0, xmm1, 221);
    _mm_store_ps(&v[1][0], xmm4);
    _mm_store_ps(&v[3][0], xmm0);
    return Mat4f(v);
}
#endif
template<>
Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept;


template<typename T>
inline Mat4<T> Mat4<T>::MultiplyAoSoA(const Mat4<T>& rhs) const noexcept
{
    const auto lhsT(Transpose());
    std::array<Vec4f, 4> v;
    for (int column = 0; column < 4; column++)
    {
    	for(int row = 0; row < 4; row++)
    	{
            const auto result = Vec4f::Dot(lhsT[row], rhs.columns_[column]);
            v[column][row] = result;
    	}
        
    }
    return Mat4<T>(v);
}

#ifdef __SSE__
template<>
inline Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept
{
    const auto lhsT(Transpose());
    std::array<Vec4f, 4> v;
    for (int column = 0; column < 4; column++)
    {

        __m128 c = _mm_load_ps(&rhs[column][0]);
    	
        __m128 x = _mm_load_ps(&lhsT[0][0]);
        __m128 y = _mm_load_ps(&lhsT[1][0]);
        __m128 z = _mm_load_ps(&lhsT[2][0]);
        __m128 w = _mm_load_ps(&lhsT[3][0]);


        x = _mm_mul_ps(x, c);
        y = _mm_mul_ps(y, c);
        z = _mm_mul_ps(z, c);
        w = _mm_mul_ps(w, c);

        x = _mm_add_ps(x, y);
        z = _mm_add_ps(z, w);

        x = _mm_add_ps(x, z);
        _mm_store_ps(&v[column][0], x);
    }
    return Mat4f(v);
}
#endif

#ifdef EMSCRIPTEN
template<>
inline Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept
{
    const auto lhsT(Transpose());
    std::array<Vec4f, 4> v;
    for (int column = 0; column < 4; column++)
    {

        v4f c = *(v4f*)(&rhs[column][0]);

        v4f x = *(v4f*)(&lhsT[0][0]);
        v4f y = *(v4f*)(&lhsT[1][0]);
        v4f z = *(v4f*)(&lhsT[2][0]);
        v4f w = *(v4f*)(&lhsT[3][0]);


        x = (x*c);
        y = (y*c);
        z = (z*c);
        w = (w*c);

        x = (x+y);
        z = (z+w);

        x = (x+z);
        *(v4f*)(&v[column][0]) =  x;
    }
    return Mat4f(v);
}
#endif

//TODO Implement Matrix Translation
template<>
inline Transform3d Transform3d::Translate(const Transform3d& transform, Vec3f pos)
{
    // TODO: Why isn't Identity mat working? Inline?
    // Question: comment disable CPU scaling?

    Transform3d output = transform;

    Transform3d translationMat = Transform3d();

    translationMat[3][0] = pos[0];
    translationMat[3][1] = pos[1];
    translationMat[3][2] = pos[2];
    translationMat[3][3] = 0.0f;

    return output + translationMat;
}

//TODO Implement Matrix Scale
template<>
inline Transform3d Transform3d::Scale(const Transform3d& transform, Vec3f scale)
{
    // TODO: Why isn't this approach working?
    // Transform3d output = transform;

    Transform3d scalingMat = Transform3d();
// rotate then scale then translate
    scalingMat[0][0] = scale[0];
    scalingMat[1][1] = scale[1];
    scalingMat[2][2] = scale[2];
    scalingMat[3][3] = 1.0f;

    return transform * scalingMat; // scaling * transform

    /*Transform3d output = transform;

    output[0][0] *= scale[0];
    output[1][1] *= scale[1];
    output[2][2] *= scale[2];

    return output;*/

}

//TODO Implement Matrix Rotation with angle and axis
template<>
inline Transform3d Transform3d::Rotate(const Transform3d& transform, radian_t angle, Vec3f axis)
{
    // todo separate header

    angle = angle * (PI / 180.0f); // use mathemtatics .h

    // TODO: why isn't this approach working with mat*mat?
    // Transform3d output = transform;
    Transform3d rotationMat = Transform3d();
    axis = axis / axis.GetMagnitude();
    const float x = axis[0];
    const float y = axis[1];
    const float z = axis[2];
    const float c = cosf(angle);
    const float s = sinf(angle);
    const float t = 1.0f - c;

    rotationMat[0][0] = t*x*x + c;
    rotationMat[0][1] = t*x*y + s*z;
    rotationMat[0][2] = t*x*z - s*y;
    rotationMat[0][3] = 0.0f;

    rotationMat[1][0] = t*x*y - s*z;
    rotationMat[1][1] = t*y*y + c;
    rotationMat[1][2] = t*y*z + s*x;
    rotationMat[1][3] = 0.0f;

    rotationMat[2][0] = t*x*z + s*y;
    rotationMat[2][1] = t*y*z - s*x;
    rotationMat[2][2] = t*z*z + c;
    rotationMat[2][3] = 0.0f;

    rotationMat[3][0] = 0.0f;
    rotationMat[3][1] = 0.0f;
    rotationMat[3][2] = 0.0f;
    rotationMat[3][3] = 1.0f;

    return transform * rotationMat;

    // TODO: Why isn't this approach working with *=

    // TODO: add individual functions for matrices generation (rotation, translation, scaling, etc.)

    /*Transform3d output = transform;
    axis = axis / axis.GetMagnitude();
    const float x = axis[0];
    const float y = axis[1];
    const float z = axis[2];
    const float c = cosf(angle);
    const float s = sinf(angle);
    const float t = 1.0f - c;

    output[0][0] *= t*x*x + c;
    output[0][1] *= t*x*y + s*z;
    output[0][2] *= t*x*z - s*y;

    output[1][0] *= t*x*y - s*z;
    output[1][1] *= t*y*y + c;
    output[1][2] *= t*y*z + s*x;

    output[2][0] *= t*x*z + s*y;
    output[2][1] *= t*y*z - s*x;
    output[2][2] *= t*z*z + c;

    return output;*/

    // Copy paste du pokengine...
    const float angleCos = cos(angle);
    const float angleSin = sin(angle);

    Vec3 axisNorma = axis / axis.GetMagnitude();
    Vec3 temp = axisNorma * (1.0f - angleCos);

    Mat4f rotate = Mat4f();

    rotate[0][0] = angleCos + temp.x * axisNorma.x;
    rotate[0][1] = temp.x * axisNorma.y + angleSin * axisNorma.z;
    rotate[0][2] = temp.x * axisNorma.z - angleSin * axisNorma.y;

    rotate[1][0] = temp.y * axisNorma.x - angleSin * axisNorma.z;
    rotate[1][1] = angleCos + temp.y * axisNorma.y;
    rotate[1][2] = temp.y * axisNorma.z + angleSin * axisNorma.x;

    rotate[2][0] = temp.z * axisNorma.x + angleSin * axisNorma.y;
    rotate[2][1] = temp.z * axisNorma.y - angleSin * axisNorma.x;
    rotate[2][2] = angleCos + temp.z * axisNorma.z;

    Mat4f result = Mat4f();
    result[0] = transform[0] * rotate[0][0] + transform[1] * rotate[0][1] +
                transform[2] * rotate[0][2];
    result[1] = transform[0] * rotate[1][0] + transform[1] * rotate[1][1] +
                transform[2] * rotate[1][2];
    result[2] = transform[0] * rotate[2][0] + transform[1] * rotate[2][1] +
                transform[2] * rotate[2][2];
    result[3] = transform[3];

    return result;
}

//TODO Implement Matrix Rotation with Quaternion
template<>
inline Transform3d Transform3d::Rotate(const Transform3d& transform, Quaternion quaternion)
{
    // Code taken from https://stackoverflow.com/questions/1556260/convert-quaternion-rotation-to-rotation-matrix

    // TODO: test once quaterions are implemented.

    /*Transform3d output = transform;

    return output * FromQuaternion(quaternion);*/

    (void) quaternion;
    return transform;
}

//TODO Implement Matrix Rotation with Simple Euler Angles
template<>
inline Transform3d Transform3d::Rotate(const Transform3d& transform, EulerAngles eulerAngles)
{
    // Question: can I modify the function's signature? Rotate(const Vec3f) instead

    // TODO: why isn't this working because of the *=

    /*
    const Vec3f radians = eulerAngles * (M_PI / 180.0f);
    const float rotX = eulerAngles[0];
    const float rotY = eulerAngles[1];
    const float rotZ = eulerAngles[2];
    Transform3d output = transform;
    const float cosX = cosf(rotX);
    const float sinX = sinf(rotX);
    const float cosY = cosf(rotY);
    const float sinY = sinf(rotY);
    const float cosZ = cosf(rotZ);
    const float sinZ = sinf(rotZ);

    // Rotation around X:
    output[1][1] *= cosX;
    output[2][1] *= -sinX;
    output[1][2] *= sinX;
    output[2][2] *= cosX;

    // Rotation around Y:
    output[0][0] *= cosY;
    output[2][0] *= sinY;
    output[0][2] *= -sinY;
    output[2][2] *= cosY;

    // Rotation around Z:
    output[0][0] *= cosZ;
    output[1][0] *= -sinZ;
    output[0][1] *= sinZ;
    output[1][1] *= cosZ;

    return output;*/

    // TODO: Why isn't this working? scaling bug instead of rotation

    /*const float rotX = eulerAngles[0];
    const float rotY = eulerAngles[1];
    const float rotZ = eulerAngles[2];
    Transform3d output = transform;
    const float cosX = cosf(rotX);
    const float sinX = sinf(rotX);
    const float cosY = cosf(rotY);
    const float sinY = sinf(rotY);
    const float cosZ = cosf(rotZ);
    const float sinZ = sinf(rotZ);

    Transform3d rotationMatX = Transform3d();
    Transform3d rotationMatY = Transform3d();
    Transform3d rotationMatZ = Transform3d();
    rotationMatX[0][0] = 1.0f;
    rotationMatX[1][1] = 1.0f;
    rotationMatX[2][2] = 1.0f;
    rotationMatX[3][3] = 1.0f;
    rotationMatY[0][0] = 1.0f;
    rotationMatY[1][1] = 1.0f;
    rotationMatY[2][2] = 1.0f;
    rotationMatY[3][3] = 1.0f;
    rotationMatZ[0][0] = 1.0f;
    rotationMatZ[1][1] = 1.0f;
    rotationMatZ[2][2] = 1.0f;
    rotationMatZ[3][3] = 1.0f;

    // Rotation around X:
    rotationMatX[1][1] *= cosX;
    rotationMatX[2][1] *= -sinX;
    rotationMatX[1][2] *= sinX;
    rotationMatX[2][2] *= cosX;
    output = output * rotationMatX;

    // Rotation around Y:
    rotationMatY[0][0] *= cosY;
    rotationMatY[2][0] *= sinY;
    rotationMatY[0][2] *= -sinY;
    rotationMatY[2][2] *= cosY;
    output = output * rotationMatY;

    // Rotation around Z:
    rotationMatZ[0][0] *= cosZ;
    rotationMatZ[1][0] *= -sinZ;
    rotationMatZ[0][1] *= sinZ;
    rotationMatZ[1][1] *= cosZ;
    output = output * rotationMatZ;

    return output;*/

    (void) eulerAngles;
    return transform;
}

template<>
const inline Mat4f Mat4f::Identity = Mat4f(
        std::array<Vec4f, 4>
        {
                Vec4f(1, 0, 0, 0),
                Vec4f(0, 1, 0, 0),
                Vec4f(0, 0, 1, 0),
                Vec4f(0, 0, 0, 1)});
template <>
const inline Mat4f Mat4f::Zero = Mat4f(
        std::array<Vec4f, 4>
        {
            Vec4f::Zero,
            Vec4f::Zero,
            Vec4f::Zero,
            Vec4f::Zero
        });

template <>
inline Mat4f Mat4f::Perspective(radian_t fovy, float aspect, float near, float far)
{
    assert(fabsf(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);

    const float tanHalfFovy = tanf(fovy.value() / 2.0f);
    Mat4f perspective{Mat4f::Zero};

    perspective[0][0] = 1.0f/ (aspect * tanHalfFovy);
    perspective[1][1] = 1.0f / (tanHalfFovy);
    perspective[2][2] = - (far + near) / (far - near);
    perspective[2][3] = - 1.0f;
    perspective[3][2] = - (2.0f * far * near) / (far - near);
    return perspective;
}

template<>
inline Transform3d Transform3d::FromQuaternion(Quaternion quaternion)
{
    // TODO: is this working?

    /*float w = quaternion[0];
    float x = quaternion[1];
    float y = quaternion[2];
    float z = quaternion[3];

    const float n = 1.0f / (sqrtf(x*x+y*y+z*z+w*w));
    x *= n;
    y *= n;
    z *= n;
    w *= n;

    Transform3d rotationMat = Transform3d();

    rotationMat[0][0] = 1.0f - (2.0f * y * y) - (2.0f * z * z);
    rotationMat[1][0] = (2.0f*x*y) - (2.0f*z*w);
    rotationMat[2][0] = (2.0f*x*z) + (2.0f*y*w);
    rotationMat[3][0] = 0.0f;

    rotationMat[0][1] = (2.0f*x*y + 2.0f*z*w);
    rotationMat[1][1] = 1.0f - (2.0f*x*x) - (2.0f*z*z);
    rotationMat[2][1] = (2.0f*y*z) - (2.0f*x*w);
    rotationMat[3][1] = 0.0f;

    rotationMat[0][2] = (2.0f*x*z) - (2.0f*y*w);
    rotationMat[1][2] = (2.0f*y*z) + (2.0f*x*w);
    rotationMat[2][2] = 1.0f - (2.0f*x*x) - (2.0f*y*y);
    rotationMat[3][2] = 0.0f;

    rotationMat[0][3] = 0.0f;
    rotationMat[1][3] = 0.0f;
    rotationMat[2][3] = 0.0f;
    rotationMat[3][3] = 1.0f;

    return rotationMat;*/

    (void) quaternion;
    return Mat4f::Identity;
}

}