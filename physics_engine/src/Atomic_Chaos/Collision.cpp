#include <cmath>
#include <random>
#include "Collision.h"
#include "Particle.h"

//--------------- Helper functions ---------------
float Collision::distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	sf::Vector2f d = p2 - p1;  // or p1 - p2, doesn't matter for distance
	return std::sqrt(d.x * d.x + d.y * d.y);
}

float Collision::dotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

//--------------- detecting collisions ---------------
bool Collision::checkParticleCollision(const Particle& p1, const Particle& p2)
{
	float dist = distance(p1.Position, p2.Position);
	float radiusSum = p1.shape.getRadius() + p2.shape.getRadius();
	return(dist <= radiusSum);
}

bool Collision::checkWallCollision(const sf::Vector2f& position, float radius, const sf::Vector2f& maxSize, const sf::Vector2f& minSize)
{
	return (position.x - radius <= minSize.x || position.x + radius >= maxSize.x || position.y - radius <= minSize.y || position.y + radius >= maxSize.y);
}

//--------------- resolving Particle collisions ---------------
void Collision::resolveParticleCollision(Particle& p1, Particle& p2)
{
    float r1 = p1.shape.getRadius();
    float r2 = p2.shape.getRadius();
    float dist = distance(p1.Position, p2.Position);
    float radiusSum = r1 + r2;

    // Exit if not overlapping
    if (dist >= radiusSum) return;

    // Near-zero distance case
    sf::Vector2f d = p1.Position - p2.Position;
    if (dist < 1e-6f) {
        d = sf::Vector2f(1.0f, 0.0f);
        dist = radiusSum;
    }

    // Normalized direction vectors
    sf::Vector2f normal = d / dist;
    sf::Vector2f tangent(-normal.y, normal.x);

    // Relative velocity
    sf::Vector2f v = p1.Velocity - p2.Velocity;

    // Normal velocity (approach speed)
    float v_n = dotProduct(v, normal);

    // Only resolve if moving toward each other
    if (v_n >= 0.0f) return;

    // --- PHASE 1: NORMAL IMPULSE (Bouncing) ---

    float m1 = p1.getMass();
    float m2 = p2.getMass();
    float restitution = 1.0f;  // Perfectly elastic

    // Normal impulse 
    float J_n = -(1.0f + restitution) * v_n / (1.0f / m1 + 1.0f / m2);

    // Apply normal impulse
    p1.Velocity += (J_n / m1) * normal;
    p2.Velocity -= (J_n / m2) * normal;

    // --- PHASE 2: TANGENTIAL IMPULSE (Friction & Spin) ---

    // Tangential velocity (sliding speed at contact)
    float v_t = dotProduct(v, tangent)
        + p1.getAngleV() * r1
        - p2.getAngleV() * r2;

    // Moment of inertia (solid circles)
    float I1 = 0.5f * m1 * r1 * r1;
    float I2 = 0.5f * m2 * r2 * r2;

    // Effective mass for tangential motion
    float invMt = (1.0f / m1) + (1.0f / m2) + (r1 * r1) / I1 + (r2 * r2) / I2;
    float mEff_t = 1.0f / invMt;

    // Tangential impulse (to eliminate sliding)
    float J_t = -v_t * mEff_t;

     // Clamp by friction coefficient
     float mu = 0.5f;  // Friction coefficient
     J_t = std::clamp(J_t, -mu * std::abs(J_n), mu * std::abs(J_n));

    // Apply tangential impulse to linear velocities
    p1.Velocity += (J_t / m1) * tangent;
    p2.Velocity -= (J_t / m2) * tangent;

    // Apply tangential impulse to angular velocities
    p1.getAngleV() += (r1 * J_t) / I1;
    p2.getAngleV() -= (r2 * J_t) / I2;

    // --- POSITIONAL CORRECTION (Prevent sinking) ---

    const float epsilon = 0.001f;
    float overlap = radiusSum - dist;
    sf::Vector2f correction = normal * (overlap / 2.0f + epsilon);

    // Update positions
    p1.Position += correction;
    p2.Position -= correction;

    // Sync visual shapes
    p1.shape.setPosition(p1.Position);
    p2.shape.setPosition(p2.Position);
}

//--------------- resolving Wall collisions ---------------
void Collision::resolveWallCollision(Particle& particle,float dt, const sf::Vector2f& maxSize, const sf::Vector2f& minSize)
{
    float radius = particle.shape.getRadius();
    const float restitution = 1.0f; // perfectly elastic collision (no energy loss)

    float tc = Collision::computeTOI(particle.Position, particle.Velocity, particle.shape.getRadius(), dt, maxSize, minSize);
    if (tc >= 0.0f && tc <= 1.0f)
    {
        // ---- Collision occurs within this frame ----

        // 1. Move up to the collision point
        particle.Position += particle.Velocity * (tc * dt);

        // 2. Reflect velocity depending on which boundary was hit
        if (particle.Position.x - radius <= minSize.x || particle.Position.x + radius >= maxSize.x)
            particle.Velocity.x = -particle.Velocity.x * restitution;

        if (particle.Position.y - radius <= minSize.y || particle.Position.y + radius >= maxSize.y)
            particle.Velocity.y = -particle.Velocity.y * restitution;

        // 3. Move remaining time after the bounce
        float remaining = 1.0f - tc;
        particle.Position += particle.Velocity * (remaining * dt);
    }
    else
    {
        // ---- No collision this frame ----
        particle.Position += particle.Velocity * dt;
    }

    // ----------------------- CORNER FIX -----------------------
    // Clamp particle inside the box after all movement
    particle.Position.x = std::clamp(particle.Position.x, minSize.x + radius, maxSize.x - radius);
    particle.Position.y = std::clamp(particle.Position.y, minSize.y + radius, maxSize.y - radius);

    // Sync visual shape
    particle.shape.setPosition(particle.Position);
}

// ---------- Calculating Compute Time Of Impact ----------
float Collision::computeTOI(const sf::Vector2f& position, const sf::Vector2f& velocity,
	float radius, float dt, const sf::Vector2f& maxSize,
	const sf::Vector2f& minSize)
{
	float tc = -1.0f; // no collision by default

	auto considerCandidate = [&](float tCandidate)
	{
		// Valid candidate must be within [0, 1]
		if (tCandidate >= 0.0f && tCandidate <= 1.0f)
		{
			if (tc < 0.0f || tCandidate < tc)
			tc = tCandidate;
		}
	};

	// --- X boundaries ---
	if (velocity.x < 0.0f) // moving left
	{
		float tLeft = (minSize.x + radius - position.x) / (velocity.x * dt);
		considerCandidate(tLeft);
	}
	else if (velocity.x > 0.0f) // moving right
	{
		float tRight = (maxSize.x - radius - position.x) / (velocity.x * dt);
		considerCandidate(tRight);
	}

	// --- Y boundaries ---
	if (velocity.y < 0.0f) // moving up
	{
		float tTop = (minSize.y + radius - position.y) / (velocity.y * dt);
		considerCandidate(tTop);
	}
	else if (velocity.y > 0.0f) // moving down
	{
		float tBottom = (maxSize.y - radius - position.y) / (velocity.y * dt);
		considerCandidate(tBottom);
	}
	return tc; // -1 if no collision this frame, otherwise 0 <= tc <= 1
}