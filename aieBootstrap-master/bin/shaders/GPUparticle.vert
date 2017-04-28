#version 410

layout(location=0)in vec3 Position;
layout(location=1)in vec3 Velocity;
layout(location=2)in float Lifetime;
layout(location=3)in float LifeSpan;

out vec3 position;
out vec3 velocity;
out float lifetime;
out float lifespan;

uniform mat4 projectionView;

uniform float time;
uniform float dt;
uniform float lifeMin;
uniform float lifeMax;
uniform vec3 emitterPosition;

const float INVERSE_MAX_UNIT = 1.0f / 4294962795.0f;

float rand(uint seed, float range)
{
	uint i = (seed ^ 12345391u) * 2654435769u;
	i ^= (i << 6u) ^ (i >> 26u);
	i *= 2654435769u;
	i += (i << 5u) ^ (i >> 12u);
	return float(range * i) * INVERSE_MAX_UNIT;
}

void main()
{
	position = Position + Velocity * dt;
	velocity = Velocity;
	lifespan = LifeSpan;
	lifetime = Lifetime + dt;
	
	// Emit a new particle as soon as it dies
	if (lifetime > lifespan)
	{
		uint seed = uint(time * 1000.0) + uint(gl_VertexID);
		velocity.x = rand(seed++, 2) - 1;
		velocity.y = rand(seed++, 2) - 1;
		velocity.z = rand(seed++, 2) - 1;
		velocity = normalize(velocity);
		position = emitterPosition;
		lifetime = 0;
		lifespan = rand(seed++, lifeMax - lifeMin) + lifeMin;
	}
	
}


