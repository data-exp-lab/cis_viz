#ifndef __RAY__
#define __RAY__

#include "point.hpp"
#include "normal.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;
class Point;
class Normal;

//Normal* Normal;

//double x, y, z;
namespace raytrace
{
    template<typename T>
    class Vector
    {
    private:
        //T x, y, z;
    public:
        //class Normal *Normal;

        T x, y, z;
        Vector();
        Vector(const T &n);
        Vector(const T &s, const T &t, const T &u);
        Vector(const Vector &vector);
        Vector(const Normal &n);// : x(n.x), y(n.y), z(n.z){}
        T getX();
        T getY();
        T getZ();
        Vector<T> operator + (const T &n) const;
        Vector<T> operator + (const Vector<T> &n) const;
        Vector<T> operator - (const T &n) const;
        Vector<T> operator - (const Vector<T> &n) const;
        Vector<T> operator * (const T &n) const;
        Vector<T> operator * (const Vector<T> &n) const;
        Vector<T> operator / (const T &n) const;
        Vector<T> operator / (const Vector<T> &n) const;
        Vector<T> operator += (const T &n);
        Vector<T> operator += (const Vector<T> &n);
        Vector<T> operator= (const Point& rhs);
        Vector<T> operator^(const Vector<T>& v);
        
        //Vector<T> length(void)
        //{
        //    return (sqrt(x * x + y * y + z * z));
        //}
        friend std::ostream & operator << (std::ostream &os, const Vector<T> &v);
        T dotProduct(const Vector<T> &v) const;
        Vector<T> crossProduct(const Vector<T> &v) const;
        T magnitude(void) const;
        Vector<T> &normalize();
        T sum() const;
                                                                   
       T getVectX();
       T getVectY();
       T getVectZ();
        
        //Vect(const Normal &n) : x(n.x), y(n.y), z(n.z){}
        
        Vector<T> vectAdd(Vector<T> v);
        
        Vector<T> vectMult(T scalar);
        
        Vector<T> negative();
        
        Vector<T> operator=(const Vector<T>& rhs);
        
        T length(void);
    };

    inline double operator/(const double a, const raytrace::Vect& v)
    {
        return(a / v.x, a / v.y, a / v.z);
    }

    
/*
    class Camera
    {
    private:
        Vect eye;
        Vect lookAt;
        Vect up;
        float right;
        float left;
        float top;
        float bottom;
        float focalLength;
    public:
        Camera() : right(0.0), left(0.0), top(0.0), bottom(0.0), focalLength(0.0){}
        
        Camera(const Vect &e, const Vect &la, const Vect &u, const float &fl) : eye(e), lookAt(la), up(u), right(0.0), left(0.0), top(0.0), bottom(0.0), focalLength(fl){}
        
        Vect getEye() const
        {
            return eye;
        }
        
        //GET CAMERA COORDINATES
        Vect getW() const
        {
            return (eye - lookAt).normalize();
        }
        Vect getU() const
        {
            return (up.crossProduct(getW())).normalize();
        }
        
        Vect getV() const
        {
            return getW().crossProduct(getU());
        }
        
        //GETTER AND SETTER METHODS
        float getRight() const
        {
            return right;
        }
        
        float getLeft() const
        {
            return left;
        }
        
        float getTop() const
        {
            return top;
        }
        float getBottom() const
        {
            return bottom;
        }
        float getFocalLength() const
        {
            return focalLength;
        }
        void setRight(const float &r)
        {
            right = r;
        }
        void setLeft(const float &l)
        {
            left = l;
        }
        void setTop(const float &t)
        {
            top = t;
        }
        void setBottom(const float &b)
        {
            bottom = b;
        }
    };
 */
    class Ray
    {

    public:
        Point origin;
        Vect direction;
        Vect position;
        double photonFlux2;
        
        Vect origin_vect;
        Vect direction_vect;
        
        Ray(void);
        Ray(const Point &ori, const raytrace::Vect &dir);
        Ray(const Point &origin, const raytrace::Vect& direction, const double pf);
        Ray(const Ray& ray);
        Ray(const Ray* ray_ptr);
        Ray& operator= (const Ray& rhs);
        
        Ray(const raytrace::Vect &position, const raytrace::Vect &direction);
        
        ~Ray(void);
        
        Point getOrigin() const;
        Vect getOriginVect() const;
        Vect getDirection() const;
    };
 /*
    class Light
    {
    private:
        Vect position;
        Vect color;
    public:
        Light(){}
        Light(const Vect pos, const Vect col) : position(pos), color(col){}
        Vect getPosition() const
        {
            return position;
        }
        Vect getColor() const
        {
            return color;
        }
    };
    
    class Material
    {
    private:
        Vect KA;
        Vect KD;
        Vect KS;
        Vect KR;
        Vect KO;
        float level;
        float eta;
    public:
        Material() : level(0.0) {}
        Material(const Vect &ka, const Vect &kd, const Vect &ks, const Vect &kr, const Vect &ko, const float &lv, const float &e) : KA(ka), KD(kd), KS(ks), KR(kr), KO(ko), level(lv), eta(e){}
        Vect getKA() const
        {
            return KA;
        }
        Vect getKD() const
        {
            return KD;
        }
        Vect getKS() const
        {
            return KS;
        }
        Vect getKR() const
        {
            return KR;
        }
        Vect getKO() const
        {
            return KO;
        }
        float getLevel() const
        {
            return level;
        }
        float getETA() const
        {
            return eta;
        }
    };
    
    class Shape
    {
    protected:
        Vect position;
        Material material;
        std::vector<Light> lights;
    public:
        Shape(){}
        virtual ~Shape(){}
        virtual bool intersect(const Ray &ray, float &t0, float &t1) = 0;
        Vect getPos() const
        {
            return position;
        }
        Material getMat() const
        {
            return material;
        }
        std::vector<Light> getLights() const
        {
            return lights;
        }
    };
    
    class Sphere : public Shape
    {
    private:
        float radius;
        float radSqrd;
    public:
        Sphere() : radius(0.0), radSqrd(0.0){}
        Sphere(const Vect &pos, const float &r, const Material &mat, const std::vector<Light> &lights) : radius(r), radSqrd(r * r)
        {
            Shape::position = pos;
            Shape::material = mat;
            Shape::lights = lights;
        }
        bool intersect(const Ray &ray, float &t0, float &t1);
    };
    
    class Triangle : public Shape
    {
    private:
        Vect A;
        Vect B;
        Vect C;
        Vect surfaceNorm;
    public:
        Triangle(){}
        Triangle(const Vect &a, const Vect &b, const Vect &c, const Vect &sn, const Material &mat, const std::vector<Light> &lights) : A(a), B(b), C(c), surfaceNorm(sn)
        {
            Shape::material = mat;
            Shape::lights = lights;
        }
        bool intersect(const Ray &ray, float &t0, float &t1);
        Vect getSideA() const
        {
            return A;
        }
        Vect getSideB() const
        {
            return B;
        }
        Vect getSideC() const
        {
            return C;
        }
    };
    
    class Scene
    {
    private:
        std::vector<Shape*> shapes;
        Camera camera;
        int height;
        int width;
        std::vector<std::string> split(const std::string &str, const char &delimit);
    public:
        Scene(const std::string &fileName, const int &h, const int &w);
        void render(void);
        Vect draw(const Ray &ray, const int &level);
    };
*/}

template <typename T> std::ostream& operator << (std::ostream &os, const raytrace::Vector<T> &v);
template <typename T, typename T2> T operator*(const T2 a, const raytrace::Vector<T> & v);

#endif

