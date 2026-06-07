#include "Horizon/Scene/Scene.h"
#include "Horizon/ECS/Entity.h"
#include "Horizon/ECS/Components.h"
#include "Horizon/Renderer/RenderCommand.h"
#include "Horizon/Renderer/Shader.h"
#include "Horizon/Renderer/OrthographicCamera.h"
#include "Horizon/Renderer/Renderer2D.h"

#include <glad/glad.h>
#include "Horizon/Renderer/GLM.h"

namespace Horizon {

    static Ref<Shader> s_QuadShader;
    static uint32_t s_QuadVAO = 0;
    static uint32_t s_QuadVBO = 0;

    static void InitQuadRenderer()
    {
        if (s_QuadVAO != 0) return;

        // Create shader
        std::string vertexSrc = R"(
            #version 450 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec2 a_TexCoord;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec2 v_TexCoord;

            void main()
            {
                v_TexCoord = a_TexCoord;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 450 core
            layout(location = 0) out vec4 color;

            in vec2 v_TexCoord;

            uniform vec4 u_Color;
            uniform sampler2D u_Texture;
            uniform float u_TilingFactor;

            void main()
            {
                color = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Color;
            }
        )";

        s_QuadShader = Shader::Create("DefaultQuadShader", vertexSrc, fragmentSrc);

        // Create VAO / VBO
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        glCreateVertexArrays(1, &s_QuadVAO);
        glBindVertexArray(s_QuadVAO);

        glCreateBuffers(1, &s_QuadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, s_QuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        uint32_t ebo;
        glCreateBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    Scene::Scene()
    {
        InitQuadRenderer();
    }

    Ref<Scene> Scene::Clone() const
    {
        // Simple shallow clone: create a new Scene and copy registry data
        Ref<Scene> scene = CreateRef<Scene>();
        // Note: entt registry clone is not trivial; perform basic copy by iterating entities
        auto view = m_Registry.view<TransformComponent, TagComponent>();
        for (auto entity : view)
        {
            auto [transform, tag] = view.get<TransformComponent, TagComponent>(entity);
            Entity e = scene->CreateEntity(tag.Tag);
            e.AddComponent<TransformComponent>(transform);
            if (m_Registry.any_of<SpriteRendererComponent>(entity))
            {
                auto& sprite = m_Registry.get<SpriteRendererComponent>(entity);
                e.AddComponent<SpriteRendererComponent>(sprite);
            }
            if (m_Registry.any_of<CameraComponent>(entity))
            {
                auto& cam = m_Registry.get<CameraComponent>(entity);
                e.AddComponent<CameraComponent>(cam);
            }
        }
        return scene;
    }

    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_Registry.destroy(entity.m_EntityHandle);
    }

    void Scene::OnUpdateRuntime(Timestep ts)
    {
        // Find main camera
        CameraComponent* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
                if (camera.Primary)
                {
                    mainCamera = &camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
        }

        if (mainCamera)
        {
            // Compute View-Projection
            float aspect = (float)m_ViewportWidth / (float)m_ViewportHeight;
            float orthoLeft = -mainCamera->OrthoSize * aspect * 0.5f;
            float orthoRight = mainCamera->OrthoSize * aspect * 0.5f;
            float orthoBottom = -mainCamera->OrthoSize * 0.5f;
            float orthoTop = mainCamera->OrthoSize * 0.5f;

            OrthographicCamera cam(orthoLeft, orthoRight, orthoBottom, orthoTop);
            cam.SetPosition(glm::vec3(0.0f));

            // Render sprites using Renderer2D batch
            Renderer2D::BeginScene(cam);

            auto group = m_Registry.view<TransformComponent, SpriteRendererComponent>();
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                int entityID = (int)entity;
                Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, sprite.Texture, sprite.TilingFactor, entityID);
            }
        Renderer2D::EndScene();
        }
    }

    void Scene::OnUpdateEditor(Timestep ts, const OrthographicCamera& camera)
    {
        Renderer2D::BeginScene(camera);
        auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
        for (auto entity : view)
        {
            auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);
            int entityID = (int)entity;
            Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, sprite.Texture, sprite.TilingFactor, entityID);
        }
        Renderer2D::EndScene();
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
    }

} // namespace Horizon
