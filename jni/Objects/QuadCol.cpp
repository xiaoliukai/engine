#include "QuadCol.h"
#include "../Logger.h"

namespace star
{
	QuadCol::QuadCol()
	{
		mShader = new Shader();
		if(!mShader->Init(_T("ColorShader.vert"), _T("ColorShader.frag")))
		{
			star::Logger::GetSingleton()->Log(star::LogLevel::Info, _T("Making Shader Failed"));
		}

		createSquare();
		rotMatrix = glm::mat4(1.0f);
		scaleMatrix = glm::mat4(1.0f);
		translateMatrix = glm::mat4(1.0f);
	}

	QuadCol::~QuadCol()
	{

	}

	void QuadCol::createSquare()
	{
		squareVertices[0] = -0.5f;
		squareVertices[1] = -0.5f;
		squareVertices[2] = 0.5f;
		squareVertices[3] = -0.5f;
		squareVertices[4] = -0.5f;
		squareVertices[5] = 0.5f;
		squareVertices[6] = 0.5f;
		squareVertices[7] = 0.5f;
	}

	void QuadCol::Draw()
	{
		mShader->Bind();
		modelMatrix = translateMatrix*rotMatrix*scaleMatrix;
		GLint mvpID = glGetUniformLocation(mShader->id(), "MVP");
		glUniformMatrix4fv(mvpID,1,GL_FALSE,&modelMatrix[0][0]);

		GLint colorID = glGetUniformLocation(mShader->id(), "inputColor");
		glUniform4f(mvpID,1.0,1.0,0.0,1.0);

		GLint posID =glGetAttribLocation(mShader->id(),"position");
		glVertexAttribPointer(posID, 2, GL_FLOAT,0,0, squareVertices);
		glEnableVertexAttribArray(posID);

		glDrawArrays(GL_TRIANGLE_STRIP,0,4);
		glDisableVertexAttribArray(posID);
		mShader->Unbind();
	}

	void QuadCol::Rotate( const glm::mat4& matrix, float angle, const vec3& axis )
	{
		rotMatrix = glm::rotate(matrix,angle,axis);
	}

	void QuadCol::Scale( const glm::mat4& matrix, const vec3& scaling )
	{
		scaleMatrix = glm::scale(matrix,scaling);
	}

	void QuadCol::Translate( const glm::mat4& matrix, const vec3& translation )
	{
		translateMatrix = glm::translate(matrix,translation);
	}
}
