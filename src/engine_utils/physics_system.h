#include "ecs.h"

namespace ect
{
    struct PhysicsBody
    {
        float vx, vy;
    };

    // A physics system that integrates velocity into transform each frame
    void PhysicsSystem(Registry &reg, float dt)
    {
        reg.view2<TransformHot, PhysicsBody>([&](Entity, TransformHot &t, const Velocity &v)
                                          {
        t.x += v.vx * dt;
        t.y += v.vy * dt; });
    }

}