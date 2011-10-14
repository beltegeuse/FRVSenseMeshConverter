#pragma once
#include "C3/Window.h"
#include "AssimpReader.h"
#include <gl/glew.h>
class Visualizer
{
private:
	C3::Window m_Win;
	const AssimpReader& m_MeshReader;
	/*GLuint ** m_VBOs;*/

public:
	Visualizer(const AssimpReader& meshReader) :
		m_MeshReader(meshReader) // m_VBOs(NULL)
	{
		C3::WindowMode mode(800,600);
		C3::OpenGLContextSettings openGLSettings(3,1);

		// Create the window
		std::cout << "Creation ..." << std::endl;
		m_Win.Create(mode,"FRVSenseMeshExporter Visu",openGLSettings);

		glewInit();

		// Initialise OpenGL
		glViewport(0, 0, 800, 600);

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective(45.0f,(GLfloat)800/(GLfloat)600,0.1f,100.0f);

		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspectiv


		// Creation Ressources
		/*m_VBOs = new GLuint*[meshReader.Meshs.size()];*/
	}

	~Visualizer()
	{
		/*for(unsigned int i = 0; i < meshReader.Meshs.size(); i++)
		{
			glDeleteBuffers(6, m_VBOs[i]);
			delete[] m_VBOs[i];
		}
		delete[] m_VBOs;*/
	}

	void Display()
	{
		for(unsigned int i = 0; i < m_MeshReader.Meshs.size(); i++)
		{
			// Current Mesh
			const AssimpMesh& mesh = m_MeshReader.Meshs[i];

			/*m_VBOs[i] = new GLuint[6];
			glGenBuffers( 6, m_VBOs[i]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs[i][0]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*, m_indices, GL_STATIC_DRAW);*/
			glBegin(GL_TRIANGLES);
			for(unsigned int j = 0; j < mesh.Indices.size(); j++)
			{
				if(!mesh.Vertex.empty())
				{
					glm::vec4 vertex = mesh.Vertex[mesh.Indices[j]];
					glVertex4f(vertex.x, vertex.y, vertex.z, vertex.w);
				}
			}
			glEnd();

		}
	}
	
	void Update()
	{
		// Events
		C3::Event event;
		while(m_Win.PoolEvent(event))
		{
			//std::cout << "Event !" << std::endl;
			if(event.Type == C3::Event::Closed)
			{
				std::cout << "Close ... " << std::endl;
				m_Win.Close();
			}
			else if(event.Type == C3::Event::KeyPressed)
			{
				if(event.Key.Code == C3::Key::Escape)
				{
					std::cout << "Close ... " << std::endl;
					m_Win.Close();
				}
			}
		}
	}

	void Run()
	{
		while(m_Win.IsOpened())
		{
			Update();

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity( );
			gluLookAt(3,4,2,0,0,0,0,0,1);

			Display();

			m_Win.Display();
		}
	}
};