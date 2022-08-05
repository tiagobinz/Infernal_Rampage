#include "Counter.h"

Counter::Counter(Transform _ObjectTransform) : GameObject ("counter", _ObjectTransform)
{
}

glm::vec2 Counter::GetUVOffset()
{
    return glm::vec2
    (
        (Number%4) *  0.25,
        (Number/4) * -0.25
    );
}
