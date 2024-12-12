#pragma once

#include <glm/vec3.hpp>

namespace Rosewood 
{
	//Using vec4 to respect alignement and minimum offset of std140 layout of uniform buffers 

	struct PointLight
	{
		glm::vec4 Position;

		glm::vec4 Ambient;
		glm::vec4 Diffuse;
		glm::vec4 Specular;

		glm::vec4 Attenuation; // vec4( constant, linear, quadratic, ... )

		PointLight() = default;
		PointLight(glm::vec4 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec4 attenuation)
			: Position(position), Ambient(ambient), Diffuse(diffuse), Specular(specular), Attenuation(attenuation) {}
	};

	struct SpotLight {
		glm::vec4 Position;
		glm::vec4 Direction;
		glm::vec4 CutOff; // vec4( innerCutoff, outerCutoff, ..., ... )

		glm::vec4 Ambient;
		glm::vec4 Diffuse;
		glm::vec4 Specular;

		glm::vec4 Attenuation; // vec4( constant, linear, quadratic, ... )

		SpotLight() = default;
		SpotLight(glm::vec4 position, glm::vec4 direction, glm::vec4 cutOff, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec4 attenuation)
			: Position(position), Direction(direction), CutOff(cutOff), Ambient(ambient), Diffuse(diffuse), Specular(specular), Attenuation(attenuation) {}
	};

	struct DirectionalLight {
		glm::vec4 Direction;

		glm::vec4 Ambient;
		glm::vec4 Diffuse;
		glm::vec4 Specular;

		DirectionalLight() = default;
		DirectionalLight(glm::vec4 direction, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
			: Direction(direction), Ambient(ambient), Diffuse(diffuse), Specular(specular) {}
	};


}