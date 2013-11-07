#include "SpriteComponent.h"
#include "../../Logger.h"
#include "../../Assets/TextureManager.h"
#include "../../Graphics/GraphicsManager.h"
#include "../TransformComponent.h"
#include "../../Objects/FreeCamera.h"
#include "../CameraComponent.h"
#include "../../Objects/Object.h"
#include "../../Graphics/SpriteBatch.h"

namespace star
{
	SpriteComponent::SpriteComponent(const tstring& filepath,const tstring& spriteName, bool bIsHUDElement, int widthSegments, int heightSegments)
		: BaseComponent()
		, m_Width(0)
		, m_WidthSegments(widthSegments)
		, m_CurrentWidthSegment(0)
		, m_Heigth(0)
		, m_HeightSegments(heightSegments)
		, m_CurrentHeightSegment(0)
		, m_Shader()
		, m_FilePath(filepath)
		, m_SpriteName(spriteName)
		, m_bIsHudElement(bIsHUDElement)
	{
	}

	void SpriteComponent::InitializeComponent()
	{
		#ifdef DESKTOP
				Filepath texshaderVertex(_T("WinShaders/"), _T("Texture_Shader.vert"));
				Filepath texshaderFrag(_T("WinShaders/"), _T("Texture_Shader.frag"));
		#else
				Filepath texshaderVertex(_T("AndroidShaders/"), _T("BaseTexShader.vert"));
				Filepath texshaderFrag(_T("AndroidShaders/"), _T("BaseTexShader.frag"));
		#endif

				if(!m_Shader.Init(texshaderVertex.GetFullPath(),texshaderFrag.GetFullPath()))
				{
					Logger::GetInstance()->Log(star::LogLevel::Info, _T("Making Shader Failed"));
				}
		
		TextureManager::GetInstance()->LoadTexture(m_FilePath.GetFullPath(),m_SpriteName);
		m_Width = TextureManager::GetInstance()->GetTextureDimensions(m_SpriteName).x / m_WidthSegments;
		m_Heigth =  TextureManager::GetInstance()->GetTextureDimensions(m_SpriteName).y / m_HeightSegments;

		CreateVertices();
		CreateIndices();
	}

	SpriteComponent::~SpriteComponent()
	{
		// [COMMENT] can you explain me why you wouldn't want
		// to destroy the texture once this spritecomponent
		// is deleted. (you can do that via skype)
		// [COMMENT] @ Simon: We should discuss if RemoveTexture should be called here. 
		// Really depends of the structure of the TextureManager

		//TextureManager::GetInstance()->DeleteTexture(m_SpriteName);
	}

	void SpriteComponent::CreateVertices()
	{
		m_Vertices[0] = (GLfloat)m_Width;
        m_Vertices[1] = (GLfloat)m_Heigth;
        m_Vertices[2] = 0;
        m_Vertices[3] = (GLfloat)m_Width;
        m_Vertices[4] = 0;
        m_Vertices[5] = 0;
        m_Vertices[6] = 0;
        m_Vertices[7] = (GLfloat)m_Heigth;
        m_Vertices[8] = 0;
        m_Vertices[9] = 0;
        m_Vertices[10] = 0;
        m_Vertices[11] = 0;
	}

	void SpriteComponent::CreateIndices()
	{
		float startX = static_cast<float>(m_CurrentWidthSegment) / static_cast<float>(m_WidthSegments);
		float endX = 1.0f / m_WidthSegments;
		float startY = static_cast<float>(m_CurrentHeightSegment) / static_cast<float>(m_HeightSegments);
		float endY = 1.0f / m_HeightSegments;

		m_UvCoords[0] = startX + endX;
		m_UvCoords[1] = startY + endY;
		m_UvCoords[2] = startX + endX;
		m_UvCoords[3] = startY;
		m_UvCoords[4] = startX;
		m_UvCoords[5] = startY + endY;
		m_UvCoords[6] = startX;
		m_UvCoords[7] = startY;
	}

	void SpriteComponent::Draw()
	{
		SpriteBatch::GetInstance()->AddSpriteToQueue(this, m_bIsHudElement);
	}

	const tstring& SpriteComponent::GetFilePath() const
	{
		return m_FilePath.GetPath();
	}

	const tstring& SpriteComponent::GetName() const
	{
		return m_SpriteName;
	}

	int32 SpriteComponent::GetWidth() const
	{
		return (m_Width/m_WidthSegments);
	}

	int32 SpriteComponent::GetHeight() const
	{
		return (m_Heigth/m_HeightSegments);
	}

	std::vector<GLfloat> SpriteComponent::GetVertices() const
	{
		std::vector<GLfloat> vertices;
		vertices.clear();
		
		for(int i = 0; i < 12; ++i)
		{
			vertices.push_back(m_Vertices[i]);
		}
		return vertices;
	}

	std::vector<GLfloat> SpriteComponent::GetUVCoords() const
	{
		std::vector<GLfloat> uvCoords;
		uvCoords.clear();
		
		for(int i = 0; i < 8; ++i)
		{
			uvCoords.push_back(m_UvCoords[i]);
		}
		return uvCoords;
	}

	void SpriteComponent::SetCurrentSegment(int widthSegment, int heightSegment)
	{
		m_CurrentWidthSegment = widthSegment;
		m_CurrentHeightSegment = m_HeightSegments - heightSegment - 1;

		CreateIndices();
	}

	void SpriteComponent::SetTexture( const tstring& filepath, const tstring& spriteName, bool bIsHUDElement /*= false*/, int widthSegments /*= 1*/, int heightSegments /*= 1*/ )
	{
		m_Width = 0;
		m_WidthSegments = widthSegments;
		m_CurrentWidthSegment = 0;
		m_Heigth = 0;
		m_HeightSegments = heightSegments;
		m_CurrentHeightSegment = 0;
		m_FilePath = filepath;
		m_SpriteName = spriteName;
		m_bIsHudElement = bIsHUDElement;

		TextureManager::GetInstance()->LoadTexture(m_FilePath.GetFullPath(),m_SpriteName);
		m_Width = TextureManager::GetInstance()->GetTextureDimensions(m_SpriteName).x / m_WidthSegments;
		m_Heigth =  TextureManager::GetInstance()->GetTextureDimensions(m_SpriteName).y / m_HeightSegments;

		CreateVertices();
		CreateIndices();
	}

}
