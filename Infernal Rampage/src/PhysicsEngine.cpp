#include "PhysicsEngine.h"
#include "matrices.h"
#include "GameObject.h"

#include <algorithm>
#include <map>

OverlapResult PhysicsEngine::CheckCapsuleSphere(vec4 CapLocation, float CapHalfHeight, float CapRadius, vec4 SphLocation, float SphRadius)
{
    glm::vec4 CapsulePoint = ClosestPointOnSegment
    (
        SphLocation,
        CapLocation + vec4(0,-CapHalfHeight,0,0),
        CapLocation + vec4(0, CapHalfHeight,0,0)
    );

    vec4 Dir = CapsulePoint - SphLocation;
    auto Distance = norm(Dir);

    if (Distance <= SphRadius || Distance <= CapRadius)
    {
        return OverlapResult(true);
    }
    else
    {
        return OverlapResult(false);
    }
}

OverlapResult PhysicsEngine::CheckPlaneSphere(vec4 PBase, vec4 PNormal, vec4 SphLocation, float SphRadius)
{
    auto PlanePoint = ProjectToPlane
    (
        SphLocation,
        PBase,
        PNormal
    );

    auto Dir = PlanePoint - SphLocation;
    auto Distance = norm(Dir);

    if (Distance <= SphRadius)
    {
        return OverlapResult(true);
    }
    else
    {
        return OverlapResult(false);
    }
}

OverlapResult PhysicsEngine::CheckCapsulePlane(vec4 CapLocation, float CapHalfHeight, float CapRadius, vec4 PBase, vec4 PNormal)
{
    vec4 CapBottom  = CapLocation + vec4(0,-CapHalfHeight,0,0);
    vec4 CapTop     = CapLocation + vec4(0, CapHalfHeight,0,0);

    vec4 BottomProj = ProjectToPlane(CapBottom, PBase, PNormal);
    vec4 TopProj    = ProjectToPlane(CapTop, PBase, PNormal);

    float BottomD   = norm(CapBottom - BottomProj);
    float TopD      = norm(CapTop - TopProj);

    if(BottomD < TopD)
        return OverlapResult(BottomD < CapRadius);
    else
        return OverlapResult(TopD < CapRadius);
}

glm::vec4 PhysicsEngine::ClosestPointOnSegment(glm::vec4 P, glm::vec4 Start, glm::vec4 End)
{
	const glm::vec4 Segment = End - Start;
	const glm::vec4 VectToPoint = P - Start;

	// See if closest point is before StartPoint
	const float Dot1 = dotproduct(VectToPoint, Segment);
	if( Dot1 <= 0 )
	{
		return Start;
	}

	// See if closest point is beyond EndPoint
	const float Dot2 = dotproduct(Segment, Segment);
	if( Dot2 <= Dot1 )
	{
		return End;
	}

	// Closest Point is within segment
	return Start + Segment * (Dot1 / Dot2);
}

glm::vec4 PhysicsEngine::ProjectToPlane(vec4 P, vec4 PlaneBase, vec4 PlaneNormal)
{
	return P - dotproduct((P - PlaneBase), PlaneNormal) * PlaneNormal;
}
