#pragma once

#include <Rosewood.h>

class Grid
{
public:
	Grid(Rosewood::Ref<Rosewood::Scene> scene);
	~Grid();


private:
	Rosewood::Ref<Rosewood::Scene> m_Scene;
	Rosewood::EntityID m_GridEntityID;

};