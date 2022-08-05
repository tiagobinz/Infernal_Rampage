#pragma once

#include <vector>
#include "Transform.h"

using namespace glm;

struct OverlapResult
{
    bool DidOverlap;

    OverlapResult(bool _did)
    {
        DidOverlap = _did;
    }
};

class PhysicsEngine
{
public:
    // Check overlap a capsule and a plane
    static OverlapResult CheckCapsulePlane(vec4 CapLocation, float CapHalfHeight, float CapRadius, vec4 PBase, vec4 PNormal);
    // Check overlap between a capsule and a sphere
    static OverlapResult CheckCapsuleSphere(vec4 CapLocation, float CapHalfHeight, float CapRadius, vec4 SphLocation, float SphRadius);
    // Check overlap between a plane and a sphere
    static OverlapResult CheckPlaneSphere(vec4 PBase, vec4 PNormal, vec4 SphLocation, float SphRadius);

    // Helper functions:
    static vec4 ClosestPointOnSegment(vec4 P, vec4 Start, vec4 End);
    static vec4 ProjectToPlane(vec4 P, vec4 PlaneBase, vec4 PlaneNormal);
};
