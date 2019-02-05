#include "advent.h"

struct Vec3
{
	int x{ 0 };
	int y{ 0 };
	int z{ 0 };

	int len() const
	{
		return std::abs(x) + std::abs(y) + std::abs(z);
	}
};

namespace std
{
	template <>
	struct hash<Vec3>
	{
		size_t operator()(const Vec3& vec) const noexcept
		{
			return _Hash_bytes(static_cast<const unsigned char*>(static_cast<const void*>(&vec)), sizeof(vec));
		}
	};
}

Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
{
	return Vec3{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

bool operator==(const Vec3& lhs, const Vec3& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

struct Particle
{
	Particle(int id)
		: id(id) { }
	int id{ 0 };
	Vec3 position;
	Vec3 velocity;
	Vec3 acceleration;
};

void parseParticle(const std::string& line, Particle& particle)
{
	std::istringstream ss(line);
	std::string token;
	std::array<int, 9> numbers;
	int i{ 0 };
	while (std::getline(ss, token, ','))
	{
		if (token.find('<') != token.npos)
		{
			token = token.substr(token.find('<') + 1);
		}
		numbers[i++] = std::stoi(token);
	}
	{
		particle.position.x = numbers[0];
		particle.position.y = numbers[1];
		particle.position.z = numbers[2];
		particle.velocity.x = numbers[3];
		particle.velocity.y = numbers[4];
		particle.velocity.z = numbers[5];
		particle.acceleration.x = numbers[6];
		particle.acceleration.y = numbers[7];
		particle.acceleration.z = numbers[8];
	}
}

Vec3 simulate(Particle& particle)
{
	particle.velocity = particle.velocity + particle.acceleration;
	particle.position = particle.position + particle.velocity;
	return particle.position;
}


void BugFix<20>::solve1st()
{
	std::list<Particle> particles;

	std::string line;
	int id = 0;
	while (std::getline(*mIn, line))
	{
		particles.push_back(Particle{ id });
		parseParticle(line, particles.back());
		++id;
	}

	int minAccel = particles.front().acceleration.len();
	std::for_each(particles.cbegin(), particles.cend(), [&minAccel](const Particle& particle) {
		if (particle.acceleration.len() < minAccel)
			minAccel = particle.acceleration.len();
	});
	particles.remove_if([&minAccel](const Particle& particle) { return particle.acceleration.len() > minAccel; });

	std::for_each(particles.begin(), particles.end(), [](Particle& particle) {
		if (particle.acceleration.x != 0)
			particle.velocity.x = 0;
		if (particle.acceleration.y != 0)
			particle.velocity.y = 0;
		if (particle.acceleration.z != 0)
			particle.velocity.z = 0;
	});

	int minVel = particles.front().velocity.len();
	std::for_each(particles.cbegin(), particles.cend(), [&minVel](const Particle& particle) {
		if (particle.velocity.len() < minVel)
			minVel = particle.velocity.len();
	});
	particles.remove_if([&minVel](const Particle& particle) { return particle.velocity.len() > minVel; });

	if (particles.size() == 1)
		*mOut << particles.front().id << std::endl;
	else
		*mOut << "Something went wrong. Apply more thorough logic" << std::endl;
}

void BugFix<20>::solve2nd()
{
	std::list<Particle> particles;
	std::map<int, Particle*> particlesMap;

	std::string line;
	int id = 0;
	while (std::getline(*mIn, line))
	{
		particles.push_back(Particle{ id });
		particlesMap[id] = &particles.back();
		parseParticle(line, particles.back());
		++id;
	}

	constexpr int NumUnchangedSequence{ 1000 };
	int numUnchanged{ 0 };
	while (numUnchanged < NumUnchangedSequence && particlesMap.size() > 1)
	{
		std::unordered_map<Vec3, std::set<Particle*>> ordParticles;
		for (auto& particle : particlesMap)
		{
			const Vec3 newPos = simulate(*particle.second);
			ordParticles[newPos].insert(particle.second);
		}
		bool erased = false;
		for (const auto& pp : ordParticles)
		{
			if (pp.second.size() > 1)
			{
				for (const auto& remP : pp.second)
				{
					particlesMap.erase(remP->id);
					erased = true;
				}
			}
		}
		if (erased)
			numUnchanged = 0;
		else
			++numUnchanged;
	}

	*mOut << particlesMap.size() << std::endl;
}
