#include "Grid.h"

Grid::Grid(Rosewood::Ref<Rosewood::Scene> scene)
 : m_Scene(scene)
{
	Rosewood::ShaderLibrary::Load(Rosewood::ShaderSpecification{ "grid",			"../../shaders/Grid/grid.vert", "../../shaders/Grid/grid.frag" });

	std::array<glm::vec3, 4>	vertices = { glm::vec3(1., 0., 1.), glm::vec3(-1., 0., 1.), glm::vec3(-1., 0., -1.), glm::vec3(1., 0., -1.) };
	std::array<uint32_t, 6>		indices = { 0, 3, 1, 3, 2, 1 };
	Rosewood::BufferLayout		layout = { { Rosewood::ShaderDataType::Float3,				"a_Position"		} };

	Rosewood::Ref<Rosewood::VertexBuffer>	vb = Rosewood::VertexBuffer::Create((BYTE*)vertices.data(), layout.GetStride() * Rosewood::Utils::SizeToUint32(vertices.size()), layout);
	Rosewood::Ref<Rosewood::IndexBuffer>	ib = Rosewood::IndexBuffer::Create(indices.data(), Rosewood::Utils::SizeToUint32(indices.size()));
	Rosewood::Ref<Rosewood::VertexArray>	va = Rosewood::VertexArray::Create();
	va->AddVertexBuffer(vb);
	va->SetIndexBuffer(ib);

	m_GridEntityID = m_Scene->GetRegistry()->CreateEntity();
	m_Scene->GetRegistry()->AddComponent<Rosewood::TransformComponent>(m_GridEntityID, Rosewood::TransformData(glm::vec3(0.f), glm::quat(), glm::vec3(1000.f)));

	Rosewood::MaterialSpecification materialSpec;
	materialSpec.ShaderName = "grid";
	materialSpec.CullMode = Rosewood::CullMode::CullDisabled;
	materialSpec.Opacity = Rosewood::Opacity::Transparent;
	Rosewood::Ref<Rosewood::Material> material = std::make_shared<Rosewood::Material>(materialSpec);
	std::vector<Rosewood::Ref<Rosewood::VertexArray>> vas = { va };
	std::vector<std::string> names = { "material" };
	std::vector<Rosewood::Ref<Rosewood::Material>> materials = { material };

	m_Scene->GetRegistry()->AddComponent<Rosewood::RenderableObject3D>(m_GridEntityID, vas, materials, names, false);
}

Grid::~Grid()
{
	m_Scene->GetRegistry()->DestroyEntity(m_GridEntityID);
}
