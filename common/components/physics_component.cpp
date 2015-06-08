#include "physics_component.h"

namespace fd {

void PhysicsComponent::OnConnected() {
  static std::string BDATpos("position");
  static std::string BDATorient("orientation");

      
  if(!m_ownerBus->GetOwnerData(BDATorient, true, &m_pOwnerOrientation)
      || !m_ownerBus->GetOwnerData(BDATpos, true, &m_pOwnerPosition)) {
    assert(false);
    SelfDestruct();
  }

  static std::string BDATpushVelocity("pushVelocity"); //already don't remember what BDAT stands for
  if(!m_ownerBus->GetOwnerData(BDATpushVelocity, true, &m_pOwnerPushVelocity)) {
    m_pOwnerPushVelocity = NULL; // this is ok
  }
  static std::string BDATcollideFlag("collidingLastFrame"); // bumblefuck data asinine tag?
  if(!m_ownerBus->GetOwnerData(BDATcollideFlag, true, &m_pOwnerCollidingLastFrame)) {
    m_pOwnerCollidingLastFrame = NULL;
  }

  RegisterSignal(std::string("Step"), this, &PhysicsComponent::OnStepSignal);
  // This is a bad sign... Should we do a typeid system?
  RegisterSignal(std::string("DestroyPhysics"), (Component*)this, &Component::SelfDestruct);
  RegisterSignal(std::string("AddImpulse"), this, &PhysicsComponent::OnImpulse);
}
    
void PhysicsComponent::OnImpulse(const Vec4f& impulse) {
  m_velocity += impulse;
}
    
void PhysicsComponent::OnStepSignal(float delta) {
  m_velocity += m_pPhysics->m_gravity * delta;
  if(m_pOwnerPushVelocity) {
    m_velocity += *m_pOwnerPushVelocity;
    *m_pOwnerPushVelocity = Vec4f(0,0,0,0);
  }

  Vec4f frameVelocity = m_velocity * delta;

  // yeah it's becoming quickly clear that a slow progression of physics bits
  // is the wrong way to proceed. Should just do a proper decoupled system
  // where there are rigidbody + shape things in a physics scene.
  Vec4f possibleVelocity;
  bool hadGroundCollision = false;
  Vec4f hitNormal;
  if(m_pShape->DoesMovementCollide(*m_pOwnerOrientation, *m_pOwnerPosition,
      m_velocity, delta, possibleVelocity, hitNormal)) {
    frameVelocity = possibleVelocity * delta;
    m_velocity = possibleVelocity;
    const float groundCollisionThreshold = 0.1f;
    if(hitNormal.dot(m_pPhysics->m_groundNormal) > groundCollisionThreshold) {
      hadGroundCollision = true;
    }
  }

  // not quite right as we should really just be doing friction based on
  // ground collision
  if(hadGroundCollision) {
    static float frictionCoef = -10.0f;
    m_velocity += (m_velocity * (frictionCoef * delta));
  }
  if(m_pOwnerCollidingLastFrame) {
    *m_pOwnerCollidingLastFrame = hadGroundCollision;
  }

  *m_pOwnerPosition += frameVelocity;

  //float distance = 0.0f; 

  //if(m_pPhysics->RayCast(*m_pOwnerPosition, m_velocity, &distance)) {
  //  if(deltaPos.lengthSq() > (distance*distance)) {
  //    deltaPos = m_velocity.normalized() * (distance - m_pPhysics->m_cushion);
  //  }
  //} 
  //
  //*m_pOwnerPosition += deltaPos;
      
  // right now movement can mess things up since we're doing direct set
  // so clamp to ground
  m_pPhysics->ClampToGround(m_pOwnerPosition, &m_velocity);
}

} // namespace fd