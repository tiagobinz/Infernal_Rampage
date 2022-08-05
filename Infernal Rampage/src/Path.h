#pragma once

#include "Transform.h"

using namespace std;
using namespace glm;

struct Path
{
    vector<vec4> Points;

    Path(vector<vec4> _Points)
    {
        Points = _Points;
    }

    vec4 GetPositionAtTime(float t)
    {
        vector<vec4> tmp = Points;
        int i = Points.size() - 1;
        while (i > 0) {
            for (int k = 0; k < i; k++)
                tmp[k] = tmp[k] + t * ( tmp[k+1] - tmp[k] );
            i--;
        }
        vec4 answer = tmp[0];
        return answer;
    }
};
