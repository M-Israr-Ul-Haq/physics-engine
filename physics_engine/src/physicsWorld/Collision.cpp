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

//--------------- resolving collisions ---------------
void Collision::resolveParticleCollision(Particle& p1, Particle& p2)
{
    float dist = distance(p1.Position, p2.Position);
    float radiusSum = p1.shape.getRadius() + p2.shape.getRadius();

    // Early exit if not overlapping
    if (dist >= radiusSum) return;

    // Handle near-zero distance case
    sf::Vector2f d = p1.Position - p2.Position;
    if (dist < 1e-6f) {
        d = sf::Vector2f(1.0f, 0.0f);
        dist = radiusSum;
    }

    // Normalized direction vector
    sf::Vector2f normal = d / dist;

    // Relative velocity
    sf::Vector2f v = p1.Velocity - p2.Velocity;
    float vDotN = dotProduct(v, normal);

    // Only resolve if moving toward each other
    if (vDotN >= 0.0f) return;

    // Elastic collision response
    float mSum = p1.getMass() + p2.getMass();
    float impulse = (2.0f * vDotN) / mSum;

    p1.Velocity -= impulse * p2.getMass() * normal;
    p2.Velocity += impulse * p1.getMass() * normal;

    // Separate overlapping particles with epsilon
    const float epsilon = 0.001f;
    float overlap = radiusSum - dist;
    sf::Vector2f correction = normal * (overlap / 2.0f + epsilon);
    
    p1.shape.move(correction);
    p2.shape.move(-correction);
    p1.shape.setPosition(p1.Position);
    p2.shape.setPosition(p2.Position);
}

void Collision::resolveWallCollision(Particle& particle,float dt, const sf::Vector2f& maxSize, const sf::Vector2f& minSize)
{
    float radius = particle.shape.getRadius();
    const float restitution = 1.0f; // perfectly elastic collision (no energy loss)

    float tc = Collision::computeTOI(particle.Position, particle.Velocity, particle.shape.getRadius(), dt, maxSize, minSize);
    if (tc >= 0.0f && tc <= 1.0f)
    {
        // ---- Collision occurs within this frame ----

        // 2. Move up to the collision point
        particle.Position += particle.Velocity * (tc * dt);

        // 3. Reflect velocity depending on which boundary was hit
        if (particle.Position.x - radius <= minSize.x || particle.Position.x + radius >= maxSize.x)
            particle.Velocity.x = -particle.Velocity.x * restitution;

        if (particle.Position.y - radius <= minSize.y || particle.Position.y + radius >= maxSize.y)
            particle.Velocity.y = -particle.Velocity.y * restitution;

        // 4. Move remaining time after the bounce
        float remaining = 1.0f - tc;
        particle.Position += particle.Velocity * (remaining * dt);
    }
    else
    {
        // ---- No collision this frame ----
        particle.Position += particle.Velocity * dt;
    }
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
	return tc; // -1 if no collision this frame, otherwise 0 ≤ tc ≤ 1
}

//--------------- SPIN the particles ---------------
void Collision::spinningParticles(Particle& particle)
{
    sf::Angle angle = sf::degrees(1.0f);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-0.5f, 0.5f);

    particle.setAngleA(dist(gen));       // optional, or use to accelerate spin

    // accumulate angle and velocity
    angle += sf::degrees(particle.getAngleV());
    particle.getAngleV() += particle.getAngleA();

    particle.shape.rotate(particle.getAngleV() * angle );
    particle.axis.rotate(particle.getAngleV() * angle );
}