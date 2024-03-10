#pragma once
#include <raylib.h>
#include <vector>
extern Model ghostModel;
namespace MassShoot {
	namespace Particles {
		class Particle {
		public:
			Vector3 position;
			Vector3 velocity;
			int life;
			int size;
		};
		class ParticleSystem {
		public:
			std::vector<Particle> particles;
			Color particleColor;
			int maxlife;
			void CalculateNextPos();
			void SpawnParticles(int number, int size, Vector3 position);
			void RenderParticles();
		};
	}
}