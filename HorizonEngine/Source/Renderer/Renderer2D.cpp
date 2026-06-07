// Batch renderer implementation
#include "Horizon/Renderer/Renderer2D.h"
#include "Horizon/Renderer/Shader.h"
#include "Horizon/Renderer/Texture.h"
#include "Horizon/Renderer/Camera.h"
#include <glad/glad.h>
#include <array>

namespace Horizon {

	struct QuadVertex { glm::vec3 Position; glm::vec4 Color; glm::vec2 TexCoord; float TexIndex; float Tiling; };

struct QuadVertexPicking { glm::vec3 Position; int EntityID; };

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 16; // depends on GPU

		uint32_t VAO = 0, VBO = 0, IBO = 0;
		uint32_t IndexCount = 0;
		QuadVertex* VertexBufferBase = nullptr;
		QuadVertex* VertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		Ref<Shader> QuadShader;
		Ref<Shader> IDShader;
		glm::mat4 ViewProjection;
		// Immediate quad for ID rendering
		uint32_t ImmediateVAO = 0, ImmediateVBO = 0, ImmediateEBO = 0;
	} s_Data;

	static uint32_t* CreateQuadIndices()
	{
		uint32_t* indices = new uint32_t[Renderer2DData::MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < Renderer2DData::MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;
			offset += 4;
		}
		return indices;
	}

	void Renderer2D::Init()
	{
		// Create shader
		const char* vert = R"(
			#version 450 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			layout(location = 2) in vec2 a_TexCoord;
			layout(location = 3) in float a_TexIndex;
			layout(location = 4) in float a_Tiling;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec4 v_Color;
			out vec2 v_TexCoord;
			out float v_TexIndex;
			out float v_Tiling;

			void main()
			{
				v_Color = a_Color;
				v_TexCoord = a_TexCoord;
				v_TexIndex = a_TexIndex;
				v_Tiling = a_Tiling;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		const char* frag = R"(
			#version 450 core
			layout(location = 0) out vec4 color;

			in vec4 v_Color;
			in vec2 v_TexCoord;
			in float v_TexIndex;
			in float v_Tiling;

			uniform sampler2D u_Textures[16];

			void main()
			{
				int idx = int(v_TexIndex);
				vec4 texColor = v_Color;
				if (idx >= 0)
					texColor = texture(u_Textures[idx], v_TexCoord * v_Tiling) * v_Color;
				color = texColor;
			}
		)";

		s_Data.QuadShader = Shader::Create("Renderer2D_Quad", vert, frag);

		// ID shader: outputs integer id to layout(location = 1)
		const char* idVert = R"(
			#version 450 core
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		const char* idFrag = R"(
			#version 450 core
			layout(location = 1) out int outID;
			uniform int u_EntityID;
			void main()
			{
				outID = u_EntityID;
			}
		)";

		s_Data.IDShader = Shader::Create("Renderer2D_ID", idVert, idFrag);

		// Create buffers
		glCreateVertexArrays(1, &s_Data.VAO);
		glBindVertexArray(s_Data.VAO);

		glCreateBuffers(1, &s_Data.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
		glBufferData(GL_ARRAY_BUFFER, Renderer2DData::MaxVertices * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Color));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexCoord));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexIndex));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Tiling));

		uint32_t* indices = CreateQuadIndices();
		glCreateBuffers(1, &s_Data.IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Renderer2DData::MaxIndices * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		delete[] indices;

		s_Data.VertexBufferBase = new QuadVertex[Renderer2DData::MaxVertices];

		// Create immediate quad (unit quad) for ID rendering
		float quadVertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		uint32_t quadIndices[] = { 0,1,2, 2,3,0 };
		glCreateVertexArrays(1, &s_Data.ImmediateVAO);
		glBindVertexArray(s_Data.ImmediateVAO);
		glCreateBuffers(1, &s_Data.ImmediateVBO);
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.ImmediateVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
		glCreateBuffers(1, &s_Data.ImmediateEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.ImmediateEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
		glBindVertexArray(0);
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.VertexBufferBase;
		glDeleteBuffers(1, &s_Data.VBO);
		glDeleteBuffers(1, &s_Data.IBO);
		glDeleteVertexArrays(1, &s_Data.VAO);
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection", s_Data.ViewProjection);

		s_Data.IndexCount = 0;
		s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
		s_Data.TextureSlotIndex = 1;

		// White texture at slot 0 (not implemented here: assume texture 0 is bound to white)
	}

	void Renderer2D::EndScene()
	{
		size_t dataSize = (uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase;
		if (dataSize == 0) return;

		glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, s_Data.VertexBufferBase);

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i])
				s_Data.TextureSlots[i]->Bind(i);
		}

		s_Data.QuadShader->Bind();
		// set texture samplers
		int samplers[Renderer2DData::MaxTextureSlots];
		for (int i = 0; i < (int)Renderer2DData::MaxTextureSlots; ++i) samplers[i] = i;
		s_Data.QuadShader->SetIntArray("u_Textures", samplers, Renderer2DData::MaxTextureSlots);

		glBindVertexArray(s_Data.VAO);
		glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor, int entityID)
	{
		if (s_Data.IndexCount >= Renderer2DData::MaxIndices)
		{
			EndScene();
			s_Data.IndexCount = 0;
			s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
		}

		float texIndex = 0.0f;
		if (texture && texture->IsLoaded())
		{
			// find existing texture slot
			bool found = false;
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
			{
				if (s_Data.TextureSlots[i] && s_Data.TextureSlots[i]->GetPath() == texture->GetPath())
				{
					texIndex = (float)i; found = true; break;
				}
			}
			if (!found)
			{
				if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				{
					EndScene();
					s_Data.IndexCount = 0;
					s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
				}
				texIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}

		static glm::vec4 quadVertexPositions[4] = {
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};

		static glm::vec2 texCoords[4] = { {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}, {0.0f,1.0f} };

		for (int i = 0; i < 4; ++i)
		{
			s_Data.VertexBufferPtr->Position = transform * quadVertexPositions[i];
			s_Data.VertexBufferPtr->Color = color;
			s_Data.VertexBufferPtr->TexCoord = texCoords[i];
			s_Data.VertexBufferPtr->TexIndex = texIndex;
			s_Data.VertexBufferPtr->Tiling = tilingFactor;
			s_Data.VertexBufferPtr++;
		}

		// If an entityID is provided, render a dedicated pass writing the integer ID to the ID attachment.
		if (entityID != -1)
		{
			// Flush the batch first
			EndScene();

			// Bind ID shader and render an immediate quad with u_EntityID
			s_Data.IDShader->Bind();
			s_Data.IDShader->SetMat4("u_ViewProjection", s_Data.ViewProjection);
			s_Data.IDShader->SetMat4("u_Transform", transform);
			s_Data.IDShader->SetInt("u_EntityID", entityID);

			glBindVertexArray(s_Data.ImmediateVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);

			// Re-begin the scene so regular rendering can continue
			s_Data.QuadShader->Bind();
			s_Data.QuadShader->SetMat4("u_ViewProjection", s_Data.ViewProjection);
			s_Data.IndexCount = 0;
			s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
			s_Data.TextureSlotIndex = 1;
		}

		s_Data.IndexCount += 6;
	}

} // namespace Horizon
