#include "Spritesheet.h"
#include "../Input/XMLContainer.h"
#include "../Helpers/Helpers.h"

namespace star
{
	Spritesheet::Spritesheet()
		: Dictionary<tstring, SpriteAnimation>()
		, m_Name(EMPTY_STRING)
		, m_DefaultAnimation(EMPTY_STRING)
		, m_FramesHorizontal(0)
		, m_FramesVertical(0)
	{

	}

	Spritesheet::Spritesheet(XMLContainer & spritesheet)
		: Dictionary<tstring, SpriteAnimation>()
		, m_Name(EMPTY_STRING)
		, m_DefaultAnimation(EMPTY_STRING)
	{
		auto attributes = spritesheet.GetAttributes();
		m_Name = attributes[_T("name")];
		m_DefaultAnimation = attributes[_T("default")];
		m_FramesHorizontal = string_cast<int>(attributes[_T("width")]);
		m_FramesVertical = string_cast<int>(attributes[_T("height")]);
		int amount = m_FramesHorizontal * m_FramesVertical;
		float speed = string_cast<float>(attributes[_T("speed")]);

		auto it = spritesheet.lower_bound(_T("animation"));
		auto end_animation = spritesheet.upper_bound(_T("animation"));

		do
		{
			auto animation = it->second;
			auto animation_attributes = animation->GetAttributes();
			tstring aName = animation_attributes[_T("name")];
			float aSpeed = string_cast<float>(animation_attributes[_T("speed")]);
			int aRepeat = string_cast<int>(animation_attributes[_T("repeat")]);
			tstring frames = animation->GetValue();

			vec2 uv;
			uv.x = 1.0f / (float)m_FramesHorizontal;
			uv.y = 1.0f / (float)m_FramesVertical;

			SpriteAnimation spriteAnimation(aName, uv, aSpeed * speed, aRepeat,
				frames, m_FramesHorizontal, amount);

			//[COMMENT] A memory leak starts below
			insert(std::pair<tstring, SpriteAnimation>(aName, spriteAnimation));

			++it;
		} while( it != end_animation );
	}

	Spritesheet::Spritesheet(const Spritesheet & yRef)
		: Dictionary<tstring, SpriteAnimation>(yRef)
		, m_Name(yRef.m_Name)
		, m_DefaultAnimation(yRef.m_DefaultAnimation)
	{

	}

	Spritesheet::Spritesheet(Spritesheet && yRef)
		: Dictionary<tstring, SpriteAnimation>(yRef)
		, m_Name(yRef.m_Name)
		, m_DefaultAnimation(yRef.m_DefaultAnimation)
	{

	}

	Spritesheet::Spritesheet(iterator begin, iterator end)
		: Dictionary<tstring, SpriteAnimation>(begin, end)
		, m_Name(EMPTY_STRING)
		, m_DefaultAnimation(EMPTY_STRING)
	{

	}

	Spritesheet::~Spritesheet()
	{

	}
	
	Spritesheet & Spritesheet::operator=(const Spritesheet& yRef)
	{
		Dictionary<tstring, SpriteAnimation>::operator=(yRef);
		m_Name = yRef.m_Name;
		m_DefaultAnimation = yRef.m_DefaultAnimation;

		return *this;
	}
	
	void Spritesheet::SetName(const tstring & name)
	{
		m_Name = name;
	}

	const tstring & Spritesheet::GetName() const
	{
		return m_Name;
	}

	const tstring & Spritesheet::GetDefaultAnimation() const
	{
		return m_DefaultAnimation;
	}

	int Spritesheet::GetFramesHorizontal() const
	{
		return m_FramesHorizontal;
	}

	int Spritesheet::GetFramesVertical() const
	{
		return m_FramesVertical;
	}
}
