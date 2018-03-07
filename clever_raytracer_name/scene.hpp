using namespace std;
using namespace raytrace;

#include "shape.hpp"

class Scene
{
private:
    std::vector<Shape*> shapes;
    Camera camera;
    int height, width;
    std::vector<std::string> split(const std::string &str, const char &delimit);
public:
    Scene(const std::string &fileName, const int &h, const int &w);
    void render(void);
    Vect draw(const Ray &ray, const int &level);
};


