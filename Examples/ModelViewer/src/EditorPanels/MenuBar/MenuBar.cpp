#include <imgui.h>
#include "MenuBar.h"

MenuBar::MenuBar(std::shared_ptr<SceneHierachy> sceneHierarchy)
	: m_Scene(sceneHierarchy->GetScene()), m_SceneHierarchy(sceneHierarchy)
{
	Rosewood::TextureSpecification texSpec;
		texSpec.MinFiltering = Rosewood::TextureFiltering::Linear;
		texSpec.MagFiltering = Rosewood::TextureFiltering::Linear;
	m_CheckerBoardTexture = Rosewood::Texture2D::Create("./assets/textures/checkerboard.jpg", texSpec);
}

void MenuBar::OnImGuiRender()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Import model..."))
			{
				std::string filePath;
				std::string fileName;
				std::string fileExtension;
				bool result = Rosewood::Utils::OpenFileExplorer(filePath, fileName, fileExtension);

				if (result)
				{
					if (fileExtension == "obj")
					{
						Rosewood::ModelImport::ImportedModel model = Rosewood::ModelImport::Import(filePath, Rosewood::ModelImport::FileFormat::OBJ);

						for (Rosewood::Mesh& mesh : model.MeshObjects)
						{
							Rosewood::EntityID entityID = m_Scene->GetRegistry()->CreateEntity();
							m_SceneHierarchy->SetSelectedEntity(entityID);
							m_SceneHierarchy->AddEntityToHierachy(entityID);

							m_Scene->GetRegistry()->AddComponent<Rosewood::TagComponent>(entityID, mesh.m_MeshData.ObjectName);

							glm::mat4 transform = glm::mat4(1.0f);
							m_Scene->GetRegistry()->AddComponent<Rosewood::TransformComponent>(entityID, transform);

							std::vector<Rosewood::Ref<Rosewood::Material>> materials(mesh.m_MeshData.Surfaces.size());

							Rosewood::MaterialSpecification materialSpec;
							materialSpec.ShaderName = "simple/flat";

							for (int i = 0; i < mesh.m_MeshData.Surfaces.size(); i++)
							{
								Rosewood::Ref<Rosewood::Material> material = std::make_shared<Rosewood::Material>(materialSpec);

								glm::vec3 ambient = glm::vec3(1.f);
								glm::vec3 diffuse = glm::vec3(.8f);
								glm::vec3 specular = glm::vec3(.5f);
								float shininess = 16.;

								material->SetNamedParameter("Ambient", ambient);
								material->SetNamedParameter("Diffuse", diffuse);
								material->SetNamedParameter("Specular", specular);
								material->SetNamedParameter("Shininess", shininess);

								materials[i] = material;

								material->SetTextureBinding(0, m_CheckerBoardTexture);
							}

							m_Scene->GetRegistry()->AddComponent<Rosewood::RenderableObject3D>(entityID, mesh.MakeVertexArrays(), std::move(materials), mesh.m_MeshData.SlotNames);

						}
					}
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}
