#include "trace.h"

std::pair<bool,float> Intersect( Shape** shapes, int num_shapes, Ray const& ray)
{
    int i;
    std::pair<bool,float> intersection;
    std::pair<bool,float> min_intersection;
    min_intersection = std::make_pair(false,100000000000);
    for(i = 0; i < num_shapes; ++i)
    {
        intersection = shapes[i] ->Intersect(ray);
        if(intersection.first && intersection.second < min_intersection.second)
        {
            min_intersection = intersection;
        }
    }
    return min_intersection;
}
