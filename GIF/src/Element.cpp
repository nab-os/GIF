/**
 * Element class implementation
 */

#include "Element.h"

#include <iostream>

using namespace std;
using namespace glm;
using namespace GIF;

Element::Element(): m__points(), m__shader()
{

    m__shader = new Shader("default-120");
    m__shader->load();

}


Element::~Element()
{



}


void Element::load()
{

	int sizeVerticesBytes = this->getVerticesSize();
	//int sizeUVsBytes = this->getUVsSize();

	float* vertices = this->getVerticesFloat();
	//float* UVs = this->getUVsFloat();

	GLuint& idVAO = this->getVAO();
	GLuint& idVBO = this->getVBO();

	//===============================VBO================================

	// Destruction d'un éventuel ancien VBO
	if (glIsBuffer(idVBO) == GL_TRUE)
		glDeleteBuffers(1, &idVBO);


	// Génération de l'ID
	glGenBuffers(1, &idVBO);


	// Verrouillage du VBO
	glBindBuffer(GL_ARRAY_BUFFER, idVBO);


		// Allocation de la mémoire vidéo
		glBufferData(GL_ARRAY_BUFFER, sizeVerticesBytes/* + sizeUVsBytes + sizeNormalsBytes*3*/, 0, GL_STATIC_DRAW); // * 3 for Tangent and Bytangent

		// Transfert des données
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeVerticesBytes, vertices);
		//glBufferSubData(GL_ARRAY_BUFFER, sizeVerticesBytes,  sizeUVsBytes,  UVs);

	// Déverrouillage de l'objet
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//=======================================================================
	// Destruction d'un éventuel ancien VAO

	if (glIsVertexArray(idVAO) == GL_TRUE)
		glDeleteVertexArrays(1, &idVAO);


	// Génération de l'identifiant du VAO
	glGenVertexArrays(1, &idVAO);


	// Verrouillage du VAO
	glBindVertexArray(idVAO);


		// Verrouillage du VBO
		glBindBuffer(GL_ARRAY_BUFFER, idVBO);

			// Accès aux vertices dans la mémoire vidéo

			//Vertices
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glEnableVertexAttribArray(0);

			//UVs
			//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeVerticesBytes));
			//glEnableVertexAttribArray(1);

		// Déverrouillage du VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Déverrouillage du VAO
	glBindVertexArray(0);

	/*
	cout << "[Element] load() : Vertices : \n";
	vector<vec3> vecVertices = this->getVertices();
	for (int i = 0; i < this->getVertexCount(); i++)
	{
		cout << vecVertices[i].x << ", " << vecVertices[i].y << ", " << vecVertices[i].z << endl;
	}
	cout << "[Element] load() : UVs : \n";
	for (int i = 0; i < this->getUVCount(); i++)
	{
		cout << m__UVs[i].x << ", " << m__UVs[i].y << endl;
	}
	*/

	free(vertices);
	//free(UVs);


}


void Element::render(glm::mat4 projection, glm::mat4 model)
{

    glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glUseProgram(m__shader->getProgramID());

	glBindVertexArray(getVAO());

	m__shader->envoyerMat4("projection", projection);
	m__shader->envoyerMat4("model", model);

	//s->envoyerVec3("colorDiffuse", getMaterial()->getDiffuseColor());

	m__shader->envoyer1I("texture", 0);

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);*/

	glDrawArrays(GL_TRIANGLES, 0, getVertexCount());

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
*/
	glBindVertexArray(0);

	glUseProgram(0);

}



//=====Vertices=====
int Element::getVertexCount()
{

	return m__points.size();

}


int Element::getVerticesSize()
{

	int size = m__points.size();

	return size * 3 * sizeof(float);

}


vector<vec3> Element::getVertices()
{

	return m__points;

}


float* Element::getVerticesFloat()
{

	return this->vec3ToFloat(m__points);

}


/*
//=====UVs=====
int Element::getUVCount()
{

	return m__UVs.size();

}


int Element::getUVsSize()
{

	return m__UVs.size() * 2 * sizeof(float);

}


vector<vec2> Element::getUVs()
{

	return m__UVs;

}


float* Element::getUVsFloat()
{

	float* res = (float*)malloc(sizeof(float) * m__UVs.size() * 2);

	for (unsigned int i = 0; i < m__UVs.size(); i++)
	{

		res[i*2] = m__UVs[i].x;
		res[i*2 + 1] = m__UVs[i].y;

	}

	return res;

}*/

float* Element::vec3ToFloat(vector<vec3> tab)
{

	float* res = (float*)malloc(sizeof(float) * tab.size() * 3);

	for (unsigned int i = 0; i < tab.size(); i++)
	{

		res[i*3] = tab[i].x;
		res[i*3 + 1] = tab[i].y;
		res[i*3 + 2] = tab[i].z;

	}

	return res;

}
