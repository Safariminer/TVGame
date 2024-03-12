#include <raylib.h>
#include <iostream>
#include "Particles.h"
#include <raymath.h>

void MassShoot::Particles::ParticleSystem::CalculateNextPos(){
	for (int i = 0; i < particles.size(); i++) {
		particles.at(i).position = Vector3Add(particles.at(i).position, particles.at(i).velocity);
		particles.at(i).life++;
		if (particles.at(i).life > maxlife) {
			particles.erase(particles.begin() + i);
			i--;
		}
	}
}

void MassShoot::Particles::ParticleSystem::SpawnParticles(int number, int size, Vector3 position)
{
	for (int i = 0; i < number; i++) {
		SetRandomSeed(GetTime() + i);
		MassShoot::Particles::Particle newParticle;
		newParticle.velocity.x = (float)GetRandomValue(-100, 100) / 10;
		newParticle.velocity.y = (float)GetRandomValue(-100, 100) / 10;
		newParticle.velocity.z = (float)GetRandomValue(-100, 100) / 10;
		newParticle.position = position;
		newParticle.size = size;
		newParticle.life = 0;
		particles.push_back(newParticle);
		if (particles.size() > 5000) {
			particles.erase(particles.begin());
			printf("PARTICLE SYSTEM WARNING: OVERFLOW\n");
		}
		printf("New Particle: %f %f %f %f %f %f %i %i\n", newParticle.position.x, newParticle.position.y, newParticle.position.z, newParticle.velocity.x, newParticle.velocity.y, newParticle.velocity.z, newParticle.size, newParticle.life);
	}
}

void MassShoot::Particles::ParticleSystem::RenderParticles()
{
	for (int i = 0; i < particles.size(); i++) {
		DrawModel(ghostModel, particles.at(i).position, 0.4f, WHITE);
	}
}
