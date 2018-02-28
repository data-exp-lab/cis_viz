#include "ray.hpp"
#include "point.hpp"
using namespace std;
using namespace raytrace;

float mix(const float &a, const float &b, const float &mix)
{
    return b * mix + a * (1 - mix);
}

Ray::Ray(void) : origin(0.0), direction(0.0, 0.0, 1.0), photonFlux2(0)
{}

Ray::Ray(const Point &origin, const Vect &direction, const double pf) : origin(origin), direction(direction), photonFlux2(pf)
{}

Ray::Ray(const Ray& ray) : origin(ray.origin), direction(ray.direction), photonFlux2(ray.photonFlux2)
{}

Ray::Ray(const Ray* ray_ptr) : origin(ray_ptr->origin), direction(ray_ptr->direction), photonFlux2(ray_ptr->photonFlux2)
{}

Ray& Ray::operator= (const Ray& rhs)
{
    if(this == &rhs)
    {
        return (*this);
    }
    
    origin = rhs.origin;
    direction = rhs.direction;
    
    return (*this);
}

Ray::~Ray(void)
{
    
}


/*Vect refractRay(Vect const &W, Vect const &pt, Vect const &N, float const &eta1, float const &eta2, Shape const &shape)
{
    float etar = eta1 / eta2;
    float a = etar * -1;
    float wn = W.dotProduct(N);
    float radsq = 1 - (etar * etar) * (1 - wn * wn);
    Vect T(0.0);
    if(radsq > 0)
    {
        float b = (etar * wn) - sqrt(radsq);
        T = (W * a) + (N * b);
    }
    return T;
}

Ray refractExit(Vect const &W, Vect const &pt, float const &eta_in, Shape const &shape)
{
    Ray refR;
    Vect T1 = refractRay(W, pt, (pt - shape.getPos()).normalize(), 1.0, eta_in, shape);
    float t1_sum = T1.sum();
    if(t1_sum != 0.0)
    {
        Vect exit = pt + T1 * 2 * (shape.getPos() - pt).dotProduct(T1);
        Vect Nin = shape.getPos() - exit;
        Vect T2 = refractRay(T1 * -1, exit, Nin, eta_in, 1.0, shape);
        refR = Ray(exit, T2);
    }
    return refR;
}*/

//INLINE FUNCTIONS
/*
inline Vect Vect::operator- (void) const
{
    return (Vect(-x, -y, -z));
}

inline double Vect::len_squared(void)
{
    return (x * x + y * y + z * z);
}

inline Vect Vect::operator* (const double a) const
{
    return (Vect(x * a, y * a, z * a));
}

inline Vect Vect::operator/ (const double a) const
{
    return (Vect(x / a, y / a, z / a));
}

inline Vect Vect::operator+ (const Vect& v) const
{
    return (Vect(x + v.x, y + v.y, z + v.z));
}

inline Vect Vect::operator- (const Vect& v) const
{
    return (Vect(x - v.x, y - v.y, z - v.z));
}

inline double Vect::operator* (const Vect& v) const
{
    return (x * v.x + y * v.y + z * v.z);
}

inline raytrace::Vect Vect::operator^ (const Vect& v) const
{
    return (Vect(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x));
}

inline Vect& Vect::operator+= (const Vect& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    
    return (*this);
}
*/



/*
bool Sphere::intersect(const Ray &ray, float &t0, float &t1)
{
    Vect L = ray.getOrigin();
    Vect D = ray.getDirection();
    Vect tv = getPos() - L;
    float v = tv.dotProduct(D);
    if(v < 0)
    {
        return false;
    }
    float dsq = tv.dotProduct(tv) - (v * v);
    if(dsq > radSqrd)
    {
        return false;
    }
    float b = sqrt(radSqrd - dsq);
    t0 = v - b;
    t1 = v + b;
    
    return true;
}

bool Triangle::intersect(const Ray &ray, float &t0, float &t1)
{
    Vect E1 = B - A;
    Vect E2 = C - A;
    
    Vect dir = ray.getDirection();
    Vect ori = ray.getOrigin();
    
    Vect pVect = dir.crossProduct(E2);
    float det = E1.dotProduct(pVect);
    float invDet = 1 / det;
    
    if(det < 0.0000001f)
    {
        return false;
    }
    
    Vect tVect = ori - A;
    float u = tVect.dotProduct(pVect)*invDet;
    if(u < 0.0 || u > 1)
    {
        return false;
    }
    
    Vect qVect = tVect.crossProduct(E1);
    float v = dir.dotProduct(qVect) * invDet;
    
    if(v < 0 || u + v > 1)
    {
        return false;
    }
    t0 = E2.dotProduct(qVect) * invDet;
    
    return true;
}

Vect Scene::draw(const Ray &ray, const int &level)
{
    Vect ori = ray.getOrigin();
    Vect dir = ray.getDirection();
    
    float tNear = INFINITY;
    Vect pixelValue;
    
    const Shape *shape = NULL;
    
    if(!shapes.empty())
    {
        for(unsigned i=0; i < shapes.size(); i++)
        {
            float t0 = INFINITY;
            float t1 = INFINITY;
            if(shapes[i]->intersect(ray, t0, t1))
            {
                if(t0 < 0)
                {
                    t0 = t1;
                }
                if(t0 < tNear)
                {
                    tNear = t0;
                    shape = shapes[i];
                }
            }
        }
    }
    
    if(!shape)
    {
        return Vect(0.0);
    }
    Vect ptos = ori + (dir * tNear);
    Vect snorm = (ptos - shape->getPos()).normalize();
    float bias = 1.0e-4;
    
    for(unsigned i=0; i < shape->getLights().size(); i++)
    {
        Vect ptl = (shape->getLights()[i].getPosition() - ptos).normalize();
        Vect eml = shape->getLights()[i].getColor();
        if(snorm.dotProduct(ptl) > 0)
        {
            pixelValue += eml * shape->getMat().getKD() * std::max(float(0), snorm.dotProduct(ptl));
            pixelValue += eml * shape->getMat().getKS() * std::max(float(0), (float)pow(snorm.dotProduct(ptl), shape->getMat().getLevel()));
        }
    }
    
    if(level > 0)
    {
        float faceLevel = dir.dotProduct(snorm);
        float lenseEffect = mix(pow(1 - faceLevel, 4), 1, 0.1);
        
        Vect reflectDirect(dir - snorm * 2 * faceLevel);
        reflectDirect.normalize();
        
        Ray reflectRay((ptos + snorm * bias), reflectDirect);
        Vect reflection(draw(reflectRay, level - 1));
        Ray refractRay = refractExit(dir * -1, ptos, shape->getMat().getETA(), *shape);
        Vect refraction(0.0);
        
        if(refractRay.getDirection().sum() && shape->getMat().getKO().sum())
        {
            refraction = draw(refractRay, level - 1);
        }
        pixelValue += (reflection * lenseEffect * shape->getMat().getKR() + refraction * lenseEffect * shape->getMat().getKO()) * shape->getMat().getKD();
    }
    return pixelValue;
}

std::vector<std::string> Scene::split(const std::string &str, const char &delimit)
{
    std::vector<std::string> result;
    std::string data;
    std::stringstream ss(str);
    
    while(getline(ss, data, delimit))
    {
        result.push_back(data);
    }
    return result;
}

Scene::Scene(const std::string &fileName, const int &h, const int &w)
{
    height = h;
    width = w;
    int lineCount = 1;
    std::vector<std::string> lineSplit;
    std::ifstream infile;
    infile.open(fileName, std::ios::binary | std::ios::in);
    
    if(!infile)
    {
        std::cerr << "Error: file " << fileName << " does not exist." << std::endl;
        exit(102);
    }
    
    //READ FROM CAMERA
    float x,y,z;
    std::string line;
    getline(infile, line);
    lineSplit = split(line, ' ');
    
    if(lineSplit[0] != "eye")
    {
        std::cerr << "Error: incorrect file format in line: " << lineCount << std::endl;
        std::cerr << "Line shows: " << line << std::endl;
        exit(103);
    }
    
    //CAMERA EYE
    x = std::stof(lineSplit[1]);
    y = std::stof(lineSplit[2]);
    z = std::stof(lineSplit[3]);
    
    lineCount++;
    Vect eye(x,y,z);
    
    getline(infile, line);
    lineSplit = split(line, ' ');
    
    if(lineSplit[0] != "look")
    {
        std::cerr << "Error: incorrect file format in line: " << lineCount << std::endl;
        std::cerr << "Line shows: " << line << std::endl;
        exit(103);
    }
    
    //CAMERA DIRECTION
    x = std::stof(lineSplit[1]);
    y = std::stof(lineSplit[2]);
    z = std::stof(lineSplit[3]);
    
    lineCount++;
    Vect lookAt(x,y,z);
    
    getline(infile, line);
    lineSplit = split(line, ' ');
    
    if(lineSplit[0] != "up")
    {
        std::cerr << "Error: incorrect file format in line: " << lineCount << std::endl;
        std::cerr << "Line shows: " << line << std::endl;
        exit(103);
    }
    
    //UP DIRECTION
    x = std::stof(lineSplit[1]);
    y = std::stof(lineSplit[2]);
    z = std::stof(lineSplit[3]);
    
    lineCount++;
    Vect up(x,y,z);
    
    getline(infile, line);
    lineSplit = split(line, ' ');
    
    if(lineSplit[0] != "fl")
    {
        std::cerr << "Error: incorrect file format in line: " << lineCount << std::endl;
        std::cerr << "Line shows: " << line << std::endl;
        exit(103);
    }
    
    float focalLength = std::stof(lineSplit[1]);
    lineCount++;
    
    Camera c(eye, lookAt, up, focalLength);
    camera = c;
    
    getline(infile, line);
    lineSplit = split(line, ' ');
    
    if(lineSplit[0] != "plane")
    {
        std::cerr << "Error: incorrect file format in line: " << lineCount << std::endl;
        std::cerr << "Line shows: " << line << std::endl;
        exit(103);
    }
    
    lineCount++;
    camera.setRight(std::stof(lineSplit[1]));
    camera.setLeft(std::stof(lineSplit[2]));
    camera.setTop(std::stof(lineSplit[3]));
    camera.setBottom(std::stof(lineSplit[4]));
    
    //LIGHTS
    getline(infile, line);
    lineSplit = split(line, ' ');
    std::vector<Light> lights;
    
    while(lineSplit[0] == "light")
    {
        Light light(Vect(std::stof(lineSplit[1]), std::stof(lineSplit[2]), std::stof(lineSplit[3])),
                    Vect(std::stof(lineSplit[4]), std::stof(lineSplit[5]), std::stof(lineSplit[6])));
        lights.push_back(light);
        
        lineCount++;
        getline(infile, line);
        lineSplit = split(line, ' ');
    }
    
    while(lineSplit[0] == "material")
    {
        Material material(Vect(std::stof(lineSplit[1]), std::stof(lineSplit[2]), std::stof(lineSplit[3])), Vect(std::stof(lineSplit[4]), std::stof(lineSplit[5]), std::stof(lineSplit[6])), Vect(std::stof(lineSplit[7]), std::stof(lineSplit[8]), std::stof(lineSplit[9])), Vect(std::stof(lineSplit[10]), std::stof(lineSplit[11]), std::stof(lineSplit[12])), Vect(std::stof(lineSplit[13]), std::stof(lineSplit[14]), std::stof(lineSplit[15])), std::stof(lineSplit[16]), std::stof(lineSplit[17]));
        
        lineCount++;
        getline(infile, line);
        lineSplit = split(line, ' ');
        
        while(lineSplit[0] == "sphere" || lineSplit[0] == "triangle")
        {
            if(lineSplit[0] == "sphere")
            {
                Sphere * sphere = new Sphere(Vect(std::stof(lineSplit[1]), std::stof(lineSplit[2]), std::stof(lineSplit[3])), std::stof(lineSplit[4]), material, lights);
                shapes.push_back(sphere);
            }
            
            if(lineSplit[0] == "triangle")
            {
                Triangle * triangle = new Triangle(Vect(std::stof(lineSplit[1]), std::stof(lineSplit[2]), std::stof(lineSplit[3])), Vect(std::stof(lineSplit[4]), std::stof(lineSplit[5]), std::stof(lineSplit[6])), Vect(std::stof(lineSplit[7]), std::stof(lineSplit[8]), std::stof(lineSplit[9])), Vect(std::stof(lineSplit[10]), std::stof(lineSplit[11]), std::stof(lineSplit[12])), material, lights);
                shapes.push_back(triangle);
            }
            
            lineCount++;
            getline(infile, line);
            lineSplit = split(line, ' ');
        }
    }

    infile.close();
}

void Scene::render(void)
{
    Vect W = camera.getW();
    Vect U = camera.getU();
    Vect V = camera.getV();
    Vect E = camera.getEye();
    
    float fl = camera.getFocalLength();
    float r = camera.getRight();
    float l = camera.getLeft();
    float t = camera.getTop();
    float b = camera.getBottom();
    float px = 0.0, py = 0.0;
    
    Vect ori;
    Vect dir;
    Vect *image = new Vect[width * height];
    Vect *pixel = image;
    float aspectRatio = width / (float)height;
    
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++, pixel++)
        {
            px = (float)j / (width - 1) * (r - l) + l;
            py = (float) i / (height - 1) * (t - b) + b;
            px *= aspectRatio;
            ori = E + (W * -fl) + (U * px) + (V * py);
            dir = (ori - E).normalize();
            Ray ray(ori, dir);
            *pixel = draw(ray, 5);
        }
    }
    
    std::ofstream outFile;
    outFile.open("output.ppm", std::ios::binary | std::ios::out);
    outFile << "P6\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < width * height; ++i)
    {
        outFile << (unsigned char)(std::min(float(1), image[i].getX()) * 255) << (unsigned char)(std::min(float(1), image[i].getY()) * 255) << (unsigned char)(std::min(float(1), image[i].getZ()) * 255);
    }
    
    outFile.close();
    delete [] image;
}


*/

/**********************/
/*
int testFunction()
{
    return 0;
    //image width and height
    const int W = 500;
    const int H = 500;
    
    const Vect white(255,255,255);
    const Vect red(0,255,0);
    const Vect black(0,0,0);
    
    Sphere sphere(Vect((W*.5),(H*.5),50),50);
    Sphere light(Vect(0,0,50),1);
    
    std::ofstream out("out.ppm");
    out << "P3\n" << W << '\n' << H << '\n' << "255\n";
    
    double t;
    Vect pix_col(black);
    
    //For each pixel
    for(int y = 0; y < H; y++)
    {
        for(int x = 0; x < W; x++)
        {
            pix_col = black;
            
            //Send a ray through each pixel
            Ray ray(Vect(x,y,0),Vect(0,0,1));
            
            //Check for intersections
            if(sphere.intersect(ray,t))
            {
                //color the pixel
                //pixel_col[y][x] = white;
                
                const Vect pi = ray.o + ray.d*t;
                
                const Vect L = light.c - pi;
                const Vect N = sphere.getNormal(pi);
                const double dt = dotProduct(L.normalize(),N.normalize());
                
                pix_col = (red + white*dt) * .5;
                col255(pix_col);

            }
            
            out << (int)pix_col.x << ' '
            << (int)pix_col.y << ' '
            << (int)pix_col.z << '\n';
        }
    }
    return 0;
}*/











